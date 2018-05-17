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

package com.duy.editor;

import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.content.res.Resources;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.support.annotation.IdRes;
import android.support.annotation.NonNull;
import android.support.v4.app.FragmentManager;
import android.support.v4.view.GravityCompat;
import android.support.v4.view.ViewPager;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.text.TextUtils;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;

import com.afollestad.materialdialogs.folderselector.FolderChooserDialog;
import com.duy.ccppcompiler.BuildConfig;
import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.compiler.CompileManager;
import com.duy.ccppcompiler.compiler.CompileTask;
import com.duy.ccppcompiler.compiler.CompilerFactory;
import com.duy.ccppcompiler.compiler.INativeCompiler;
import com.duy.ccppcompiler.diagnostic.DiagnosticFragment;
import com.duy.ccppcompiler.diagnostic.DiagnosticPresenter;
import com.duy.ccppcompiler.filemanager.SrcFileManager;
import com.duy.ccppcompiler.ui.dialogs.DialogNewFile;
import com.duy.ccppcompiler.ui.examples.ExampleActivity;
import com.duy.ide.filemanager.FileManager;
import com.jecelyin.android.file_explorer.FileExplorerActivity;
import com.jecelyin.common.utils.DLog;
import com.jecelyin.common.utils.IOUtils;
import com.jecelyin.common.utils.SysUtils;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.FullScreenActivity;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.adapter.EditorFragmentPagerAdapter;
import com.jecelyin.editor.v2.adapter.IEditorPagerAdapter;
import com.jecelyin.editor.v2.common.Command;
import com.jecelyin.editor.v2.ui.dialog.ChangeThemeDialog;
import com.jecelyin.editor.v2.ui.dialog.CharsetsDialog;
import com.jecelyin.editor.v2.ui.dialog.GotoLineDialog;
import com.jecelyin.editor.v2.ui.dialog.LangListDialog;
import com.jecelyin.editor.v2.ui.dialog.WrapCharDialog;
import com.jecelyin.editor.v2.ui.editor.EditorDelegate;
import com.jecelyin.editor.v2.ui.manager.MenuManager;
import com.jecelyin.editor.v2.ui.manager.RecentFilesManager;
import com.jecelyin.editor.v2.ui.manager.TabManager;
import com.jecelyin.editor.v2.ui.settings.SettingsActivity;
import com.jecelyin.editor.v2.ui.widget.SymbolBarLayout;
import com.jecelyin.editor.v2.ui.widget.menu.MenuDef;
import com.jecelyin.editor.v2.ui.widget.menu.MenuFactory;
import com.jecelyin.editor.v2.ui.widget.menu.MenuItemInfo;
import com.jecelyin.editor.v2.utils.DBHelper;
import com.pdaxrom.packagemanager.EnvironmentPath;
import com.pdaxrom.packagemanager.PackageManagerActivity;
import com.sothree.slidinguppanel.SlidingUpPanelLayout;

import java.io.File;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;

import jackpal.androidterm.MultiTermActivity;


/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class EditorActivity extends FullScreenActivity
        implements MenuItem.OnMenuItemClickListener
        , FolderChooserDialog.FolderCallback
        , SharedPreferences.OnSharedPreferenceChangeListener {
    private static final String TAG = "MainActivity";

    private static final int RC_OPEN_FILE = 1;
    private final static int RC_SAVE = 3;
    private static final int RC_SETTINGS = 5;

    public Toolbar mToolbar;
    public ViewPager mEditorPager;
    public RecyclerView mMenuRecyclerView;
    public DrawerLayout mDrawerLayout;
    public RecyclerView mTabRecyclerView;
    public TextView mVersionTextView;
    public SymbolBarLayout mSymbolBarLayout;
    public SlidingUpPanelLayout mSlidingUpPanelLayout;
    private TabManager mTabManager;
    private Preferences mPreferences;
    private MenuManager mMenuManager;
    private long mExitTime;
    private DiagnosticPresenter mDiagnosticPresenter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mPreferences = Preferences.getInstance(this);
        MenuManager.init(this);

        setContentView(R.layout.activity_main);

        mToolbar = findViewById(R.id.toolbar);
        mEditorPager = findViewById(R.id.view_pager);
        mMenuRecyclerView = findViewById(R.id.menuRecyclerView);
        mDrawerLayout = findViewById(R.id.drawer_layout);
        mDrawerLayout.addDrawerListener(new DrawerLayout.SimpleDrawerListener() {
            @Override
            public void onDrawerOpened(View drawerView) {
                super.onDrawerOpened(drawerView);
                hideSoftInput();
            }
        });
        mTabRecyclerView = findViewById(R.id.tabRecyclerView);
        mVersionTextView = findViewById(R.id.versionTextView);

        mSymbolBarLayout = findViewById(R.id.symbolBarLayout);
        mSymbolBarLayout.setOnSymbolCharClickListener(new SymbolBarLayout.OnSymbolCharClickListener() {
            @Override
            public void onClick(View v, String text) {
                insertText(text);
            }
        });

        setStatusBarColor(mDrawerLayout);

        bindPreferences();
        setScreenOrientation();

        mDrawerLayout.setEnabled(false);
        mDrawerLayout.setScrimColor(Color.TRANSPARENT);

        final String version = SysUtils.getVersionName(this);
        mVersionTextView.setText(version);

        start();
    }

    private void bindPreferences() {
        mDrawerLayout.setKeepScreenOn(mPreferences.isKeepScreenOn());
        mSymbolBarLayout.setVisibility(mPreferences.isReadOnly() ? View.GONE : View.VISIBLE);
        mPreferences.registerOnSharedPreferenceChangeListener(this);
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {
        if (mToolbar == null)
            return;
        switch (key) {
            case Preferences.KEY_KEEP_SCREEN_ON:
                mToolbar.setKeepScreenOn(sharedPreferences.getBoolean(key, false));
                break;
            case Preferences.KEY_SCREEN_ORIENTATION:
                setScreenOrientation();
                break;
            case Preferences.KEY_READ_ONLY:
                mSymbolBarLayout.setVisibility(mPreferences.isReadOnly() ? View.GONE : View.VISIBLE);
                break;
        }
    }

    private void setScreenOrientation() {
        int orgi = mPreferences.getScreenOrientation();

        if (Preferences.SCREEN_ORIENTATION_AUTO == orgi) {
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED);
        } else if (Preferences.SCREEN_ORIENTATION_LANDSCAPE == orgi) {
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        } else if (Preferences.SCREEN_ORIENTATION_PORTRAIT == orgi) {
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        }
    }

    private void start() {
        mEditorPager.setVisibility(View.VISIBLE);
        initUI();
        processIntent();
    }

    private void initUI() {
        mMenuRecyclerView.setLayoutManager(new LinearLayoutManager(this));
        mTabRecyclerView.setLayoutManager(new LinearLayoutManager(this));
        mDrawerLayout.setEnabled(true);

        initToolbar();

        if (mMenuManager == null) {
            mMenuManager = new MenuManager(this);
        }
        final View toggleView = findViewById(R.id.btn_toggle_panel);
        mSlidingUpPanelLayout = findViewById(R.id.diagnostic_panel);
        mSlidingUpPanelLayout.addPanelSlideListener(new SlidingUpPanelLayout.SimplePanelSlideListener() {
            @Override
            public void onPanelSlide(View panel, float slideOffset) {
                toggleView.animate().rotation(180 * slideOffset).start();
            }
        });

        FragmentManager fm = getSupportFragmentManager();
        String tag = DiagnosticFragment.class.getSimpleName();
        DiagnosticFragment diagnosticFragment = (DiagnosticFragment)
                fm.findFragmentByTag(tag);
        if (diagnosticFragment == null) {
            diagnosticFragment = DiagnosticFragment.newInstance();
        }
        fm.beginTransaction().replace(R.id.container_diagnostic_list_view, diagnosticFragment, tag)
                .commit();
        mDiagnosticPresenter = new DiagnosticPresenter(diagnosticFragment, this, mTabManager);
    }

    private void initToolbar() {
        Resources res = getResources();
        mToolbar.setNavigationIcon(R.drawable.ic_drawer_raw);
        mToolbar.setNavigationContentDescription(R.string.tab);

        Menu menu = mToolbar.getMenu();
        List<MenuItemInfo> items = MenuFactory.getInstance(this).getToolbarIcon();
        for (MenuItemInfo item : items) {
            MenuItem menuItem = menu.add(MenuDef.GROUP_TOOLBAR, item.getItemId(), Menu.NONE, item.getTitleResId());
            menuItem.setIcon(MenuManager.makeToolbarNormalIcon(this, item.getIconResId()));

            menuItem.setOnMenuItemClickListener(this);
            menuItem.setShowAsAction(MenuItem.SHOW_AS_ACTION_ALWAYS);
        }
        MenuItem menuItem = menu.add(MenuDef.GROUP_TOOLBAR, R.id.m_menu, Menu.NONE, getString(R.string.more_menu));
        menuItem.setIcon(R.drawable.ic_more_horiz_white_24dp);
        menuItem.setOnMenuItemClickListener(this);
        menuItem.setShowAsAction(MenuItem.SHOW_AS_ACTION_ALWAYS);

        mTabManager = new TabManager(this);
    }

    @Override
    protected void onNewIntent(Intent intent) {
        super.onNewIntent(intent);
        setIntent(intent);
        processIntent();
    }

    private void processIntent() {
        try {
            if (!processIntentImpl()) {
                UIUtils.alert(getContext(), getString(R.string.cannt_handle_intent_x, getIntent().toString()));
            }
        } catch (Throwable e) {
            DLog.e(e);
            UIUtils.alert(getContext(), getString(R.string.handle_intent_x_error, getIntent().toString() + "\n" + e.getMessage()));
        }
    }

    private boolean processIntentImpl() throws Throwable {
        Intent intent = getIntent();
        DLog.d("intent=" + intent);
        if (intent == null)
            return true; //pass hint

        String action = intent.getAction();
        // action == null if change theme
        if (action == null || Intent.ACTION_MAIN.equals(action)) {
            return true;
        }

        if (Intent.ACTION_VIEW.equals(action) || Intent.ACTION_EDIT.equals(action)) {
            if (intent.getScheme().equals("content")) {
                InputStream attachment = getContentResolver().openInputStream(intent.getData());
                try {
                    String text = IOUtils.toString(attachment);
                    openText(text);
                } catch (OutOfMemoryError e) {
                    UIUtils.toast(this, R.string.out_of_memory_error);
                }

                return true;
            } else if (intent.getScheme().equals("file")) {
                Uri mUri = intent.getData();
                String file = mUri != null ? mUri.getPath() : null;
                if (!TextUtils.isEmpty(file)) {
                    openFile(file);
                    return true;
                }
            }

        } else if (Intent.ACTION_SEND.equals(action) && intent.getExtras() != null) {
            Bundle extras = intent.getExtras();
            CharSequence text = extras.getCharSequence(Intent.EXTRA_TEXT);

            if (text != null) {
                openText(text);
                return true;
            } else {
                Object stream = extras.get(Intent.EXTRA_STREAM);
                if (stream != null && stream instanceof Uri) {
                    openFile(((Uri) stream).getPath());
                    return true;
                }
            }
        }

        return false;
    }

    /**
     * @param menuResId
     * @param status    {@link MenuDef#STATUS_NORMAL}, {@link MenuDef#STATUS_DISABLED}
     */
    public void setMenuStatus(@IdRes int menuResId, int status) {
        MenuItem menuItem = mToolbar.getMenu().findItem(menuResId);
        if (menuItem == null) {
            return;
        }
        Drawable icon = menuItem.getIcon();
        if (status == MenuDef.STATUS_DISABLED) {
            menuItem.setEnabled(false);
            menuItem.setIcon(MenuManager.makeToolbarDisabledIcon(icon));
        } else {
            menuItem.setEnabled(true);
            if (menuItem.getGroupId() == MenuDef.GROUP_TOOLBAR) {
                menuItem.setIcon(MenuManager.makeToolbarNormalIcon(icon));
            } else {
                menuItem.setIcon(MenuManager.makeMenuNormalIcon(icon));
            }
        }
    }

    @Override
    public boolean onMenuItemClick(MenuItem item) {
        onMenuClick(item.getItemId());
        return true;
    }

    private void onMenuClick(int id) {
        Command.CommandEnum commandEnum;

        closeMenu();

        switch (id) {
            case R.id.action_new_file:
                createNewFile();
                break;
            case R.id.action_open:
                String sourceDir = EnvironmentPath.getSdCardSourceDir();
                String homeDir = BuildConfig.DEBUG ? EnvironmentPath.getToolchainsDir(this) : sourceDir;
                FileExplorerActivity.startPickFileActivity(this, sourceDir, homeDir, RC_OPEN_FILE);
                break;
            case R.id.m_goto_line:
                new GotoLineDialog(this).show();
                break;
            case R.id.m_history:
                RecentFilesManager rfm = new RecentFilesManager(this);
                rfm.setOnFileItemClickListener(new RecentFilesManager.OnFileItemClickListener() {
                    @Override
                    public void onClick(String file, String encoding) {
                        openFile(file, encoding, 0);
                    }
                });
                rfm.show(getContext());
                break;
            case R.id.m_wrap:
                new WrapCharDialog(this).show();
                break;
            case R.id.m_highlight:
                new LangListDialog(this).show();
                break;
            case R.id.m_menu:
                hideSoftInput();

                mDrawerLayout.postDelayed(new Runnable() {
                    @Override
                    public void run() {
                        mDrawerLayout.openDrawer(GravityCompat.END);
                    }
                }, 200);

                break;
            case R.id.action_save_all:
                UIUtils.toast(this, R.string.save_all);
                saveAll(true);
                break;
            case R.id.m_theme:
                new ChangeThemeDialog(getContext()).show();
                break;
            case R.id.m_fullscreen:
                boolean fullscreenMode = mPreferences.isFullScreenMode();
                mPreferences.setFullScreenMode(!fullscreenMode);
                UIUtils.toast(this, fullscreenMode
                        ? R.string.disabled_fullscreen_mode_message
                        : R.string.enable_fullscreen_mode_message);
                break;
            case R.id.m_readonly:
                boolean readOnly = !mPreferences.isReadOnly();
                mPreferences.setReadOnly(readOnly);
                doCommandForAllEditor(new Command(Command.CommandEnum.READONLY_MODE));
                break;
            case R.id.m_encoding:
                new CharsetsDialog(this).show();
                break;
            case R.id.action_run:
                compileAndRun();
                break;
            case R.id.m_settings:
                SettingsActivity.startActivity(this, RC_SETTINGS);
                break;

            case R.id.action_c_example:
                ExampleActivity.openExample(this, "c");
                break;
            case R.id.action_cpp_example:
                ExampleActivity.openExample(this, "cpp");
                break;
            case R.id.action_install_add_on:
                startActivity(new Intent(this, PackageManagerActivity.class));
                break;
            case R.id.action_open_terminal:
                openTerminal();
                break;
            default:
                commandEnum = MenuFactory.getInstance(this).idToCommandEnum(id);
                if (commandEnum != Command.CommandEnum.NONE)
                    doCommand(new Command(commandEnum));
        }
    }

    private void openTerminal() {
        Intent intent = new Intent(this, MultiTermActivity.class);
        intent.putExtra("envp", EnvironmentPath.buildEnv(this));
        startActivity(intent);
    }

    public void createNewFile() {
        DialogNewFile
                .create(this, new UIUtils.OnShowInputCallback() {
                    @Override
                    public void onConfirm(CharSequence input) {
                        File srcDir = SrcFileManager.getSourceDir(EditorActivity.this);
                        File file = new File(srcDir, input.toString());
                        if (com.duy.utils.IOUtils.createNewFile(file)) {
                            mTabManager.newTab(file);
                        }
                    }
                })
                .show();
    }

    public void saveAll(boolean inBackgroundThread) {
        EditorFragmentPagerAdapter editorPagerAdapter = mTabManager.getEditorPagerAdapter();
        ArrayList<EditorDelegate> allEditor = editorPagerAdapter.getAllEditor();
        for (EditorDelegate editorDelegate : allEditor) {
            editorDelegate.getDocument().save(inBackgroundThread, null);
        }
    }

    private void compileAndRun() {
        saveAll(false);
        EditorDelegate currentEditor = getCurrentEditorDelegate();
        File[] srcFiles = new File[1];
        if (currentEditor != null) {
            String path = currentEditor.getPath();
            srcFiles[0] = new File(path);
        }

        // TODO: 17-May-18 improve
        CompilerFactory.CompileType compilerType;
        if (srcFiles[0].getName().toLowerCase().endsWith(".cpp")) {
            compilerType = CompilerFactory.CompileType.G_PLUS_PLUS;
        } else if (srcFiles[0].getName().toLowerCase().endsWith(".c")) {
            compilerType = CompilerFactory.CompileType.GCC;
        } else {
            compilerType = CompilerFactory.CompileType.GCC;
        }
        INativeCompiler compiler = CompilerFactory.createCompiler(EditorActivity.this, compilerType);
        CompileManager compileManager = new CompileManager(EditorActivity.this);
        compileManager.setDiagnosticPresenter(mDiagnosticPresenter);
//
        CompileTask compileTask = new CompileTask(compiler, srcFiles, compileManager);
        compileTask.execute();

//        File file = srcFiles[0];
//        Intent intent = new Intent(this, BuildActivity.class);
//        intent.putExtra(BuildConstants.EXTRA_FILE_NAME, file.getAbsolutePath());
//        intent.putExtra(BuildConstants.EXTRA_FORCE_BUILD, false);
//        startActivity(intent);
    }

    @Override
    public boolean onPrepareOptionsMenu(Menu menu) {
        onMenuClick(R.id.m_menu);
        return false;
    }

    public void closeMenu() {
        if (mDrawerLayout.isDrawerOpen(GravityCompat.START)) {
            mDrawerLayout.closeDrawer(GravityCompat.START);
        }
        if (mDrawerLayout.isDrawerOpen(GravityCompat.END)) {
            mDrawerLayout.closeDrawer(GravityCompat.END);
        }
    }

    @Override
    public void onFolderSelection(@NonNull FolderChooserDialog dialog, @NonNull File file) {

    }

    private void hideSoftInput() {
        doCommand(new Command(Command.CommandEnum.HIDE_SOFT_INPUT));
    }

    private void showSoftInput() {
        doCommand(new Command(Command.CommandEnum.SHOW_SOFT_INPUT));
    }

    public void doCommandForAllEditor(Command command) {
        for (EditorDelegate editorDelegate : mTabManager.getEditorPagerAdapter().getAllEditor()) {
            editorDelegate.doCommand(command);
        }
    }

    public void doCommand(Command command) {
        EditorDelegate editorDelegate = getCurrentEditorDelegate();
        if (editorDelegate != null) {
            editorDelegate.doCommand(command);
            if (command.what == Command.CommandEnum.HIGHLIGHT) {
                mToolbar.setTitle(editorDelegate.getToolbarText());
            }
        }
    }

    private EditorDelegate getCurrentEditorDelegate() {
        if (mTabManager == null || mTabManager.getEditorPagerAdapter() == null)
            return null;
        return mTabManager.getEditorPagerAdapter().getCurrentEditorDelegate();
    }

    public void startPickPathActivity(String path, String encoding) {
        FileExplorerActivity.startPickPathActivity(this, path, encoding, RC_SAVE);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (resultCode != RESULT_OK)
            return;
        switch (requestCode) {
            case RC_OPEN_FILE:
                if (data == null)
                    break;
                openFile(FileExplorerActivity.getFile(data), FileExplorerActivity.getFileEncoding(data), data.getIntExtra("offset", 0));
                break;
            case RC_SAVE:
                String file = FileExplorerActivity.getFile(data);
                String encoding = FileExplorerActivity.getFileEncoding(data);

                IEditorPagerAdapter adapter = mTabManager.getEditorPagerAdapter();
                EditorDelegate delegate = adapter.getCurrentEditorDelegate();
                if (delegate != null) {
                    adapter.updateDescriptor(file, encoding);
                    delegate.saveTo(new File(file), encoding);
                }
                break;
            case RC_SETTINGS:
                break;
        }
    }

    private void openText(CharSequence content) {
        if (TextUtils.isEmpty(content))
            return;
        FileManager fileManager = new FileManager(this);
        File newFile = fileManager.createNewFile("_" + System.currentTimeMillis() + ".txt");
        if (IOUtils.writeFile(newFile, content.toString())) {
            mTabManager.newTab(newFile);
        }
    }

    private void openFile(String file) {
        openFile(file, null, 0);
    }

    public void openFile(String filePath, String encoding, int offset) {
        //ensure file exist, can read/write
        if (TextUtils.isEmpty(filePath)) {
            return;
        }
        File file = new File(filePath);
        if (!file.isFile()) {
            UIUtils.toast(this, R.string.file_not_exists);
            return;
        }
        if (!file.canRead()) {
            UIUtils.alert(this, this.getString(R.string.cannt_read_file, file.getPath()));
            return;
        }

        if (!mTabManager.newTab(file, offset, encoding)) {
            return;
        }
        DBHelper.getInstance(this).addRecentFile(filePath, encoding);
    }

    public void insertText(CharSequence text) {
        if (text == null)
            return;
        Command c = new Command(Command.CommandEnum.INSERT_TEXT);
        c.object = text;
        doCommand(c);
    }

    public TabManager getTabManager() {
        return mTabManager;
    }


    @Override
    public void onBackPressed() {
        if (mDrawerLayout != null) {
            if (mDrawerLayout.isDrawerOpen(Gravity.START)) {
                mDrawerLayout.closeDrawer(Gravity.START);
                return;
            }
            if (mDrawerLayout.isDrawerOpen(Gravity.END)) {
                mDrawerLayout.closeDrawer(Gravity.END);
                return;
            }
        }
        if (mSlidingUpPanelLayout != null) {
            SlidingUpPanelLayout.PanelState state = mSlidingUpPanelLayout.getPanelState();
            if (state == SlidingUpPanelLayout.PanelState.EXPANDED
                    || state == SlidingUpPanelLayout.PanelState.DRAGGING) {
                mSlidingUpPanelLayout.setPanelState(SlidingUpPanelLayout.PanelState.COLLAPSED);
                return;
            }
        }

        if ((System.currentTimeMillis() - mExitTime) > 2000) {
            UIUtils.toast(getContext(), R.string.press_again_will_exit);
            mExitTime = System.currentTimeMillis();
        } else {
            mTabManager.onDestroy();
            super.onBackPressed();
        }
    }

    public String getCurrentLang() {
        EditorDelegate editorDelegate = getCurrentEditorDelegate();
        if (editorDelegate == null)
            return null;

        return editorDelegate.getLang();
    }

    public RecyclerView getMenuRecyclerView() {
        return mMenuRecyclerView;
    }

    public RecyclerView getTabRecyclerView() {
        return mTabRecyclerView;
    }
}
