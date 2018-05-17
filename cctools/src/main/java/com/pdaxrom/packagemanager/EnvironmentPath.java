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

package com.pdaxrom.packagemanager;

import android.content.Context;
import android.os.Environment;

import java.io.File;

/**
 * Created by Duy on 16-May-18.
 */

public class EnvironmentPath {
    public static final String APPLICATION_DIR_NAME = "CCtools";
    public static final String INSTALLED_PACKAGE_DIR = "/installed/";

    public static String getToolchainsDir(Context context) {
        return context.getCacheDir().getParentFile().getAbsolutePath() + "/root";
    }

    public static String getRootDir(Context context) {
        return new File(context.getCacheDir().getParentFile(), "root").getAbsolutePath();
    }

    public static String getSdCardDir() {
        return Environment.getExternalStorageDirectory().getPath() + "/" + APPLICATION_DIR_NAME;
    }

    public static String getSdCardBackupDir() {
        return getSdCardDir() + "/backup";
    }

    public static String getSdCardTmpDir() {
        return getSdCardDir() + "/tmp";
    }

    public static String getServiceDir(Context context) {
        return new File(getToolchainsDir(context), "/cctools/services").getAbsolutePath();
    }

    public static String getHomeDir(Context context) {
        return new File(getToolchainsDir(context), "/cctools/home").getAbsolutePath();
    }
}
