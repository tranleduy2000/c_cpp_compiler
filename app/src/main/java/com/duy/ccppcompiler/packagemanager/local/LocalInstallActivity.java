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

package com.duy.ccppcompiler.packagemanager.local;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
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
import com.duy.ccppcompiler.compiler.CompileSetting;
import com.duy.ccppcompiler.compiler.compilers.GCCCompiler;
import com.duy.ccppcompiler.compiler.shell.GccCommandResult;
import com.duy.ccppcompiler.packagemanager.Environment;
import com.duy.ccppcompiler.packagemanager.PackageInstaller;
import com.duy.ccppcompiler.packagemanager.RepoParser;
import com.duy.ccppcompiler.packagemanager.RepoUtils;
import com.duy.ccppcompiler.packagemanager.model.InstallPackageInfo;
import com.duy.ccppcompiler.packagemanager.model.PackageInfo;
import com.duy.ccppcompiler.packagemanager.model.PackagesLists;
import com.duy.common.DLog;
import com.duy.editor.CodeEditorActivity;
import com.jecelyin.editor.v2.FullScreenActivity;

import org.apache.commons.compress.utils.IOUtils;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.Arrays;
import java.util.List;

/**
 * Created by Duy on 22-Apr-18.
 */

public class LocalInstallActivity extends FullScreenActivity {

    private static final String[] PERMISSIONS = new String[]{
            Manifest.permission.READ_EXTERNAL_STORAGE,
            Manifest.permission.WRITE_EXTERNAL_STORAGE};

    private static final String TAG = "InstallActivity";
    private static final int RC_INSTALL_COMPILER = 12312;
    private ProgressBar mProgressBar;
    private TextView mTxtMessage;
    private ScrollView mScrollView;
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
            installCompilerFailed();
        }
    }

    private void installCompilerFailed() {
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

    private void installCompiler() {
        new InstallCompilerTask().execute();
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
            try {
                File file = new File(getCacheDir(), "tmp.c");
                FileOutputStream output = new FileOutputStream(file);
                org.apache.commons.io.IOUtils.write("#include <stdio.h> int main(){ return 0; }", output);
                output.close();

                LocalInstallActivity context = LocalInstallActivity.this;

                GCCCompiler compiler = new GCCCompiler(context, new CompileSetting(context));
                GccCommandResult result = compiler.compile(new File[]{file});

                return result != null && result.getResultCode() == 0;
            } catch (IOException e) {
                e.printStackTrace();
            }

            return false;
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

    @SuppressLint("StaticFieldLeak")
    private class InstallCompilerTask extends AsyncTask<Void, Object, Boolean> {

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            mTxtMessage.append("Copy asset data to external storage\n");
        }

        @Override
        protected Boolean doInBackground(Void... voids) {
            RepoUtils.setVersion();
            AssetManager assets = getAssets();
            if (DLog.DEBUG) DLog.d(TAG, "doInBackground: copy data from assets");
            try {
                //extract assets data to local storage
                String[] names = assets.list("compiler");
                if (DLog.DEBUG) DLog.d(TAG, "names = " + Arrays.toString(names));
                for (String name : names) {
                    publishProgress("Copying " + " " + name + "\n");
                    if (DLog.DEBUG) DLog.d(TAG, "doInBackground: copying " + name);

                    File file = new File(Environment.getSdCardBackupDir(), name);
                    InputStream input = assets.open("compiler/" + name);
                    OutputStream output = new FileOutputStream(file);
                    IOUtils.copy(input, output);
                    output.close();
                    input.close();

                    publishProgress("Copy " + name + " completed\n");
                }

                //read list packages
                InputStream packagesFile = assets.open("compiler/Packages");
                String content = org.apache.commons.io.IOUtils.toString(packagesFile);
                packagesFile.close();

                RepoParser parser = new RepoParser();
                PackagesLists allPackages = new PackagesLists();
                allPackages.setAvailablePackages(parser.parseRepoXml(content));

                String compilerPackage = "build-essential-gcc-compact";
                InstallPackageInfo installPackageInfo = new InstallPackageInfo(allPackages, compilerPackage);
                List<PackageInfo> compilerPackagesList = installPackageInfo.getPackagesList();
                if (DLog.DEBUG) DLog.d(TAG, "compilerPackagesList = " + compilerPackagesList);

                //install packages
                PackageInstaller packageInstaller = new PackageInstaller(LocalInstallActivity.this);
                for (PackageInfo packageInfo : compilerPackagesList) {
                    File zipFile = new File(Environment.getSdCardBackupDir(), packageInfo.getFileName());
                    if (!zipFile.exists()) {
                        throw new FileNotFoundException("Can not find packages in assets data " + packageInfo);
                    }
                    publishProgress("Installing " + packageInfo.getName() + "\n");
                    packageInstaller.install(zipFile, packageInfo);
                    publishProgress("Install complete " + packageInfo.getName() + "\n");
                }

                return true;
            } catch (Exception e) {
                e.printStackTrace();
                publishProgress("Error " + e.getMessage());
            }
            return false;
        }

        @Override
        protected void onProgressUpdate(Object... values) {
            super.onProgressUpdate(values);
            mTxtMessage.append(String.valueOf(values[0]));
            mScrollView.fullScroll(View.FOCUS_DOWN);
        }

        @Override
        protected void onPostExecute(Boolean result) {
            super.onPostExecute(result);
            if (result) {
                mTxtMessage.append("Install complete\n");
                openEditor();
            } else {
                installCompiler();
            }

//            Intent intent = new Intent(InstallActivity.this, PackageManagerActivity.class);
//            intent.putExtra(PackageManagerActivity.EXTRA_CMD, PackageManagerActivity.ACTION_INSTALL);
//            intent.putExtra(PackageManagerActivity.EXTRA_DATA, "build-essential-gcc");
//            startActivityForResult(intent, RC_INSTALL_COMPILER);
        }
    }
}
