package com.duy.ccppcompiler;

import android.Manifest;
import android.content.Context;
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

import com.duy.ccppcompiler.utils.ApkUtils;
import com.duy.ccppcompiler.utils.ExtractCallback;
import com.duy.ccppcompiler.utils.IOUtils;
import com.duy.common.DLog;

import java.io.File;
import java.io.InputStream;
import java.util.Arrays;

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
    private ExtractDataTask mExtractDataTask;

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
        extracted &= !BuildConfig.DEBUG;
        if (!extracted) {
            mExtractDataTask = new ExtractDataTask(this, new ExtractCallback() {
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
        Intent intent = new Intent(this, MainActivity.class);
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


    private static class ExtractDataTask extends AsyncTask<Void, CharSequence, Boolean> {
        private Context mContext;
        @Nullable
        private ExtractCallback mCallback;
        private Exception exception;

        ExtractDataTask(Context context, @Nullable ExtractCallback callback) {
            mContext = context;
            mCallback = callback;
        }

        @Override
        protected Boolean doInBackground(Void... voids) {
            if (DLog.DEBUG) DLog.d(TAG, "doInBackground() called with: voids = [" + voids + "]");
            try {
                final File internalDir = mContext.getFilesDir();
                final File gccDir = new File(internalDir, Constants.GCC_DIR_NAME);
                if (gccDir.exists()) {
                    publishProgress("Delete old file");
                    IOUtils.delete(gccDir);
                }

                publishProgress("Extracting GCC library");
                final InputStream inputStream = mContext.getAssets().open(Constants.GCC_ASSET_FILE);
                boolean success = IOUtils.unzip(inputStream, internalDir);

                if (success) {
                    final String sep = File.separator;

                    final File binDir1 = new File(gccDir, "bin");
                    final File binDir2 = new File(gccDir, "arm-linux-androideabi" + sep + "bin");
                    final File binDir3 = new File(gccDir, "libexec" + sep + "gcc" + sep + "arm-linux-androideabi" + sep + Constants.GCC_VERSION);

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
}
