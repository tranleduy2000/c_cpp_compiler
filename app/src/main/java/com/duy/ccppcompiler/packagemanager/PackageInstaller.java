package com.duy.ccppcompiler.packagemanager;

import android.content.Context;
import android.os.StatFs;
import android.support.annotation.WorkerThread;
import android.util.Log;

import com.duy.ccppcompiler.compiler.shell.ShellUtils;
import com.duy.ccppcompiler.packagemanager.exceptions.BadArchiveException;
import com.duy.ccppcompiler.packagemanager.exceptions.NotEnoughCacheException;
import com.duy.ccppcompiler.packagemanager.model.PackageInfo;
import com.duy.common.DLog;
import com.pdaxrom.utils.Utils;

import org.apache.commons.io.IOUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;

public class PackageInstaller {
    private static final String TAG = "PackageInstaller";
    private final String mToolchainDir;
    private final String mInstalledDir;
    private final String mCCToolsDir;
    private final Context mContext;

    public PackageInstaller(Context context) {
        mContext = context;
        mToolchainDir = Environment.getToolchainsDir(mContext);
        mInstalledDir = Environment.getInstalledPackageDir(mContext);
        mCCToolsDir = Environment.getCCtoolsDir(mContext);
    }

    public void install(File zipFile, PackageInfo packageInfo) throws NotEnoughCacheException, BadArchiveException {
        //unpack file
        final String tempPath = zipFile.getAbsolutePath();
        final String toPath = mToolchainDir;

        Log.d(TAG, "Unpack file " + tempPath + " to " + toPath);
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
            throw new NotEnoughCacheException(needMem, (long) cacheAvailSize);
        }

        File logFile = new File(mInstalledDir, packageInfo.getName() + ".list");
        if (Utils.unzip(tempPath, toPath, logFile.getAbsolutePath()) != 0) {
            if (logFile.exists()) {
                logFile.delete();
            }
            throw new BadArchiveException(zipFile.getName());
        }


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
    }


    @WorkerThread
    private void finishInstallPackage(File postinstFile) throws IOException {
        if (DLog.DEBUG) {
            FileInputStream input = new FileInputStream(postinstFile);
            String content = IOUtils.toString(input);
            input.close();
            DLog.d(TAG, "finishInstallPackage called with \n" + content);
        }
        // Move Examples to sd card
        File examples = new File(mCCToolsDir, "Examples");
        if (examples.exists()) {
            try {
                Log.i(TAG, "Move Examples to SD card");
                Utils.copyDirectory(examples, new File(Environment.getSdCardExampleDir()));
                Utils.deleteDirectory(examples);
            } catch (IOException e) {
                e.printStackTrace();
                Log.e(TAG, "Can't copy examples directory " + e);
            }
        }

        Log.i(TAG, "Execute postinst file " + postinstFile);

        Utils.chmod(postinstFile.getAbsolutePath(), 493/*0755*/);
        ShellUtils.exec(mContext, postinstFile);
    }


}
