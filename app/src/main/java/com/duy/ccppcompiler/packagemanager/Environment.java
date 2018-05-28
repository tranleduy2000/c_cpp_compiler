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

package com.duy.ccppcompiler.packagemanager;

import android.content.Context;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.support.annotation.Nullable;

import com.pdaxrom.utils.Utils;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FilenameFilter;
import java.io.InputStreamReader;

import jackpal.androidterm.TermSettings;

/**
 * Created by Duy on 16-May-18.
 */

public class Environment {
    public static final String APPLICATION_DIR_NAME = "CCPlusPlusNIDE";
    private static final String TAG = "Environment";

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
        String path = new File(getToolchainsDir(context), "installed").getAbsolutePath();
        return mkdirIfNotExist(path);
    }

    public static String getDalvikCacheDir(Context context) {
        File path = new File(getToolchainsDir(context), "cctools/var/dalvik/dalvik-cache");
        return mkdirIfNotExist(path);
    }

    /**
     * @return temp directory for execute file
     */
    public static String getTmpExeDir(Context context) {
        File file = new File(Environment.getToolchainsDir(context), "tmpdir");
        return mkdirIfNotExist(file);
    }

    /**
     * @return temp directory for execute file
     */
    public static String getTmpDir(Context context) {
        File file = new File(Environment.getToolchainsDir(context), "t");
        return mkdirIfNotExist(file);
    }

    /**
     * External storage
     */
    public static String getSdCardHomeDir() {
        File path = new File(android.os.Environment.getExternalStorageDirectory().getPath(), APPLICATION_DIR_NAME);
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

    /**
     * ANDROID_SOCKET_zygote=9
     * EMULATED_STORAGE_SOURCE=/mnt/shell/emulated
     * ANDROID_STORAGE=/storage
     * ANDROID_BOOTLOGO=1
     * EXTERNAL_STORAGE=/storage/emulated/legacy
     * ANDROID_ASSETS=/system/app
     * ASEC_MOUNTPOINT=/mnt/asec
     * PATH=/sbin:/vendor/bin:/system/sbin:/system/bin:/system/xbin
     * LOOP_MOUNTPOINT=/mnt/obb
     * BOOTCLASSPATH=/system/framework/core.jar:/system/framework/conscrypt.jar:/system/framework/okhttp.jar:/system/framework/core-junit.jar:/system/framework/bouncycastle.jar:/system/framework/ext.jar:/system/framework/framework.jar:/system/framework/framework2.jar:/system/framework/telephony-common.jar:/system/framework/voip-common.jar:/system/framework/mms-common.jar:/system/framework/android.policy.jar:/system/framework/services.jar:/system/framework/apache-xml.jar:/system/framework/webviewchromium.jar
     * EMULATED_STORAGE_TARGET=/storage/emulated
     * ANDROID_DATA=/data
     * ANDROID_PROPERTY_WORKSPACE=8,0
     * ANDROID_ROOT=/system
     * LD_LIBRARY_PATH=/vendor/lib:/system/lib
     */
    public static String[] buildDefaultEnv(Context context) {
        final SharedPreferences pref = PreferenceManager.getDefaultSharedPreferences(context);
        final TermSettings settings = new TermSettings(context.getResources(), pref);
        final String cctoolsDir = getCCtoolsDir(context);
        final String tmpExeDir = getTmpExeDir(context);

        return new String[]{
                "TMPDIR=" + tmpExeDir,
                "TMPEXEDIR=" + tmpExeDir,
                "TEMPDIR=" + tmpExeDir,
                "TEMP=" + tmpExeDir,
                "PATH=" + joinPath(cctoolsDir + "/bin", cctoolsDir + "/sbin", System.getenv("PATH")),
                "HOME=" + getHomeDir(context),

                "ANDROID_ASSETS=" + getEnv("ANDROID_ASSETS", "/system/app"),
                "ANDROID_BOOTLOGO=" + getEnv("ANDROID_BOOTLOGO", "1"),
                "ANDROID_DATA=" + joinPath(cctoolsDir + "/var/dalvik", getEnv("ANDROID_DATA", null)),
                "ANDROID_ROOT=" + getEnv("ANDROID_ROOT", "/system"),
                "ANDROID_PROPERTY_WORKSPACE=" + getEnv(context, "ANDROID_PROPERTY_WORKSPACE"),
                "BOOTCLASSPATH=" + getBootClassPath(),
                "EXTERNAL_STORAGE=" + android.os.Environment.getExternalStorageDirectory().getPath(),
                "LD_LIBRARY_PATH=" + joinPath(cctoolsDir + "/lib", getEnv("LD_LIBRARY_PATH", null)),

                "CCTOOLSDIR=" + cctoolsDir,
                "CCTOOLSRES=" + context.getPackageResourcePath(),
                "CFGDIR=" + getShareDir(context),
                "SHELL=" + getShell(context),
                "TERM=" + settings.getTermType(),
                "PS1=$ ",
                "SDDIR=" + getSdCardHomeDir(),
        };
    }

    private static String getShareDir(Context context) {
        File file = new File(getCCtoolsDir(context), "share");
        return mkdirIfNotExist(file);
    }

    private static String getBootClassPath() {
        String bootClassPath = getEnv("BOOTCLASSPATH", null);
        if (bootClassPath == null) {
            bootClassPath = findBootClassPath();
        }
        if (bootClassPath == null || bootClassPath.isEmpty()) {
            bootClassPath = "/system/framework/core.jar:" +
                    "/system/framework/ext.jar:" +
                    "/system/framework/framework.jar:" +
                    "/system/framework/android.policy.jar:" +
                    "/system/framework/services.jar";
        }
        return bootClassPath;
    }

    @Nullable
    private static String findBootClassPath() {
        String classPath = null;
        File dir = new File("/system/framework");
        if (dir.exists() && dir.isDirectory()) {
            FilenameFilter filter = new FilenameFilter() {
                public boolean accept(File dir, String name) {
                    String lowercaseName = name.toLowerCase();
                    return lowercaseName.endsWith(".jar");
                }
            };

            String[] list = dir.list(filter);
            for (int i = 0; i < list.length; i++) {
                String file = list[i];
                if (i != 0) {
                    classPath += ":";
                }
                classPath += "/system/framework/" + file;
            }
        }
        return classPath;
    }

    private static String getEnv(String name, String defValue) {
        String value = System.getenv(name);
        if (value != null) {
            return value;
        } else {
            return defValue;
        }
    }

    private static String joinPath(String... paths) {
        StringBuilder sb = new StringBuilder();
        for (String path : paths) {
            if (path != null && !path.isEmpty()) {
                if (sb.length() != 0) {
                    sb.append(File.pathSeparator);
                }
                sb.append(path);
            }
        }
        return sb.toString();
    }

    public static String getShell(Context context) {
        return "/system/bin/sh";
    }

    protected static String getEnv(Context context, String variable) {
        String ret = null;
        final String cctoolsDir = getCCtoolsDir(context);
        String[] envp = {
                "TMPDIR=" + android.os.Environment.getExternalStorageDirectory().getPath(),
                "PATH=" + joinPath(cctoolsDir + "/bin", cctoolsDir + "/sbin", System.getenv("PATH")),
                "ANDROID_ASSETS=" + getEnv("ANDROID_ASSETS", "/system/app"),
                "ANDROID_BOOTLOGO=" + getEnv("ANDROID_BOOTLOGO", "1"),
                "ANDROID_DATA=" + joinPath(cctoolsDir + "/var/dalvik", getEnv("ANDROID_DATA", null)),
                "ANDROID_ROOT=" + getEnv("ANDROID_ROOT", "/system"),
                "CCTOOLSDIR=" + cctoolsDir,
                "CCTOOLSRES=" + context.getPackageResourcePath(),
                "LD_LIBRARY_PATH=" + joinPath(cctoolsDir + "/lib", getEnv("LD_LIBRARY_PATH", null)),
                "HOME=" + getHomeDir(context),
                "SHELL=" + getShell(context),
                "TERM=xterm",
                "PS1=$ ",
                "SDDIR=" + getSdCardHomeDir(),
                "EXTERNAL_STORAGE=" + android.os.Environment.getExternalStorageDirectory().getPath(),
        };
        String[] argv = {"/system/bin/sh", "-c", "set"};
        int[] pId = new int[1];
        FileDescriptor fd = Utils.createSubProcess(cctoolsDir, argv[0], argv, envp, pId);
        FileInputStream fis = new FileInputStream(fd);
        DataInputStream in = new DataInputStream(fis);
        BufferedReader reader = new BufferedReader(new InputStreamReader(in));
        String line;
        try {
            while ((line = reader.readLine()) != null) {
                if (line.startsWith(variable + "=")) {
                    if (line.contains("=")) {
                        ret = line.substring(line.indexOf("=") + 1);
                        break;
                    }
                }
            }
            in.close();
            Utils.waitFor(pId[0]);
        } catch (Exception ignored) {
        }
        return ret;
    }

    public static String[] join(String[] env1, String[] env2) {
        String[] env = new String[env1.length + env2.length];
        System.arraycopy(env1, 0, env, 0, env1.length);
        System.arraycopy(env2, 0, env, env1.length, env2.length);
        return env;
    }

    public static void mkdirs(Context context) {
        Environment.getServiceDir(context);
        Environment.getHomeDir(context);
        Environment.getToolchainsDir(context);

        Environment.getSdCardHomeDir();
        Environment.getSdCardBackupDir();
        Environment.getSdCardTmpDir();

    }

    public static String getSdCardDir() {
        return android.os.Environment.getExternalStorageDirectory().getAbsolutePath();
    }

    public static String getSdCardExampleDir() {
        File file = new File(getSdCardHomeDir(), "Examples");
        return mkdirIfNotExist(file);
    }
}
