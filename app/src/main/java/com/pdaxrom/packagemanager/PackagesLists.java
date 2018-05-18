package com.pdaxrom.packagemanager;

import android.support.annotation.NonNull;

import java.util.ArrayList;
import java.util.List;

class PackagesLists {
    @NonNull
    private List<PackageInfo> availablePackages;
    @NonNull
    private List<PackageInfo> installedPackages;

    PackagesLists() {
        availablePackages = new ArrayList<>();
        installedPackages = new ArrayList<>();
    }

    PackagesLists(@NonNull List<PackageInfo> availablePackages, @NonNull List<PackageInfo> installedPackages) {
        this.availablePackages = availablePackages;
        this.installedPackages = installedPackages;
    }

    @NonNull
    List<PackageInfo> getInstalledPackages() {
        return installedPackages;
    }

    void setInstalledPackages(@NonNull List<PackageInfo> installedPackages) {
        this.installedPackages = installedPackages;
    }

    @NonNull
    List<PackageInfo> getAvailablePackages() {
        return availablePackages;
    }

    void setAvailablePackages(@NonNull List<PackageInfo> availablePackages) {
        this.availablePackages = availablePackages;
    }
}
