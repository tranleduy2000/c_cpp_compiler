package com.duy.ccppcompiler.packagemanager.repo;

public abstract class PackageRepositoryImpl implements IPackageRepository {
    protected boolean destroy;

    public PackageRepositoryImpl() {
        destroy = false;
    }

    @Override
    public void destroy() {
        destroy = true;
    }

    public boolean isDestroy() {
        return destroy;
    }
}
