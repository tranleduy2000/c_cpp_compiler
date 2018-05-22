package com.duy.ccppcompiler.packagemanager;

import android.annotation.SuppressLint;
import android.os.AsyncTask;
import android.os.StatFs;
import android.support.annotation.NonNull;
import android.support.annotation.UiThread;
import android.support.annotation.WorkerThread;
import android.util.Log;

import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.compiler.shell.ShellUtils;
import com.duy.ccppcompiler.packagemanager.exceptions.NotEnoughStorageException;
import com.duy.ccppcompiler.packagemanager.model.InstallPackageInfo;
import com.duy.ccppcompiler.packagemanager.model.PackageInfo;
import com.duy.ccppcompiler.packagemanager.model.PackagesLists;
import com.duy.common.DLog;
import com.pdaxrom.utils.Utils;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;

import static android.app.Activity.RESULT_OK;
import static com.duy.ccppcompiler.packagemanager.PackageManagerActivity.ACTION_INSTALL;
import static com.duy.ccppcompiler.packagemanager.PackageManagerActivity.ACTION_UPDATE;

/**
 * Download an install package and all dependencies
 */
@SuppressWarnings("ResultOfMethodCallIgnored")
class InstallPackagesTask extends AsyncTask<InstallPackageInfo, Object, Void> {
    private static final String TAG = "InstallPackagesTask";
    private static final int INSTALL_COMPLETE = 0;
    private static final int DOWNLOAD_ERROR = 1;
    private static final int UPDATE_TITLE = 2;
    private static final int UPDATE_PROCESS = 3;
    private static final int UPDATE_MESSAGE = 4;

    @SuppressLint("StaticFieldLeak")
    private PackageManagerActivity mActivity;
    private PackagesLists mPackagesLists;
    private String mInstalledDir, mBackupDir, mToolchainDir, mSdCardDir;

    InstallPackagesTask(PackageManagerActivity activity) {
        mActivity = activity;
        mPackagesLists = activity.mPackagesLists;
    }

    @Override
    protected void onPreExecute() {
        super.onPreExecute();
        mActivity.showProgress(R.string.pkg_installpackagetask, R.string.pkg_installpackage);
        mBackupDir = Environment.getSdCardBackupDir();
        mInstalledDir = Environment.getInstalledPackageDir(mActivity);
        mToolchainDir = Environment.getToolchainsDir(mActivity);
        mSdCardDir = Environment.getSdCardDir();
    }

    @Override
    protected Void doInBackground(InstallPackageInfo... installPackageInfos) {
        InstallPackageInfo info = installPackageInfos[0];
        downloadPackages(info);
        return null;
    }

    @WorkerThread
    private boolean checkPackageInstalled(PackageInfo packageInfo) {
        File packageDesc = new File(mInstalledDir, packageInfo.getName() + ".pkgdesc");
        if (packageDesc.exists()) {
            PackageInfo oldPackage = RepoUtils.getPackageByName(mPackagesLists.getInstalledPackages(), packageInfo.getName());
            if (oldPackage != null && packageInfo.getVersion().equals(oldPackage.getVersion())) {
                Log.i(TAG, "Package " + packageInfo.getName() + " already installed.");
                return true;
            } else {
                mActivity.uninstallPackage(packageInfo.getName());
                if (oldPackage != null) {
                    File oldPackageFile = new File(mBackupDir, oldPackage.getFileName());
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
                mActivity.uninstallPackage(oldPackage.getName());
                File oldPackageFile = new File(mBackupDir, oldPackage.getFileName());
                if (oldPackageFile.exists()) {
                    oldPackageFile.delete();
                }
            }

        }
        return false;
    }

    @WorkerThread
    private void downloadPackages(InstallPackageInfo info) {
        //first checkPackageInstalled
        final List<PackageInfo> needInstall = new ArrayList<>();
        for (PackageInfo packageInfo : info.getPackagesList()) {
            if (!checkPackageInstalled(packageInfo)) {
                needInstall.add(packageInfo);
            }
        }
        if (needInstall.isEmpty()) {
            publishProgress(INSTALL_COMPLETE);
            return;
        }

        final Iterator<PackageInfo> packageInfoIterator = needInstall.iterator();
        final PackageDownloadListener downloadListener = new PackageDownloadListener() {

            @Override
            public void onDownloadComplete(PackageInfo packageInfo, @NonNull File downloadedFile) {
                if (DLog.DEBUG) DLog.d(TAG, "onComplete() called with:");
                if (DLog.DEBUG) DLog.d(TAG, "packageInfo = " + packageInfo);
                if (DLog.DEBUG) DLog.d(TAG, "downloadedFile = " + downloadedFile);

                //unpack file
                final String tempPath = downloadedFile.getAbsolutePath();
                final String toPath = mToolchainDir;

                publishProgress(UPDATE_MESSAGE, mActivity.getString(R.string.unpacking_file) + " " + downloadedFile.getName() + "...");
                Log.d(TAG, "Unpack file " + tempPath + " to " + toPath);
                try {
                    int needMem = Utils.unzippedSize(tempPath);
                    if (needMem < 0) {
                        throw new RuntimeException("bad archive");
                    }
                    StatFs stat = new StatFs(toPath);
                    double cacheAvailSize = stat.getAvailableBlocks();
                    Log.d(TAG, "Unzipped size " + needMem);
                    Log.d(TAG, "Available (blocks) " + cacheAvailSize + "(" + stat.getBlockSize() + ")");
                    cacheAvailSize *= stat.getBlockSize();
                    if (cacheAvailSize < needMem) {
                        String message = mActivity.getString(R.string.cache_no_memory) +
                                Utils.humanReadableByteCount(needMem, false) +
                                mActivity.getString(R.string.cache_no_memory1) +
                                Utils.humanReadableByteCount((int) cacheAvailSize, false) +
                                mActivity.getString(R.string.cache_no_memory2);
                        onFailure(new IOException(message));
                        return;
                    }

                    File logFile = new File(mInstalledDir, packageInfo.getName() + ".list");
                    if (Utils.unzip(tempPath, toPath, logFile.getAbsolutePath()) != 0) {
                        if (logFile.exists()) {
                            logFile.delete();
                        }
                        throw new RuntimeException("bad archive");
                    }
                } catch (Exception e) {
                    e.printStackTrace();
                    downloadedFile.delete();
                    Log.d(TAG, "Corrupted archive, restart application and try install again");
                    onFailure(new IOException(mActivity.getString(R.string.bad_archive) + " (" + downloadedFile.getName() + ")"));
                    return;
                }

                publishProgress(UPDATE_MESSAGE, mActivity.getString(R.string.wait_message));
                if (DLog.DEBUG) DLog.d(TAG, "onComplete: copy info files");
                // Move package info files from root directory
                String[] infoFiles = {"pkgdesc", "prerm", "postinst"};
                for (String infoFilePath : infoFiles) {
                    File file = new File(mToolchainDir, infoFilePath);
                    if (file.exists()) {
                        try {
                            File infoFile = new File(mInstalledDir, packageInfo.getName() + "." + infoFilePath);
                            Log.i(TAG, "Copy file to " + infoFile);
                            Utils.copyDirectory(file, infoFile);
                            if (infoFilePath.equals("postinst")) {
                                finishInstallPackage(file);
                            }
                        } catch (IOException e) {
                            e.printStackTrace();
                            Log.e(TAG, "Copy " + infoFilePath + " file failed " + e);
                        }
                        file.delete();
                    }
                }

                if (isCancelled()) {
                    return;
                }

                //download next package
                if (packageInfoIterator.hasNext()) {
                    download(packageInfoIterator.next(), this);
                } else {
                    mActivity.updateInstalledPackages();
                    publishProgress(INSTALL_COMPLETE);
                }
            }

            @Override
            public void onFailure(@NonNull Exception e) {
                publishProgress(DOWNLOAD_ERROR, e);
            }

            @Override
            public void onDownloadProgress(PackageInfo packageInfo, long totalRead, long fileSize) {
                if (!isCancelled()) {
                    publishProgress(UPDATE_MESSAGE, mActivity.getString(com.pdaxrom.cctools.R.string.received) + " " +
                            Utils.humanReadableByteCount(totalRead, false) + " " +
                            mActivity.getString(com.pdaxrom.cctools.R.string.from) + " " +
                            Utils.humanReadableByteCount(fileSize, false) + " " +
                            mActivity.getString(com.pdaxrom.cctools.R.string.bytes));

                    if (fileSize > 20 * 1024 * 1024) {
                        publishProgress(UPDATE_PROCESS, totalRead / (fileSize / 100));
                    } else {
                        publishProgress(UPDATE_PROCESS, totalRead * 100 / fileSize);
                    }
                }
            }
        };

        download(packageInfoIterator.next(), downloadListener);
    }

    @Override
    protected void onProgressUpdate(Object... values) {
        super.onProgressUpdate(values);
        int code = (int) values[0];
        switch (code) {
            case INSTALL_COMPLETE:
                onInstallComplete();
                break;
            case DOWNLOAD_ERROR:
                onDownloadError((Exception) values[1]);
                break;
            case UPDATE_TITLE:
                mActivity.updateProgressTitle((String) values[1]);
                break;
            case UPDATE_MESSAGE:
                mActivity.updateMessage((String) values[1]);
                break;
            case UPDATE_PROCESS:
                mActivity.updateProgress(Integer.valueOf(String.valueOf(values[1])));
                break;
        }
    }

    @UiThread
    private void onInstallComplete() {
        mActivity.hideProgress();
        if (ACTION_UPDATE.equals(mActivity.mAction) || ACTION_INSTALL.equals(mActivity.mAction)) {
            mActivity.setResult(RESULT_OK);
            mActivity.finish();
        }
        mActivity.showPackages(mPackagesLists.getAvailablePackages());
    }

    @UiThread
    private void onDownloadError(Exception e) {
        if (isCancelled()) return;

        String error;
        if (e instanceof NotEnoughStorageException) {
            error = mActivity.getString(R.string.sd_no_memory) +
                    " " + Utils.humanReadableByteCount(((NotEnoughStorageException) e).getNeedMem(), false) +
                    " " + mActivity.getString(R.string.sd_no_memory2);
        } else {
            error = e.getMessage();
        }
        mActivity.hideProgress();
        mActivity.showError(error);
    }

    @WorkerThread
    private void download(final PackageInfo packageInfo, final PackageDownloadListener listener) {
        Log.i(TAG, "Install " + packageInfo.getName() + " -> " + packageInfo.getFileName());
        publishProgress(UPDATE_TITLE, mActivity.getString(R.string.pkg_installpackagetask) + " " + packageInfo.getName());

        if (DLog.DEBUG) DLog.d(TAG, "download: use firebsae");
        mActivity.mFirebasePackageRepository.download(new File(mBackupDir), packageInfo, new PackageDownloadListener() {
            @Override
            public void onDownloadComplete(PackageInfo packageInfo, File downloadedFile) {
                listener.onDownloadComplete(packageInfo, downloadedFile);
            }

            @Override
            public void onFailure(@NonNull Exception e) {
                e.printStackTrace();
                //failed, use ubuntu server
                mActivity.mUbuntuServerPackageRepository.download(new File(mBackupDir), packageInfo, listener);
            }

            @Override
            public void onDownloadProgress(PackageInfo packageInfo, long totalRead, long fileSize) {
                listener.onDownloadProgress(packageInfo, totalRead, fileSize);
            }
        });

    }

    @WorkerThread
    private void finishInstallPackage(File postinstFile) {
        if (DLog.DEBUG)
            DLog.d(TAG, "finishInstallPackage() called with: postinstFile = [" + postinstFile + "]");
        // Move Examples to sd card
        String cCtoolsDir = Environment.getCCtoolsDir(mActivity);
        File examples = new File(cCtoolsDir, "Examples");
        if (examples.exists()) {
            try {
                Log.i(TAG, "Move Examples to SD card");
                Utils.copyDirectory(examples, new File(mSdCardDir, "CCTools/Examples"));
                Utils.deleteDirectory(examples);
            } catch (IOException e) {
                e.printStackTrace();
                Log.e(TAG, "Can't copy examples directory " + e);
            }
        }

        Log.i(TAG, "Execute postinst file " + postinstFile);
        Utils.chmod(postinstFile.getAbsolutePath(), 0x1ed/*0755*/);
        ShellUtils.execCommand(mActivity, postinstFile.getAbsolutePath());
    }
}
