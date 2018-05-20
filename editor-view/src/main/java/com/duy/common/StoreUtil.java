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

package com.duy.common;

import android.app.Activity;
import android.content.ActivityNotFoundException;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.net.Uri;

import com.jecelyin.common.utils.DLog;

/**
 * Created by Duy on 10-Jul-17.
 */

public class StoreUtil {
    private static final String TAG = "StoreUtil";

    /**
     * Go to Google Play app
     *
     * @param context - android context
     * @param appId   - an application id
     */
    public static void gotoPlayStore(Activity context, String appId) {
        if (DLog.DEBUG)
            DLog.d(TAG, "gotoPlayStore() called with: context = [" + context + "], appId = [" + appId + "]");
        Uri uri = Uri.parse(String.format("market://details?id=%s", appId));
        Intent goToMarket = new Intent(Intent.ACTION_VIEW, uri);
        // To count with Play market backstack, After pressing back button,
        // to taken back to our application, we need to add following flags to intent.
        goToMarket.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY | Intent.FLAG_ACTIVITY_MULTIPLE_TASK);
        try {
            context.startActivity(goToMarket);
        } catch (Exception e) {
            if (DLog.DEBUG) DLog.e(e);
            try {
                Uri link = Uri.parse("http://play.google.com/store/apps/details?id=" + appId);
                Intent intent = new Intent(Intent.ACTION_VIEW, link);
                context.startActivity(intent);
            } catch (Exception e2) {
                if (DLog.DEBUG) DLog.e(e2);
            }
        }
    }

    /**
     * Go to Google Play app
     *
     * @param context     - android context
     * @param appId       - an application id
     * @param requestCode - activity request code
     */
    public static void gotoPlayStore(Activity context, String appId, int requestCode) {
        Uri uri = Uri.parse("market://details?id=" + appId);
        Intent goToMarket = new Intent(Intent.ACTION_VIEW, uri);
        // To count with Play market backstack, After pressing back button,
        // to taken back to our application, we need to add following flags to intent.
        goToMarket.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY | Intent.FLAG_ACTIVITY_MULTIPLE_TASK);
        try {
            context.startActivity(goToMarket);
        } catch (ActivityNotFoundException e) {
            String uriString = "http://play.google.com/store/apps/details?id=" + appId;
            gotoToLink(context, uriString, requestCode);
        }
    }

    /**
     * Use openBrowser
     *
     * @param context
     * @param uriString
     * @param request
     */
    @Deprecated
    public static void gotoToLink(Activity context, String uriString, int request) {
        openBrowser(context, uriString, request);
    }

    public static void openBrowser(Activity context, String uriString, int request) {
        Uri link = Uri.parse(uriString);
        Intent intent = new Intent(Intent.ACTION_VIEW, link);
        context.startActivityForResult(intent, request);
    }

    /**
     * {@link ShareUtil#shareApp(Activity, String)}
     */
    @Deprecated
    public static void shareApp(Activity context, String appId) {
        ShareUtil.shareApp(context, appId);
    }

    public static void shareThisApp(Activity context) {
        ShareUtil.shareThisApp(context);
    }

    public static void moreApp(Activity mainActivity) {
        String location = "https://play.google.com/store/apps/dev?id=7055567654109499514";
        Uri uri = Uri.parse(location);
        Intent goToMarket = new Intent(Intent.ACTION_VIEW, uri);
        // To count with Play market backstack, After pressing back button,
        // to taken back to our application, we need to add following flags to intent.
        goToMarket.addFlags(Intent.FLAG_ACTIVITY_NO_HISTORY | Intent.FLAG_ACTIVITY_MULTIPLE_TASK);
        try {
            mainActivity.startActivity(goToMarket);
        } catch (ActivityNotFoundException e) {
            mainActivity.startActivity(new Intent(Intent.ACTION_VIEW,
                    Uri.parse(location)));
        }
    }

    public static boolean isAppInstalled(Context context, String appId) {
        try {
            context.getPackageManager().getApplicationInfo(appId, 0);
            return true;
        } catch (PackageManager.NameNotFoundException e) {
            return false;
        }
    }
}
