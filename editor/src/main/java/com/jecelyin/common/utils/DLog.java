package com.jecelyin.common.utils;

import java.util.Arrays;


public class DLog {
    private static final String TAG = "DLog";
    public static boolean DEBUG = true;

    public static int v(String tag, String msg) {
        if (!DEBUG)
            return 0;
        return android.util.Log.v(tag, msg);
    }

    public static int v(String tag, String msg, Throwable tr) {
        if (!DEBUG)
            return 0;
        return android.util.Log.v(tag, msg, tr);
    }

    public static int d(String msg) {
        return d(TAG, msg);
    }

    public static int d(String tag, String msg) {
        if (!DEBUG)
            return 0;
        return android.util.Log.d(tag, msg);
    }

    public static int d(String tag, String msg, Throwable tr) {
        if (!DEBUG)
            return 0;
        return android.util.Log.d(tag, msg, tr);
    }

    public static int d(String format, Object... args) {
        return d(TAG, String.format(format, args));
    }

    public static int d(Throwable t) {
        return d(TAG, t.getMessage(), t);
    }

    public static int i(String tag, String msg) {
        if (!DEBUG)
            return 0;
        return android.util.Log.i(tag, msg);
    }

    public static int i(String tag, String msg, Throwable tr) {
        if (!DEBUG)
            return 0;
        return android.util.Log.i(tag, msg, tr);
    }

    public static int w(String msg) {
        return android.util.Log.w(TAG, msg);
    }

    public static int w(String tag, String msg) {
        return android.util.Log.w(tag, msg);
    }

    public static int w(String tag, String msg, Throwable tr) {
        return android.util.Log.w(tag, msg, tr);
    }

    public static int w(String tag, Throwable tr) {
        return android.util.Log.w(tag, tr);
    }

    public static int e(String tag, String msg) {
        return logError(tag, msg, null);
    }

    public static int e(String tag, String msg, Throwable tr) {
        return logError(tag, msg, tr);
    }

    public static int e(String msg) {
        return e(TAG, msg);
    }

    public static int e(String msg, Throwable t) {
        return e(TAG, msg, t);
    }

    public static int e(String format, Object... args) {
        return e(TAG, String.format(format, args));
    }

    public static int e(Throwable t) {
        if (t == null)
            return 0;
        return logError(TAG, t.getMessage(), t);
    }

    private static int logError(String tag, String msg, Throwable t) {
        return android.util.Log.e(tag, msg, t);
    }

    public static void log(Object... params) {
        if (DLog.DEBUG) DLog.d(TAG, "log: " + Arrays.toString(params));
    }
}
