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

    /**
     * Internal storage
     */
    public static String getToolchainsDir(Context context) {
        File path = new File(context.getCacheDir().getParentFile(), "root");
        return mkdirIfNotExist(path);
    }

    public static String getCCtoolsDir(Context c) {
        File path = new File(getToolchainsDir(c), "cctools");
        return mkdirIfNotExist(path);
    }

    public static String getServiceDir(Context context) {
        String path = new File(getToolchainsDir(context), "/cctools/services").getAbsolutePath();
        return mkdirIfNotExist(path);
    }

    public static String getHomeDir(Context context) {
        String path = new File(getToolchainsDir(context), "/cctools/home").getAbsolutePath();
        return mkdirIfNotExist(path);
    }

    public static String getInstalledPackageDir(Context context) {
        String path = new File(getToolchainsDir(context), INSTALLED_PACKAGE_DIR).getAbsolutePath();
        return mkdirIfNotExist(path);
    }

    public static String getDalvikCacheDir(Context context) {
        File path = new File(getToolchainsDir(context), "/cctools/var/dalvik/dalvik-cache");
        return mkdirIfNotExist(path);
    }


    /**
     * External storage
     */
    public static String getSdCardHomeDir() {
        File path = new File(Environment.getExternalStorageDirectory().getPath(), APPLICATION_DIR_NAME);
        return mkdirIfNotExist(path);
    }

    public static String getSdCardBackupDir() {
        String path = getSdCardHomeDir() + "/backup";
        return mkdirIfNotExist(path);
    }

    public static String getSdCardTmpDir() {
        String path = getSdCardHomeDir() + "/tmp";
        return mkdirIfNotExist(path);
    }

    public static String getSdCardSourceDir() {
        return mkdirIfNotExist(new File(getSdCardHomeDir(), "src"));
    }


    private static String mkdirIfNotExist(String path) {
        File file = new File(path);
        return mkdirIfNotExist(file);
    }

    private static String mkdirIfNotExist(File file) {
        if (!file.exists()) {
            file.mkdir();
        }
        return file.getAbsolutePath();
    }

}
