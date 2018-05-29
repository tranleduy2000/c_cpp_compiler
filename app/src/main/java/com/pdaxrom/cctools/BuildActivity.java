package com.pdaxrom.cctools;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Handler;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.text.InputType;
import android.util.Log;
import android.widget.EditText;
import android.widget.TextView;

import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.packagemanager.Environment;
import com.jecelyin.common.utils.DLog;
import com.pdaxrom.utils.Utils;

import org.gjt.sp.jedit.Catalog;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;

@SuppressWarnings("ResultOfMethodCallIgnored")
public class BuildActivity extends AppCompatActivity {
    private static final String TAG = "BuildActivity";

    private TextView mLogView;
    private Thread mCmdThread;

    private String mCommand;

    private String mFilePath;
    private String mWorkDir;
    private String mTmpDir;
    private String mTmpExeDir;
    private String mOutFile;

    private boolean mForceRun;
    private boolean mRunExe = false;
    private boolean mBuildNativeActivity;
    private boolean mExecJava;
    private boolean mExecObjC;

    private int mProcessId;
    private int mExitCode;
    private String runme_ca;
    private String runme_na;
    private Handler handler = new Handler();
    private Context context = this;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_cctools_build);

        mLogView = findViewById(R.id.buildLog);
        Intent intent = getIntent();

        final String systemShell = "SHELL=/system/bin/sh";
        mFilePath = intent.getStringExtra(BuildConstants.EXTRA_FILE_NAME);

        mWorkDir = new File(mFilePath).getParentFile().toString();
        mForceRun = false;
        mTmpExeDir = Environment.getTmpExeDir(this);
        mTmpDir = Environment.getSdCardTmpDir();
        runme_ca = mTmpDir + "/runme_ca";
        runme_na = mTmpDir + "/runme_na";
        deleteIfExist(new File(runme_ca));
        deleteIfExist(new File(runme_na));

        SharedPreferences mPrefs = getSharedPreferences(CCToolsActivity.SHARED_PREFS_NAME, 0);
        mLogView.setTextSize(Float.valueOf(mPrefs.getString("fontsize", "12")));

        showTitle(getString(R.string.buildwindow_name) + " - " + mFilePath);

        String fileName = new File(mFilePath).getName();
        if (fileName.contentEquals("Makefile") || fileName.contentEquals("makefile")) {
            mCommand = "make " + systemShell;

        } else {
            int dotPos = fileName.lastIndexOf(".");
            if (dotPos != -1) {
                mExecJava = false;
                mExecObjC = false;
                String ext = fileName.substring(dotPos);
                mOutFile = fileName.substring(0, dotPos);
                Log.i(TAG, "extension [" + ext + "]");

                if (ext.contentEquals(".mk") || ext.contentEquals(".mak")) {
                    mCommand = "make -f " + fileName + " " + systemShell;
                    argsDialog(getString(R.string.make_title), getString(R.string.make_args));
                    return;

                } else if (Catalog.getModeByName("C").acceptFile(mFilePath, fileName)) {
                    mCommand = "gcc " + fileName;
                    mCommand += " " + mPrefs.getString("force_ccopts", "");

                } else if (Catalog.getModeByName("C++").acceptFile(mFilePath, fileName)) {
                    mCommand = "g++ " + fileName;
                    mCommand += " " + mPrefs.getString("force_cxxopts", "");

                } else if ((ext.contentEquals(".f") || ext.contentEquals(".f90") || ext.contentEquals(".f95")
                        || ext.contentEquals(".f03")) && new File(Environment.getCCtoolsDir(context), "/bin/f77").exists()) {
                    mCommand = "f77 " + fileName;
                    mCommand += " " + mPrefs.getString("force_ccopts", "");

                } else if (Catalog.getModeByName("Java").acceptFile(mFilePath, fileName) && (new File(Environment.getCCtoolsDir(context), "bin/javac")).exists()) {
                    mCommand = "javac-single " + mOutFile;
                    mExecJava = true;
                }

                if (ext.equals(".m") || ext.equals(".mm")) {
                    mExecObjC = true;
                }
            }
        }

        if (mCommand != null && !mCommand.contentEquals("")) {
            gccDialog(getString(R.string.gcc_title));
            return;
        }


        Log.i(TAG, "Unknown filetype, nothing to do");
        output(getString(R.string.unknown_filetype) + "\n");
        output(getString(R.string.known_filetypes) + "\n");
    }

    private void deleteIfExist(File file) {
        if (file.exists()) {
            file.delete();
        }
    }

    @Override
    protected void onDestroy() {
        Log.i(TAG, "Finish cmdline thread before activity exit");
        if (mCmdThread != null && mCmdThread.isAlive()) {
            mCmdThread.interrupt();
            Utils.hangupProcessGroup(mProcessId);
        }
        super.onDestroy();
    }

    private void argsDialog(String title, String message) {
        final EditText input = new EditText(this);
        input.setInputType(InputType.TYPE_CLASS_TEXT);
        input.setSingleLine(true);

        mCmdThread = new MyThread();
        mCmdThread.start();
    }

    private void gccDialog(String title) {
        SharedPreferences mPrefs = getSharedPreferences(CCToolsActivity.SHARED_PREFS_NAME, 0);
        mBuildNativeActivity = mPrefs.getBoolean("force_native_activity", false);
        mRunExe = true;
        mForceRun = mPrefs.getBoolean("force_run", true);
        if (mExecJava) {
            // nothing here yet
        } else {
            if (mBuildNativeActivity) {
                mOutFile = "lib" + mOutFile + ".so";
                mCommand += " -I" + Environment.getCCtoolsDir(context) + "/sources/native_app_glue"
                        + " " + Environment.getCCtoolsDir(context) + "/sources/native_app_glue/android_native_app_glue.c"
                        + " -o " + mOutFile
                        + " -Wl,-soname," + mOutFile + " -shared"
                        + " -Wl,--no-undefined -Wl,-z,noexecstack"
                        + " -llog -landroid -lm";
            } else {
                mCommand += " -o " + mOutFile;
            }
            if (mExecObjC) {
                mCommand += " -lobjc";
            }
        }
        mCmdThread = new MyThread();
        mCmdThread.start();
    }

    private void showTitle(final String str) {
        Runnable proc = new Runnable() {
            public void run() {
                setTitle(str);
            }
        };
        handler.post(proc);
    }

    private void output(final String str) {
        Runnable proc = new Runnable() {
            public void run() {
                mLogView.append(str);
            }
        };
        handler.post(proc);
    }

    private void showProgress(final boolean status) {
        Runnable proc = new Runnable() {
            public void run() {
                setProgressBarIndeterminateVisibility(status);
            }
        };
        handler.post(proc);
    }

    @Nullable
    private String checkAndRead(String fName) {
        File f = new File(fName);
        if (!f.exists()) {
            return null;
        }
        try {
            FileInputStream fin = new FileInputStream(fName);
            byte[] buf = new byte[(int) (f.length() - 1)];
            fin.read(buf);
            fin.close();
            f.delete();
            return new String(buf, "UTF-8");
        } catch (Exception e) {
            e.printStackTrace();
        }
        f.delete();
        return null;
    }

    public class MyThread extends Thread {
        @Override
        public void run() {
            try {
                showProgress(true);
                String[] workingEnv = {"PWD=" + mWorkDir, "TMPDIR=" + mTmpDir, "TEMP=" + mTmpDir, "TMPEXEDIR=" + mTmpExeDir,};
                String[] defaultEnv = Environment.buildDefaultEnv(context);
                workingEnv = Environment.join(workingEnv, defaultEnv);

                String[] argv = new String[]{"/system/bin/sh"};
                int[] pId = new int[1];
                FileDescriptor fileDescriptor = Utils.createSubProcess(mWorkDir, argv[0], argv, workingEnv, pId);
                mProcessId = pId[0];
                if (mProcessId > 0) {
                    try {
                        BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(fileDescriptor)));
                        FileOutputStream outputStream = new FileOutputStream(fileDescriptor);
                        Thread execThread = new Thread() {
                            public void run() {
                                DLog.d(TAG, "Waiting for hangup session");
                                mExitCode = Utils.waitFor(mProcessId);
                                DLog.d(TAG, "Subprocess exited: " + mExitCode);
                            }
                        };
                        execThread.start();

                        outputStream.write("export PS1=''\n".getBytes());
                        mCommand = "exec " + mCommand + "\n";
                        outputStream.write(mCommand.getBytes());
                        do {
                            String errstr = reader.readLine();
                            output(errstr + "\n");
                            if (DLog.DEBUG) DLog.w(TAG, "stdout: " + errstr);
                        } while (execThread.isAlive());

                        if (mExitCode != 0) {
                            output(getString(R.string.build_error) + " " + mExitCode + "\n");
                            showTitle(getString(R.string.buildwindow_name_error) + " - " + mFilePath);
                        } else
                            showTitle(getString(R.string.buildwindow_name_done) + " - " + mFilePath);

                        Log.e(TAG, "process exit code " + mExitCode);
                        outputStream.close();
                        reader.close();
                    } catch (IOException ie) {
                        Log.e(TAG, "exception " + ie);
                    }
                }
            } catch (Exception ie) {
                ie.printStackTrace();
                Log.e(TAG, "exec() " + ie);
            }
            output("\n" + getString(R.string.build_done) + "\n\n");
            showProgress(false);

            String runme = checkAndRead(runme_na);
            if (runme != null) {
                mOutFile = (new File(runme)).getName();
                mWorkDir = (new File(runme)).getParent();
                mBuildNativeActivity = true;
                mRunExe = true;
                mExitCode = 0;
            }

            runme = checkAndRead(runme_ca);
            if (runme != null) {
                mOutFile = (new File(runme)).getName();
                mWorkDir = (new File(runme)).getParent();
                mBuildNativeActivity = false;
                mRunExe = true;
                mExitCode = 0;
            }

            String javaClass = "";
            if (mExecJava) {
                javaClass = mOutFile;
                mOutFile = mOutFile + ".jar";
            }

            if (mRunExe && (new File(mWorkDir + "/" + mOutFile).exists()) && mExitCode == 0) {
                if (mBuildNativeActivity) {
                    Intent intent = new Intent(BuildActivity.this, LauncherNativeActivity.class);
                    intent.putExtra("activity_file", mWorkDir + "/" + mOutFile);
                    startActivity(intent);
                } else {
                    Intent intent = new Intent(BuildActivity.this, LauncherConsoleActivity.class);
                    if (mExecJava) {
                        intent.putExtra(BuildConstants.EXTRA_EXEC_FILE,
                                Environment.getCCtoolsDir(context) + "/bin/java -cp " + mWorkDir + "/" + mOutFile + " " + javaClass);
                    } else {
                        intent.putExtra(BuildConstants.EXTRA_EXEC_FILE, mWorkDir + "/" + mOutFile);
                    }
                    intent.putExtra(BuildConstants.EXTRA_FORCE_BUILD, mForceRun);
                    startActivity(intent);
                }
            }
        }
    }
}
