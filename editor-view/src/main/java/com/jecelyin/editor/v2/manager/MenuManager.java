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

package com.jecelyin.editor.v2.manager;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.drawable.Drawable;
import android.support.v4.content.ContextCompat;

import com.duy.ide.editor.editor.R;
import com.jecelyin.common.utils.DrawableUtils;
import com.jecelyin.editor.v2.adapter.MainMenuAdapter;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class MenuManager {
    private static int toolbarIconNormalColor;
    private static int toolbarIconDisabledColor;
    private static int menuIconNormalColor;

    private MainMenuAdapter mAdapter;

    public MenuManager(Context context) {
        mAdapter = new MainMenuAdapter(context);
    }

    public MainMenuAdapter getAdapter() {
        return mAdapter;
    }

    public static void init(Context context) {
        int[] attrs = new int[]{
                R.attr.toolbarIconNormalColor,
                R.attr.toolbarIconDisabledColor,
                R.attr.menuIconNormalColor,
        };
        TypedArray a = context.obtainStyledAttributes(attrs);
        toolbarIconNormalColor = a.getColor(0, 0);
        toolbarIconDisabledColor = a.getColor(1, 0);
        menuIconNormalColor = a.getColor(2, 0);
        a.recycle();
    }

    public static Drawable makeToolbarNormalIcon(Context context, int resId) {
        Drawable d = ContextCompat.getDrawable(context, resId);
        return DrawableUtils.tintDrawable(d, toolbarIconNormalColor);
    }

    public static Drawable makeToolbarNormalIcon(Drawable d) {
        return DrawableUtils.tintDrawable(d, toolbarIconNormalColor);
    }

    public static Drawable makeToolbarDisabledIcon(Drawable d) {
        return DrawableUtils.tintDrawable(d, toolbarIconDisabledColor);
    }

    public static Drawable makeMenuNormalIcon(Context context, int resId) {
        Drawable d = ContextCompat.getDrawable(context, resId);
        return DrawableUtils.tintDrawable(d, menuIconNormalColor);
    }

    public static Drawable makeMenuNormalIcon(Drawable d) {
        return DrawableUtils.tintDrawable(d, menuIconNormalColor);
    }
}
