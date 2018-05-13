package com.pdaxrom.cctools;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.os.Environment;
import android.text.InputType;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

import com.actionbarsherlock.app.SherlockActivity;
import com.pdaxrom.utils.FileDialog;
import com.pdaxrom.utils.SelectionMode;
import com.pdaxrom.utils.Utils;
import com.pdaxrom.utils.XMLParser;

public class FlexiDialogActivity extends SherlockActivity {
	private final static String TAG = "FlexiDialog";
	
	private final static int REQUEST_DIALOG_FILE_SELECTOR = 1000;
	private final static int MAX_REQUESTS_FROM_FILE_DIALOG = 20;
	
	protected static final String PKGS_LISTS_DIR = "/installed/";

	private Context context = this;
	
    List<NamedView> namedViews = null;
    private int fileSelectorId;
    
	private String toolchainDir;
	private String sdHomeDir;
	private String tmpDir;
	private String filesDir;
	private String serviceDir;
	
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        sdHomeDir = Environment.getExternalStorageDirectory().getPath() + "/CCTools";
        tmpDir = sdHomeDir + "/tmp";
        filesDir = sdHomeDir + "/backup";

        if (!(new File(sdHomeDir)).exists()) {
        	(new File(sdHomeDir)).mkdir();
        }
        if (!(new File(tmpDir)).exists()) {
        	(new File(tmpDir)).mkdir();
        }
        if (!(new File(filesDir)).exists()) {
        	(new File(filesDir)).mkdir();
        }
        
        toolchainDir = getCacheDir().getParentFile().getAbsolutePath() + "/root";
        if (!(new File(toolchainDir)).exists()) {
        	(new File(toolchainDir)).mkdir();
        }
        
        String dalvikCache = toolchainDir + "/cctools/var/dalvik/dalvik-cache";
        if (!(new File(dalvikCache)).exists()) {
        	(new File(dalvikCache)).mkdirs();
        }
        
        updateClassPathEnv();
        
        if (!(new File(toolchainDir + PKGS_LISTS_DIR)).exists()) {
        	(new File(toolchainDir + PKGS_LISTS_DIR)).mkdir();
        }

        serviceDir = toolchainDir + "/cctools/services";
        if (!(new File(serviceDir)).exists()) {
        	(new File(serviceDir)).mkdir();
        }

    }
    
    protected String getToolchainDir() {
    	return toolchainDir;
    }
    
    protected String getTempDir() {
    	return tmpDir;
    }
    
    protected String getSDHomeDir() {
    	return sdHomeDir;
    }
    
    protected String getServiceDir() {
    	return serviceDir;
    }
    
    public class NamedView {
    	private View	view;
    	private String	name;
    	private int		id;
    	
    	NamedView(View view, String name) {
    		this.view = view;
    		this.name = name;
    		this.id = -1;
    	}

    	NamedView(View view, String name, int id) {
    		this.view = view;
    		this.name = name;
    		this.id = id;
    	}
    	
    	public View getView() {
    		return view;
    	}
    	
    	public String getName() {
    		return name;
    	}
    	
    	public int getId() {
    		return id;
    	}
    }

    private View getNamedView(List<NamedView> views, String name) {
    	for (NamedView view: views) {
    		if (view.getName().equals(name)) {
    			return view.getView();
    		}
    	}
    	return null;
    }

    private View getNamedView(List<NamedView> views, int id) {
    	for (NamedView view: views) {
    		if (view.getId() == id) {
    			return view.getView();
    		}
    	}
    	return null;
    }

    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
    	super.onActivityResult(requestCode, resultCode, data);
    	
		if (resultCode == RESULT_OK) {
			if (requestCode >= REQUEST_DIALOG_FILE_SELECTOR &&
					requestCode < REQUEST_DIALOG_FILE_SELECTOR +
						MAX_REQUESTS_FROM_FILE_DIALOG) {
				EditText edit = (EditText) getNamedView(namedViews, 
						requestCode - REQUEST_DIALOG_FILE_SELECTOR);
				if (edit != null) {
					edit.setText(data.getStringExtra(FileDialog.RESULT_PATH));
				}
			}
		}

    }
    
    /**
     * Create Dialog popup window from xml template
     * @param ruleFile xml template file
     * @param lastOpenedDir last opened directory
     */
    protected void dialogFromRule(String ruleFile, final String lastOpenedDir) {
		XMLParser xmlParser = new XMLParser();
		String xml = xmlParser.getXmlFromFile(ruleFile);
		if (xml != null) {
			Document doc = xmlParser.getDomElement(xml);
			if (doc != null) {
				NodeList nl = doc.getElementsByTagName("new");
				Element e = (Element) nl.item(0);
				String title = e.getAttribute("title");
				nl = e.getElementsByTagName("view");

				namedViews = new ArrayList<NamedView>();
				
		        TableLayout table = new TableLayout(context);
		        table.setColumnStretchable(1, true);
		        
		        fileSelectorId = 0;
				for (int i = 0; i < nl.getLength(); i++) {
					Element ne = (Element) nl.item(i);
					Log.i(TAG, "-- " + ne.getTagName());
					Log.i(TAG, "--- " + ne.getAttribute("type"));
					Log.i(TAG, "--- " + ne.getAttribute("title"));
					Log.i(TAG, "--- " + ne.getAttribute("name"));
					if (ne.getAttribute("type").equals("edit")) {
						TableRow row = new TableRow(context);
						
						TextView view = new TextView(context);
						view.setText(ne.getAttribute("title"));
						
						EditText edit = new EditText(context);
						edit.setInputType(edit.getInputType() & ~InputType.TYPE_TEXT_FLAG_MULTI_LINE);
						edit.setHint(ne.getAttribute("hint"));

						namedViews.add(new NamedView(edit, ne.getAttribute("name")));

						row.addView(view);
						row.addView(edit);
						table.addView(row);
					} else if (ne.getAttribute("type").equals("dirpath") || ne.getAttribute("type").equals("filepath")) {
						TableRow row = new TableRow(context);

						TextView view = new TextView(context);
						view.setText(ne.getAttribute("title"));
						
						EditText edit = new EditText(context);
						edit.setInputType(edit.getInputType() & ~InputType.TYPE_TEXT_FLAG_MULTI_LINE);
						edit.setHint(ne.getAttribute("hint"));
						
						namedViews.add(new NamedView(edit, ne.getAttribute("name"), fileSelectorId));
						
						ImageButton button = new ImageButton(context);
						button.setImageResource(R.drawable.folder);
						
						if (ne.getAttribute("type").equals("dirpath")) {
							button.setOnClickListener(new OnClickListener() {
								private int id = fileSelectorId++;
								
					        	public void onClick(View v) {
					        		Intent intent = new Intent(getBaseContext(), FileDialog.class);
					        		String dir = lastOpenedDir;
					        		if (dir == null || !new File(dir).exists()) {
					        			dir = Environment.getExternalStorageDirectory().getPath();
					        		}
					        		intent.putExtra(FileDialog.START_PATH, dir);
					        		intent.putExtra(FileDialog.SELECTION_MODE, SelectionMode.MODE_SELECT_DIR);
					        		startActivityForResult(intent, REQUEST_DIALOG_FILE_SELECTOR + id);    	
					        		
					        	}
					        });
						} else {
							button.setOnClickListener(new OnClickListener() {
								private int id = fileSelectorId++;
								
					        	public void onClick(View v) {
					        		Intent intent = new Intent(getBaseContext(), FileDialog.class);
					        		String dir = lastOpenedDir;
					        		if (dir == null || !new File(dir).exists()) {
					        			dir = Environment.getExternalStorageDirectory().getPath();
					        		}
					        		intent.putExtra(FileDialog.START_PATH, dir);
					        		intent.putExtra(FileDialog.SELECTION_MODE, SelectionMode.MODE_OPEN);
					        		startActivityForResult(intent, REQUEST_DIALOG_FILE_SELECTOR + id);    	
					        		
					        	}
					        });							
						}
						
						row.addView(view);
						row.addView(edit);
						row.addView(button);
						table.addView(row);
					}
				}
				
				nl = e.getElementsByTagName("command");
				e = (Element) nl.item(0);
				final String execAttr = e.getAttribute("exec");
				
				new AlertDialog.Builder(context)
				.setTitle(title)
//				.setMessage(title)
				.setView(table)
				.setPositiveButton(getText(R.string.button_continue), new DialogInterface.OnClickListener() {
					private String exec = execAttr;
					public void onClick(DialogInterface dialog, int which) {
						exec.replaceAll("\\s+", " ");
						String[] argv = exec.split("\\s+");
						for (int i = 0; i < argv.length; i++) {
							if (argv[i].startsWith("@") && argv[i].endsWith("@")) {
								String var = argv[i].substring(1, argv[i].length() - 1);
								EditText edit = (EditText) getNamedView(namedViews, var);
								if (edit != null) {
									argv[i] = edit.getText().toString();
								}
							}
							
							Log.i(TAG, ":: " + argv[i]);
						}
						system(argv, false);
						namedViews = null;
					}
				})
				.setCancelable(true)
				.show();

			}
		}
    }
    
    /**
     * Execute a shell command
     * @param argv
     */
    protected void system(String[] argv) {
    	system(argv, true);
    }
    
    /**
     * Execute a shell command
     * @param argv
     * @param waitForFinish
     */
	protected void system(String[] argv, boolean waitForFinish) {
		String cctoolsDir = toolchainDir + "/cctools";
		String bootClassPath = getEnv(cctoolsDir, "BOOTCLASSPATH");
		if (bootClassPath == null) {
			bootClassPath = Utils.getBootClassPath();
		}
		if (bootClassPath == null) {
			bootClassPath = "/system/framework/core.jar:/system/framework/ext.jar:/system/framework/framework.jar:/system/framework/android.policy.jar:/system/framework/services.jar"; 
		}
		String[] envp = {
				"TMPDIR=" + Environment.getExternalStorageDirectory().getPath(),
				"PATH=" + cctoolsDir + "/bin:" + cctoolsDir + "/sbin:/sbin:/vendor/bin:/system/sbin:/system/bin:/system/xbin",
				"ANDROID_ASSETS=/system/app",
				"ANDROID_BOOTLOGO=1",				
				"ANDROID_DATA=" + cctoolsDir + "/var/dalvik",
				"ANDROID_PROPERTY_WORKSPACE=" + getEnv(cctoolsDir, "ANDROID_PROPERTY_WORKSPACE"),
				"ANDROID_ROOT=/system",
				"BOOTCLASSPATH=" + bootClassPath,
				"CCTOOLSDIR=" + cctoolsDir,
				"CCTOOLSRES=" + getPackageResourcePath(),
				"LD_LIBRARY_PATH=" + cctoolsDir + "/lib:/system/lib:/vendor/lib",
				"HOME=" + cctoolsDir + "/home",
				"SHELL=" + getShell(),
				"TERM=xterm",
				"PS1=$ ",
				"SDDIR=" + sdHomeDir,
				"EXTERNAL_STORAGE=" + Environment.getExternalStorageDirectory().getPath(),
				};
		try {
			Process p = Runtime.getRuntime().exec(argv, envp);
			if (waitForFinish) {
				BufferedReader in = new BufferedReader(  
						new InputStreamReader(p.getErrorStream()));  
				String line = null;  
				while ((line = in.readLine()) != null) {  
					Log.i(TAG, "stderr: " + line);
				}			
				p.waitFor();
			}
		} catch (Exception e) {
			Log.i(TAG, "Exec exception " + e);
		}		
	}
	
	protected void updateClassPathEnv() {
		String cpEnvDir = toolchainDir + "/cctools/etc";
		if (! (new File(cpEnvDir)).exists()) {
			(new File(cpEnvDir)).mkdirs();
		}
		try {
			String env = "CCTOOLSCP=" + getPackageResourcePath() + "\n";
			FileOutputStream outf = new FileOutputStream(cpEnvDir + "/cp.env");
			outf.write(env.getBytes());
			outf.close();
		} catch (IOException e) {
			Log.e(TAG, "create cp.env " + e);
		}	
	}
	
	protected String getEnv(String baseDir, String variable) {
		String ret = null;
		String[] envp = {
				"TMPDIR=" + Environment.getExternalStorageDirectory().getPath(),
				"PATH=" + baseDir + "/bin:" + baseDir + "/sbin:/sbin:/vendor/bin:/system/sbin:/system/bin:/system/xbin",
				"ANDROID_ASSETS=/system/app",
				"ANDROID_BOOTLOGO=1",				
				"ANDROID_DATA=" + baseDir + "/var/dalvik",
				"ANDROID_ROOT=/system",
				"CCTOOLSDIR=" + baseDir,
				"CCTOOLSRES=" + getPackageResourcePath(),
				"LD_LIBRARY_PATH=" + baseDir + "/lib",
				"HOME=" + baseDir + "/home",
				"SHELL=" + getShell(),
				"TERM=xterm",
				"PS1=$ ",
				"SDDIR=" + sdHomeDir,
				"EXTERNAL_STORAGE=" + Environment.getExternalStorageDirectory().getPath(),
				};
		String[] argv = { "/system/bin/sh", "-c", "set"};
		int[] pId = new int[1];
		FileDescriptor fd = Utils.createSubProcess(baseDir, argv[0], argv, envp, pId);
		FileInputStream fis = new FileInputStream(fd);
		DataInputStream in = new DataInputStream(fis);
		BufferedReader reader = new BufferedReader(new InputStreamReader(in));
		String line = "";
		try {
			while((line = reader.readLine()) != null) {
				if (line.startsWith(variable + "=")) {
					if (line.indexOf("=") != -1) {
						ret = line.substring(line.indexOf("=") + 1);
						break;
					}
				}
			}
			in.close();
			Utils.waitFor(pId[0]);
		} catch (Exception e) {
			Log.e(TAG, "exception " + e);
		}
		return ret;
	}
	
	protected String getShell() {
		String[] shellList = {
				toolchainDir + "/cctools/bin/bash",
				toolchainDir + "/cctools/bin/ash",
		};
		
		for (String shell: shellList) {
			if ((new File(shell)).exists()) {
				return shell;
			}
		}
		return "/system/bin/sh";
	}
}