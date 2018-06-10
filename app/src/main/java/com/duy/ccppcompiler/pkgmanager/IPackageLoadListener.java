package com.duy.ccppcompiler.pkgmanager;

import android.support.annotation.NonNull;

import com.duy.ccppcompiler.pkgmanager.model.PackageInfo;

import java.util.List;

public interface IPackageLoadListener {
    void onSuccess(List<PackageInfo> packages);

    void onFailed(@NonNull Exception e);
}
