/*
 * Copyright (C) 2018 Tran Le Duy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package com.duy.common;

import android.support.annotation.NonNull;
import android.util.Log;

import com.duy.ide.editor.editor.BuildConfig;


/**
 * Android logcat
 * Setup Crashlytics https://firebase.google.com/docs/crashlytics/get-started?authuser=0
 */
public class DLog {
    /**
     * Show log
     */
    public static final boolean DEBUG;
    private static final String TAG = "DLog";

    /**
     * Android environment
     */
    public static boolean ANDROID = true;

    static {
        DEBUG = BuildConfig.DEBUG;
    }

    /**
     * Debug log
     * <p>
     * -
     */
    public static void d(Object msg) {
        if (DEBUG) {
            if (ANDROID) {
                Log.d(TAG, msg.toString());
            } else {
                System.out.println(TAG + ": " + msg.toString());
            }
        }
    }

    /**
     * debug log
     */
    public static void d(String TAG, Object message) {
        if (DEBUG) {
            if (ANDROID) {
                Log.d(TAG, message.toString());
            } else {
                System.out.println(TAG + ": " + message.toString());
            }
        }
    }

    public static void d(String tag, String message, Throwable throwable) {
        if (DEBUG) {
            if (ANDROID) {
                Log.d(tag, message, throwable);
            } else {
                System.out.println(TAG + ": " + message);
            }
        }
    }

    /**
     * warning log
     */
    public static void w(Object msg) {
        if (DEBUG) {
            if (ANDROID) {
                Log.w(TAG, msg.toString());
            } else {
                System.out.println(TAG + ": " + msg.toString());
            }
        }
    }

    /**
     * warning log
     */
    public static void w(String TAG, Object msg) {
        if (DEBUG) {
            if (ANDROID) {
                Log.w(TAG, msg.toString());
            } else {
                System.out.println(TAG + ": " + msg.toString());
            }
        }
    }

    /**
     * warning log
     */
    public static void w(String TAG, Object msg, Throwable e) {
        if (DEBUG) {
            if (ANDROID) {
                Log.w(TAG, msg.toString(), e);
            } else {
                System.out.println(TAG + ": " + msg.toString());
                e.printStackTrace();
            }
        }
    }

    /**
     * Error log
     */
    public static void e(Throwable exception) {
        if (DEBUG) {
            if (ANDROID) {
                Log.e(TAG, "Error ", exception);
            } else {
                System.err.println(TAG + ": " + exception.getMessage());
                exception.printStackTrace();
            }
        }
    }

    /**
     * Error log
     */
    public static void e(String TAG, Throwable e) {
        if (DEBUG) {
            if (ANDROID) {
                Log.e(TAG, "Error ", e);
            } else {
                System.err.println(TAG + ": " + e.getMessage());
                e.printStackTrace();
            }
        }
    }

    /**
     * error log
     */
    public static void e(@NonNull String TAG, @NonNull String exception) {
        if (DEBUG) {
            if (ANDROID) {
                Log.e(TAG, exception);
            } else {
                System.err.println(TAG + ": " + exception);
            }
        }
    }

    /**
     * Error log
     */
    public static void e(@NonNull String TAG, @NonNull String msg, @NonNull Throwable e) {
        if (DEBUG) {
            if (ANDROID) {
                Log.e(TAG, msg, e);
            } else {
                System.err.println(TAG + ": " + msg);
                e.printStackTrace();
            }
        }
    }

    /**
     * info log
     */
    public static void i(@NonNull Object msg) {
        if (DEBUG) {
            if (ANDROID) {
                Log.i(TAG, msg.toString());
            } else {
                System.out.println(TAG + ": " + msg.toString());
            }
        }
    }

    /**
     * info log
     */
    public static void i(String tag, @NonNull Object msg) {
        if (DEBUG) {
            if (ANDROID) {
                Log.i(tag, msg.toString());
            } else {
                System.err.println(tag + ": " + msg);
            }
        }
    }

    public static void reportException(@NonNull Throwable e) {
        if (!DEBUG) {
//            Crashlytics.logException(e);
        }
    }

    /**
     * Report an error to firebase server
     *
     * @param throwable - any error
     */
    @Deprecated
    public static void reportServer(Throwable throwable) {
        if (ANDROID) {
//            Crashlytics.logException(throwable);
        } else {
            System.err.println("Fatal exception : ");
            throwable.printStackTrace();
        }
    }


}
