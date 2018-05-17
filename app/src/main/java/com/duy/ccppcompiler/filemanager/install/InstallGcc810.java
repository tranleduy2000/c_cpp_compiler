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

import android.content.Context;
import android.os.AsyncTask;
import android.support.annotation.Nullable;

import com.duy.common.DLog;
import com.duy.ccppcompiler.compiler.shell.CompileResult;
import com.duy.ccppcompiler.compiler.shell.ShellUtils;
import com.duy.utils.ExtractCallback;
import com.duy.utils.IOUtils;
import com.duy.utils.ZipExtractor;

import java.io.File;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;

class InstallGcc810 extends AsyncTask<Void, CharSequence, Boolean> {
    private static final String TAG = "InstallGcc720";
    private Context mContext;
    @Nullable
    private ExtractCallback mCallback;
    private Exception exception;

    InstallGcc810(Context context, @Nullable ExtractCallback callback) {
        mContext = context;
        mCallback = callback;
    }

    @Override
    protected Boolean doInBackground(Void... voids) {
        if (DLog.DEBUG) DLog.d(TAG, "doInBackground() called with: voids = [" + voids + "]");
        try {
            final File internalDir = mContext.getFilesDir();

            {
                File debug = new File("mnt/sdcard/arm-linux-androideabi-gcc");
                if (DLog.DEBUG) DLog.d(TAG, "debug = " + debug);
                File programPath = new File(internalDir, "arm-linux-androideabi-gcc");
                boolean result = com.jecelyin.common.utils.IOUtils.copyFile(debug, programPath);
                if (DLog.DEBUG) DLog.d(TAG, "result = " + result);
                programPath.setExecutable(true);
                HashMap<String, String> env = new HashMap<>();
                ArrayList<String> args = new ArrayList<>();
                args.add("-v");
                CompileResult compileResult = ShellUtils.execCommand(programPath.getAbsolutePath(), args, env);
                if (DLog.DEBUG) DLog.d(TAG, "shellResult = " + compileResult);
            }

            final File gccDir = new File(internalDir, "gcc-8.1.0");
            if (gccDir.exists()) {
                publishProgress("Delete old file");
                IOUtils.delete(gccDir);
            }
            gccDir.mkdirs();

            publishProgress("Extracting GCC library");
            final InputStream inputStream = mContext.getAssets().open("gcc-8.1.0.zip");
            boolean success = ZipExtractor.unzip(inputStream, internalDir);

            if (success) {
                final String sep = File.separator;

                final File binDir1 = new File(gccDir, "bin");
                final File binDir2 = new File(gccDir, "arm-linux-androideabi" + sep + "bin");
                final File binDir3 = new File(gccDir, "libexec" + sep + "gcc" + sep + "arm-linux-androideabi" + sep + "8.1.0");

                publishProgress("Make executable for " + binDir1.getName());
                changeToExecutable(binDir1);

                publishProgress("Make executable for " + binDir2.getName());
                changeToExecutable(binDir2);

                publishProgress("Make executable for " + binDir3.getName());
                changeToExecutable(binDir3);
                return true;
            }
        } catch (Exception e) {
            exception = e;
            e.printStackTrace();
        }
        return false;
    }

    @Override
    protected void onProgressUpdate(CharSequence... values) {
        super.onProgressUpdate(values);
        if (DLog.DEBUG)
            DLog.d(TAG, "onProgressUpdate() called with: values = [" + Arrays.toString(values) + "]");
        if (!isCancelled()) {
            if (mCallback != null) {
                mCallback.onNewMessage(values[0]);
            }
        }
    }

    @Override
    protected void onPostExecute(Boolean result) {
        super.onPostExecute(result);
        if (mCallback == null || isCancelled()) {
            return;
        }
        if (result) {
            mCallback.onComplete();
        } else {
            mCallback.onFailed(exception);
        }
    }

    private void changeToExecutable(File file) {
        if (file.isDirectory()) {
            File[] files = file.listFiles();
            if (files != null) {
                for (File child : files) {
                    changeToExecutable(child);
                }
            }
        } else {
            IOUtils.changeToExecutable(file);
        }
    }
}