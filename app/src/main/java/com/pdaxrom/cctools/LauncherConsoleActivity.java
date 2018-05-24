package com.pdaxrom.cctools;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.InputType;
import android.util.Log;
import android.widget.EditText;

import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.console.TermActivity;
import com.duy.ccppcompiler.packagemanager.Environment;
import com.pdaxrom.utils.Utils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

public class LauncherConsoleActivity extends AppCompatActivity {
    private final static String TAG = "LauncherConsoleActivity";
    private Context context = this;
    private String outFile;
    private String workDir;
    private String cmd;

    private boolean forceRun;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        String execFile = getIntent().getExtras().getString(BuildConstants.EXTRA_EXEC_FILE);
        forceRun = getIntent().getExtras().getBoolean(BuildConstants.EXTRA_FORCE_BUILD, false);

        if (execFile == null) {
            finish();
            return;
        }

        if (new File(execFile).exists()) {
            Log.i(TAG, "console executable file " + execFile);
            outFile = (new File(execFile)).getName();
            workDir = (new File(execFile)).getParentFile().getAbsolutePath();
            String tmpExeDir = Environment.getTmpExeDir(context);
            if (copyFile(execFile, tmpExeDir + "/" + outFile)) {
                Utils.chmod(tmpExeDir + "/" + outFile, 0x1ed); //S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH
                cmd = tmpExeDir + "/" + outFile;
            }
        } else {
            outFile = "";
            Log.i(TAG, "Trying to execute cmdline " + execFile);
            workDir = getIntent().getExtras().getString(BuildConstants.EXTRA_WORK_DIR);
            cmd = execFile;
        }

    }

    @Override
    public void onStart() {
        super.onStart();
        if (forceRun) {
            Intent myIntent = new Intent(LauncherConsoleActivity.this, TermActivity.class);
            myIntent.putExtra(BuildConstants.EXTRA_FILE_NAME, cmd);
            myIntent.putExtra(BuildConstants.EXTRA_WORK_DIR, workDir);
            startActivity(myIntent);
            finish();
            return;
        }

        final EditText input = new EditText(context);
        input.setInputType(InputType.TYPE_CLASS_TEXT);
        input.setSingleLine(true);
        new AlertDialog.Builder(context)
                .setTitle(getString(R.string.exe_dialog) + " " + outFile)
                .setMessage(getString(R.string.exe_args))
                .setView(input)
                .setPositiveButton(getString(R.string.button_continue), new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int whichButton) {
                        Intent myIntent = new Intent(LauncherConsoleActivity.this, TermActivity.class);
                        myIntent.putExtra(BuildConstants.EXTRA_FILE_NAME, cmd + " " + input.getText().toString());
                        myIntent.putExtra(BuildConstants.EXTRA_WORK_DIR, workDir);
                        startActivity(myIntent);
                        finish();
                    }
                }).setNegativeButton(getString(R.string.button_cancel), new DialogInterface.OnClickListener() {
            public void onClick(DialogInterface dialog, int whichButton) {
                finish();
            }
        }).setOnCancelListener(new DialogInterface.OnCancelListener() {
            public void onCancel(DialogInterface dialog) {
                finish();
            }
        }).show();
    }

    private boolean copyFile(String srcfile, String dstfile) {
        try {
            FileInputStream in = new FileInputStream(srcfile);
            FileOutputStream out = new FileOutputStream(dstfile);
            byte[] buf = new byte[100 * 1024];
            int i;
            while ((i = in.read(buf)) != -1) {
                out.write(buf, 0, i);
            }
            in.close();
            out.close();
        } catch (IOException e) {
            Log.e(TAG, "Error copying file: " + e);
            return false;
        }
        return true;
    }

}