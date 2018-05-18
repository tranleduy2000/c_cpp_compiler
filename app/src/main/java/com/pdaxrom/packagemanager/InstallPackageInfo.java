package com.pdaxrom.packagemanager;

import java.util.ArrayList;
import java.util.List;

public class InstallPackageInfo {
    private static final String TAG = "InstallPackageInfo";
    private static boolean debug = false;

    private String pkg = null;
    private List<PackageInfo> packages = null;
    private int mInstallSize = 0;
    private int mDownloadSize = 0;

    InstallPackageInfo() {
        packages = new ArrayList<>();
    }

    InstallPackageInfo(PackagesLists packagesLists, String pkg) {
        installPackageInfo(packagesLists, pkg, null);
    }

    InstallPackageInfo(PackagesLists packagesLists, String pkg, List<PackageInfo> packages) {
        installPackageInfo(packagesLists, pkg, packages);
    }

    public static void enableDebug(boolean debug) {
        InstallPackageInfo.debug = debug;
    }

    public void addPackage(PackagesLists packagesLists, String pkg) {
        if (RepoUtils.isContainsPackage(packagesLists.getAvailablePackages(), pkg)) {
            if (this.pkg == null) {
                this.pkg = pkg;
            } else {
                this.pkg += " " + pkg;
            }
            getDepends(packagesLists, pkg, packages);
            calculateSizes();
        }
    }

    private void installPackageInfo(PackagesLists packagesLists, String pkg, List<PackageInfo> list) {
        this.pkg = pkg;
        if (list == null) {
            this.packages = new ArrayList<>();
        } else {
            this.packages = list;
        }
        getDepends(packagesLists, this.pkg, this.packages);
        calculateSizes();
    }

    public String getName() {
        return pkg;
    }

    public void calculateSizes() {
        mInstallSize = 0;
        mDownloadSize = 0;
        for (PackageInfo pkg : packages) {
            mInstallSize += pkg.getSize();
            mDownloadSize += pkg.getFileSize();
        }
    }

    public int getDownloadSize() {
        return mDownloadSize;
    }

    public int getInstallSize() {
        return mInstallSize;
    }

    public List<PackageInfo> getPackagesList() {
        return packages;
    }

    public String getPackagesStrings() {
        StringBuilder packages = new StringBuilder();
        boolean isFirstAdded = false;
        for (PackageInfo pkg : this.packages) {
            if (isFirstAdded) {
                packages.append(" ").append(pkg.getName());
            } else {
                packages.append(pkg.getName());
            }
            isFirstAdded = true;
        }
        return packages.toString();
    }

    private void getDepends(PackagesLists packagesLists, String packageWithVariants, List<PackageInfo> list) {
        String[] packageVariants = packageWithVariants.replace('|', ' ').split("\\s+");
        if (debug) {
            System.out.println(TAG + " packageVariants = " + packageWithVariants.replace('|', ' '));
        }
        String firstPackage = packageVariants[0];
        for (String packageVariant : packageVariants) {
            if (debug) {
                System.out.println(TAG + " packageVariant = " + packageVariant);
            }
            if (RepoUtils.isContainsPackage(list, packageVariant)) {
                return;
            }
            if (RepoUtils.isContainsPackage(packagesLists.getInstalledPackages(), packageVariant)) {
                firstPackage = packageVariant;
                break;
            }
        }

        for (PackageInfo info : packagesLists.getAvailablePackages()) {
            if (firstPackage.equals(info.getName())) {
                String deps = info.getDepends();
                if (debug) {
                    System.out.println(TAG + " package deps = " + deps);
                }
                if (deps != null && !deps.equals("")) {
                    deps = deps.replaceAll("\\s+", " ");
                    for (String dep : deps.split("\\s+")) {
                        if (debug) {
                            System.out.println(TAG + " check package = " + dep);
                        }
                        getDepends(packagesLists, dep, list);
                    }
                }
                PackageInfo installedPackage = RepoUtils.getPackageByName(
                        packagesLists.getInstalledPackages(), firstPackage);
                if (installedPackage != null) {
                    if (installedPackage.getVersion().equals(info.getVersion())) {
                        if (debug) {
                            System.out.println(TAG + " the same version, skip package = " + firstPackage);
                        }
                        break;
                    }
                }
                list.add(info);
                if (debug) {
                    System.out.println(TAG + " add package = " + firstPackage);
                }
                break;
            }
        }
    }
}
