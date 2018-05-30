package com.duy.c.cpp.compiler.sdlplugin;

import android.Manifest;
import android.annotation.SuppressLint;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.util.Log;

import org.libsdl.app.SDLActivity;

import java.io.File;
import java.io.IOException;
import java.io.InputStream;

public class sdlpluginActivity extends SDLActivity {
    private static final String TAG = "sdlpluginActivity";
    private static final int RC_PERMISSION_WRITE_EXTERNAL_STORAGE = 2;

    private static final String CPP_NIDE_WIKI_URL = "https://github.com/tranleduy2000/c_cpp_compiler/wiki/";

    private File mSdCardAppDir;

    private ProgressDialog mProgressDialog;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        Log.d(TAG, "onCreate()");

        if (isPermissionGranted()) {
            initUI();
        } else {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                String[] permission = {Manifest.permission.WRITE_EXTERNAL_STORAGE};
                requestPermissions(permission, RC_PERMISSION_WRITE_EXTERNAL_STORAGE);
            } else {
                initUI();
            }
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (grantResults.length != 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            initUI();
        }
    }

    private boolean isPermissionGranted() {
        int result = ActivityCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE);
        return result == PackageManager.PERMISSION_GRANTED;
    }


    private void initUI() {
        mSdCardAppDir = new File(Environment.getExternalStorageDirectory(), "CCPlusPlusNIDE");
        if (!mIsPaused && (getIntent().getExtras() == null || !getIntent().hasExtra("sdlmain"))) {
            if (mSdCardAppDir.exists() && mSdCardAppDir.isDirectory()) {
                File include = new File(mSdCardAppDir, "/SDL/include");
                File lib = new File(mSdCardAppDir, "/SDL/lib");
                if (!include.exists() || !lib.exists()) {
                    new InstallDevFilesTask().execute();
                } else {
                    showAboutDialog();
                }
            } else {
                showAboutDialog();
            }
        }
    }

    private void showAboutDialog() {
        String versionName;
        try {
            versionName = getPackageManager().getPackageInfo(getPackageName(), 0).versionName;
        } catch (NameNotFoundException e) {
            versionName = "1.0";
        }
        String message = getString(R.string.about_dialog_text) +
                " " + versionName + "\n" +
                getString(R.string.sdl_version) + " " + Utils.getSDLVersion(Utils.Lib_SDL) + "\n" +
                getString(R.string.sdl_image_version) + " " + Utils.getSDLVersion(Utils.Lib_SDL_image) + "\n" +
                getString(R.string.sdl_mixer_version) + " " + Utils.getSDLVersion(Utils.Lib_SDL_mixer) + "\n" +
                getString(R.string.sdl_net_version) + " " + Utils.getSDLVersion(Utils.Lib_SDL_net) + "\n" +
                getString(R.string.sdl_ttf_version) + " " + Utils.getSDLVersion(Utils.Lib_SDL_ttf) + "\n\n" +
                getString(R.string.about_dialog_text3) + "\n" +
                CPP_NIDE_WIKI_URL + "\n";

        AlertDialog.Builder builder = new AlertDialog.Builder(this)
                .setTitle(getString(R.string.about_dialog))
                .setMessage(message)
                .setNegativeButton(R.string.exit, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        dialog.cancel();
                    }
                });
        if (!isAppInstalled("com.duy.c.cpp.compiler")) {
            builder.setPositiveButton(R.string.install_cpp_nide, new DialogInterface.OnClickListener() {
                @Override
                public void onClick(DialogInterface dialog, int which) {
                    gotoPlayStore("com.duy.c.cpp.compiler");
                    dialog.cancel();
                }
            });
        }
        builder.setOnCancelListener(new DialogInterface.OnCancelListener() {
            @Override
            public void onCancel(DialogInterface dialog) {
                finish();
            }
        });
        builder.create().show();
    }

    private void gotoPlayStore(String packageName) {
        try {
            startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("market://details?id=" + packageName)));
        } catch (android.content.ActivityNotFoundException anfe) {
            startActivity(new Intent(Intent.ACTION_VIEW, Uri.parse("https://play.google.com/store/apps/details?id=" + packageName)));
        }
    }

    public boolean isAppInstalled(String packageName) {
        try {
            getPackageManager().getApplicationInfo(packageName, 0);
            return true;
        } catch (PackageManager.NameNotFoundException e) {
            return false;
        }
    }

    @Override
    protected void onDestroy() {
        if (mProgressDialog != null && mProgressDialog.isShowing()) {
            mProgressDialog.dismiss();
        }
        super.onDestroy();
    }

    @SuppressLint("StaticFieldLeak")
    private class InstallDevFilesTask extends AsyncTask<Void, String, Void> {
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            mProgressDialog = ProgressDialog.show(getContext(),
                    getString(R.string.app_name),
                    getString(R.string.update_checking), true);
        }

        @Override
        protected void onProgressUpdate(String... values) {
            super.onProgressUpdate(values);
            mProgressDialog.setMessage(values[0]);
        }

        @Override
        protected Void doInBackground(Void... params) {
            try {
                File sdlDir = new File(mSdCardAppDir, "SDL");
                File libDir = new File(sdlDir, "lib");
                libDir.mkdirs();
                Utils.copyDirectory(new File(getCacheDir().getParentFile().getAbsolutePath(), "lib"), libDir);

                new File(libDir, "libmain.so").delete();
                new File(libDir, "libccsdlplugin.so").delete();

                String arch = Build.CPU_ABI;
                if (arch.startsWith("mips")) {
                    arch = "mips";
                }
                publishProgress(getString(R.string.update_install_libs));
                InputStream is = getAssets().open("sdlmain-" + arch + ".zip");
                Utils.unpackZip(is, libDir.getAbsolutePath());
                is.close();

                publishProgress(getString(R.string.update_install_headers));
                is = getAssets().open("headers.zip");
                Utils.unpackZip(is, sdlDir.getAbsolutePath());
                is.close();

                publishProgress(getString(R.string.update_install_examples));
                is = getAssets().open("examples.zip");
                Utils.unpackZip(is, mSdCardAppDir.getAbsolutePath());
                is.close();
            } catch (IOException e) {
                e.printStackTrace();
                Log.e(TAG, "Error installing dev files " + e);
            }
            return null;
        }

        @Override
        protected void onPostExecute(Void result) {
            super.onPostExecute(result);
            mProgressDialog.dismiss();
            showAboutDialog();
        }
    }
}
