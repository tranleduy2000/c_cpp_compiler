package com.duy.ccppcompiler;

import android.content.Context;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.duy.ccppcompiler.utils.ApkUtils;
import com.duy.ccppcompiler.utils.IOUtils;

import java.io.File;
import java.io.InputStream;

/**
 * Created by Duy on 22-Apr-18.
 */

public class InstallActivity extends AppCompatActivity {
    private static final String KEY_EXTRACTED = "KEY_EXTRACTED";
    private static final String KEY_APP_VERSION = "KEY_APP_VERSION";

    private ProgressBar mProgressBar;
    private TextView mTxtMessage;
    @Nullable
    private ExtractDataTask mExtractDataTask;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_install);
        mProgressBar = findViewById(R.id.progress_bar);
        mTxtMessage = findViewById(R.id.txt_message);

        extractData();
    }

    private void extractData() {
        final SharedPreferences preferences = getPreferences(MODE_PRIVATE);
        final String appVersion = ApkUtils.getAppVersion(this);
        boolean extracted = preferences.getBoolean(KEY_EXTRACTED, false)
                && preferences.getString(KEY_APP_VERSION, "").equals(appVersion);
        if (!extracted) {
            mExtractDataTask = new ExtractDataTask(this, new ExtractCallback() {
                @Override
                public void onNewMessage(CharSequence message) {
                    mTxtMessage.setText(message);
                }

                @Override
                public void onComplete() {
                    SharedPreferences.Editor editor = preferences.edit();
                    editor.putBoolean(KEY_EXTRACTED, true);
                    editor.putString(KEY_APP_VERSION, appVersion);
                    editor.apply();
                    mTxtMessage.setText("Complete");
                    openEditor();
                }

                @Override
                public void onFailed(@Nullable Exception e) {
                    mTxtMessage.setError(e.getMessage());
                }
            });
        } else {
            openEditor();
        }
    }

    private void openEditor() {
        if (permissionGranted()) {
            startMainActivity();
        } else {
            requestPermission();
        }
    }

    private void startMainActivity() {
    }

    private boolean permissionGranted() {
        return false;
    }

    private void requestPermission() {

    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            startMainActivity();
        }
    }

    interface ExtractCallback {
        void onNewMessage(CharSequence message);

        void onComplete();

        void onFailed(@Nullable Exception e);
    }

    private static class ExtractDataTask extends AsyncTask<File, Void, Boolean> {
        private Context mContext;
        @Nullable
        private ExtractCallback mCallback;
        private Exception exception;

        ExtractDataTask(Context context, @Nullable ExtractCallback callback) {
            mContext = context;
            mCallback = callback;
        }

        @Override
        protected Boolean doInBackground(File... files) {
            try {
                final File internalDir = mContext.getFilesDir();
                final File gccDir = new File(internalDir, Constants.GCC_DIR_NAME);
                if (gccDir.exists()) {
                    IOUtils.delete(gccDir);
                    if (isCancelled()) {
                        return false;
                    }
                }
                final InputStream inputStream = mContext.getAssets().open(Constants.GCC_ASSET_FILE);
                if (IOUtils.unzip(inputStream, internalDir)) {
                    final String sep = File.separator;

                    final File binDir1 = new File(gccDir, "bin");
                    final File binDir2 = new File(gccDir, "arm-linux-androideabi" + sep + "bin");
                    final File binDir3 = new File(gccDir, "libexec" + sep + "gcc" + sep + "arm-linux-androideabi" + sep + Constants.GCC_VERSION);

                    if (isCancelled()) {
                        return false;
                    }
                    changeToExecutable(binDir1);
                    if (isCancelled()) {
                        return false;
                    }
                    changeToExecutable(binDir2);
                    if (isCancelled()) {
                        return false;
                    }
                    changeToExecutable(binDir3);

                    return true;
                } else {
                    return false;
                }
            } catch (Exception e) {
                exception = e;
                e.printStackTrace();
            }
            return false;
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
