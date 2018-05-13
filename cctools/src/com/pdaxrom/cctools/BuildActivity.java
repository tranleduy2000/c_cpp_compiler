package com.pdaxrom.cctools;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.pdaxrom.utils.LogItem;
import com.pdaxrom.utils.Utils;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.text.InputType;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.widget.CheckBox;
import android.widget.CompoundButton;
import android.widget.CompoundButton.OnCheckedChangeListener;
import android.widget.EditText;
import android.widget.TextView;

public class BuildActivity extends Activity {
	private static final String TAG = "cctools-build";
	private static final String PREFS_NAME = "GCCArgsFile";
	private static final String SYSTEM_SHELL = "/system/bin/sh";
	private String systemShell;
	private TextView buildLog;
	private String fileName;
	private String cctoolsDir;
	private String cmdline;
	private Thread cmdThread;
	private String workDir;
	private String outFile;
	private String tmpDir;
	private String tmpExeDir;
	private boolean forceBuild;
	private boolean forceRun;
	private boolean runExe;
	private boolean buildNativeActivity;
	private boolean execJava;
	private boolean execObjC;
	
	private int mProcId;
	private FileDescriptor mFd;
	private int mExitCode;

	private String runme_ca;
	private String runme_na;

	public static ArrayList<LogItem> errorsList = new ArrayList<LogItem>();
	
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        requestWindowFeature(Window.FEATURE_INDETERMINATE_PROGRESS);
        setContentView(R.layout.build);
        
        systemShell	= new String("SHELL=" + SYSTEM_SHELL);
        buildLog	= (TextView) findViewById(R.id.buildLog);
        cmdThread	= (Thread) getLastNonConfigurationInstance();
        fileName	= getIntent().getExtras().getString("filename");
        cctoolsDir	= getIntent().getExtras().getString("cctoolsdir");
        workDir		= (new File(fileName)).getParentFile().toString();
        tmpDir		= getIntent().getExtras().getString("tmpdir");
        forceBuild	= getIntent().getExtras().getBoolean("force", false);
        forceRun	= false;
        tmpExeDir	= getCacheDir().getParentFile().getAbsolutePath() + "/root" + "/tmp";
        if (!(new File(tmpExeDir)).exists()) {
        	(new File(tmpExeDir)).mkdir();
        }

        runme_ca = tmpDir + "/runme_ca";
        runme_na = tmpDir + "/runme_na";
        
        if ((new File(runme_ca)).exists()) {
        	(new File(runme_ca)).delete();
        }

        if ((new File(runme_na)).exists()) {
        	(new File(runme_na)).delete();
        }
        
        Log.i(TAG, "shell " + systemShell);
        Log.i(TAG, "filename " + fileName);
        Log.i(TAG, "cctoolspath " + cctoolsDir);
        Log.i(TAG, "workdir " + workDir);
        Log.i(TAG, "tmpexedir " + tmpExeDir);

        SharedPreferences mPrefs = getSharedPreferences(CCToolsActivity.SHARED_PREFS_NAME, 0);
        buildLog.setTextSize(Float.valueOf(mPrefs.getString("fontsize", "12")));
        
        showTitle(getString(R.string.buildwindow_name) + " - " + fileName);
        
        String infile = (new File(fileName)).getName();
        if (infile.contentEquals("Makefile") || infile.contentEquals("makefile")) {
        	cmdline = "make" + " " + systemShell;
        	argsDialog(getString(R.string.make_title), getString(R.string.make_args));
        	return;
        } else {
        	int dotPos = infile.lastIndexOf(".");
        	if (dotPos != -1) {
        		execJava = false;
        		execObjC = false;
            	String ext = infile.substring(dotPos);
            	outFile = infile.substring(0, dotPos);
            	Log.i(TAG, "extension [" + ext + "]");
            	if (ext.contentEquals(".mk") || ext.contentEquals(".mak")) {
            		cmdline = "make -f " + infile + " " + systemShell;
                	argsDialog(getString(R.string.make_title), getString(R.string.make_args));
                	return;
            	} else if (ext.contentEquals(".c") || ext.contentEquals(".s") ||
            			   ext.endsWith(".m")) {
            		cmdline = "gcc " + infile;
            		if (forceBuild) {
            			cmdline += " " + mPrefs.getString("force_ccopts", "");
            		}
            	} else if (ext.contentEquals(".c++") || ext.contentEquals(".cpp") ||
            			   ext.endsWith(".mm")) {
            		cmdline = "g++ " + infile;
            		if (forceBuild) {
            			cmdline += " " + mPrefs.getString("force_cxxopts", "");
            		}
            	} else if ((ext.contentEquals(".f") || ext.contentEquals(".f90") ||
            				ext.contentEquals(".f95") || ext.contentEquals(".f03")) &&
            			(new File(cctoolsDir + "/bin/f77")).exists()) {
            		cmdline = "f77 " + infile;
            		if (forceBuild) {
            			cmdline += " " + mPrefs.getString("force_ccopts", "");
            		}
            	} else if ((new File(cctoolsDir + "/bin/javac")).exists() &&
            				ext.contentEquals(".java")) {
            		cmdline = "javac-single " + outFile;
            		execJava = true;
//            		if (forceBuild) {
//            			cmdline += " " + mPrefs.getString("force_ccopts", "");
//            		}
            	}
            	
            	if (ext.equals(".m") || ext.equals(".mm")) {
            		execObjC = true;
            	}
        	}
        }

        if (cmdline != null && !cmdline.contentEquals("")) {
        	gccDialog(getString(R.string.gcc_title));
        	return;
        }
    	Log.i(TAG, "Unknown filetype, nothing to do");
    	output(getString(R.string.unknown_filetype) + "\n");
    	output(getString(R.string.known_filetypes) + "\n");
    }
    
	public Object onRetainNonConfigurationInstance() {
		return cmdThread;
	}

	protected void onDestroy() {
		Log.i(TAG, "Finish cmdline thread before activity exit");
		if (cmdThread != null && cmdThread.isAlive()) {
			cmdThread.interrupt();
			Utils.hangupProcessGroup(mProcId);
		}
		super.onDestroy();
	}

	private void argsDialog(String title, String message) {
		final EditText input = new EditText(this);
		input.setInputType(InputType.TYPE_CLASS_TEXT);
		input.setSingleLine(true);

		if (forceBuild) {
            cmdThread = new MyThread();
            cmdThread.start();
            return;
		}
		
		new AlertDialog.Builder(this)
	    .setTitle(title)
	    .setMessage(message)
	    .setView(input)
	    .setPositiveButton(getString(R.string.button_continue), new DialogInterface.OnClickListener() {
	        public void onClick(DialogInterface dialog, int whichButton) {
	            String cmdargs = input.getText().toString();
	            cmdline += " " + cmdargs;
	            cmdThread = new MyThread();
	            cmdThread.start();
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

	private void gccDialog(String title) {
		if (forceBuild) {
			SharedPreferences mPrefs = getSharedPreferences(CCToolsActivity.SHARED_PREFS_NAME, 0);
			buildNativeActivity = mPrefs.getBoolean("force_native_activity", false);
			runExe = true;
			forceRun = mPrefs.getBoolean("force_run", true);
			if (execJava) {
				// nothing here yet
			} else {
	    		if (buildNativeActivity) {
	    			outFile = "lib" + outFile + ".so";
	    			cmdline += 	" -I" + cctoolsDir + "/sources/native_app_glue"
	    						+ " " + cctoolsDir + "/sources/native_app_glue/android_native_app_glue.c"
	    						+ " -o " + outFile
	    						+ " -Wl,-soname," + outFile + " -shared"
	    						+ " -Wl,--no-undefined -Wl,-z,noexecstack"
	    						+ " -llog -landroid -lm";
	    		} else {
	    			cmdline += " -o " + outFile;
	    		}
	    		if (execObjC) {
	    			cmdline += " -lobjc";
	    		}
			}
            cmdThread = new MyThread();
            cmdThread.start();
            return;
		}
		//View view = getActivity().getLayoutInflater().inflate(R.layout.gccargs, null);
		final View view = LayoutInflater.from(this).inflate(R.layout.gccargs, null);
		final EditText cb_edit = (EditText) view.findViewById(R.id.gccargs);
		final CheckBox cb_link = (CheckBox) view.findViewById(R.id.gccbuildexe);
		final CheckBox cb_native = (CheckBox) view.findViewById(R.id.gccnativeactivity);
		final CheckBox cb_run = (CheckBox) view.findViewById(R.id.gccrunexe);
		
		if (Build.VERSION.SDK_INT < 9)
			((CheckBox) view.findViewById(R.id.gccnativeactivity)).setEnabled(false);
		
		cb_run.setEnabled(false);
		cb_link.setOnCheckedChangeListener(new OnCheckedChangeListener() {
				public void onCheckedChanged(CompoundButton buttonView,
						boolean isChecked) {
					//cb_native.setEnabled(isChecked);
					cb_run.setEnabled(isChecked);
				}
		});
		
		SharedPreferences settings = getSharedPreferences(PREFS_NAME, 0);
		cb_edit.setText(settings.getString("gcc_edit", ""));
		cb_link.setChecked(settings.getBoolean("gcc_link", false));
		cb_native.setChecked(settings.getBoolean("gcc_native", false));
		cb_run.setChecked(settings.getBoolean("gcc_run", false));
		
		new AlertDialog.Builder(this)
	    .setTitle(title)
	    .setView(view)
	    .setPositiveButton(getString(R.string.button_continue), new DialogInterface.OnClickListener() {
	        public void onClick(DialogInterface dialog, int whichButton) {
	        	String gccargs = ((EditText) view.findViewById(R.id.gccargs)).getText().toString();
	        	boolean buildexe = ((CheckBox) view.findViewById(R.id.gccbuildexe)).isChecked();
	        	buildNativeActivity = ((CheckBox) view.findViewById(R.id.gccnativeactivity)).isChecked();
	        	runExe = ((CheckBox) view.findViewById(R.id.gccrunexe)).isChecked();

	        	SharedPreferences settings = getSharedPreferences(PREFS_NAME, 0);
	        	SharedPreferences.Editor editor = settings.edit();
	        	
	        	if (!gccargs.contentEquals(settings.getString("gcc_edit", ""))) {
	        		editor.putString("gcc_edit", gccargs);
	        	}	        	
	        	if (settings.getBoolean("gcc_run", false) != runExe) {
	        		editor.putBoolean("gcc_run", runExe);
	        	}
	        	if (settings.getBoolean("gcc_link", false) != buildexe) {
	        		editor.putBoolean("gcc_link", buildexe);
	        	}
	        	if (settings.getBoolean("gcc_native", false) != buildNativeActivity) {
	        		editor.putBoolean("gcc_native", buildNativeActivity);
	        	}
	        	
	        	editor.commit();
	        	
	        	if (buildexe) {
	        		if (buildNativeActivity) {
	        			outFile = "lib" + outFile + ".so";
	        			cmdline += 	" -I" + cctoolsDir + "/sources/native_app_glue"
	        						+ " " + cctoolsDir + "/sources/native_app_glue/android_native_app_glue.c"
	        						+ " -o " + outFile
	        						+ " -Wl,-soname," + outFile + " -shared"
	        						+ " -Wl,--no-undefined -Wl,-z,noexecstack"
	        						+ " -llog -landroid";
	        		} else {
	        			cmdline += " -o " + outFile;
	        		}
		    		if (execObjC) {
		    			cmdline += " -lobjc";
		    		}
	        	} else {
	        		if (buildNativeActivity) {
	        			cmdline += " -I" + cctoolsDir + "/sources/native_app_glue";
	        		}
	        		cmdline += " -c";
	        		runExe = false;
	        	}
	            cmdline += " " + gccargs;
	            cmdThread = new MyThread();
	            cmdThread.start();
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

    private Handler handler = new Handler();
    
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
    			buildLog.append(str);
    		}
    	};
    	handler.post(proc);
    }
    
    private void show_progress(final boolean status) {
    	Runnable proc = new Runnable() {
    		public void run() {
    	        setProgressBarIndeterminateVisibility(status);
    		}
    	};
    	handler.post(proc);
    }
    
    public class MyThread extends Thread {
    	public void run() {
    		try {
    	        show_progress(true);
    			Log.i(TAG, "execute " + cmdline + "\n");
//    			output(cmdline + "\n");
    			String[] envp = {
    					"PWD=" + workDir,
    					"TMPDIR=" + tmpDir,
    					"PATH=" + cctoolsDir + "/bin:" + cctoolsDir + "/sbin:/sbin:/vendor/bin:/system/sbin:/system/bin:/system/xbin",
    					"ANDROID_ASSETS=/system/app",
    					"ANDROID_BOOTLOGO=1",				
    					"ANDROID_DATA=" + cctoolsDir + "/var/dalvik",
    					"ANDROID_ROOT=/system",
    					"CCTOOLSDIR=" + getCacheDir().getParentFile().getAbsolutePath() + "/root" + "/cctools",
    					"CCTOOLSRES=" + getPackageResourcePath(),
    					"LD_LIBRARY_PATH=" + cctoolsDir + "/lib:/system/lib:/vendor/lib",
    					"HOME=" + cctoolsDir + "/home",
    					"TMPEXEDIR=" + tmpExeDir,
    					"PS1=''"
    					};
    			String shell = "/system/bin/sh";
    	        shell = shell.replaceAll("\\s+", " ");
    			String[] argv = shell.split("\\s+");
    			int[] pId = new int[1];
    			mFd = Utils.createSubProcess(workDir, argv[0], argv, envp, pId);
    			mProcId = pId[0];
    			if (mProcId > 0) {
        			try {
        				Utils.setPtyUTF8Mode(mFd, true);
        				Utils.setPtyWindowSize(mFd, 64, 128, 0, 0);
        				BufferedReader procout = new BufferedReader(new InputStreamReader(new FileInputStream(mFd)));
        				FileOutputStream procin = new FileOutputStream(mFd);
        				Thread execThread = new Thread() {
        					public void run() {
        						Log.i(TAG, "Waiting for hangup session");
        						mExitCode = Utils.waitFor(mProcId);
        						Log.i(TAG, "Subprocess exited: " + mExitCode);
        					}
        				};
        				execThread.start();
        				procin.write(new String("export PS1=''\n").getBytes());
        				Pattern pat1 = Pattern.compile("^(\\S+):(\\d+):(\\d+): (\\S+|\\S+ \\S+): (.*)$");
        				Pattern pat2 = Pattern.compile("^(\\S+):(\\d+): (\\S+|\\S+ \\S+): (.*)$");
        				Pattern patClearNewLine = Pattern.compile("(\\x08)\\1+");
        				errorsList.clear();
        				cmdline = "exec " + cmdline + "\n";
        				procin.write(cmdline.getBytes());
        				int skipStrings = 3; //skip echos from two command strings
        				do {
        					String errstr = null;
        					try {
        						errstr = procout.readLine();
        						// remove escape sequence
        						errstr = errstr.replaceAll("\u001b\\[([0-9]|;)*m", "");
        						// remove clearing new line
        						Matcher m = patClearNewLine.matcher(errstr);
        						if (m.find()) {
        							int length = m.end() - m.start();
        							if (m.start() > length) {
        								errstr = errstr.substring(0, m.start() - length) + errstr.substring(m.end());
        							}
        						}
        					} catch (IOException e) {
        						break;
        					}
    						if (errstr == null) {
    							break;
    						}
    						Matcher m = pat1.matcher(errstr);
    						if (m.find()) {
    							Log.e(TAG, "out " + m.group(1) + "|" + m.group(2) + "|" + m.group(3) + "|" + m.group(4) + "|" + m.group(5));
    							int idx = errorsList.size() - 1;
    							if (idx >= 0 &&
    								errorsList.get(idx).getFile().contentEquals(m.group(1)) &&
    								errorsList.get(idx).getType().contentEquals(m.group(4)) &&
    								errorsList.get(idx).getLine() == Integer.parseInt(m.group(2)) &&
    								errorsList.get(idx).getPos() == Integer.parseInt(m.group(3))) {
    								errorsList.get(idx).setMessage(errorsList.get(idx).getMessage() + " " + m.group(5));
    							} else {
    								LogItem item = new LogItem(m.group(4), m.group(1), Integer.parseInt(m.group(2)), Integer.parseInt(m.group(3)), m.group(5));
    								errorsList.add(item);
    							}
    						} else {
    							m = pat2.matcher(errstr);
    							if (m.find()) {
        							Log.e(TAG, "out " + m.group(1) + "|" + m.group(2) + "|" + m.group(3) + "|" + m.group(4));    								
        							int idx = errorsList.size() - 1;
        							if (idx >= 0 &&
        								errorsList.get(idx).getFile().contentEquals(m.group(1)) &&
        								errorsList.get(idx).getType().contentEquals(m.group(3)) &&
        								errorsList.get(idx).getLine() == Integer.parseInt(m.group(2))) {
        								errorsList.get(idx).setMessage(errorsList.get(idx).getMessage() + " " + m.group(4));
        							} else {
        								LogItem item = new LogItem(m.group(3), m.group(1), Integer.parseInt(m.group(2)), -1, m.group(4));
        								errorsList.add(item);
        							}
    							}
    						}
    						if (skipStrings > 0) {
    							skipStrings--;
    						} else {
    							output(errstr + "\n");
    						}
    						Log.i(TAG, errstr);
        				} while(execThread.isAlive());
						if (mExitCode != 0) {
							output(getString(R.string.build_error) + " " + mExitCode + "\n");
					        showTitle(getString(R.string.buildwindow_name_error) + " - " + fileName);
						} else
					        showTitle(getString(R.string.buildwindow_name_done) + " - " + fileName);

						Log.e(TAG, "process exit code " + mExitCode);
						procin.close();
        				procout.close();
        			} catch (IOException ie) {
        				Log.e(TAG, "exception " + ie);
        			}    				
    			}    			
    		} catch (Exception ie) {
    			Log.e(TAG, "exec() " + ie);
    		}
    		output("\n" + getString(R.string.build_done) +"\n\n");
            show_progress(false);

            String runme = checkAndRead(runme_na);
            if (runme != null) {
            	outFile = (new File(runme)).getName();
            	workDir = (new File(runme)).getParent();
            	buildNativeActivity = true;
            	runExe = true;
            	mExitCode = 0;
            }

            runme = checkAndRead(runme_ca);
            if (runme != null) {
            	outFile = (new File(runme)).getName();
            	workDir = (new File(runme)).getParent();
            	buildNativeActivity = false;
            	runExe = true;
            	mExitCode = 0;
            }
            
            String javaClass = "";
            if (execJava) {
            	javaClass = outFile;
            	outFile = outFile + ".jar";
            }
            
            if (runExe && (new File(workDir + "/" + outFile).exists()) && mExitCode == 0) {
            	if (buildNativeActivity && Build.VERSION.SDK_INT >= 9) {
            		Intent i = new Intent(BuildActivity.this, LauncherNativeActivity.class);
            		i.putExtra("activity_file", workDir + "/" + outFile);
            		startActivity(i);
            	} else {
            		Intent i = new Intent(BuildActivity.this, LauncherConsoleActivity.class);
            		if (execJava) {
            			i.putExtra("executable_file", cctoolsDir + "/bin/java -cp " + workDir + "/" + outFile + " " + javaClass);
            		} else {
            			i.putExtra("executable_file", workDir + "/" + outFile);
            		}
            		i.putExtra("cctoolsdir", cctoolsDir);
            		i.putExtra("force", forceRun);
            		startActivity(i);
            	}
            }
    	}
    }
    
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
		}
		f.delete();
    	return null;
    }
}
