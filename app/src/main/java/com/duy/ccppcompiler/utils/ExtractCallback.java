package com.duy.ccppcompiler.utils;

import android.support.annotation.MainThread;
import android.support.annotation.Nullable;

public interface ExtractCallback {
    @MainThread
    void onNewMessage(CharSequence message);

    @MainThread
    void onComplete();

    @MainThread
    void onFailed(@Nullable Exception e);
}