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

package com.duy.ccppcompiler.pkgmanager.model;

import android.support.annotation.NonNull;

import java.util.ArrayList;
import java.util.List;

public class PackagesLists {
    @NonNull
    private List<PackageInfo> availablePackages;
    @NonNull
    private List<PackageInfo> installedPackages;

    public PackagesLists() {
        availablePackages = new ArrayList<>();
        installedPackages = new ArrayList<>();
    }

    PackagesLists(@NonNull List<PackageInfo> availablePackages, @NonNull List<PackageInfo> installedPackages) {
        this.availablePackages = availablePackages;
        this.installedPackages = installedPackages;
    }

    @NonNull
    public List<PackageInfo> getInstalledPackages() {
        return installedPackages;
    }

    public void setInstalledPackages(@NonNull List<PackageInfo> installedPackages) {
        this.installedPackages = installedPackages;
    }

    @NonNull
    public List<PackageInfo> getAvailablePackages() {
        return availablePackages;
    }

    public void setAvailablePackages(@NonNull List<PackageInfo> availablePackages) {
        this.availablePackages = availablePackages;
    }
}
