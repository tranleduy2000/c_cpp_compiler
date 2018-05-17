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

package com.duy.ccppcompiler.filemanager.install;

import android.Manifest;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.MainThread;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.content.ContextCompat;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.ProgressBar;
import android.widget.ScrollView;
import android.widget.TextView;

import com.duy.ccppcompiler.BuildConfig;
import com.duy.ccppcompiler.R;
import com.duy.common.DLog;
import com.duy.utils.ApkUtils;
import com.duy.utils.ExtractCallback;
import com.duy.editor.EditorActivity;

/**
 * Created by Duy on 22-Apr-18.
 */

public class InstallActivity extends AppCompatActivity {
    private static final String KEY_EXTRACTED = "KEY_EXTRACTED";
    private static final String KEY_APP_VERSION = "KEY_APP_VERSION";

    private static final String[] PERMISSIONS = new String[]{
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.WRITE_EXTERNAL_STORAGE};

    private static final String TAG = "InstallActivity";
    private ProgressBar mProgressBar;
    private TextView mTxtMessage;
    private ScrollView mScrollView;
    @Nullable
    private AsyncTask<Void, CharSequence, Boolean> mExtractDataTask;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_install);
        mProgressBar = findViewById(R.id.progress_bar);
        mTxtMessage = findViewById(R.id.txt_message);
        mScrollView = findViewById(R.id.scroll_view);

        extractData();
    }

    @Override
    protected void onStop() {
        super.onStop();
        if (mExtractDataTask != null) {
            mExtractDataTask.cancel(true);
        }
    }

    private void extractData() {
        if (DLog.DEBUG) DLog.d(TAG, "extractData() called");
        final SharedPreferences preferences = getPreferences(MODE_PRIVATE);
        final String appVersion = ApkUtils.getAppVersion(this);
        boolean extracted = preferences.getBoolean(KEY_EXTRACTED, false)
                && preferences.getString(KEY_APP_VERSION, "").equals(appVersion);
        if (DLog.DEBUG) DLog.d(TAG, "extracted = " + extracted);
        //in debug mode, always extract data
        if (BuildConfig.DEBUG) {
            extracted = false;
        }
        if (!extracted) {
            mExtractDataTask = new InstallGcc810(this, new ExtractCallback() {
                @Override
                public void onNewMessage(CharSequence message) {
                    mTxtMessage.append(message);
                    mTxtMessage.append("\n");
                    mScrollView.fullScroll(View.FOCUS_DOWN);
                }

                @Override
                public void onComplete() {
                    SharedPreferences.Editor editor = preferences.edit();
                    editor.putBoolean(KEY_EXTRACTED, true);
                    editor.putString(KEY_APP_VERSION, appVersion);
                    editor.apply();
                    mTxtMessage.append("Complete\n");
                    runOnUiThread(new Runnable() {
                        @Override
                        public void run() {
                            openEditor();
                        }
                    });
                }

                @Override
                public void onFailed(@Nullable final Exception e) {
                    if (e != null) {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                mTxtMessage.append("Error: " + e.getMessage());
                                openEditor();
                            }
                        });
                    }
                }
            });
            mExtractDataTask.execute();
        } else {
            openEditor();
        }
    }

    @MainThread
    private void openEditor() {
        if (DLog.DEBUG) DLog.d(TAG, "openEditor() called");
        if (permissionGranted()) {
            closeAndStartMainActivity();
        } else {
            requestPermission();
        }
    }

    private void closeAndStartMainActivity() {
        if (DLog.DEBUG) DLog.d(TAG, "closeAndStartMainActivity() called");
        Intent intent = new Intent(this, EditorActivity.class);
        startActivity(intent);
        overridePendingTransition(0, 0);
        finish();
    }

    private boolean permissionGranted() {
        for (String permission : PERMISSIONS) {
            if (!(ContextCompat.checkSelfPermission(this, permission) == PackageManager.PERMISSION_GRANTED)) {
                return false;
            }
        }
        return true;
    }

    private void requestPermission() {
        if (DLog.DEBUG) DLog.d(TAG, "requestPermission() called");
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            requestPermissions(PERMISSIONS, 0);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            closeAndStartMainActivity();
        } else {
            mTxtMessage.setText("Can not run this app without read/write storage permission. " +
                    "Please open setting and granted permission for this app.");
        }
    }


}
