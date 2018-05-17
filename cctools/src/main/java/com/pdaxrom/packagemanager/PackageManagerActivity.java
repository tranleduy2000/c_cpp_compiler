package com.pdaxrom.packagemanager;

import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.StatFs;
import android.support.annotation.MainThread;
import android.support.annotation.NonNull;
import android.support.annotation.UiThread;
import android.support.annotation.WorkerThread;
import android.support.v7.widget.Toolbar;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.Log;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.EditText;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.TextView;

import com.jecelyin.common.utils.DLog;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.FullScreenActivity;
import com.pdaxrom.cctools.R;
import com.pdaxrom.utils.Utils;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;

@SuppressWarnings("unchecked")
public class PackageManagerActivity extends FullScreenActivity {
    public static final String EXTRA_CMD = "command";
    public static final String EXTRA_DATA = "data";
    public static final String ACTION_INSTALL = "install";
    public static final String ACTION_UNINSTALL = "uninstall";
    public static final String ACTION_UPDATE = "update";

    public static final int SDK_2_NDK_ARM[] = {
            /*   1   2   3   4   5   6   7   8   9  10  a11  12  13  14  15  16  17  18  19  20  21  22  23  */
            -1, -1, -1, 3, 4, 5, 5, 5, 8, 9, 9, 9, 9, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, -1
    };
    public static final int SDK_2_NDK_MIPS[] = {
            /*   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20 */
            -1, -1, -1, -1, -1, -1, -1, -1, -1, 9, 9, -1, -1, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, -1
    };
    public static final int SDK_2_NDK_X_86[] = {
            /*   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20 */
            -1, -1, -1, -1, -1, -1, -1, -1, -1, 9, 9, -1, -1, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, -1
    };

    private static final String TAG = "PkgMgrActivity";
    @NonNull
    private final Handler mHandler = new Handler();
    private String errorString = null;
    private Context context = this;
    @NonNull
    private PackagesLists mPackagesLists = new PackagesLists();
    private String mAction = null;
    private String mActivityData = null;
    // Last list position
    private int mLastPosition = 0;
    private ListView mListView;
    private String mSdCardDir;
    private String mBackupDir;
    private String mToolchainDir;
    private ProgressDialog mProgressDialog;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_package_manager);
        setSupportActionBar((Toolbar) findViewById(R.id.toolbar));
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        setTitle(R.string.title_menu_add_ons);

        setupDirs();
        setupVersion();

        mListView = getListView();
        mListView.requestFocus();

        mAction = null;
        mActivityData = null;
        if (getIntent().getExtras() != null) {
            String cmd = getIntent().getExtras().getString(EXTRA_CMD);

            Log.i(TAG, "External command " + cmd);

            if (ACTION_UPDATE.equals(cmd)) {
                mAction = cmd;
            } else if (ACTION_INSTALL.equals(cmd)) {
                mAction = cmd;
                mActivityData = getIntent().getExtras().getString(EXTRA_DATA);
            } else if (ACTION_UNINSTALL.equals(cmd)) {
                mAction = cmd;
                mActivityData = getIntent().getExtras().getString(EXTRA_DATA);
            }

            new DownloadRepoTask().execute(getReposList());
            return;
        } else {
            EditText inputSearch = findViewById(R.id.inputSearch);
            inputSearch.addTextChangedListener(new TextWatcher() {
                @Override
                public void afterTextChanged(Editable s) {
                }

                @Override
                public void beforeTextChanged(CharSequence s, int start, int count, int after) {

                }

                @Override
                public void onTextChanged(CharSequence s, int start, int before,
                                          int count) {
                    if (start > 0) {
                        mListView.setFilterText(s.toString());
                    } else {
                        mListView.clearTextFilter();
                    }
                }
            });
            new DownloadRepoTask().execute(getReposList());
        }

        mListView.setOnItemClickListener(new OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                // getting values from selected ListItem
                final String name = ((TextView) view.findViewById(R.id.pkg_name)).getText().toString();
                File logFile = new File(EnvironmentPath.getInstalledPackageDir(context), name + ".list");
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
                    new PrepareToInstallTask().execute(name);
                }
            }
        });
        mListView.setTextFilterEnabled(true);
    }

    @Override
    protected void onPause() {
        super.onPause();
        mLastPosition = this.getListView().getFirstVisiblePosition();
    }

    private ListView getListView() {
        return findViewById(R.id.list_view);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_package_manager, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int i = item.getItemId();
        if (i == R.id.action_update) {
            (new DownloadRepoTask()).execute(getReposList());

        } else if (i == R.id.action_repo_mirrors) {
            editReposList();
        }
        return super.onOptionsItemSelected(item);
    }

    private void setupDirs() {
        mSdCardDir = EnvironmentPath.getSdCardHomeDir();
        mBackupDir = EnvironmentPath.getSdCardBackupDir();
        mToolchainDir = EnvironmentPath.getToolchainsDir(this);

        //create of not exist
        EnvironmentPath.getSdCardTmpDir();
        EnvironmentPath.getServiceDir(this);
        EnvironmentPath.getHomeDir(this);
    }

    /**
     * Define sdk and ndk for current device
     */
    private void setupVersion() {
        int sdkVersion = Build.VERSION.SDK_INT;
        int ndkVersion;
        String ndkArch;
        if (Build.CPU_ABI.startsWith("arm")) {
            ndkArch = "armel";
            if (SDK_2_NDK_ARM.length > sdkVersion) {
                ndkVersion = SDK_2_NDK_ARM[sdkVersion];
            } else {
                ndkVersion = SDK_2_NDK_ARM[SDK_2_NDK_ARM.length - 1];
            }
        } else if (Build.CPU_ABI.startsWith("mips")) {
            ndkArch = "mipsel";
            if (SDK_2_NDK_MIPS.length > sdkVersion) {
                ndkVersion = SDK_2_NDK_MIPS[sdkVersion];
            } else {
                ndkVersion = SDK_2_NDK_MIPS[SDK_2_NDK_MIPS.length - 1];
            }
        } else {
            ndkArch = "i686";
            if (SDK_2_NDK_X_86.length > sdkVersion) {
                ndkVersion = SDK_2_NDK_X_86[sdkVersion];
            } else {
                ndkVersion = SDK_2_NDK_X_86[SDK_2_NDK_X_86.length - 1];
            }
        }

        RepoUtils.setVersion(Build.CPU_ABI, ndkArch, ndkVersion);
    }

    @UiThread
    private void showPackages(List<PackageInfo> repo) {
        ArrayList<HashMap<String, String>> menuItems = new ArrayList<>();

        for (PackageInfo info : repo) {
            // creating new HashMap
            HashMap<String, String> map = new HashMap<String, String>();

            // adding each child node to HashMap key => value
            map.put(RepoUtils.KEY_NAME, info.getName());
            map.put(RepoUtils.KEY_VERSION, info.getVersion());
            map.put(RepoUtils.KEY_DESC, info.getDescription());
            map.put(RepoUtils.KEY_DEPENDS, info.getDepends());
            map.put(RepoUtils.KEY_FILESIZE, Utils.humanReadableByteCount(info.getFileSize(), false));
            map.put(RepoUtils.KEY_SIZE, Utils.humanReadableByteCount(info.getSize(), false));
            map.put(RepoUtils.KEY_FILE, info.getFile());

            File logFile = new File(EnvironmentPath.getInstalledPackageDir(this), info.getName() + ".list");
            if (logFile.exists()) {
                map.put(RepoUtils.KEY_STATUS, getString(R.string.pkg_installed));
            } else {
                map.put(RepoUtils.KEY_STATUS, getString(R.string.pkg_notinstalled));
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
                        RepoUtils.KEY_NAME,
                        RepoUtils.KEY_VERSION,
                        RepoUtils.KEY_DESC,
                        RepoUtils.KEY_DEPENDS,
                        RepoUtils.KEY_FILE,
                        RepoUtils.KEY_FILESIZE,
                        RepoUtils.KEY_SIZE,
                        RepoUtils.KEY_STATUS},
                new int[]{
                        R.id.pkg_name,
                        R.id.pkg_version,
                        R.id.pkg_desciption,
                        R.id.pkg_deps,
                        R.id.pkg_file,
                        R.id.pkg_filesize,
                        R.id.pkg_size,
                        R.id.pkg_status});

        getListView().setAdapter(adapter);

        if (mLastPosition > 0 && mLastPosition < repo.size()) {
            getListView().setSelection(mLastPosition);
        }

    }

    private void showProgress(int title, int message) {
        mProgressDialog = new ProgressDialog(context);
        mProgressDialog.setTitle(getString(title));
        mProgressDialog.setMessage(getString(message));
        mProgressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
        mProgressDialog.setCancelable(false);
        mProgressDialog.show();
    }

    private void hideProgress() {
        if (mProgressDialog != null && mProgressDialog.isShowing()) {
            mProgressDialog.dismiss();
        }
    }

    @MainThread
    private void updateProgress(final int progress) {
        mHandler.post(new Runnable() {
            public void run() {
                mProgressDialog.setProgress(progress);
            }
        });
    }

    @WorkerThread
    private void updateProgress(final String progress) {
        mHandler.post(new Runnable() {
            public void run() {
                mProgressDialog.setMessage(progress);
            }
        });
    }

    @WorkerThread
    private void updateProgressTitle(final String out) {
        mHandler.post(new Runnable() {
            public void run() {
                mProgressDialog.setTitle(out);
            }
        });
    }

    @UiThread
    private void showError(String message) {
        new AlertDialog.Builder(context)
                .setTitle(R.string.pkgmgr_name)
                .setMessage(message)
                .setNeutralButton(getText(R.string.button_continue), new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        if (mAction != null) {
                            setResult(RESULT_CANCELED);
                            finish();
                        }
                    }
                })
                .setCancelable(false)
                .setIcon(R.drawable.baseline_error_black_24)
                .show();
    }

    /**
     * Uninstall a package and all dependencies
     *
     * @param name - name of package
     * @return {@code true} if uninstall success
     */
    @SuppressWarnings("ResultOfMethodCallIgnored")
    private boolean uninstallPackage(String name) {
        if (name != null) {
            //update UI
            updateProgressTitle(getString(R.string.title_package_uninstall) + " " + name);
            updateProgress(getString(R.string.wait_message));
            updateProgress(0);

            //delete file
            String prermFile = new File(EnvironmentPath.getInstalledPackageDir(context), name + ".prerm").getAbsolutePath();
            if ((new File(prermFile)).exists()) {
                Log.i(TAG, "Execute prerm script " + prermFile);
                Utils.chmod(prermFile, 0x1ed);
                system(prermFile);
                new File(prermFile).delete();
            }
            updateProgress(25);
            String descFile = new File(EnvironmentPath.getInstalledPackageDir(context), name + ".pkgdesc").getAbsolutePath();
            if ((new File(descFile)).exists()) {
                new File(descFile).delete();
            }


            //remove package in list installed
            String logFile = new File(EnvironmentPath.getInstalledPackageDir(context), name + ".list").getAbsolutePath();
            if (!(new File(logFile)).exists()) {
                updateProgress(100);
                return false;
            }

            updateProgress(50);

            // TODO: 17-May-18 Do not remove dependencies package because other package need this package
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
            updateProgress(100);
        }
        return true;
    }

    private List<String> getReposList() {
        String defaultUrl = "http://cctools.info/packages";
        if (Build.VERSION.SDK_INT >= 21) {
            defaultUrl += "-pie";
        }

        List<String> list = null;
        File reposListFile = new File(EnvironmentPath.getToolchainsDir(this), "cctools/etc/repos.list");
        if (reposListFile.exists()) {
            try {
                FileInputStream fin = new FileInputStream(reposListFile);
                DataInputStream in = new DataInputStream(fin);
                BufferedReader reader = new BufferedReader(new InputStreamReader(in));
                String line;
                while ((line = reader.readLine()) != null) {
                    if (list == null) {
                        list = new ArrayList<>();
                    }
                    line = line.trim();
                    if (line.length() > 0) {
                        if (line.startsWith("#") || line.startsWith(";")) {
                            continue;
                        }
                        list.add(line);
                    }
                }
                in.close();
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        if (list == null) {
            list = new ArrayList<>();
            list.add(defaultUrl);
        }
        return list;
    }

    @MainThread
    private void editReposList() {
        final EditText ed = new EditText(this);
        ed.setSingleLine(false);
        List<String> list = getReposList();
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < list.size(); i++) {
            String url = list.get(i);
            sb.append(url);
            if (i != list.size() - 1) {
                sb.append("\n");
            }
        }

        UIUtils.showInputDialog(this, getString(R.string.title_repo_list), "", sb.toString(),
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

    private void system(String cmdline) {
        String cctoolsDir = new File(mToolchainDir, "cctools").getAbsolutePath();
        String javaBootClassPath = getEnv(cctoolsDir, "BOOTCLASSPATH");
        if (javaBootClassPath == null) {
            javaBootClassPath = Utils.getBootClassPath();
        }

        if (javaBootClassPath == null) {
            javaBootClassPath = "/system/framework/core.jar:" +
                    "/system/framework/ext.jar:/system/framework/framework.jar:" +
                    "/system/framework/android.policy.jar:" +
                    "/system/framework/services.jar";
        }

        final String[] envp = {
                "TMPDIR=" + Environment.getExternalStorageDirectory().getPath(),
                "PATH=" + cctoolsDir + "/bin:" + cctoolsDir + "/sbin:"+System.getenv("PATH"),
                "ANDROID_ASSETS=/system/app",
                "ANDROID_BOOTLOGO=1",
                "ANDROID_DATA=" + cctoolsDir + "/var/dalvik",
                "ANDROID_PROPERTY_WORKSPACE=" + getEnv(cctoolsDir, "ANDROID_PROPERTY_WORKSPACE"),
                "ANDROID_ROOT=/system",
                "BOOTCLASSPATH=" + javaBootClassPath,
                "CCTOOLSDIR=" + cctoolsDir,
                "CCTOOLSRES=" + getPackageResourcePath(),
                "LD_LIBRARY_PATH=" + cctoolsDir + "/lib:/system/lib:/vendor/lib",
                "HOME=" + EnvironmentPath.getHomeDir(this),
                "SHELL=" + getShell(),
                "TERM=xterm",
                "PS1=$ ",
                "SDDIR=" + mSdCardDir,
                "EXTERNAL_STORAGE=" + Environment.getExternalStorageDirectory().getPath(),
        };
        try {
            Log.i(TAG, "exec cmd " + cmdline + ", cctoolsdir " + cctoolsDir);
            Process p = Runtime.getRuntime().exec(cmdline, envp);
            p.waitFor();
        } catch (Exception e) {
            e.printStackTrace();
            Log.i(TAG, "Exec exception " + e);
        }
    }

    private String getEnv(String cctoolsDir, String variable) {
        String ret = null;
        String[] envp = {
                "TMPDIR=" + Environment.getExternalStorageDirectory().getPath(),
                "PATH=" + cctoolsDir + "/bin:" + cctoolsDir + "/sbin:"+System.getenv("PATH"),
                "ANDROID_ASSETS=/system/app",
                "ANDROID_BOOTLOGO=1",
                "ANDROID_DATA=" + cctoolsDir + "/var/dalvik",
                "ANDROID_ROOT=/system",
                "CCTOOLSDIR=" + cctoolsDir,
                "CCTOOLSRES=" + getPackageResourcePath(),
                "LD_LIBRARY_PATH=" + cctoolsDir + "/lib",
                "HOME=" + cctoolsDir + "/home",
                "SHELL=" + getShell(),
                "TERM=xterm",
                "PS1=$ ",
                "SDDIR=" + mSdCardDir,
                "EXTERNAL_STORAGE=" + Environment.getExternalStorageDirectory().getPath(),
        };
        String[] argv = {"/system/bin/sh", "-c", "set"};
        int[] pId = new int[1];
        FileDescriptor fd = Utils.createSubProcess(cctoolsDir, argv[0], argv, envp, pId);
        FileInputStream fis = new FileInputStream(fd);
        DataInputStream in = new DataInputStream(fis);
        BufferedReader reader = new BufferedReader(new InputStreamReader(in));
        String line = "";
        try {
            while ((line = reader.readLine()) != null) {
                if (line.startsWith(variable + "=")) {
                    if (line.contains("=")) {
                        ret = line.substring(line.indexOf("=") + 1);
                        break;
                    }
                }
            }
            in.close();
            Utils.waitFor(pId[0]);
        } catch (Exception e) {
            e.printStackTrace();

            Log.e(TAG, "exception " + e);
        }
        return ret;
    }

    private String getShell() {
        String[] shellList = {
                mToolchainDir + "/cctools/bin/bash",
                mToolchainDir + "/cctools/bin/ash",
        };

        for (String shell : shellList) {
            if ((new File(shell)).exists()) {
                return shell;
            }
        }
        return "/system/bin/sh";
    }

    @SuppressLint("StaticFieldLeak")
    private class DownloadRepoTask extends AsyncTask<List<String>, Void, List<PackageInfo>> {
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            showProgress(R.string.title_repo_update, R.string.message_package_downloading);
        }

        @Override
        protected List<PackageInfo> doInBackground(List<String>[] params) {
            mPackagesLists.setInstalledPackages(RepoUtils.getRepoFromDir(EnvironmentPath.getInstalledPackageDir(context)));
            updateProgress(30);
            mPackagesLists.setAvailablePackages(RepoUtils.getRepoFromUrl(params[0]));
            updateProgress(60);

            if (mPackagesLists.getAvailablePackages().isEmpty() || mPackagesLists.getInstalledPackages().isEmpty()) {
                return null;
            }
            return RepoUtils.checkingForUpdates(mPackagesLists);
        }

        @Override
        protected void onPostExecute(List<PackageInfo> needUpdatePackages) {
            updateProgress(100);
            if (!mPackagesLists.getAvailablePackages().isEmpty() && mAction == null) {
                DLog.i(TAG, "Downloaded repo with " + mPackagesLists.getAvailablePackages().size() + " packages.");
                showPackages(mPackagesLists.getAvailablePackages());
            }

            hideProgress();
            if (mPackagesLists.getAvailablePackages().isEmpty() && mAction == null) {
                showError(getString(R.string.message_package_repo_unavailable));
                return;
            }

            if (ACTION_INSTALL.equals(mAction)) {
                if (!mPackagesLists.getAvailablePackages().isEmpty()) {
                    new PrepareToInstallTask().execute(mActivityData);
                } else {
                    showError(getString(R.string.message_package_repo_unavailable) + "\n" +
                            getString(R.string.message_package_for_install) +
                            mActivityData);
                }
                return;
            } else if (ACTION_UNINSTALL.equals(mAction)) {
                new UninstallPackagesTask().execute(mActivityData);
                return;
            }

            if (needUpdatePackages != null) {
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
                                (new InstallPackagesTask()).execute(updateInfo);
                            }
                        });
                dialog.show();
            }
            if (needUpdatePackages == null && mAction != null && mAction.equals(ACTION_UPDATE)) {
                setResult(RESULT_OK);
                finish();
            }
        }
    }

    /**
     * Prepare for downloading a package
     */
    @SuppressLint("StaticFieldLeak")
    private class PrepareToInstallTask extends AsyncTask<String, Void, InstallPackageInfo> {
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            showProgress(R.string.pkg_prepareinstalltask, R.string.pkg_prepareinstall);
        }

        @Override
        protected InstallPackageInfo doInBackground(String... params) {
            // Update installed packages list
            mPackagesLists.setInstalledPackages(RepoUtils.getRepoFromDir(EnvironmentPath.getInstalledPackageDir(context)));
            return new InstallPackageInfo(mPackagesLists, params[0]);
        }

        @Override
        protected void onPostExecute(final InstallPackageInfo info) {
            hideProgress();
            Builder dialog = new AlertDialog.Builder(context)
                    .setIcon(android.R.drawable.ic_dialog_alert)
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
                            new InstallPackagesTask().execute(info);
                        }
                    });
            dialog.show();
        }
    }

    /**
     * Download an install package and all dependencies
     */
    @SuppressWarnings("ResultOfMethodCallIgnored")
    @SuppressLint("StaticFieldLeak")
    private class InstallPackagesTask extends AsyncTask<InstallPackageInfo, Void, Boolean> {
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            showProgress(R.string.pkg_installpackagetask, R.string.pkg_installpackage);
        }

        @Override
        protected Boolean doInBackground(InstallPackageInfo... params) {
            InstallPackageInfo info = params[0];
            List<String> postinstList = new ArrayList<>();
            for (PackageInfo packageInfo : info.getPackagesList()) {

                File packageDesc = new File(EnvironmentPath.getInstalledPackageDir(context), packageInfo.getName() + ".pkgdesc");
                if (packageDesc.exists()) {
                    PackageInfo oldPackage = RepoUtils.getPackageByName(mPackagesLists.getInstalledPackages(), packageInfo.getName());
                    if (oldPackage != null && packageInfo.getVersion().equals(oldPackage.getVersion())) {
                        Log.i(TAG, "Package " + packageInfo.getName() + " already installed.");
                        continue;
                    } else {
                        uninstallPackage(packageInfo.getName());
                        if (oldPackage != null) {
                            File oldPackageFile = new File(mBackupDir, oldPackage.getFile());
                            if (oldPackageFile.exists()) {
                                oldPackageFile.delete();
                            }
                        }
                    }
                }

                if (RepoUtils.isContainsPackage(mPackagesLists.getInstalledPackages(), packageInfo.getReplaces())) {
                    Log.i(TAG, "Replace package " + packageInfo.getReplaces());
                    PackageInfo oldPackage = RepoUtils.getPackageByName(mPackagesLists.getInstalledPackages(),
                            packageInfo.getReplaces());
                    if (oldPackage != null) {
                        uninstallPackage(oldPackage.getName());
                        File oldPackageFile = new File(mBackupDir, oldPackage.getFile());
                        if (oldPackageFile.exists()) {
                            oldPackageFile.delete();
                        }
                    }

                }

                updateProgressTitle(getString(R.string.pkg_installpackagetask) + " " + packageInfo.getName());

                Log.i(TAG, "Install " + packageInfo.getName() + " -> " + packageInfo.getFile());
                File logFile = new File(EnvironmentPath.getInstalledPackageDir(context), packageInfo.getName() + ".list");
                if (!downloadAndUnpack(packageInfo.getFile(), packageInfo.getUrl(), mToolchainDir, logFile.getAbsolutePath())) {
                    if (errorString != null) {
                        errorString += "\u0020" + info.getName();
                    }
                    return false;
                }

                updateProgress(getString(R.string.wait_message));

                // Move package info files from root directory
                String[] infoFiles = {"pkgdesc", "postinst", "prerm"};
                for (String infoFile : infoFiles) {
                    if ((new File(mToolchainDir, infoFile)).exists()) {
                        String infoFilePath = new File(EnvironmentPath.getInstalledPackageDir(context),
                                packageInfo.getName() + "." + infoFile).getAbsolutePath();
                        Log.i(TAG, "Copy file to " + infoFilePath);
                        try {
                            Utils.copyDirectory(new File(mToolchainDir, infoFile), new File(infoFilePath));
                            if (infoFile.equals("postinst")) {
                                postinstList.add(packageInfo.getName());
                            }
                        } catch (IOException e) {
                            e.printStackTrace();
                            Log.e(TAG, "Copy " + infoFile + " file failed " + e);
                        }
                        new File(mToolchainDir, infoFile).delete();
                    }
                }
            }

            // Move Examples to sd card
            if (new File(mToolchainDir, "/cctools/Examples").exists()) {
                try {
                    Log.i(TAG, "Move Examples to SD card");
                    Utils.copyDirectory(new File(mToolchainDir, "/cctools/Examples"), new File(mSdCardDir, "/Examples"));
                    Utils.deleteDirectory(new File(mToolchainDir, "/cctools/Examples"));
                } catch (IOException e) {
                    e.printStackTrace();
                    Log.e(TAG, "Can't copy examples directory " + e);
                }
            }

            //Execute postinst scripts
            for (String name : postinstList) {
                File postinstFile = new File(EnvironmentPath.getInstalledPackageDir(context), name + ".postinst");
                Log.i(TAG, "Execute postinst file " + postinstFile);
                Utils.chmod(postinstFile.getAbsolutePath(), 0x1ed/*0755*/);
                system(postinstFile.getAbsolutePath());
                postinstFile.delete();
            }

            return true;
        }

        private boolean downloadAndUnpack(@NonNull final String fileName, @NonNull final String fromUrl,
                                          @NonNull final String toPath, final String log) {
            updateProgress(getString(R.string.download_file) + " " + fileName + "...");

            errorString = null;

            //download file from url
            File temp = new File(mBackupDir, fileName);
            if (!temp.exists()) {
                try {
                    int totalRead = 0;
                    Log.i(TAG, "Downloading file " + fromUrl + "/" + fileName);

                    URL url = new URL(fromUrl + "/" + fileName);
                    URLConnection cn = url.openConnection();
                    cn.setReadTimeout(3 * 60 * 1000); // timeout 3 minutes
                    cn.connect();

                    int fileSize = cn.getContentLength();
                    StatFs stat = new StatFs(mBackupDir);
                    int sdAvailSize = stat.getAvailableBlocks();// * stat.getBlockSize();
                    Log.i(TAG, "File size " + fileSize);
                    Log.i(TAG, "Available on SD (in blocks " + stat.getBlockSize() + ") " + sdAvailSize);

                    int needMem = fileSize / stat.getBlockSize();
                    if (sdAvailSize < needMem) {
                        temp.delete();
                        errorString = getString(R.string.sd_no_memory) +
                                " " + Utils.humanReadableByteCount(needMem, false) +
                                " " + getString(R.string.sd_no_memory2);
                        return false;
                    }

                    InputStream inputStream = cn.getInputStream();
                    if (inputStream == null) {
                        throw new RuntimeException("stream is null");
                    }

                    Log.i(TAG, "File is " + temp.getAbsolutePath());
                    FileOutputStream out = new FileOutputStream(temp);
                    byte buf[] = new byte[128 * 1024];
                    do {
                        int numRead = inputStream.read(buf);
                        if (numRead <= 0) {
                            break;
                        }
                        out.write(buf, 0, numRead);
                        totalRead += numRead;
                        updateProgress(getString(R.string.received) + " " + totalRead + " " + getString(R.string.from) + " " + fileSize + " " + getString(R.string.bytes));
                        if (fileSize > 20 * 1024 * 1024) {
                            updateProgress(totalRead / (fileSize / 100));
                        } else {
                            updateProgress(totalRead * 100 / fileSize);
                        }
                    } while (true);

                    inputStream.close();
                    out.close();
                    if (totalRead != fileSize) {
                        throw new RuntimeException("Partially downloaded file!");
                    }

                } catch (Exception e) {
                    e.printStackTrace();
                    temp.delete();
                    Log.i(TAG, "Error downloading file " + fileName);
                    errorString = getString(R.string.error_downloading) + " (" + fileName + ")";
                    return false;
                }
            } else {
                Log.i(TAG, "Use file " + temp.getAbsolutePath());
            }

            updateProgress(100);

            //unpack file
            String tempPath = temp.getAbsolutePath();
            updateProgress(getString(R.string.unpacking_file) + " " + fileName + "...");
            Log.i(TAG, "Unpack file " + tempPath + " to " + toPath);

            String logFile = log;
            try {
                int needMem = Utils.unzippedSize(tempPath);
                if (needMem < 0) {
                    throw new RuntimeException("bad archive");
                }
                StatFs stat = new StatFs(toPath);
                double cacheAvailSize = stat.getAvailableBlocks();
                Log.i(TAG, "Unzipped size " + needMem);
                Log.i(TAG, "Available (blocks) " + cacheAvailSize + "(" + stat.getBlockSize() + ")");
                cacheAvailSize *= stat.getBlockSize();
                if (cacheAvailSize < needMem) {
                    errorString = getString(R.string.cache_no_memory) +
                            Utils.humanReadableByteCount(needMem, false) +
                            getString(R.string.cache_no_memory1) +
                            Utils.humanReadableByteCount((int) cacheAvailSize, false) +
                            getString(R.string.cache_no_memory2);
                    return false;
                }

                if (logFile == null) {
                    logFile = new File(EnvironmentPath.getInstalledPackageDir(context), fileName + ".list").getAbsolutePath();
                }
                if (Utils.unzip(tempPath, toPath, logFile) != 0) {
                    if (new File(logFile).exists()) {
                        new File(logFile).delete();
                    }
                    throw new RuntimeException("bad archive");
                }

            } catch (Exception e) {
                e.printStackTrace();
                temp.delete();
                Log.i(TAG, "Corrupted archive, restart application and try install again");
                errorString = getString(R.string.bad_archive) + " (" + fileName + ")";
                return false;
            }
            return true;
        }

        @Override
        protected void onPostExecute(Boolean result) {
            hideProgress();
            if (result) {
                if (ACTION_UPDATE.equals(mAction) || ACTION_INSTALL.equals(mAction)) {
                    setResult(RESULT_OK);
                    finish();
                    return;
                }
            } else {
                showError(errorString);
                return;
            }
            mLastPosition = mListView.getFirstVisiblePosition();
            showPackages(mPackagesLists.getAvailablePackages());
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
            mLastPosition = mListView.getFirstVisiblePosition();
            showPackages(mPackagesLists.getAvailablePackages());
        }
    }
}
