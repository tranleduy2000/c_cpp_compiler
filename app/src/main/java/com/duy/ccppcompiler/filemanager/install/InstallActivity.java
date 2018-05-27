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
import android.support.v4.content.ContextCompat;
import android.view.View;
import android.widget.Button;
import android.widget.ProgressBar;
import android.widget.ScrollView;
import android.widget.TextView;

import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.compiler.CompileSetting;
import com.duy.ccppcompiler.compiler.compilers.GCCCompiler;
import com.duy.ccppcompiler.compiler.compilers.GPlusPlusCompiler;
import com.duy.ccppcompiler.compiler.shell.GccCommandResult;
import com.duy.ccppcompiler.packagemanager.PackageManagerActivity;
import com.duy.common.DLog;
import com.duy.editor.CodeEditorActivity;
import com.jecelyin.editor.v2.ThemeSupportActivity;

import org.apache.commons.io.IOUtils;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Created by Duy on 22-Apr-18.
 */

public class InstallActivity extends ThemeSupportActivity {

    private static final String TAG = "InstallActivity";
    private static final int RC_INSTALL_COMPILER = 12312;
    private ProgressBar mProgressBar;
    private TextView mTxtMessage;
    private ScrollView mScrollView;
    private Button mAction;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_install);
        mProgressBar = findViewById(R.id.progress_bar);
        mTxtMessage = findViewById(R.id.txt_message);
        mScrollView = findViewById(R.id.scroll_view);
        mAction = findViewById(R.id.btn_action);

        if (isPermissionGranted()) {
            new CheckCompilerInstalledTask().execute();
        } else {
            requestPermission();
        }
    }

    @Override
    protected void onStop() {
        super.onStop();
    }

    @MainThread
    private void openEditor() {
        if (DLog.DEBUG) DLog.d(TAG, "openEditor() called");
        if (isPermissionGranted()) {
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

    private boolean isPermissionGranted() {
        for (String permission : getPermissions()) {
            if (!(ContextCompat.checkSelfPermission(this, permission) == PackageManager.PERMISSION_GRANTED)) {
                return false;
            }
        }
        return true;
    }

    private String[] getPermissions() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN) {
            return new String[]{Manifest.permission.READ_EXTERNAL_STORAGE, Manifest.permission.WRITE_EXTERNAL_STORAGE};
        } else {
            return new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE};
        }
    }

    private void requestPermission() {
        if (DLog.DEBUG) DLog.d(TAG, "requestPermission() called");
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            requestPermissions(getPermissions(), 0);
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
        mTxtMessage.append("\nStart install GNU compiler");

        Intent intent = new Intent(InstallActivity.this, PackageManagerActivity.class);
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
    private class CheckCompilerInstalledTask extends AsyncTask<Void, String, Boolean> {

        @Override
        protected Boolean doInBackground(Void... voids) {
            try {
                InstallActivity context = InstallActivity.this;

                //Check C compiler
                publishProgress("Test C compiler\n");
                File file = new File(getCacheDir(), "tmp.c");
                FileOutputStream output = new FileOutputStream(file);
                IOUtils.write(/*"#include <stdio.h>\n" +*/ "int main(){ return 0; }", output);
                output.close();

                GCCCompiler compiler = new GCCCompiler(context, new CompileSetting(context));
                GccCommandResult result = compiler.compile(new File[]{file});
                if (result == null || result.getResultCode() != 0) {
                    publishProgress("Could not exec C compiler, please install compiler");
                    return false;
                }

                //Check C++ compiler
                publishProgress("Test C++ compiler\n");
                output = new FileOutputStream(file);
                IOUtils.write(/*"#include <iostream>\n" + "using namespace std;\n" +*/ "int main() { return 0; }", output);
                output.close();
                compiler = new GPlusPlusCompiler(context, new CompileSetting(context));
                result = compiler.compile(new File[]{file});
                if (result == null || result.getResultCode() != 0) {
                    publishProgress("Could not exec C++ compiler, please install compiler");
                    return false;
                }

                file.delete();
                return true;
            } catch (IOException e) {
                e.printStackTrace();
            }

            return false;
        }

        @Override
        protected void onProgressUpdate(String... values) {
            super.onProgressUpdate(values);
            mTxtMessage.append(values[0]);
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
