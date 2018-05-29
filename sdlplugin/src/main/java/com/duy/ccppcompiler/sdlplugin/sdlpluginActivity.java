package com.duy.ccppcompiler.sdlplugin;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.content.pm.PackageManager.NameNotFoundException;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.text.method.LinkMovementMethod;
import android.text.util.Linkify;
import android.util.Log;
import android.widget.TextView;

import org.libsdl.app.SDLActivity;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;

public class sdlpluginActivity extends SDLActivity {
    private static final String TAG = "sdlpluginActivity";
    private static final int RC_PERMISSION_WRITE_EXTERNAL_STORAGE = 2;

    private static final String CCTOOLS_GOOGLE_URL = "https://play.google.com/store/apps/details?id=com.duy.ccppcompiler";

    private static final String CCTOOLS_URL = "http://cctools.info";

    private File mSdCardAppDir;

    private ProgressDialog pd;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        Log.v(TAG, "onCreate()");
        super.onCreate(savedInstanceState);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, RC_PERMISSION_WRITE_EXTERNAL_STORAGE);
        }

        mSdCardAppDir = new File(Environment.getExternalStorageDirectory(), "CCPlusPlusNIDE");
        if (!super.mIsPaused && getIntent().getExtras() == null) {
            if (mSdCardAppDir.exists() && mSdCardAppDir.isDirectory()) {
                if (!new File(mSdCardAppDir + "/SDL/include").exists() ||
                        !new File(mSdCardAppDir + "/SDL/lib").exists()) {
                    Log.i(TAG, "Install dev files");

                    new InstallDevFiles().execute();
                } else {
                    aboutDialog(1);
                }
            } else {
                aboutDialog(0);
            }
        }
    }

    private void aboutDialog(int type) {
        String versionName;
        try {
            versionName = getPackageManager().getPackageInfo(getPackageName(), 0).versionName;
        } catch (NameNotFoundException e) {
            versionName = "1.0";
        }
        final TextView textView = new TextView(this);
        textView.setAutoLinkMask(Linkify.WEB_URLS);
        textView.setLinksClickable(true);
        if (type == 0) {
            String text = getString(R.string.about_dialog_text) +
                    " " +
                    versionName +
                    "\n" +
                    getString(R.string.about_dialog_text1) +
                    "\n" + CCTOOLS_GOOGLE_URL + "\n" +
                    getString(R.string.about_dialog_text2);
            textView.setText(text);
        } else {
            String text = getString(R.string.about_dialog_text) +
                    " " + versionName + "\n" +
                    getString(R.string.sdl_version) + " " + Utils.getSDLVersion(Utils.Lib_SDL) + "\n" +
                    getString(R.string.sdl_image_version) + " " + Utils.getSDLVersion(Utils.Lib_SDL_image) + "\n" +
                    getString(R.string.sdl_mixer_version) + " " + Utils.getSDLVersion(Utils.Lib_SDL_mixer) + "\n" +
                    getString(R.string.sdl_net_version) + " " + Utils.getSDLVersion(Utils.Lib_SDL_net) + "\n" +
                    getString(R.string.sdl_ttf_version) + " " + Utils.getSDLVersion(Utils.Lib_SDL_ttf) + "\n\n" +
                    getString(R.string.about_dialog_text3) + "\n" +
                    CCTOOLS_URL + "\n";
            textView.setText(text
            );
        }
        textView.setMovementMethod(LinkMovementMethod.getInstance());
        new AlertDialog.Builder(this)
                .setTitle(getString(R.string.about_dialog))
                .setView(textView)
                .setOnCancelListener(new OnCancelListener() {
                    @Override
                    public void onCancel(DialogInterface dialog) {
                        System.exit(RESULT_OK);
                    }
                })
                .show();
    }

    @SuppressLint("StaticFieldLeak")
    private class InstallDevFiles extends AsyncTask<Void, String, Void> {
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            pd = ProgressDialog.show(getContext(), getString(R.string.app_name),
                    getString(R.string.update_checking), true);
        }

        @Override
        protected void onProgressUpdate(String... values) {
            super.onProgressUpdate(values);
            pd.setMessage(values[0]);
        }

        @Override
        protected Void doInBackground(Void... params) {
            try {
                if (!(new File(mSdCardAppDir + "/SDL/lib").exists())) {
                    new File(mSdCardAppDir + "/SDL/lib").mkdirs();
                    Utils.copyDirectory(new File(getCacheDir().getParentFile().getAbsolutePath() + "/lib"),
                            new File(mSdCardAppDir + "/SDL/lib"));
                    new File(mSdCardAppDir + "/SDL/lib/libmain.so").delete();
                    new File(mSdCardAppDir + "/SDL/lib/libccsdlplugin.so").delete();
                    String arch = Build.CPU_ABI;
                    if (arch.startsWith("mips")) {
                        arch = "mips";
                    }
                    publishProgress(getString(R.string.update_install_libs));
                    InputStream is = getAssets().open("sdlmain-" + arch + ".zip");
                    Utils.unpackZip(is, mSdCardAppDir + "/SDL/lib");
                    is.close();
                }
                if (!(new File(mSdCardAppDir + "/SDL/include").exists())) {
                    publishProgress(getString(R.string.update_install_headers));
                    InputStream is = getAssets().open("headers.zip");
                    Utils.unpackZip(is, mSdCardAppDir + "/SDL");
                    is.close();
                }
                if (!(new File(mSdCardAppDir + "/Examples/SDL").exists())) {
                    publishProgress(getString(R.string.update_install_examples));
                    InputStream is = getAssets().open("examples.zip");
                    Utils.unpackZip(is, mSdCardAppDir.getAbsolutePath());
                    is.close();
                }
            } catch (IOException e) {
                Log.e(TAG, "Error installing dev files " + e);
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            super.onPostExecute(result);
            pd.dismiss();
            aboutDialog(1);
        }
    }

}