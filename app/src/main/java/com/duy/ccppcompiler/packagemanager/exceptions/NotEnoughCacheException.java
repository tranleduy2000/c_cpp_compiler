package com.duy.ccppcompiler.packagemanager.exceptions;

public class NotEnoughCacheException extends Exception {
    private final int needMem;
    private final long cacheAvailSize;

    public NotEnoughCacheException(int needMem, long cacheAvailSize) {
        this.needMem = needMem;
        this.cacheAvailSize = cacheAvailSize;
    }

    @Override
    public String getMessage() {
        return super.getMessage();
    }

    public long getCacheAvailSize() {
        return cacheAvailSize;
    }

    public int getNeedMem() {
        return needMem;
    }
}
