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

import android.support.annotation.Nullable;

import com.duy.ccppcompiler.packagemanager.model.PackageInfo;
import com.duy.ccppcompiler.packagemanager.model.PackagesLists;

import java.util.ArrayList;
import java.util.List;

public class RepoUtils {

    private static final String TAG = "RepoUtils";
    /**
     * armeabi-v7a/
     * armeabi/
     * mips/
     * x86/
     */
    public static String CPU_API;
    private static String ndkArch;
    private static int ndkVersion;

    private static boolean DEBUG = false;

    public static void setVersion(String buildAbi, String ndkArch, int ndkVersion) {
        RepoUtils.CPU_API = buildAbi;
        RepoUtils.ndkArch = ndkArch;
        RepoUtils.ndkVersion = ndkVersion;
    }

    public static boolean isContainsPackage(List<PackageInfo> repo, String pkg) {
        for (PackageInfo packageInfo : repo) {
            if (packageInfo.getName().equals(pkg)) {
                return true;
            }
        }
        return false;
    }

    public static boolean isContainsPackage(List<PackageInfo> repo, String pkg, String version) {
        for (PackageInfo packageInfo : repo) {
            if (packageInfo.getName().equals(pkg) && packageInfo.getVersion().equals(version)) {
                return true;
            }
        }
        return false;
    }

    @Nullable
    public static PackageInfo getPackageByName(List<PackageInfo> repo, String pkg) {
        for (PackageInfo packageInfo : repo) {
            if (packageInfo.getName().equals(pkg)) {
                return packageInfo;
            }
        }
        return null;
    }

    public static String replaceMacro(String str) {
        if (str != null) {
            str = str.replaceAll("\\$\\{HOSTARCH\\}", ndkArch);
            str = str.replaceAll("\\$\\{HOSTNDKARCH\\}", ndkArch);
            str = str.replaceAll("\\$\\{HOSTNDKVERSION\\}", String.valueOf(ndkVersion));
        }
        return str;
    }

    public static List<PackageInfo> checkingForUpdates(List<PackageInfo> availablePackages,
                                                       List<PackageInfo> installedPackages) {
        List<PackageInfo> list = null;

        for (PackageInfo installedPkg : installedPackages) {
            for (PackageInfo pkg : availablePackages) {
                if (installedPkg.getName().equals(pkg.getName())) {
                    if (!installedPkg.getVersion().equals(pkg.getVersion())) {
                        if (list == null) {
                            list = new ArrayList<>();
                        }
                        list.add(pkg);
                    }
                    break;
                }
            }
        }
        return list;
    }

    public static List<PackageInfo> checkingForUpdates(PackagesLists packagesLists) {
        return checkingForUpdates(packagesLists.getAvailablePackages(),
                packagesLists.getInstalledPackages());
    }
}
