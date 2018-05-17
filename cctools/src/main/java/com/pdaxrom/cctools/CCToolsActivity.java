package com.pdaxrom.cctools;

import android.app.Activity;
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
import android.os.Environment;
import android.support.v4.app.FragmentTransaction;
import android.support.v7.app.ActionBar;
import android.text.Html;
import android.text.InputType;
import android.text.method.LinkMovementMethod;
import android.text.util.Linkify;
import android.util.Log;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.AdapterView.OnItemSelectedListener;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.TextView.BufferType;
import android.widget.Toast;
import android.widget.ViewFlipper;

import com.pdaxrom.editor.CodeEditor;
import com.pdaxrom.editor.CodeEditorInterface;
import com.pdaxrom.packagemanager.EnvironmentPath;
import com.pdaxrom.packagemanager.PackageManagerActivity;
import com.pdaxrom.utils.FileDialog;
import com.pdaxrom.utils.LogItem;
import com.pdaxrom.utils.SelectionMode;
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
import java.util.HashMap;
import java.util.List;

import jackpal.androidterm.MultiTermActivity;

public class CCToolsActivity extends FlexiDialogActivity implements ActionBar.TabListener, OnSharedPreferenceChangeListener, CodeEditorInterface {
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
    private static final int TEXT_GOTO = Menu.CATEGORY_CONTAINER + 1;
    private static final int TEXT_FIND = Menu.CATEGORY_CONTAINER + 2;
    private static final int TEXT_UNDO = Menu.CATEGORY_CONTAINER + 3;
    private static final int TEXT_REDO = Menu.CATEGORY_CONTAINER + 4;
    private static final int SERVICE_STOP = 0;
    private static final int SERVICE_START = 1;
    static private final String KEY_FILE = "file";
    static private final String KEY_LINE = "line";
    static private final String KEY_POS = "pos";
    static private final String KEY_TYPE = "type";
    static private final String KEY_MESG = "mesg";
    public static Socket dialogServiceSocket;
    private Context context = this;
    private SharedPreferences mPrefs;
    private String TAG = "cctools";
    private String buildBaseDir; // Project base directory
    private boolean buildAfterSave = false;
    private boolean buildAfterLoad = false;
    private View buttBar;
    private ViewFlipper flipper;
    private List<CodeEditor> editors = null;
    private CodeEditor codeEditor;
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

        editors = new ArrayList<>();
        flipper = findViewById(R.id.flipper);

        int tabsLoaded = loadTabs();

        if (tabsLoaded == 0) {
            newFile();
        }

        showInfoAndCheckToolchain();

        ImageButton newButton = findViewById(R.id.newButton);
        newButton.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                //warnSaveDialog(WARN_SAVE_AND_NEW);
                newFile();
            }
        });
        ImageButton openButton = findViewById(R.id.pathButton);
        openButton.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                warnSaveDialog(WARN_SAVE_AND_LOAD);
            }
        });
        ImageButton saveButton = findViewById(R.id.saveButton);
        saveButton.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                saveFile();
            }
        });

        ImageButton saveAsButton = findViewById(R.id.saveAsButton);
        saveAsButton.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                saveAsFile();
            }
        });

        ImageButton playButton = findViewById(R.id.playButton);
        playButton.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                warnSaveDialog(WARN_SAVE_AND_BUILD_FORCE);
            }
        });

        ImageButton buildButton = findViewById(R.id.buildButton);
        buildButton.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                warnSaveDialog(WARN_SAVE_AND_BUILD);
            }
        });


        ImageButton logButton = findViewById(R.id.logButton);
        logButton.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                showLog();
            }
        });

        ImageButton terminalButton = findViewById(R.id.terminalButton);
        terminalButton.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                runTerminal();
            }
        });

        ImageButton undoButton = findViewById(R.id.undoButton);
        undoButton.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                codeEditor.undo();
            }
        });

        ImageButton redoButton = findViewById(R.id.redoButton);
        redoButton.setOnClickListener(new OnClickListener() {
            public void onClick(View v) {
                codeEditor.redo();
            }
        });

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
                if (!findAndShowEditorTab(fileName)) {
                    if (tabsLoaded != 0) {
                        addEditorTab();
                    }
                    if (codeEditor.loadFile(fileName)) {
                        loadFileEditPos(codeEditor);
                        newTitle(new File(fileName).getName());
                    }
                }
            }
        }
    }

    private void updateEditorPrefs(SharedPreferences prefs, CodeEditor editor) {
        editor.setTextSize(Float.valueOf(prefs.getString("fontsize", "12")));
        editor.showSyntax(prefs.getBoolean("syntax", true));
        editor.drawLineNumbers(prefs.getBoolean("drawLineNumbers", true));
        editor.drawGutterLine(prefs.getBoolean("drawLineNumbers", true));
        editor.setAutoPair(prefs.getBoolean("autopair", true));
        editor.setAutoIndent(prefs.getBoolean("autoindent", true));
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences prefs, String key) {
        Log.i(TAG, "onSharedPreferenceChanged()");
        for (CodeEditor editor : editors) {
            updateEditorPrefs(prefs, editor);
        }
        if (prefs.getBoolean("showToolBar", true)) {
            buttBar.setVisibility(View.VISIBLE);
        } else {
            buttBar.setVisibility(View.GONE);
        }
    }

    @Override
    protected void onDestroy() {
        saveTabs();
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

    public void onBackPressed() {
        boolean hasChanged = false;
        for (int i = 0; i < flipper.getChildCount(); i++) {
            if (((CodeEditor) flipper.getChildAt(i).findViewById(R.id.codeEditor)).hasChanged()) {
                hasChanged = true;
            }
        }
        if (hasChanged) {
            exitDialog();
        } else {
            super.onBackPressed();
        }
    }


    public boolean onContextItemSelected(android.view.MenuItem item) {
        switch (item.getItemId()) {
            case TEXT_GOTO:
                gotoDialog();
                break;
            case TEXT_FIND:
                searchDialog();
                break;
            case TEXT_UNDO:
                codeEditor.undo();
                break;
            case TEXT_REDO:
                codeEditor.redo();
                break;
            default:
                return super.onContextItemSelected(item);
        }
        return true;
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu, menu);
        menu.add(0, TEXT_UNDO, 0, getString(R.string.menu_undo)).setShowAsAction(MenuItem.SHOW_AS_ACTION_IF_ROOM | MenuItem.SHOW_AS_ACTION_WITH_TEXT);
        menu.add(0, TEXT_REDO, 0, getString(R.string.menu_redo)).setShowAsAction(MenuItem.SHOW_AS_ACTION_IF_ROOM | MenuItem.SHOW_AS_ACTION_WITH_TEXT);
        menu.add(0, TEXT_GOTO, 0, getString(R.string.menu_goto)).setShowAsAction(MenuItem.SHOW_AS_ACTION_IF_ROOM | MenuItem.SHOW_AS_ACTION_WITH_TEXT);
        menu.add(0, TEXT_FIND, 0, getString(R.string.menu_search)).setShowAsAction(MenuItem.SHOW_AS_ACTION_IF_ROOM | MenuItem.SHOW_AS_ACTION_WITH_TEXT);
        return true;
    }

    public boolean onOptionsItemSelected(MenuItem item) {
        int i = item.getItemId();
        if (i == R.id.item_new) {
            newFile();

        } else if (i == R.id.item_open) {
            warnSaveDialog(WARN_SAVE_AND_LOAD);

        } else if (i == R.id.item_save) {
            saveFile();

        } else if (i == R.id.item_saveas) {
            saveAsFile();

        } else if (i == R.id.item_close) {
            warnSaveDialog(WARN_SAVE_AND_CLOSE);

        } else if (i == R.id.item_run) {
            warnSaveDialog(WARN_SAVE_AND_BUILD_FORCE);

        } else if (i == R.id.item_build) {
            warnSaveDialog(WARN_SAVE_AND_BUILD);

        } else if (i == R.id.item_buildlog) {
            showLog();

        } else if (i == R.id.item_terminal) {
            runTerminal();

        } else if (i == R.id.item_pkgmgr) {
            packageManager();

        } else if (i == R.id.prefs) {
            startActivity(new Intent(this, CCtoolsPreferences.class));

        } else if (i == R.id.about) {
            aboutDialog();

        } else if (i == TEXT_GOTO) {
            gotoDialog();

        } else if (i == TEXT_FIND) {
            searchDialog();

        } else if (i == TEXT_UNDO) {
            codeEditor.undo();

        } else if (i == TEXT_REDO) {
            codeEditor.redo();

        } else if (i == R.id.project_new) {
            newProject();

        }
        return true;
    }

    @Override
    public void onTabSelected(ActionBar.Tab tab, FragmentTransaction ft) {
        flipper.setDisplayedChild(tab.getPosition());
        codeEditor = flipper.getChildAt(tab.getPosition()).findViewById(R.id.codeEditor);
    }

    public void onTabUnselected(ActionBar.Tab tab, FragmentTransaction ft) {
    }

    public void onTabReselected(ActionBar.Tab tab, FragmentTransaction ft) {
    }

    @Override
    public void textHasChanged(boolean hasChanged) {
        if (getSupportActionBar().getSelectedTab().getText() == null) {
            return;
        }
        String title = getSupportActionBar().getSelectedTab().getText().toString();
        if (!hasChanged && title.startsWith("*")) {
            title = title.substring(1);
        } else if (hasChanged && !title.startsWith("*")) {
            title = "*" + title;
        }
        getSupportActionBar().getSelectedTab().setText(title);
    }

    private String getPrefString(String key) {
        SharedPreferences settings = getPreferences(Activity.MODE_PRIVATE);
        return settings.getString(key, Environment.getExternalStorageDirectory().getPath() + "/CCTools/Examples");
    }

    private void setPrefString(String key, String value) {
        SharedPreferences settings = getPreferences(Activity.MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.putString(key, value);
        editor.apply();
    }

    private int loadTabs() {
        SharedPreferences settings = getPreferences(Activity.MODE_PRIVATE);
        int total = settings.getInt("TabsCount", 0);
        int skipped = 0;

        if (total > 0) {
            for (int i = 0; i < total; i++) {
                String fileName = settings.getString("TabN" + i, "");
                if (fileName == "") {
                    newFile();
                } else {
                    if (new File(fileName).exists()) {
                        addEditorTab();
                        if (codeEditor.loadFile(fileName)) {
                            loadFileEditPos(codeEditor);
                            newTitle(new File(fileName).getName());
                        } else {
                            newTitle(getString(R.string.new_file));
                        }
                    } else {
                        skipped++;
                    }
                }
            }
            int currentTab = settings.getInt("CurrentTab", 0);
            if (skipped > 0 && currentTab > flipper.getChildCount() - 1) {
                currentTab = 0;
            }
            getSupportActionBar().setSelectedNavigationItem(currentTab);

        }

        return total - skipped;
    }

    private void saveTabs() {
        SharedPreferences settings = getPreferences(Activity.MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.putInt("TabsCount", flipper.getChildCount());
        if (flipper.getChildCount() > 0) {
            editor.putInt("CurrentTab", flipper.getDisplayedChild());
        }
        for (int i = 0; i < flipper.getChildCount(); i++) {
            CodeEditor ce = flipper.getChildAt(i).findViewById(R.id.codeEditor);
            String fileName = ce.getFilePath();
            if (fileName == null) {
                fileName = "";
            } else {
                saveFileEditPos(ce);
            }
            editor.putString("TabN" + i, fileName);
        }
        editor.apply();
    }

    private boolean findAndShowEditorTab(String filename) {
        for (int i = 0; i < flipper.getChildCount(); i++) {
            CodeEditor e = flipper.getChildAt(i).findViewById(R.id.codeEditor);
            String name = e.getFilePath();
            if (name != null && name.equals(filename)) {
                getSupportActionBar().setSelectedNavigationItem(i);
                return true;
            }
        }
        return false;
    }

    private void addEditorTab() {
        LayoutInflater inflater = (LayoutInflater) getSystemService(Context.LAYOUT_INFLATER_SERVICE);
        flipper.addView(inflater.inflate(R.layout.editor, null));
        codeEditor = flipper.getChildAt(flipper.getChildCount() - 1).findViewById(R.id.codeEditor);
        updateEditorPrefs(mPrefs, codeEditor);
        codeEditor.setCodeEditorInterface(this);
        editors.add(codeEditor);
        registerForContextMenu(codeEditor);
        ActionBar.Tab tab = getSupportActionBar().newTab();
        //RelativeLayout view = (RelativeLayout) getLayoutInflater().inflate(R.layout.tablabel, null);
        //tab.setCustomView(view);
        tab.setTabListener(this);
        getSupportActionBar().addTab(tab);
        getSupportActionBar().selectTab(tab);
    }

    private String getLastOpenedDir() {
        return getPrefString("lastdir");
    }

    private void setLastOpenedDir(String dir) {
        setPrefString("lastdir", dir);
    }

    private void newTitle(String title) {
        //((TextView) ((RelativeLayout)getSupportActionBar().getSelectedTab().getCustomView()).findViewById(R.id.title)).setText(title);
        getSupportActionBar().getSelectedTab().setText(title);
    }

    private void newFile() {
        addEditorTab();
        newTitle(getString(R.string.new_file));
        buildAfterSave = false;
        buildAfterLoad = false;
        codeEditor.newFile();
        Toast.makeText(getBaseContext(), getString(R.string.new_file), Toast.LENGTH_SHORT).show();
    }

    private void loadFile() {
        Intent intent = new Intent(getBaseContext(), FileDialog.class);
        String dir = getLastOpenedDir();
        String fileName = codeEditor.getFilePath();
        if (fileName != null && new File(fileName).getParentFile().exists()) {
            dir = (new File(fileName)).getParent();
        }
        if (dir == null || !new File(dir).exists()) {
            dir = Environment.getExternalStorageDirectory().getPath();
        }

        intent.putExtra(FileDialog.START_PATH, dir);
        intent.putExtra(FileDialog.SELECTION_MODE, SelectionMode.MODE_OPEN);
        startActivityForResult(intent, REQUEST_OPEN);
    }

    private void saveFile() {
        String fileName = codeEditor.getFilePath();
        if (fileName == null || fileName.equals("")) {
            String dir = getLastOpenedDir();
            if (fileName != null && new File(dir).getParentFile().exists()) {
                dir = (new File(fileName)).getParent();
            }
            if (dir == null || !new File(dir).exists()) {
                dir = Environment.getExternalStorageDirectory().getPath();
            }
            Intent intent = new Intent(getBaseContext(), FileDialog.class);
            intent.putExtra(FileDialog.START_PATH, dir);
            intent.putExtra(FileDialog.SELECTION_MODE, SelectionMode.MODE_CREATE);
            startActivityForResult(intent, REQUEST_SAVE);
        } else {
            if (codeEditor.saveFile(fileName)) {
                saveFileEditPos(codeEditor);
                Toast.makeText(getBaseContext(), getString(R.string.file_saved), Toast.LENGTH_SHORT).show();
                setLastOpenedDir((new File(fileName)).getParent());
            } else {
                Toast.makeText(getBaseContext(), getString(R.string.file_not_saved), Toast.LENGTH_SHORT).show();
            }
            if (buildAfterSave) {
                buildFile(forceTmpVal);
                buildAfterSave = false;
            }
        }
    }

    private void saveAsFile() {
        Intent intent = new Intent(getBaseContext(), FileDialog.class);
        String dir = getLastOpenedDir();
        String fileName = codeEditor.getFilePath();
        if (fileName != null && new File(fileName).getParentFile().exists()) {
            dir = (new File(fileName)).getParent();
        }
        if (dir == null || !new File(dir).exists()) {
            dir = Environment.getExternalStorageDirectory().getPath();
        }
        intent.putExtra(FileDialog.START_PATH, dir);
        intent.putExtra(FileDialog.SELECTION_MODE, SelectionMode.MODE_CREATE);
        startActivityForResult(intent, REQUEST_SAVE);
    }

    private void saveFileEditPos(CodeEditor ce) {
        SharedPreferences settings = getSharedPreferences(SHARED_PREFS_FILES_EDITPOS, 0);
        SharedPreferences.Editor editor = settings.edit();
        editor.putInt(ce.getFilePath(), ce.getSelectionStart());
        editor.apply();
    }

    private void loadFileEditPos(CodeEditor ce) {
        SharedPreferences settings = getSharedPreferences(SHARED_PREFS_FILES_EDITPOS, 0);
        if (ce.getText().toString().length() >= settings.getInt(ce.getFilePath(), 0)) {
            ce.setSelection(settings.getInt(ce.getFilePath(), 0));
        }
    }

    private void build(boolean force) {
        if (codeEditor.getFilePath() == null || codeEditor.getFilePath().equals("")) {
            buildAfterLoad = true;
            forceTmpVal = force;
            loadFile();
        } else {
            buildFile(force);
        }
    }

    private void buildFile(boolean force) {
        File file = new File(codeEditor.getFilePath());
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
                    intent.putExtra(BuildConstants.EXTRA_CCTOOLS_DIR, EnvironmentPath.getCCtoolsDir(this));
                    intent.putExtra(BuildConstants.EXTRA_FORCE_BUILD, force && mPrefs.getBoolean("force_run", false));
                    startActivity(intent);
                    return;
                }

                if (ext.contentEquals(".lua") && new File(getToolchainDir(), "cctools/bin/luajit").exists()) {
                    Intent intent = new Intent(CCToolsActivity.this, LauncherConsoleActivity.class);
                    intent.putExtra(BuildConstants.EXTRA_EXEC_FILE, getToolchainDir() + "/cctools/bin/luajit " + file.getAbsolutePath());
                    intent.putExtra(BuildConstants.EXTRA_CCTOOLS_DIR, EnvironmentPath.getCCtoolsDir(this));
                    intent.putExtra(BuildConstants.EXTRA_WORK_DIR, file.getParentFile().getAbsolutePath());
                    intent.putExtra(BuildConstants.EXTRA_FORCE_BUILD, force && mPrefs.getBoolean("force_run", false));
                    startActivity(intent);
                    return;
                }

                if ((ext.equals(".pl") || ext.equals(".pm")) && new File(getToolchainDir() + "/cctools/bin/perl").exists()) {
                    Intent intent = new Intent(CCToolsActivity.this, LauncherConsoleActivity.class);
                    intent.putExtra(BuildConstants.EXTRA_EXEC_FILE, getToolchainDir() + "/cctools/bin/perl " + file.getAbsolutePath());
                    intent.putExtra(BuildConstants.EXTRA_CCTOOLS_DIR, EnvironmentPath.getCCtoolsDir(this));
                    intent.putExtra(BuildConstants.EXTRA_WORK_DIR, file.getParentFile().getAbsolutePath());
                    intent.putExtra(BuildConstants.EXTRA_FORCE_BUILD, force && mPrefs.getBoolean("force_run", false));
                    startActivity(intent);
                    return;
                }
            }

            Intent intent = new Intent(this, BuildActivity.class);
            intent.putExtra(BuildConstants.EXTRA_FILE_NAME, file.getAbsolutePath());
            intent.putExtra(BuildConstants.EXTRA_CCTOOLS_DIR, EnvironmentPath.getCCtoolsDir(this));
            intent.putExtra(BuildConstants.EXTRA_TMP_DIR, EnvironmentPath.getSdCardTmpDir());
            intent.putExtra(BuildConstants.EXTRA_FORCE_BUILD, force);
            startActivity(intent);
        }
    }

    private void showLog() {
        if (BuildActivity.errorsList.isEmpty()) {
            Toast.makeText(getBaseContext(), getString(R.string.log_empty), Toast.LENGTH_SHORT).show();
            return;
        }
        ArrayList<HashMap<String, String>> menuItems = new ArrayList<HashMap<String, String>>();
        for (LogItem item : BuildActivity.errorsList) {
            HashMap<String, String> map = new HashMap<String, String>();
            map.put(KEY_FILE, item.getFile());
            map.put(KEY_LINE, Integer.toString(item.getLine()));
            map.put(KEY_POS, Integer.toString(item.getPos()));
            map.put(KEY_TYPE, item.getType());
            String color = "<font color=\"";
            if (item.getType().contains("error")) {
                color += "red\">ERROR: ";
            } else {
                color += "yellow\">WARNING: ";
            }
            map.put(KEY_MESG, color + item.getMessage() + "</font>");
            menuItems.add(map);
        }
        final ListView listView = new ListView(this);
        listView.setAdapter(new SimpleHtmlAdapter(
                this,
                menuItems,
                R.layout.buildlog_item,
                new String[]{KEY_FILE, KEY_LINE, KEY_POS, KEY_TYPE, KEY_MESG},
                new int[]{R.id.buildlog_file, R.id.buildlog_line, R.id.buildlog_pos, R.id.buildlog_type, R.id.buildlog_mesg}
        ));
        AlertDialog.Builder dialog = new AlertDialog.Builder(this);
        dialog.setView(listView);
        final AlertDialog alertDialog = dialog.create();

        listView.setOnItemClickListener(new OnItemClickListener() {
            public void onItemClick(AdapterView<?> parent, View view, int position,
                                    long id) {
                int line = Integer.parseInt(((TextView) view.findViewById(R.id.buildlog_line)).getText().toString());
                int pos = Integer.parseInt(((TextView) view.findViewById(R.id.buildlog_pos)).getText().toString());
                String name = ((TextView) view.findViewById(R.id.buildlog_file)).getText().toString();

                if (!name.startsWith("/")) {
                    name = buildBaseDir + "/" + name;
                }

                if (!(new File(codeEditor.getFilePath())).getAbsolutePath().contentEquals((new File(name)).getAbsolutePath())) {
                    alertDialog.cancel();
                    showFileName = name;
                    showFileLine = line;
                    showFilePos = pos;
                    Log.i(TAG, "Jump to file " + showFileName);
                    warnSaveDialog(WARN_SAVE_AND_LOAD_POS);
                } else {
                    if (pos > 0) {
                        codeEditor.goToLinePos(line, pos);
                    } else {
                        codeEditor.goToLine(line);
                    }
                    alertDialog.cancel();
                }
            }
        });
        alertDialog.show();
    }

    private void packageManager() {
        Intent intent = new Intent(CCToolsActivity.this, PackageManagerActivity.class);
        startActivity(intent);
    }

    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        String fileName;
        if (resultCode == RESULT_OK) {
            if (requestCode == REQUEST_SAVE) {
                fileName = data.getStringExtra(FileDialog.RESULT_PATH);
                setLastOpenedDir((new File(fileName)).getParent());
                if (codeEditor.saveFile(fileName)) {
                    saveFileEditPos(codeEditor);
                    Toast.makeText(getBaseContext(), getString(R.string.file_saved), Toast.LENGTH_SHORT).show();
                    if (buildAfterSave) {
                        buildAfterSave = false;
                        buildFile(forceTmpVal);
                    }
                } else {
                    Toast.makeText(getBaseContext(), getString(R.string.file_not_saved), Toast.LENGTH_SHORT).show();
                    buildAfterSave = false;
                }
                newTitle(new File(fileName).getName());
            } else if (requestCode == REQUEST_OPEN) {
                fileName = data.getStringExtra(FileDialog.RESULT_PATH);
                setLastOpenedDir((new File(fileName)).getParent());
                if (!findAndShowEditorTab(fileName)) {
                    addEditorTab();
                    if (codeEditor.loadFile(fileName)) {
                        loadFileEditPos(codeEditor);
                        Toast.makeText(getBaseContext(), getString(R.string.file_loaded), Toast.LENGTH_SHORT).show();
                        if (buildAfterLoad) {
                            buildAfterLoad = false;
                            buildFile(forceTmpVal);
                        }
                    } else {
                        Toast.makeText(getBaseContext(), getString(R.string.file_not_loaded), Toast.LENGTH_SHORT).show();
                        buildAfterLoad = false;
                    }
                    newTitle(new File(fileName).getName());
                }
            }
        }
    }

    private void exitDialog() {
        new AlertDialog.Builder(this)
                .setIcon(android.R.drawable.ic_dialog_alert)
                .setTitle(getString(R.string.exit_dialog))
                .setMessage(getString(R.string.exit_dialog_text))
                .setPositiveButton(getString(R.string.exit_dialog_yes), new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        finish();
                    }
                })
                .setNegativeButton(getString(R.string.exit_dialog_no), null)
                .show();
    }

    private void loadAndShowLinePos() {
        if (findAndShowEditorTab(showFileName)) {
            if (showFilePos > 0) {
                codeEditor.goToLinePos(showFileLine, showFilePos);
            } else {
                codeEditor.goToLine(showFileLine);
            }
        } else {
            addEditorTab();
            if (codeEditor.loadFile(showFileName)) {
                Toast.makeText(getBaseContext(), getString(R.string.file_loaded), Toast.LENGTH_SHORT).show();
                if (showFilePos > 0) {
                    codeEditor.goToLinePos(showFileLine, showFilePos);
                } else {
                    codeEditor.goToLine(showFileLine);
                }
                newTitle(new File(showFileName).getName());
            } else {
                Toast.makeText(getBaseContext(), getString(R.string.file_not_loaded), Toast.LENGTH_SHORT).show();
            }
        }
    }

    private void warnSaveRequest(int req) {
        switch (req) {
            case WARN_SAVE_AND_NEW:
                newFile();
                break;
            case WARN_SAVE_AND_LOAD:
                loadFile();
                break;
            case WARN_SAVE_AND_LOAD_POS:
                loadAndShowLinePos();
                break;
            case WARN_SAVE_AND_BUILD:
                build(false);
                break;
            case WARN_SAVE_AND_BUILD_FORCE:
                build(true);
                break;
            case WARN_SAVE_AND_CLOSE:
                int i = flipper.getDisplayedChild();
                flipper.removeViewAt(i);
                getSupportActionBar().removeTabAt(i);
                if (flipper.getChildCount() == 0) {
                    finish();
                }
                break;
        }
    }

    private void warnSaveDialog(final int req) {
        if (!codeEditor.hasChanged()) {
            warnSaveRequest(req);
            return;
        }

        new AlertDialog.Builder(this)
                .setIcon(android.R.drawable.ic_dialog_alert)
                .setTitle(getString(R.string.save_warn_dialog))
                .setMessage(getString(R.string.save_warn_text))
                .setPositiveButton(getString(R.string.exit_dialog_yes), new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        warnSaveRequest(req);
                    }
                })
                .setNegativeButton(getString(R.string.exit_dialog_no), null)
                .show();
    }

    private void aboutDialog() {
        String versionName;
        try {
            versionName = context.getPackageManager().getPackageInfo(context.getPackageName(), 0).versionName;
        } catch (NameNotFoundException e) {
            versionName = "1.0";
        }
        final TextView textView = new TextView(this);
        textView.setAutoLinkMask(Linkify.WEB_URLS);
        textView.setLinksClickable(true);
        textView.setTextSize(16f);
        textView.setGravity(Gravity.CENTER_HORIZONTAL | Gravity.CENTER_VERTICAL);
        textView.setText(getString(R.string.about_dialog_text) +
                " " +
                versionName +
                "\n" + website_url + "\n" +
                getString(R.string.about_dialog_text2));
        textView.setMovementMethod(LinkMovementMethod.getInstance());
        new AlertDialog.Builder(this)
                .setTitle(getString(R.string.about_dialog))
                .setView(textView)
                .show();
    }

    private void gotoDialog() {
        final EditText input = new EditText(context);
        input.setInputType(InputType.TYPE_CLASS_NUMBER);
        input.setSingleLine(true);
        new AlertDialog.Builder(context)
                .setMessage(getString(R.string.goto_line))
                .setView(input)
                .setPositiveButton(getString(R.string.button_continue), new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        try {
                            codeEditor.goToLine(Integer.valueOf(input.getText().toString()));
                        } catch (Exception e) {
                            Log.e(TAG, "gotoDialog() " + e);
                        }
                    }
                })
                .setNegativeButton(getString(R.string.button_cancel), new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {

                    }
                })
                .show();
    }

    private void searchDialog() {
        final EditText input = new EditText(context);
        input.setInputType(InputType.TYPE_CLASS_TEXT);
        input.setSingleLine(true);
        input.setText(codeEditor.getLastSearchText());
        input.setSelection(0, codeEditor.getLastSearchText().length());
        new AlertDialog.Builder(context)
                .setMessage(getString(R.string.search_string))
                .setView(input)
                .setPositiveButton(getString(R.string.button_continue), new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {
                        codeEditor.searchText(input.getText().toString());
                    }
                })
                .setNegativeButton(getString(R.string.button_cancel), new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int which) {

                    }
                })
                .show();
    }

    private void runTerminal() {
        Intent intent = new Intent(this, MultiTermActivity.class);
        intent.putExtra("envp", EnvironmentPath.buildEnv(this));
        startActivity(intent);
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

    public class SimpleHtmlAdapter extends SimpleAdapter {
        public SimpleHtmlAdapter(Context context, List<HashMap<String, String>> items, int resource, String[] from, int[] to) {
            super(context, items, resource, from, to);
        }

        public void setViewText(TextView view, String text) {
            view.setText(Html.fromHtml(text), BufferType.SPANNABLE);
        }
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
