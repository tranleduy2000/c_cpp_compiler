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

package com.jecelyin.common.utils;

import android.Manifest;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.net.Uri;
import android.os.Environment;
import android.os.Looper;
import android.support.annotation.RequiresPermission;
import android.util.TypedValue;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.StringTokenizer;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class SysUtils {

    public static int dpAsPixels(Context context, int dp) {
        Resources resources = context.getResources();
        return (int) TypedValue.applyDimension(1, dp, resources.getDisplayMetrics());
    }

    public static File getCacheDir(Context context) {
        File cacheDir;
        if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())
                || !Environment.isExternalStorageRemovable()) {
            cacheDir = context.getExternalCacheDir();
            if (cacheDir == null)
                cacheDir = context.getCacheDir();
        } else {
            cacheDir = context.getCacheDir();
        }
        return cacheDir;
    }

    public static File getFileDir(Context context) {
        File fileDir;
        if (Environment.MEDIA_MOUNTED.equals(Environment.getExternalStorageState())
                || !Environment.isExternalStorageRemovable()) {
            fileDir = context.getExternalFilesDir(null);
            if (fileDir == null)
                fileDir = context.getFilesDir();
        } else {
            fileDir = context.getFilesDir();
        }
        return fileDir;
    }

    public static String getVersionName(Context context) {
        try {
            PackageInfo packageInfo = context.getPackageManager().getPackageInfo(context.getPackageName(), 0);
            return packageInfo.versionName;
        } catch (PackageManager.NameNotFoundException e) {
            DLog.e(e);
            return null;
        }
    }

    public static boolean isUIThread() {
        return Looper.getMainLooper().getThread() == Thread.currentThread();
    }

    public static void startWebView(Context context, String url) {
        Uri uri = Uri.parse(url);
        Intent launchBrowser = new Intent(Intent.ACTION_VIEW, uri);
        context.startActivity(launchBrowser);
    }

    /**
     * require: <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
     *
     * @param context
     * @return
     */
    @RequiresPermission(Manifest.permission.ACCESS_NETWORK_STATE)
    public static boolean isNetworkAvailable(Context context) {
        try {
            ConnectivityManager connectivityManager
                    = (ConnectivityManager) context.getApplicationContext().getSystemService(Context.CONNECTIVITY_SERVICE);
            NetworkInfo activeNetworkInfo = connectivityManager.getActiveNetworkInfo();
            return activeNetworkInfo != null && activeNetworkInfo.isConnected();
        } catch (Exception e) {
            return false;
        }
    }

    public static String getAppStoragePath(Context context) {
        File path = new File(Environment.getExternalStorageDirectory(), context.getPackageName());
        if (!path.exists()) {
            path.mkdirs();
        }
        return path.getPath();
    }

    public static List<String> getStorageDirectories(boolean removableStorageOnly) {
        List<String> list = new ArrayList<String>();
        String internalPath = Environment.getExternalStorageDirectory().getPath();

        BufferedReader buf_reader = null;
        try {
            buf_reader = new BufferedReader(new FileReader("/proc/mounts"));
            String line;
            while ((line = buf_reader.readLine()) != null) {
                StringTokenizer tokens = new StringTokenizer(line, " ");
                String unused = tokens.nextToken(); //device
                String mountPoint = tokens.nextToken(); //mount point

                unused = tokens.nextToken(); //file system
                if ("tmpfs".equals(unused))
                    continue;
                List<String> flags = Arrays.asList(tokens.nextToken().split(",")); //flags
                boolean readonly = flags.contains("ro");

                if (removableStorageOnly && mountPoint.equals(internalPath)) {
                } else if (mountPoint.startsWith("/mnt/") || mountPoint.startsWith("/storage")
                        || mountPoint.startsWith("/sdcard")) {
                    list.add(mountPoint);
                }
            }

        } catch (Exception ex) {
            DLog.e(ex);
        } finally {
            if (buf_reader != null) {
                try {
                    buf_reader.close();
                } catch (IOException ex) {
                }
            }
        }
        return list;
    }

    public static boolean isRemovableMediaStoragePath(String path) {
        List<String> directories = getStorageDirectories(true);

        for (String directory : directories) {
            if (path.startsWith(directory))
                return true;
        }

        return false;
    }

    /* Checks if external storage is available for read and write */
    public boolean isExternalStorageWritable() {
        String state = Environment.getExternalStorageState();
        if (Environment.MEDIA_MOUNTED.equals(state)) {
            return true;
        }
        return false;
    }

    /* Checks if external storage is available to at least read */
    public boolean isExternalStorageReadable() {
        String state = Environment.getExternalStorageState();
        if (Environment.MEDIA_MOUNTED.equals(state) ||
                Environment.MEDIA_MOUNTED_READ_ONLY.equals(state)) {
            return true;
        }
        return false;
    }
}
