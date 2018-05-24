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

package com.duy.ccppcompiler.packagemanager;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.MainThread;
import android.support.annotation.NonNull;
import android.support.annotation.UiThread;
import android.support.annotation.WorkerThread;
import android.support.v7.widget.SearchView;
import android.support.v7.widget.Toolbar;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.TextView;

import com.duy.ccppcompiler.compiler.shell.ShellUtils;
import com.duy.ccppcompiler.packagemanager.model.InstallPackageInfo;
import com.duy.ccppcompiler.packagemanager.model.PackageInfo;
import com.duy.ccppcompiler.packagemanager.model.PackagesLists;
import com.duy.ccppcompiler.packagemanager.repo.FirebasePackageRepository;
import com.duy.ccppcompiler.packagemanager.repo.LocalPackageRepository;
import com.duy.ccppcompiler.packagemanager.repo.UbuntuServerPackageRepository;
import com.jecelyin.common.utils.DLog;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.BaseActivity;
import com.pdaxrom.cctools.R;
import com.pdaxrom.utils.Utils;

import org.apache.commons.io.IOUtils;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

@SuppressWarnings("unchecked")
public class PackageManagerActivity extends BaseActivity {
    public static final String EXTRA_CMD = "command";
    public static final String EXTRA_DATA = "data";
    public static final String ACTION_INSTALL = "install";
    public static final String ACTION_UNINSTALL = "uninstall";
    public static final String ACTION_UPDATE = "update";

    private static final String TAG = "PkgMgrActivity";
    @NonNull
    private final Handler mHandler = new Handler();
    protected String mAction = null;
    @NonNull
    protected PackagesLists mPackagesLists = new PackagesLists();
    protected LocalPackageRepository mLocalPackageRepository;
    protected UbuntuServerPackageRepository mUbuntuServerPackageRepository;
    protected FirebasePackageRepository mFirebasePackageRepository;
    private Context context = this;
    private String mIntentData = null;
    private ListView mListView;
    private ProgressDialog mProgressDialog;
    private SearchView mSearchView;
    private String mToolchainDir;
    private String mInstalledDir;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_package_manager);
        setSupportActionBar((Toolbar) findViewById(R.id.toolbar));
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        setTitle(R.string.title_menu_add_ons);

        Environment.mkdirs(this);
        mToolchainDir = Environment.getToolchainsDir(this);
        mInstalledDir = Environment.getInstalledPackageDir(this);
        RepoUtils.setVersion();
        getDataFromIntent();

        mLocalPackageRepository = new LocalPackageRepository(this, new File(mInstalledDir));
        mUbuntuServerPackageRepository = new UbuntuServerPackageRepository(getRepoUrl());
        mFirebasePackageRepository = new FirebasePackageRepository();

        mListView = getListView();
        mListView.requestFocus();
        mListView.setOnItemClickListener(new OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                // getting values from selected ListItem
                final String name = ((TextView) view.findViewById(R.id.pkg_name)).getText().toString();
                File logFile = new File(mInstalledDir, name + ".list");
                if (logFile.exists()) {
                    new AlertDialog.Builder(context)
                            .setTitle(getString(R.string.pkg_selected) + name)
                            .setMessage(getString(R.string.pkg_alreadyinstalled))
                            .setCancelable(false)
                            .setNeutralButton(getString(R.string.cancel), null)
                            .setNegativeButton(getString(R.string.pkg_uninstall), new DialogInterface.OnClickListener() {
                                public void onClick(DialogInterface dialog, int which) {
                                    new UninstallPackagesTask().execute(name);
                                }
                            })
                            .create()
                            .show();
                } else {
                    prepareForInstallPackage(name, true);
                }
            }
        });
        mListView.setTextFilterEnabled(true);

        downloadListPackages();
    }


    private void getDataFromIntent() {
        mAction = null;
        mIntentData = null;
        if (getIntent().getExtras() != null) {
            mAction = getIntent().getExtras().getString(EXTRA_CMD);
            if (ACTION_INSTALL.equals(mAction)) {
                mIntentData = getIntent().getExtras().getString(EXTRA_DATA);
            } else if (ACTION_UNINSTALL.equals(mAction)) {
                mIntentData = getIntent().getExtras().getString(EXTRA_DATA);
            }
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
    }

    @Override
    protected void onStop() {
        super.onStop();
        mFirebasePackageRepository.destroy();
        mUbuntuServerPackageRepository.destroy();
    }

    private ListView getListView() {
        return findViewById(R.id.list_view);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_package_manager, menu);

        // Retrieve the SearchView and plug it into SearchManager
        mSearchView = (SearchView) menu.findItem(R.id.action_search).getActionView();
        mSearchView.setOnQueryTextListener(new SearchView.OnQueryTextListener() {
            @Override
            public boolean onQueryTextSubmit(String query) {
                return false;
            }

            @Override
            public boolean onQueryTextChange(String newText) {
                if (newText.length() > 0) {
                    mListView.setFilterText(newText);
                } else {
                    mListView.clearTextFilter();
                }
                return false;
            }
        });
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int i = item.getItemId();
        if (i == R.id.action_update) {
            downloadListPackages();

        } else if (i == R.id.action_repo_mirrors) {
            editReposList();
        } else if (i == android.R.id.home) {
            if (!mSearchView.isIconified()) {
                mSearchView.setIconified(true);
                return true;
            }
        }
        return super.onOptionsItemSelected(item);
    }


    @UiThread
    protected void showPackages(List<PackageInfo> repo) {
        ArrayList<HashMap<String, String>> menuItems = new ArrayList<>();
        final String[] hidePackages = new String[]{
                "acer-a200-ics403-libc-fix",
                "android-pre-233-libc-fix",
                /*   "binutils-compact",
                   "build-essential-clang-compact",
                   "build-essential-clang-objc-compact",
                   "build-essential-fortran-compact",
                   "build-essential-gcc-compact",
                   "build-essential-gcc-objc-compact",
                   "build-essential-gcc-objc-fortran-compact",
                   "build-essential-mingw-w64",
                   "cctools-examples",
                   "cppcheck-module",
                   "gcc-mingw-w64-i686",
                   "gfortran-examples",
                   "binutils-mingw-w64-i686",
                   "mingw-w64-i686-dev",
                   "mingw-w64-examples",*/
        };
        for (PackageInfo info : repo) {
            boolean shouldShow = true;
            for (String hidePackage : hidePackages) {
                if (info.getName().contentEquals(hidePackage)) {
                    shouldShow = false;
                }
            }
            if (!shouldShow) {
                continue;
            }

            HashMap<String, String> map = new HashMap<>();

            map.put(RepoParser.KEY_PACKAGE_NAME, info.getName());
            map.put(RepoParser.KEY_VERSION, info.getVersion());
            map.put(RepoParser.KEY_DESC, info.getDescription());
            map.put(RepoParser.KEY_DEPENDS, info.getDepends());
            map.put(RepoParser.KEY_FILESIZE, Utils.humanReadableByteCount(info.getFileSize(), false));
            map.put(RepoParser.KEY_SIZE, Utils.humanReadableByteCount(info.getSize(), false));
            map.put(RepoParser.KEY_LOCAL_FILE_NAME, info.getFileName());

            File logFile = new File(mInstalledDir, info.getName() + ".list");
            if (logFile.exists()) {
                map.put(RepoParser.KEY_STATUS, getString(R.string.pkg_installed));
            } else {
                map.put(RepoParser.KEY_STATUS, getString(R.string.pkg_notinstalled));
            }

            // adding HashList to ArrayList
            menuItems.add(map);
        }

        // Adding menuItems to ListView
        ListAdapter adapter = new SimpleAdapter(
                this,
                menuItems,
                R.layout.list_item_package,
                new String[]{
                        RepoParser.KEY_PACKAGE_NAME,
                        RepoParser.KEY_VERSION,
                        RepoParser.KEY_DESC,
                        RepoParser.KEY_DEPENDS,
                        RepoParser.KEY_LOCAL_FILE_NAME,
                        RepoParser.KEY_FILESIZE,
                        RepoParser.KEY_SIZE,
                        RepoParser.KEY_STATUS},
                new int[]{
                        R.id.pkg_name,
                        R.id.pkg_version,
                        R.id.pkg_desciption,
                        R.id.pkg_deps,
                        R.id.pkg_file,
                        R.id.pkg_filesize,
                        R.id.pkg_size,
                        R.id.pkg_status});
        ListView listView = getListView();
        int selectedItemPosition = listView.getFirstVisiblePosition();
        listView.setAdapter(adapter);
        if (selectedItemPosition >= 0 && selectedItemPosition < adapter.getCount()) {
            listView.setSelection(selectedItemPosition);
        }
    }

    @UiThread
    void showProgress(int title, int message) {
        showProgress(title, message, false);
    }

    @UiThread
    private void showProgress(int title, int message, boolean intermediate) {
        mProgressDialog = new ProgressDialog(context);
        mProgressDialog.setTitle(getString(title));
        mProgressDialog.setMessage(getString(message));
        if (!intermediate) {
            mProgressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
        }
        mProgressDialog.setCancelable(false);
        mProgressDialog.show();
    }

    @UiThread
    protected void hideProgress() {
        if (mProgressDialog != null && mProgressDialog.isShowing()) {
            mProgressDialog.dismiss();
        }
    }

    @UiThread
    protected void updateProgress(final int progress) {
        mProgressDialog.setProgress(progress);
    }

    @UiThread
    protected void updateMessage(final String progress) {
        mProgressDialog.setMessage(progress);
    }

    @UiThread
    protected void updateProgressTitle(final String out) {
        mProgressDialog.setTitle(out);
    }

    @UiThread
    protected void showError(final String message) {
        Builder builder = new Builder(this);
        builder.setTitle(R.string.err)
                .setMessage(message)
                .setNegativeButton(getString(android.R.string.cancel), new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        if (mAction != null) {
                            setResult(RESULT_CANCELED);
                            finish();
                        }
                    }
                })
                .setPositiveButton(R.string.pkg_reinstall, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.dismiss();
                        downloadListPackages();
                    }
                })
                .setCancelable(false);
        builder.create().show();
    }

    /**
     * Uninstall a package and all dependencies
     *
     * @param name - name of package
     * @return {@code true} if uninstall success
     */
    @WorkerThread
    protected boolean uninstallPackage(String name) {
        if (name != null) {
            //delete file
            File permFile = new File(mInstalledDir, name + ".prerm");
            if (permFile.exists()) {
                Utils.chmod(permFile.getAbsolutePath(), 0x1ed);
                ShellUtils.exec(this, permFile);
                permFile.delete();
            }

            File descFile = new File(mInstalledDir, name + ".pkgdesc");
            if (descFile.exists()) {
                descFile.delete();
            }

            //remove package in list installed
            String logFile = new File(mInstalledDir, name + ".list").getAbsolutePath();
            if (!(new File(logFile)).exists()) {
                return false;
            }

            try {
                FileInputStream fin = new FileInputStream(logFile);
                DataInputStream in = new DataInputStream(fin);
                BufferedReader reader = new BufferedReader(new InputStreamReader(in));
                String line;
                while ((line = reader.readLine()) != null) {
                    Log.i(TAG, "Delete file: " + line);
                    new File(mToolchainDir, line).delete();
                }
                in.close();
                new File(logFile).delete();
            } catch (Exception e) {
                e.printStackTrace();
                Log.e(TAG, "Error during remove files " + e);
            }
            updateInstalledPackages();
        }
        return true;
    }

    private String getRepoUrl() {
        String defaultUrl;
        if (Build.VERSION.SDK_INT >= 21) {
            defaultUrl = "http://cctools.info/packages-pie";
        } else {
            defaultUrl = "http://cctools.info/packages";
        }
        defaultUrl += "/" + RepoUtils.CPU_API;
        return defaultUrl;
    }

    @MainThread
    private void editReposList() {
        String url = getRepoUrl();
        UIUtils.showInputDialog(this, getString(R.string.title_repo_list), "", url,
                0, new UIUtils.OnShowInputCallback() {
                    @Override
                    public void onConfirm(CharSequence input) {
                        String reposListFile = mToolchainDir + "/cctools/etc/repos.list";
                        try {
                            FileOutputStream fos = new FileOutputStream(reposListFile);
                            fos.write(input.toString().getBytes());
                            fos.close();
                        } catch (Exception e) {
                            e.printStackTrace();
                        }
                    }
                });
    }

    protected void updateInstalledPackages() {
        //local packages
        List<PackageInfo> installedPackages = mLocalPackageRepository.getInstalledPackages();
        mPackagesLists.setInstalledPackages(installedPackages);
    }

    private void downloadListPackages() {
        updateInstalledPackages();

        //local data
        try {
            InputStream packagesFile = getAssets().open("repo/Packages");
            String content = IOUtils.toString(packagesFile);
            RepoParser parser = new RepoParser();
            List<PackageInfo> packageInfos = parser.parseRepoXml(content);
            downloadPackagesComplete(packageInfos);
            return;
        } catch (IOException e) {
            e.printStackTrace();
        }

        showProgress(R.string.title_repo_update, R.string.message_package_downloading, true);
        //using firebase server
        mFirebasePackageRepository.getPackagesInBackground(new IPackageLoadListener() {
            @Override
            public void onSuccess(List<PackageInfo> packages) {
                if (DLog.DEBUG)
                    DLog.d(TAG, "mFirebasePackageRepository onSuccess() called with: packages = [" + packages + "]");
                downloadPackagesComplete(packages);
            }

            @Override
            public void onFailed(@NonNull Exception e) {
                if (DLog.DEBUG)
                    DLog.e(TAG, "mFirebasePackageRepository onFailed: Firebase not available", e);
                mUbuntuServerPackageRepository.getPackagesInBackground(new IPackageLoadListener() {
                    @Override
                    public void onSuccess(List<PackageInfo> packages) {
                        downloadPackagesComplete(packages);
                    }

                    @Override
                    public void onFailed(@NonNull Exception e) {
                        downloadListPackagesFailed(e);
                    }
                });

            }
        });
    }

    /**
     * Called when can not load list packages
     */
    private void downloadListPackagesFailed(Exception e) {
        hideProgress();
        showError(getString(R.string.message_package_repo_unavailable) + "\n" + e.getMessage());
    }

    /**
     * Called when list packages is loaded
     */
    private void downloadPackagesComplete(List<PackageInfo> packages) {
        mPackagesLists.setAvailablePackages(packages);

        //display to UI
        hideProgress();
        showPackages(mPackagesLists.getAvailablePackages());

        if (ACTION_INSTALL.equals(mAction)) {
            if (!mPackagesLists.getAvailablePackages().isEmpty()) {
                prepareForInstallPackage(mIntentData, false);
            } else {
                showError(getString(R.string.message_package_repo_unavailable) + "\n" +
                        getString(R.string.message_package_for_install) +
                        mIntentData);
            }
        } else if (ACTION_UNINSTALL.equals(mAction)) {
            new UninstallPackagesTask().execute(mIntentData);

        } else {

            List<PackageInfo> needUpdatePackages = RepoUtils.checkingForUpdates(mPackagesLists);
            if (!needUpdatePackages.isEmpty()) {
                final InstallPackageInfo updateInfo = new InstallPackageInfo();
                for (PackageInfo pkg : needUpdatePackages) {
                    updateInfo.addPackage(mPackagesLists, pkg.getName());
                }
                Log.i(TAG, "update list = " + updateInfo.getPackagesStrings());
                Builder dialog = new Builder(context)
                        .setIcon(android.R.drawable.ic_dialog_alert)
                        .setTitle(getString(R.string.pkg_selectedforupdate))
                        .setMessage(getString(R.string.pkg_selectedforupdate1) + updateInfo.getPackagesStrings()
                                + "\n\n"
                                + getString(R.string.pkg_selected2)
                                + Utils.humanReadableByteCount(updateInfo.getDownloadSize(), false)
                                + "\u0020"
                                + getString(R.string.pkg_selected3)
                                + Utils.humanReadableByteCount(updateInfo.getInstallSize(), false))
                        .setCancelable(false)
                        .setNeutralButton(getString(R.string.cancel), new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int which) {
                                if (mAction != null && mAction.equals(ACTION_UPDATE)) {
                                    setResult(RESULT_CANCELED);
                                    finish();
                                }
                            }
                        })
                        .setPositiveButton(getString(R.string.pkg_install), new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int which) {
                                Log.i(TAG, "Get install packages = " + updateInfo.getPackagesStrings());
                                (new InstallPackagesTask(PackageManagerActivity.this)).execute(updateInfo);
                            }
                        });
                dialog.show();
            }

            if (needUpdatePackages.isEmpty() && ACTION_UPDATE.equals(mAction)) {
                mProgressDialog.dismiss();
                setResult(RESULT_OK);
                finish();
            }
        }
    }

    private void prepareForInstallPackage(String name, boolean confirmBeforeInstall) {
        final InstallPackageInfo info = new InstallPackageInfo(mPackagesLists, name);
        if (confirmBeforeInstall) {
            Builder dialog = new AlertDialog.Builder(context)
                    .setTitle(getString(R.string.pkg_selected) + info.getName())
                    .setMessage(getString(R.string.pkg_selected1) + info.getPackagesStrings()
                            + "\n\n"
                            + getString(R.string.pkg_selected2)
                            + Utils.humanReadableByteCount(info.getDownloadSize(), false)
                            + "\u0020"
                            + getString(R.string.pkg_selected3)
                            + Utils.humanReadableByteCount(info.getInstallSize(), false))
                    .setCancelable(false)
                    .setNeutralButton(getString(R.string.cancel), new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int which) {
                            if (mAction != null) {
                                setResult(RESULT_CANCELED);
                                finish();
                            }
                        }
                    })
                    .setPositiveButton(getString(R.string.pkg_install), new DialogInterface.OnClickListener() {
                        public void onClick(DialogInterface dialog, int which) {
                            Log.i(TAG, "Get install packages = " + info.getPackagesStrings());
                            new InstallPackagesTask(PackageManagerActivity.this).execute(info);
                        }
                    });
            dialog.show();
        } else {
            new InstallPackagesTask(PackageManagerActivity.this).execute(info);
        }
    }


    /**
     * Uninstall list of packages
     */
    @SuppressLint("StaticFieldLeak")
    private class UninstallPackagesTask extends AsyncTask<String, Void, Boolean> {
        protected void onPreExecute() {
            super.onPreExecute();
            showProgress(R.string.title_package_uninstall, R.string.message_package_uninstall);
        }

        @Override
        protected Boolean doInBackground(String... params) {
            return uninstallPackage(params[0]);
        }

        protected void onPostExecute(Boolean result) {
            hideProgress();
            if (ACTION_UNINSTALL.equals(mAction)) {
                setResult(RESULT_OK);
                finish();
                return;
            }
            showPackages(mPackagesLists.getAvailablePackages());
        }
    }
}
