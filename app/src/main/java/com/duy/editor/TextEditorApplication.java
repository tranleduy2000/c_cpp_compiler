/*
 * Copyright (C) 2016 Jecelyin Peng <jecelyin@gmail.com>
 *
 * This file is part of 920 Text Editor.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.duy.editor;

import android.content.Context;
import android.support.multidex.MultiDexApplication;

import com.duy.common.DLog;

import java.util.Map;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class TextEditorApplication extends MultiDexApplication {
    private static final String TAG = "TextEditorApplication";
    private static Context context;

    public static Context getContext() {
        return context;
    }

    @Override
    public void onCreate() {
        super.onCreate();
        context = getApplicationContext();
        if (DLog.DEBUG) {
            Map<String, String> env = System.getenv();
            DLog.d(TAG, "onCreate: default env");
            for (Map.Entry<String, String> entry : env.entrySet()) {
                DLog.d(TAG, "entry = " + entry);
            }
        }
    }

}
