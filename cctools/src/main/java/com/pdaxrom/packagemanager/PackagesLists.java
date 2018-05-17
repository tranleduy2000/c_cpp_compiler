package com.pdaxrom.packagemanager;

import android.support.annotation.Nullable;

import java.util.List;

class PackagesLists {
    @Nullable
    private List<PackageInfo> availablePackages = null;
    @Nullable
    private List<PackageInfo> installedPackages = null;

    PackagesLists() {

    }

    PackagesLists(@Nullable List<PackageInfo> availablePackages, @Nullable List<PackageInfo> installedPackages) {
        this.availablePackages = availablePackages;
        this.installedPackages = installedPackages;
    }

    @Nullable
    List<PackageInfo> getInstalledPackages() {
        return installedPackages;
    }

    void setInstalledPackages(@Nullable List<PackageInfo> installedPackages) {
        this.installedPackages = installedPackages;
    }

    @Nullable
    List<PackageInfo> getAvailablePackages() {
        return availablePackages;
    }

    void setAvailablePackages(@Nullable List<PackageInfo> availablePackages) {
        this.availablePackages = availablePackages;
    }
}
