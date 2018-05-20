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

import android.support.annotation.NonNull;

import com.duy.ccppcompiler.packagemanager.model.PackageInfo;
import com.google.firebase.storage.FirebaseStorage;
import com.sun.tools.javac.util.Context;

import java.util.List;

/**
 * Created by Duy on 20-May-18.
 */

public class FirebasePackageRepo implements IPackageRepository {
    private FirebaseStorage mStorage;
    private Context mContext;

    public FirebasePackageRepo(Context context) {
        mContext = context;
        mStorage = FirebaseStorage.getInstance();
    }

    @NonNull
    @Override
    public List<PackageInfo> getPackages() {
        return null;
    }
}
