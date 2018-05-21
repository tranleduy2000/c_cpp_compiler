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
import android.annotation.SuppressLint;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.support.annotation.MainThread;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.annotation.VisibleForTesting;
import android.support.v4.content.ContextCompat;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.ScrollView;
import android.widget.TextView;

import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.compiler.shell.CommandResult;
import com.duy.ccppcompiler.compiler.shell.ShellUtils;
import com.duy.common.DLog;
import com.duy.editor.CodeEditorActivity;
import com.jecelyin.editor.v2.FullScreenActivity;
import com.duy.ccppcompiler.packagemanager.Environment;
import com.duy.ccppcompiler.packagemanager.PackageManagerActivity;

/**
 * Created by Duy on 22-Apr-18.
 */

public class InstallActivity extends FullScreenActivity {

    private static final String[] PERMISSIONS = new String[]{
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.WRITE_EXTERNAL_STORAGE};

    private static final String TAG = "InstallActivity";
    private static final int RC_INSTALL_COMPILER = 12312;
    private ProgressBar mProgressBar;
    private TextView mTxtMessage;
    private ScrollView mScrollView;
    @Nullable
    private AsyncTask<Void, CharSequence, Boolean> mExtractDataTask;
    private Button mAction;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_install);
        mProgressBar = findViewById(R.id.progress_bar);
        mTxtMessage = findViewById(R.id.txt_message);
        mScrollView = findViewById(R.id.scroll_view);
        mAction = findViewById(R.id.btn_action);

        if (permissionGranted()) {
            new CheckCompilerInstalledTask().execute();
        } else {
            requestPermission();
        }
    }

    @Override
    protected void onStop() {
        super.onStop();
        if (mExtractDataTask != null) {
            mExtractDataTask.cancel(true);
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
        Intent intent = new Intent(this, CodeEditorActivity.class);
        startActivity(intent);
        overridePendingTransition(0, 0);
        finish();
    }

    @VisibleForTesting(otherwise = VisibleForTesting.PROTECTED)
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
            new CheckCompilerInstalledTask().execute();
        } else {
            mTxtMessage.setText(R.string.message_need_permission);
            mAction.setVisibility(View.VISIBLE);
            mAction.setText(R.string.grant_permission);
            mAction.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    requestPermission();
                }
            });
        }
    }

    private void installCompiler() {
       /* final String[] toolchainPackage = {
                "build-essential-clang-compact",
                "build-essential-gcc-compact",
                "build-essential-fortran-compact",
                "build-essential-gcc-avr",
                "build-essential-mingw-w64",
                "build-essential-luajit"
        };*/

        Intent intent = new Intent(this, PackageManagerActivity.class);
        intent.putExtra(PackageManagerActivity.EXTRA_CMD, PackageManagerActivity.ACTION_INSTALL);
        intent.putExtra(PackageManagerActivity.EXTRA_DATA, "build-essential-gcc-compact");
        startActivityForResult(intent, RC_INSTALL_COMPILER);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (requestCode == RC_INSTALL_COMPILER) {
            if (resultCode == RESULT_OK) {
                openEditor();
            } else {
                mTxtMessage.post(new Runnable() {
                    @Override
                    public void run() {
                        mTxtMessage.setText(R.string.failed_to_install_compiler);
                        mAction.setVisibility(View.VISIBLE);
                        mAction.setText(R.string.reinstall_compiler);
                        mAction.setOnClickListener(new View.OnClickListener() {
                            @Override
                            public void onClick(View v) {
                                new CheckCompilerInstalledTask().execute();
                            }
                        });
                    }
                });

            }
        }
    }

    @SuppressLint("StaticFieldLeak")
    private class CheckCompilerInstalledTask extends AsyncTask<Void, Void, Boolean> {

        @Override
        protected Boolean doInBackground(Void... voids) {
            InstallActivity context = InstallActivity.this;
            CommandResult result = ShellUtils.execCommand(context, Environment.getHomeDir(context), "gcc-4.9 -v");
            if (DLog.DEBUG) DLog.d(TAG, "result = " + result);
            return result != null && result.getResultCode() == 0;
        }

        @Override
        protected void onPostExecute(Boolean aBoolean) {
            super.onPostExecute(aBoolean);
            if (aBoolean) {
                openEditor();
            } else {
                installCompiler();
            }
        }
    }
}
