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

import com.duy.ccppcompiler.packagemanager.PackageDownloadListener;
import com.duy.ccppcompiler.packagemanager.IPackageLoadListener;
import com.duy.ccppcompiler.packagemanager.RepoParser;
import com.duy.ccppcompiler.packagemanager.RepoUtils;
import com.duy.ccppcompiler.packagemanager.model.PackageInfo;
import com.duy.common.DLog;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.storage.FirebaseStorage;
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
    private StorageReference mStorage;
    private List<PackageInfo> mPackageInfos = null;

    public FirebasePackageRepository() {
        mStorage = FirebaseStorage.getInstance().getReference();
    }

    @Override
    public void getPackagesInBackground(final IPackageLoadListener listener) {
        if (mPackageInfos != null) {
            listener.onSuccess(mPackageInfos);
            return;
        }

        final StorageReference repo = mStorage.child(ROOT)
                .child(Build.VERSION.SDK_INT >= 21 ? PACKAGES_PIE : PACKAGES)
                .child(RepoUtils.CPU_API);
        repo.child(PACKAGES_INDEX_FILE)
                .getBytes(1024 * 1024)
                .addOnSuccessListener(new OnSuccessListener<byte[]>() {
                    @Override
                    public void onSuccess(byte[] bytes) {
                        String content = new String(bytes);
                        if (DLog.DEBUG) DLog.d(TAG, "content = " + content);
                        RepoParser repoParser = new RepoParser();
                        mPackageInfos = repoParser.parseRepoXml(content, repo.getPath());
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
    public void download(File saveToDir, PackageInfo packageInfo, PackageDownloadListener listener) {

    }

    static class FirebaseStorageContract {
        static final String ROOT = "android-repo";

    }
}
