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

package com.jecelyin.editor.v2;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.support.annotation.CallSuper;
import android.support.annotation.Nullable;
import android.support.annotation.StyleRes;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.app.AppCompatDelegate;
import android.view.MenuItem;
import android.view.View;
import android.view.WindowManager;

import com.duy.ide.core.ITheme;
import com.duy.ide.editor.editor.R;
import com.jecelyin.common.utils.DLog;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public abstract class ThemeSupportActivity extends AppCompatActivity
        implements SharedPreferences.OnSharedPreferenceChangeListener, ITheme {
    private static final String TAG = "BaseActivity";


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        AppCompatDelegate.setCompatVectorFromResourcesEnabled(true);
        setTheme(getThemeId());
        setFullScreenMode(isFullScreenMode());
    }

    @Override
    @StyleRes
    public int getThemeId() {
        return Preferences.getInstance(this).getAppTheme();
    }

    protected boolean isFullScreenMode() {
        return Preferences.getInstance(this).isFullScreenMode();
    }

    private void setFullScreenMode(boolean fullScreenMode) {
        if (fullScreenMode) {
            getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
                    WindowManager.LayoutParams.FLAG_FULLSCREEN);
            View decorView = getWindow().getDecorView();
            // Hide the status bar.
            if (android.os.Build.VERSION.SDK_INT >= android.os.Build.VERSION_CODES.JELLY_BEAN) {
                int uiOptions = View.SYSTEM_UI_FLAG_FULLSCREEN;
                decorView.setSystemUiVisibility(uiOptions);
            }
        } else {
            getWindow().clearFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);
        }
    }

    @Override
    protected void onStart() {
        super.onStart();
        PreferenceManager.getDefaultSharedPreferences(this).registerOnSharedPreferenceChangeListener(this);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                onBackPressed();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    @Override
    @CallSuper
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {
        if (key.equals(getString(R.string.pref_key_fullscreen))) {
            setFullScreenMode(isFullScreenMode());
        } else if (key.equals(getString(R.string.pref_app_theme))) {
            if (DLog.DEBUG) DLog.d(TAG, "onSharedPreferenceChanged: change theme");
        }
    }

    @Override
    protected void onStop() {
        super.onStop();
        PreferenceManager.getDefaultSharedPreferences(this).unregisterOnSharedPreferenceChangeListener(this);
    }

}
