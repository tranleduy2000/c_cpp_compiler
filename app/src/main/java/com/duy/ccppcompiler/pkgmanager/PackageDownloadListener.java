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

package com.duy.ccppcompiler.pkgmanager;

import android.support.annotation.NonNull;
import android.support.annotation.WorkerThread;

import com.duy.ccppcompiler.pkgmanager.model.PackageInfo;

import java.io.File;

/**
 * Created by Duy on 20-May-18.
 */

public interface PackageDownloadListener {
    @WorkerThread
    void onDownloadComplete(PackageInfo packageInfo, File downloadedFile);

    @WorkerThread
    void onFailure(@NonNull Exception e);

    @WorkerThread
    void onDownloadProgress(String serverName, PackageInfo packageInfo, long totalRead, long fileSize);
}
