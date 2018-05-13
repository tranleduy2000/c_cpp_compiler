package com.pdaxrom.cctools;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import com.pdaxrom.utils.Utils;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.text.InputType;
import android.util.Log;
import android.widget.EditText;

public class LauncherConsoleActivity extends Activity {
	private final static String TAG = "cctools-LauncherConsoleActivity";
	private Context context = this;
	private String outFile;
	private String workDir;
	private String tmpExeDir;
	private String cctoolsDir;
	private String cmd;
	
	private boolean forceRun;

    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        String activityFile = getIntent().getExtras().getString("executable_file");
        cctoolsDir	= getIntent().getExtras().getString("cctoolsdir");
        forceRun	= getIntent().getExtras().getBoolean("force", false);
        
        if (activityFile != null) {
            if ((new File(activityFile)).exists()) {
            	Log.i(TAG, "console executable file " + activityFile);
            	outFile = (new File(activityFile)).getName();
            	workDir = (new File(activityFile)).getParentFile().getAbsolutePath();
            	tmpExeDir = getCacheDir().getParentFile().getAbsolutePath() + "/root" + "/tmp";
            	(new File(tmpExeDir)).mkdir();
            	if (copyFile(activityFile, tmpExeDir + "/" + outFile)) {
            		Utils.chmod(tmpExeDir + "/" + outFile, 0x1ed); //S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH
            		cmd = tmpExeDir + "/" + outFile;
            		return;
            	}
            } else {
            	outFile = "";
            	Log.i(TAG, "Trying to execute cmdline " + activityFile);
            	workDir = getIntent().getExtras().getString("workdir");
            	cmd = activityFile;
            	return;
            }
        }
        finish();
    }	

    public void onStart() {
    	super.onStart();
    	if (forceRun) {
			Intent myIntent = new Intent(LauncherConsoleActivity.this, TermActivity.class);
			myIntent.putExtra("filename", cmd);
			myIntent.putExtra("cctoolsdir", cctoolsDir);
			myIntent.putExtra("workdir", workDir);
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
        			myIntent.putExtra("filename", cmd + " " + input.getText().toString());
        			myIntent.putExtra("cctoolsdir", cctoolsDir);
        			myIntent.putExtra("workdir", workDir);
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
            int i = 0;
            while ((i = in.read(buf)) != -1) {
                out.write(buf, 0, i);
            }
            in.close();
            out.close();
        } catch(IOException e) {
            Log.e(TAG, "Error copying file: " + e);
            return false;
        }
    	return true;
    }

}