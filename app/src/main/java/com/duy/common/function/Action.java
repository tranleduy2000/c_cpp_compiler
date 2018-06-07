package com.duy.common.function;

import android.support.annotation.Nullable;

public interface Action<T> {
    void execute(@Nullable T obj);
}
