package com.pdaxrom.cctools;

import java.io.File;

import com.pdaxrom.term.ShellTermSession;
import com.pdaxrom.term.TermView;
import com.pdaxrom.utils.Utils;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

public class TermActivity extends Activity {
	private final static String TAG = "cctools-terminal";

	private TermView mTermView;
	private ShellTermSession mSession;
	private String fileName;
	private String cmdline;
	private String workdir;
	private String cctoolsDir;

	private boolean isRunning = false;
	
    private Handler handler = new Handler();

    private Handler mMsgHandler = new Handler() {
	    @Override
	    public void handleMessage(Message msg) {
	    	if (!isRunning) {
	    		return;
	    	}
	    	if (msg.what == 123) {
	    		Log.i(TAG, "Message - Process exited!!!");
	    		showTitle(getString(R.string.console_name) + " - " + getString(R.string.console_finished));
	    		isRunning = false;
	    	}
	    }
	};

//	MyThread exeThread;

	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.term);

        fileName = getIntent().getStringExtra("filename");
        workdir = getIntent().getStringExtra("workdir");
        cctoolsDir = getIntent().getStringExtra("cctoolsdir");
        cmdline = fileName;

        showTitle(getString(R.string.console_name) + " - " + getString(R.string.console_executing));
        
        SharedPreferences mPrefs = getSharedPreferences(CCToolsActivity.SHARED_PREFS_NAME, 0);

		mTermView = (TermView) findViewById(R.id.emulatorView);
		mSession = createShellTermSession();
		mTermView.attachSession(mSession);
		mTermView.setDensity(getResources().getDisplayMetrics());
        mTermView.setTextSize(Integer.valueOf(mPrefs.getString("console_fontsize", "12")));
//        mEmulatorView.setControlKeyCode(KeyEvent.KEYCODE_CTRL_LEFT);
		
//		exeThread = new MyThread();
//		exeThread.start();
	}
	
	protected void onResume() {
		super.onResume();
		
		mTermView.onResume();
	}
	
	protected void onPause() {
		mTermView.onPause();
		
		super.onPause();
	}
	
	protected void onDestroy() {
		/*
    	if (exeThread != null && exeThread.isAlive()) {
    		Log.i(TAG, "kill process group");
    		mSession.hangup();
    		try {
				exeThread.wait();
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
    	}
    	 */
		if (mSession != null) {
			mSession.finish();
		}
		super.onDestroy();
	}

    public void onBackPressed() {
    	if (isRunning) {
    		Log.i(TAG, "kill process group");
    		mSession.hangup();
        } else {
             super.onBackPressed(); // this will actually finish the Activity
        }
    }

	
	private void showTitle(final String str) {
    	Runnable proc = new Runnable() {
    		public void run() {
    			setTitle(str);
    		}
    	};
    	handler.post(proc);
    }
/*
    public class MyThread extends Thread {
    	public void run() {
			Log.i(TAG, "Waiting for hangup session");
			int result = Utils.waitFor(mSession.getPid());
			Log.i(TAG, "Subprocess exited: " + result);
			showTitle(getString(R.string.console_name) + " - " + getString(R.string.console_finished));
    	}
    }
 */
	private ShellTermSession createShellTermSession() {
		cmdline = cmdline.replaceAll("\\s+", " ");
		Log.i(TAG, "Shell sesion for " + cmdline + "\n");
		String[] envp = {
				"TMPDIR=" + Environment.getExternalStorageDirectory().getPath(),
				"PATH=" + cctoolsDir + "/bin:" + cctoolsDir + "/sbin:/sbin:/vendor/bin:/system/sbin:/system/bin:/system/xbin",
				"ANDROID_ASSETS=/system/app",
				"ANDROID_BOOTLOGO=1",				
				"ANDROID_DATA=" + cctoolsDir + "/var/dalvik",
				"ANDROID_ROOT=/system",
				"CCTOOLSDIR=" + cctoolsDir,
				"CCTOOLSRES=" + getPackageResourcePath(),
				"LD_LIBRARY_PATH=" + cctoolsDir + "/lib:/system/lib:/vendor/lib",
				"HOME=" + cctoolsDir + "/home",
				"SHELL=" + getShell(cctoolsDir),
				"TERM=xterm",
				"PS1=$ ",
				};
		String[] argv = cmdline.split("\\s+");

		Log.i(TAG, "argv " + argv[0]);
		Log.i(TAG, "envp " + envp);
		
		isRunning = true;
		
		return new ShellTermSession(argv, envp, workdir, mMsgHandler);
	}
	
	private String getShell(String toolchainDir) {
		String[] shellList = {
				toolchainDir + "/bin/bash",
				toolchainDir + "/bin/ash",
		};
		
		for (String shell: shellList) {
			if ((new File(shell)).exists()) {
				return shell;
			}
		}

		return "/system/bin/sh";
	}
}
