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

import com.duy.ccppcompiler.packagemanager.DownloadListener;
import com.duy.ccppcompiler.packagemanager.RepoParser;
import com.duy.ccppcompiler.packagemanager.RepoUtils;
import com.duy.ccppcompiler.packagemanager.model.PackageInfo;
import com.duy.common.DLog;
import com.google.android.gms.tasks.OnFailureListener;
import com.google.android.gms.tasks.OnSuccessListener;
import com.google.firebase.storage.FirebaseStorage;
import com.google.firebase.storage.StorageReference;

import java.io.File;
import java.util.ArrayList;
import java.util.List;

import static com.duy.ccppcompiler.packagemanager.RepoUtils.PACKAGES_INDEX_FILE;
import static com.duy.ccppcompiler.packagemanager.repo.FirebasePackageRepo.FirebaseStorageContract.PACKAGES;
import static com.duy.ccppcompiler.packagemanager.repo.FirebasePackageRepo.FirebaseStorageContract.PACKAGES_PIE;
import static com.duy.ccppcompiler.packagemanager.repo.FirebasePackageRepo.FirebaseStorageContract.ROOT;

/**
 * Created by Duy on 20-May-18.
 */

public class FirebasePackageRepo implements IPackageRepository {
    private static final String TAG = "FirebasePackageRepo";
    private StorageReference mStorage;
    private List<PackageInfo> mPackageInfos = null;

    public FirebasePackageRepo() {
        mStorage = FirebaseStorage.getInstance().getReference();
        getPackages(); //download as son as possible
    }

    @NonNull
    @Override
    public List<PackageInfo> getPackages() {
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
                    }
                })
                .addOnFailureListener(new OnFailureListener() {
                    @Override
                    public void onFailure(@NonNull Exception e) {
                        e.printStackTrace();
                    }
                });
        if (mPackageInfos != null) {
            return mPackageInfos;
        }
        return new ArrayList<>();
    }

    @Override
    public void download(File saveToDir, PackageInfo packageInfo, DownloadListener listener) {

    }

    static class FirebaseStorageContract {
        static final String ROOT = "android-repo";
        static final String PACKAGES_PIE = "packages-pie";
        static final String PACKAGES = "packages";
    }
}
