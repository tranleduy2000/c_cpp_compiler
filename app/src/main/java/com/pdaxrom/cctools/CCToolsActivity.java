/*
 * Copyright 2018 Mr Duy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.pdaxrom.cctools;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.SharedPreferences.OnSharedPreferenceChangeListener;
import android.content.pm.PackageInfo;
import android.content.pm.PackageManager.NameNotFoundException;
import android.net.Uri;
import android.os.AsyncTask;
import android.os.Bundle;
import android.text.method.LinkMovementMethod;
import android.text.util.Linkify;
import android.util.Log;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.ViewFlipper;

import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.packagemanager.PackageManagerActivity;
import com.pdaxrom.utils.Utils;
import com.pdaxrom.utils.XMLParser;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.BindException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;

public class CCToolsActivity extends FlexiDialogActivity implements OnSharedPreferenceChangeListener {
    public static final String SHARED_PREFS_NAME = "cctoolsSettings";
    private static final String SHARED_PREFS_FILES_EDITPOS = "FilesPosition";
    private static final String website_url = "http://cctools.info";
    private static final int REQUEST_OPEN = 1;
    private static final int REQUEST_SAVE = 2;
    private static final int WARN_SAVE_AND_NEW = 1;
    private static final int WARN_SAVE_AND_LOAD = 2;
    private static final int WARN_SAVE_AND_LOAD_POS = 3;
    private static final int WARN_SAVE_AND_BUILD = 4;
    private static final int WARN_SAVE_AND_BUILD_FORCE = 5;
    private static final int WARN_SAVE_AND_CLOSE = 6;
    private static final int SERVICE_STOP = 0;
    private static final int SERVICE_START = 1;
    public static Socket dialogServiceSocket;
    private Context context = this;
    private SharedPreferences mPrefs;
    private String TAG = "cctools";
    private String buildBaseDir; // Project base directory
    private boolean buildAfterSave = false;
    private boolean buildAfterLoad = false;
    private View buttBar;
    private ViewFlipper flipper;
    private boolean forceTmpVal;
    private String showFileName;
    private int showFileLine;
    private int showFilePos;

    private Thread dialogServiceThread;
    private ServerSocket dialogServerSocket;
    private int toolchainPackageToInstall = 0;

    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_cctools);


        mPrefs = getSharedPreferences(SHARED_PREFS_NAME, 0);

        flipper = findViewById(R.id.flipper);

        showInfoAndCheckToolchain();
        buttBar = findViewById(R.id.toolButtonsBar);

        dialogServiceThread = dialogService(13527);
        serviceStartStop(SERVICE_START);

        mPrefs.registerOnSharedPreferenceChangeListener(this);
        onSharedPreferenceChanged(mPrefs, null);

        // Get intent, action and MIME type
        Intent intent = getIntent();
        String action = intent.getAction();
        String type = intent.getType();

        if ((Intent.ACTION_VIEW.equals(action) ||
                Intent.ACTION_EDIT.equals(action)) &&
                type != null) {
            if (type.startsWith("text/")) {
                Uri uri = intent.getData();
                String fileName = uri.getPath();
                Log.i(TAG, "Load external file " + fileName);
            }
        }
    }

    @Override
    protected void onDestroy() {
        serviceStartStop(SERVICE_STOP);

        if (dialogServiceThread.isAlive()) {
            Log.i(TAG, "Stop dialog service");
            dialogServiceThread.interrupt();
        }
        if ((dialogServerSocket != null) &&
                (!dialogServerSocket.isClosed())) {
            try {
                dialogServerSocket.close();
            } catch (IOException e) {
                Log.e(TAG, "Error close dialogServerSocket " + e);
            }
        }

        Log.i(TAG, "Clean temp directory");
        Utils.deleteDirectory(new File(getToolchainDir() + "/tmp"));
        super.onDestroy();
    }

    private String getPrefString(String key) {
        SharedPreferences settings = getPreferences(MODE_PRIVATE);
        return settings.getString(key, android.os.Environment.getExternalStorageDirectory().getPath() + "/CCTools/Examples");
    }

    private void setPrefString(String key, String value) {
        SharedPreferences settings = getPreferences(MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.putString(key, value);
        editor.apply();
    }

    private String getLastOpenedDir() {
        return getPrefString("lastdir");
    }


    private void buildFile(boolean force) {
        File file = new File("");
        Log.i(TAG, "build activity " + file);
        if (file.exists()) {
            buildBaseDir = file.getParentFile().getAbsolutePath();
            String fileName = file.getName();
            SharedPreferences mPrefs = getSharedPreferences(CCToolsActivity.SHARED_PREFS_NAME, 0);

            if (fileName.lastIndexOf(".") != -1) {
                String ext = fileName.substring(fileName.lastIndexOf("."));
                if (ext.contentEquals(".sh")) {
                    Intent intent = new Intent(CCToolsActivity.this, LauncherConsoleActivity.class);
                    intent.putExtra(BuildConstants.EXTRA_EXEC_FILE, file.getAbsolutePath());
                    intent.putExtra(BuildConstants.EXTRA_FORCE_BUILD, force && mPrefs.getBoolean("force_run", false));
                    startActivity(intent);
                    return;
                }

                if (ext.contentEquals(".lua") && new File(getToolchainDir(), "cctools/bin/luajit").exists()) {
                    Intent intent = new Intent(CCToolsActivity.this, LauncherConsoleActivity.class);
                    intent.putExtra(BuildConstants.EXTRA_EXEC_FILE, getToolchainDir() + "/cctools/bin/luajit " + file.getAbsolutePath());
                    intent.putExtra(BuildConstants.EXTRA_WORK_DIR, file.getParentFile().getAbsolutePath());
                    intent.putExtra(BuildConstants.EXTRA_FORCE_BUILD, force && mPrefs.getBoolean("force_run", false));
                    startActivity(intent);
                    return;
                }

                if ((ext.equals(".pl") || ext.equals(".pm")) && new File(getToolchainDir() + "/cctools/bin/perl").exists()) {
                    Intent intent = new Intent(CCToolsActivity.this, LauncherConsoleActivity.class);
                    intent.putExtra(BuildConstants.EXTRA_EXEC_FILE, getToolchainDir() + "/cctools/bin/perl " + file.getAbsolutePath());
                    intent.putExtra(BuildConstants.EXTRA_WORK_DIR, file.getParentFile().getAbsolutePath());
                    intent.putExtra(BuildConstants.EXTRA_FORCE_BUILD, force && mPrefs.getBoolean("force_run", false));
                    startActivity(intent);
                    return;
                }
            }

            Intent intent = new Intent(this, BuildActivity.class);
            intent.putExtra(BuildConstants.EXTRA_FILE_NAME, file.getAbsolutePath());
            intent.putExtra(BuildConstants.EXTRA_FORCE_BUILD, force);
            startActivity(intent);
        }
    }


    private void newProject() {
        final Spinner spinner = new Spinner(this);
        List<String> list = new ArrayList<String>();
        final List<String> projects = new ArrayList<String>();

        FilenameFilter filter = new FilenameFilter() {
            public boolean accept(File dir, String name) {
                String lowercaseName = name.toLowerCase();
                return lowercaseName.endsWith(".xml");
            }
        };

        String[] rulesDirs = {
                getToolchainDir() + "/cctools/share/project",
                getSDHomeDir() + "/share/project"
        };

        for (String rulesDir : rulesDirs) {
            File dir = new File(rulesDir);
            if (dir.exists() && dir.isDirectory()) {
                for (String fileName : dir.list(filter)) {
                    XMLParser xmlParser = new XMLParser();
                    String xml = xmlParser.getXmlFromFile(rulesDir + "/" + fileName);
                    if (xml == null) {
                        continue;
                    }
                    Document doc = xmlParser.getDomElement(xml);
                    if (doc == null) {
                        Log.i(TAG, "bad xml file " + rulesDir + "/" + fileName);
                        continue;
                    }
                    NodeList nl = doc.getElementsByTagName("cctools-project");
                    Element e = (Element) nl.item(0);
                    if (e == null) {
                        continue;
                    }
                    String title = e.getAttribute("title");
                    if (title != null && !title.equals("")) {
                        list.add(title);
                        projects.add(rulesDir + "/" + fileName);
                    }
                }
            }
        }

        ArrayAdapter<String> dataAdapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, list);
        dataAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        spinner.setAdapter(dataAdapter);

        spinner.setOnItemSelectedListener(new OnItemSelectedListener() {
            public void onItemSelected(AdapterView<?> arg0, View view,
                                       int position, long id) {
                Log.i(TAG, "selected " + projects.get(position));
                toolchainPackageToInstall = position;
            }

            public void onNothingSelected(AdapterView<?> arg0) {
                // TODO Auto-generated method stub
            }
        });

        new AlertDialog.Builder(this)
                .setTitle(getText(R.string.project_new_project))
                .setMessage(getText(R.string.project_new_project_message))
                .setView(spinner)
                .setPositiveButton(getText(R.string.button_continue), new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        int i = spinner.getSelectedItemPosition();
                        Log.i(TAG, "selected project rule " + projects.get(i));

                        dialogFromRule(projects.get(i), getLastOpenedDir());
                    }
                })
                .setCancelable(true)
                .show();
    }

    private void showInfoAndCheckToolchain() {
        PackageInfo packageInfo;
        try {
            packageInfo = getPackageManager().getPackageInfo(getPackageName(), 0);
            int versionCode = packageInfo.versionCode;
            if (getPrefString("versionCode").equals(String.valueOf(versionCode))) {
                installOrUpgradeToolchain();
            } else {
                setPrefString("versionCode", String.valueOf(versionCode));
                String language = getResources().getConfiguration().locale.getLanguage();

                InputStream stream = null;
                try {
                    stream = getAssets().open("whatsnew-" + language);
                } catch (IOException e) {
                    Log.e(TAG, "Assets file whatsnew" + language + " not found");
                    stream = null;
                }
                if (stream == null) {
                    try {
                        stream = getAssets().open("whatsnew");
                    } catch (IOException e) {
                        Log.e(TAG, "Assets file whatsnew not found");
                        installOrUpgradeToolchain();
                        return;
                    }
                }

                StringBuilder buf = new StringBuilder();
                BufferedReader in = new BufferedReader(new InputStreamReader(stream));
                String str;
                String message = "";

                try {
                    while ((str = in.readLine()) != null) {
                        buf.append(str + "\n");
                    }
                    in.close();
                    message = buf.toString();
                } catch (IOException e) {
                    Log.e(TAG, "Error reading whatsnew file");
                }

                final TextView textView = new TextView(this);
                textView.setAutoLinkMask(Linkify.WEB_URLS);
                textView.setLinksClickable(true);
                textView.setMovementMethod(LinkMovementMethod.getInstance());
                textView.setText(message);

                new AlertDialog.Builder(this)
                        .setIcon(android.R.drawable.ic_dialog_info)
                        .setTitle(getString(R.string.whatisnew))
                        .setView(textView)
                        .setNeutralButton(getString(R.string.button_continue), new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int which) {
                                installOrUpgradeToolchain();
                            }
                        })
                        .setCancelable(false)
                        .show();

            }
        } catch (NameNotFoundException e) {
            Log.e(TAG, "Package name not found: " + e);
        }
    }

    private void installOrUpgradeToolchain() {
        if (!getPrefString("use_package_manager").equals("yes")) {
            (new RemoveOldToolchainTask()).execute((Void) null);
        } else if (!getPrefString("toolchain_installed").equals("yes")) {
            installToolchainPackage();
        } else {
            if (mPrefs.getBoolean("updater", true)) {
                Intent intent = new Intent(this, PackageManagerActivity.class);
                intent.putExtra(PackageManagerActivity.EXTRA_CMD, PackageManagerActivity.ACTION_UPDATE);
                startActivity(intent);
            }
        }
    }

    private void installToolchainPackage() {
        final String[] toolchainPackage = {
                "build-essential-clang-compact",
                "build-essential-gcc-compact",
                "build-essential-fortran-compact",
                "build-essential-gcc-avr",
                "build-essential-mingw-w64",
                "build-essential-luajit"
        };

        setPrefString("toolchain_installed", "yes");

        final Spinner spinner = new Spinner(context);
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this,
                R.array.toolchain_selector, android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinner.setAdapter(adapter);

        spinner.setOnItemSelectedListener(new OnItemSelectedListener() {
            public void onItemSelected(AdapterView<?> arg0, View view,
                                       int position, long id) {
                Log.i(TAG, "selected " + toolchainPackage[position]);
                toolchainPackageToInstall = position;
            }

            public void onNothingSelected(AdapterView<?> arg0) {
                // TODO Auto-generated method stub
            }
        });

        new AlertDialog.Builder(this)
                .setTitle(getText(R.string.toolchain_selector))
                .setMessage(getText(R.string.toolchain_selectormsg))
                .setView(spinner)
                .setPositiveButton(getText(R.string.pkg_install), new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        Intent intent = new Intent(context, PackageManagerActivity.class);
                        intent.putExtra(PackageManagerActivity.EXTRA_CMD, PackageManagerActivity.ACTION_INSTALL);
                        intent.putExtra(PackageManagerActivity.EXTRA_DATA, toolchainPackage[toolchainPackageToInstall]);
                        startActivity(intent);
                    }
                })
                .setCancelable(false)
                .show();
    }

    private Thread dialogService(final int port) {
        Log.i(TAG, "Launch dialog service (port " + port + ")");
        Thread t = new Thread() {
            public void run() {
                try {
                    dialogServerSocket = new ServerSocket();
                    dialogServerSocket.setReuseAddress(true);
                    dialogServerSocket.bind(new InetSocketAddress(port));
                    Log.i(TAG, "Waiting for incoming requests");
                    while (true) {
                        dialogServiceSocket = dialogServerSocket.accept();
                        Log.i(TAG, "Dialog request from " + dialogServiceSocket.getInetAddress().toString());
                        if (!dialogServiceSocket.getInetAddress().toString().equals("/127.0.0.1")) {
                            continue;
                        }
                        Intent intent = new Intent(CCToolsActivity.this, DialogWindow.class);
                        startActivity(intent);
                        Log.i(TAG, "Waiting for finish dialog activity");
                        while (!dialogServiceSocket.isClosed()) {
                            Thread.sleep(300);
                        }
                        Log.i(TAG, "Dialog activity finished");
                    }
                } catch (BindException e) {
                    Log.e(TAG, "bind failed, try again");
                } catch (IOException e) {
                    Log.e(TAG, "ServerSocket " + e);
                } catch (InterruptedException e) {
                    Log.e(TAG, "Interrupted " + e);
                }
            }
        };
        t.start();
        return t;
    }

    private void serviceStartStop(final int cmd) {
        String serviceCmd;
        if (cmd == SERVICE_START) {
            serviceCmd = "start";
        } else {
            serviceCmd = "stop";
        }
        Log.i(TAG, "Console services " + serviceCmd);
        File dir = new File(getServiceDir());
        if (dir.exists()) {
            String services[] = dir.list();
            for (final String service : services) {
                Log.i(TAG, "Service " + service + " " + serviceCmd);
                new Thread() {
                    public void run() {
                        String serviceCmd;
                        if (cmd == SERVICE_START) {
                            serviceCmd = "start";
                        } else {
                            serviceCmd = "stop";
                        }
                        String[] argv = {
                                getServiceDir() + "/" + service,
                                serviceCmd
                        };
                        system(argv);
                    }
                }.start();
            }
        }
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {

    }

    private class RemoveOldToolchainTask extends AsyncTask<Void, Void, Void> {
        protected void onPreExecute() {
            super.onPreExecute();
        }

        protected Void doInBackground(Void... params) {
            final String[] oldPackages = {
                    "cctools-examples-1.00.zip", "cctools-examples-1.01.zip", "cctools-examples-1.02.zip",
                    "cctools-examples-1.03.zip", "platform-arm-3.zip", "platform-arm-8.zip",
                    "platform-mips-14.zip", "platform-x86-14.zip", "toolchain-arm.zip",
                    "platform-arm-14.zip", "platform-arm-4.zip", "platform-arm-9.zip",
                    "platform-mips-18.zip", "platform-x86-18.zip", "toolchain-mips.zip",
                    "platform-arm-18.zip", "platform-arm-5.zip", "platform-common.zip",
                    "platform-mips-9.zip", "platform-x86-9.zip", "toolchain-x86.zip"};

            if (getPrefString("use_package_manager").equals("yes")) {
                return null;
            }

            FilenameFilter filter = new FilenameFilter() {
                public boolean accept(File dir, String name) {
                    String lowercaseName = name.toLowerCase();
                    for (String file : oldPackages) {
                        if (lowercaseName.equals(file + ".list")) {
                            return true;
                        }
                    }
                    return false;
                }
            };
            File dir = new File(getToolchainDir() + PKGS_LISTS_DIR);
            if (dir.isDirectory()) {
                for (String fileName : dir.list(filter)) {
                    try {
                        Log.i(TAG, "uninstalling " + dir.getPath() + "/" + fileName);
                        FileInputStream fin = new FileInputStream(dir.getPath() + "/" + fileName);
                        DataInputStream in = new DataInputStream(fin);
                        BufferedReader reader = new BufferedReader(new InputStreamReader(in));
                        String line = "";
                        while ((line = reader.readLine()) != null) {
                            Log.i(TAG, "Delete file: " + line);
                            (new File(getToolchainDir() + "/" + line)).delete();
                        }
                        in.close();
                        (new File(dir.getPath() + "/" + fileName)).delete();
                    } catch (Exception e) {
                        Log.e(TAG, "Error during remove files " + e);
                    }
                }
            }
            setPrefString("use_package_manager", "yes");
            return null;
        }

        protected void onPostExecute(Void result) {
            super.onPostExecute(result);
            installToolchainPackage();
        }
    }
}
