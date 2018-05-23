/*
 * Copyright 2018 Mr Duy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.duy.ccppcompiler.packagemanager.repo;

import android.os.Build;
import android.support.annotation.NonNull;

import com.duy.ccppcompiler.packagemanager.IPackageLoadListener;
import com.duy.ccppcompiler.packagemanager.PackageDownloadListener;
import com.duy.ccppcompiler.packagemanager.RepoParser;
import com.duy.ccppcompiler.packagemanager.RepoUtils;
import com.duy.ccppcompiler.packagemanager.model.PackageInfo;
import com.duy.common.DLog;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.storage.FileDownloadTask;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.OnProgressListener;
import com.google.firebase.storage.StorageReference;

import java.io.File;
import java.util.List;

import static com.duy.ccppcompiler.packagemanager.repo.FirebasePackageRepository.FirebaseStorageContract.ROOT;
import static com.duy.ccppcompiler.packagemanager.repo.RepoConstants.PACKAGES;
import static com.duy.ccppcompiler.packagemanager.repo.RepoConstants.PACKAGES_INDEX_FILE;
import static com.duy.ccppcompiler.packagemanager.repo.RepoConstants.PACKAGES_PIE;

/**
 * Created by Duy on 20-May-18.
 * <p>
 * In current version, I use Firebase free storage with 1GB download per day (30 user/day)
 * If reached limit, use {@link UbuntuServerPackageRepository}
 */
public class FirebasePackageRepository extends PackageRepositoryImpl {
    private static final String TAG = "FirebasePackageRepository";
    private StorageReference mRepo;
    private List<PackageInfo> mPackageInfos = null;

    public FirebasePackageRepository() {
        mRepo = FirebaseStorage.getInstance().getReference();
        mRepo = mRepo.child(ROOT)
                .child(Build.VERSION.SDK_INT >= 21 ? PACKAGES_PIE : PACKAGES)
                .child(RepoUtils.CPU_API);
    }

    @NonNull
    @Override
    public void getPackagesInBackground(final IPackageLoadListener listener) {
        if (mPackageInfos != null) {
            listener.onSuccess(mPackageInfos);
            return;
        }
        final int oneMegabytes = 1024 * 1024;
        mRepo.child(PACKAGES_INDEX_FILE)
                .getBytes(oneMegabytes)
                .addOnSuccessListener(new OnSuccessListener<byte[]>() {
                    @Override
                    public void onSuccess(byte[] bytes) {
                        String content = new String(bytes);
                        if (DLog.DEBUG) DLog.d(TAG, "content = " + content);
                        RepoParser repoParser = new RepoParser();
                        mPackageInfos = repoParser.parseRepoXml(content);
                        listener.onSuccess(mPackageInfos);
                    }
                })
                .addOnFailureListener(new OnFailureListener() {
                    @Override
                    public void onFailure(@NonNull Exception e) {
                        e.printStackTrace();
                        listener.onFailed(e);
                    }
                });
    }

    @Override
    public void download(File saveToDir, final PackageInfo packageInfo, final PackageDownloadListener listener) {
        final File localFile = new File(saveToDir, packageInfo.getFileName());
        if (localFile.exists() && localFile.length() != 0) {
            listener.onDownloadComplete(packageInfo, localFile);
            return;
        }
        StorageReference fileRef = mRepo.child(packageInfo.getFileName());
        String path = fileRef.getPath();
        if (DLog.DEBUG) DLog.d(TAG, "path = " + path);
        fileRef.getFile(localFile)
                .addOnSuccessListener(new OnSuccessListener<FileDownloadTask.TaskSnapshot>() {
                    @Override
                    public void onSuccess(FileDownloadTask.TaskSnapshot taskSnapshot) {
                        listener.onDownloadComplete(packageInfo, localFile);
                    }
                })
                .addOnProgressListener(new OnProgressListener<FileDownloadTask.TaskSnapshot>() {
                    @Override
                    public void onProgress(FileDownloadTask.TaskSnapshot taskSnapshot) {
                        listener.onDownloadProgress(packageInfo, taskSnapshot.getBytesTransferred(), taskSnapshot.getTotalByteCount());
                    }
                })
                .addOnFailureListener(new OnFailureListener() {
                    @Override
                    public void onFailure(@NonNull Exception exception) {
                        localFile.delete();
                        listener.onFailure(exception);
                    }
                });
    }

    static class FirebaseStorageContract {
        static final String ROOT = "android-repo";

    }
}
