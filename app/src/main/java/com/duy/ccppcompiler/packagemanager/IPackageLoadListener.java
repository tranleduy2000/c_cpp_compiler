package com.duy.ccppcompiler.packagemanager;

import android.support.annotation.NonNull;

import com.duy.ccppcompiler.packagemanager.model.PackageInfo;

import java.util.List;

public interface IPackageLoadListener {
    void onSuccess(List<PackageInfo> packages);

    void onFailed(@NonNull Exception e);
}
