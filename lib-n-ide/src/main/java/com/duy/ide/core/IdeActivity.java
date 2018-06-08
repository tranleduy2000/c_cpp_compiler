/*
 *  Copyright (C)  2018  Duy Tran Le
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.duy.ide.core;

import android.Manifest;
import android.app.ProgressDialog;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.graphics.Rect;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.support.annotation.CallSuper;
import android.support.annotation.IdRes;
import android.support.annotation.LayoutRes;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.design.widget.NavigationView;
import android.support.v4.app.ActivityCompat;
import android.support.v4.app.FragmentManager;
import android.support.v4.view.GravityCompat;
import android.support.v4.view.ViewPager;
import android.support.v4.widget.DrawerLayout;
import android.support.v7.app.ActionBarDrawerToggle;
import android.support.v7.widget.Toolbar;
import android.text.TextUtils;
import android.view.Gravity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.SubMenu;
import android.view.View;
import android.view.ViewTreeObserver;
import android.widget.TextView;

import com.duy.common.StoreUtil;
import com.duy.file.explorer.FileExplorerActivity;
import com.duy.ide.code.api.CodeFormatProvider;
import com.duy.ide.code.format.CodeFormatProviderImpl;
import com.duy.ide.database.SQLHelper;
import com.duy.ide.diagnostic.DiagnosticContract;
import com.duy.ide.diagnostic.DiagnosticPresenter;
import com.duy.ide.diagnostic.parser.PatternAwareOutputParser;
import com.duy.ide.diagnostic.view.DiagnosticFragment;
import com.duy.ide.editor.editor.BuildConfig;
import com.duy.ide.editor.editor.R;
import com.duy.ide.file.FileManager;
import com.duy.ide.file.SaveListener;
import com.duy.ide.file.dialogs.DialogNewFile;
import com.duy.ide.settings.EditorSettingsActivity;
import com.jecelyin.common.utils.DLog;
import com.jecelyin.common.utils.IOUtils;
import com.jecelyin.common.utils.SysUtils;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.ThemeSupportActivity;
import com.jecelyin.editor.v2.common.Command;
import com.jecelyin.editor.v2.dialog.CharsetsDialog;
import com.jecelyin.editor.v2.dialog.GotoLineDialog;
import com.jecelyin.editor.v2.dialog.LangListDialog;
import com.jecelyin.editor.v2.editor.EditorDelegate;
import com.jecelyin.editor.v2.editor.IEditorDelegate;
import com.jecelyin.editor.v2.editor.IEditorStateListener;
import com.jecelyin.editor.v2.editor.task.SaveAllTask;
import com.jecelyin.editor.v2.manager.MenuManager;
import com.jecelyin.editor.v2.manager.RecentFilesManager;
import com.jecelyin.editor.v2.manager.TabManager;
import com.jecelyin.editor.v2.widget.SymbolBarLayout;
import com.jecelyin.editor.v2.widget.menu.MenuDef;
import com.jecelyin.editor.v2.widget.menu.MenuFactory;
import com.jecelyin.editor.v2.widget.menu.MenuGroup;
import com.jecelyin.editor.v2.widget.menu.MenuItemInfo;
import com.ogaclejapan.smarttablayout.SmartTabLayout;
import com.sothree.slidinguppanel.SlidingUpPanelLayout;

import org.gjt.sp.jedit.Catalog;
import org.gjt.sp.jedit.Mode;

import java.io.File;
import java.io.InputStream;
import java.util.List;
import java.util.Map;

import static android.view.View.VISIBLE;


/**
 * Some method should override:
 * <p>
 * {@link #populateDiagnostic(DiagnosticContract.Presenter)} init diagnostic, you should set output
 * parser for std out and std error by using method
 * {@link DiagnosticContract.Presenter#setOutputParser(PatternAwareOutputParser...)}
 */
public abstract class IdeActivity extends ThemeSupportActivity implements MenuItem.OnMenuItemClickListener,
        IEditorStateListener,
        SharedPreferences.OnSharedPreferenceChangeListener {

    protected static final int RC_OPEN_FILE = 1;
    private final static int RC_SAVE_AS = 3;
    private static final int RC_SETTINGS = 5;
    private static final int RC_PERMISSION_WRITE_STORAGE = 5001;

    //Handle create on MainThread, use for update UI
    private final Handler mHandler = new Handler();
    public SlidingUpPanelLayout mSlidingUpPanelLayout;

    protected TabManager mTabManager;
    protected DiagnosticPresenter mDiagnosticPresenter;

    protected Toolbar mToolbar;
    protected DrawerLayout mDrawerLayout;
    @Nullable
    protected SymbolBarLayout mSymbolBarLayout;
    @Nullable
    protected SmartTabLayout mTabLayout;
    @Nullable
    protected TextView mTxtDocumentInfo;

    protected Preferences mPreferences;
    private long mExitTime;
    private KeyBoardEventListener mKeyBoardListener;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(getRootLayoutId());
        initToolbar();
        MenuManager.init(this);

        mPreferences = Preferences.getInstance(this);
        mPreferences.registerOnSharedPreferenceChangeListener(this);

        mTabLayout = findViewById(R.id.tab_layout);
        mTxtDocumentInfo = findViewById(R.id.txt_document_info);
        mDrawerLayout = findViewById(R.id.drawer_layout);
        mDrawerLayout.setKeepScreenOn(mPreferences.isKeepScreenOn());
        mDrawerLayout.addDrawerListener(new DrawerLayout.SimpleDrawerListener() {
            @Override
            public void onDrawerOpened(View drawerView) {
                super.onDrawerOpened(drawerView);
                mDrawerLayout.requestFocus();
                hideSoftInput();
            }
        });

        ActionBarDrawerToggle actionBarDrawerToggle = new ActionBarDrawerToggle(this,
                mDrawerLayout, mToolbar, R.string.open_drawer, R.string.close_drawer);
        mDrawerLayout.addDrawerListener(actionBarDrawerToggle);
        actionBarDrawerToggle.syncState();
        mSymbolBarLayout = findViewById(R.id.symbolBarLayout);
        if (mSymbolBarLayout != null) {
            mSymbolBarLayout.setOnSymbolCharClickListener(
                    new SymbolBarLayout.OnSymbolCharClickListener() {
                        @Override
                        public void onClick(View v, String text) {
                            insertText(text);
                        }
                    });
        }

        setScreenOrientation();

        TextView versionView = findViewById(R.id.versionTextView);
        versionView.setText(SysUtils.getVersionName(this));

        initMenuView();
        intiDiagnosticView();


        //attach listener hide/show keyboard
        mKeyBoardListener = new KeyBoardEventListener(this);
        mDrawerLayout.getViewTreeObserver().addOnGlobalLayoutListener(mKeyBoardListener);

//        initLeftNavigationView(findViewById(R.id.left_nav_view));

        //final, create editor
        mTabManager = new TabManager(this, (ViewPager) findViewById(R.id.editor_view_pager));

        processIntent();
    }

    private void initToolbar() {
        mToolbar = findViewById(R.id.toolbar);
        setSupportActionBar(mToolbar);
        setTitle("");
    }

    private void intiDiagnosticView() {
        if (mTabManager == null) {
            throw new RuntimeException("Create TabManager before call intiDiagnosticView");
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
        DiagnosticFragment diagnosticFragment = (DiagnosticFragment) fm.findFragmentByTag(tag);
        if (diagnosticFragment == null) {
            diagnosticFragment = DiagnosticFragment.newInstance();
        }
        fm.beginTransaction()
                .replace(R.id.container_diagnostic_list_view, diagnosticFragment, tag)
                .commit();

        mDiagnosticPresenter = new DiagnosticPresenter(diagnosticFragment, this, mTabManager, mHandler);
        populateDiagnostic(mDiagnosticPresenter);
    }

    /**
     * Called when create diagnostic view
     */
    protected abstract void populateDiagnostic(@NonNull DiagnosticContract.Presenter diagnosticPresenter);

    /**
     * @return main layout id
     */
    @LayoutRes
    protected abstract int getRootLayoutId();

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {
        super.onSharedPreferenceChanged(sharedPreferences, key);
        switch (key) {
            case Preferences.KEY_KEEP_SCREEN_ON:
                if (mToolbar != null) {
                    mToolbar.setKeepScreenOn(sharedPreferences.getBoolean(key, false));
                }
                break;
            case Preferences.KEY_SCREEN_ORIENTATION:
                setScreenOrientation();
                break;
            case Preferences.KEY_READ_ONLY:
                if (mSymbolBarLayout != null) {
                    mSymbolBarLayout.setVisibility(mPreferences.isReadOnly() ? View.GONE : VISIBLE);
                }
                break;
        }
    }

    private void setScreenOrientation() {
        int orientation = mPreferences.getScreenOrientation();
        if (Preferences.SCREEN_ORIENTATION_AUTO == orientation) {
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED);
        } else if (Preferences.SCREEN_ORIENTATION_LANDSCAPE == orientation) {
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
        } else if (Preferences.SCREEN_ORIENTATION_PORTRAIT == orientation) {
            setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);
        }
    }

    private void initMenuView() {
        NavigationView rightMenu = findViewById(R.id.menuNavView);
        Menu menu = rightMenu.getMenu();
        onCreateNavigationMenu(menu);
        rightMenu.setNavigationItemSelectedListener(
                new NavigationView.OnNavigationItemSelectedListener() {
                    @Override
                    public boolean onNavigationItemSelected(@NonNull MenuItem item) {
                        return onOptionsItemSelected(item);
                    }
                });
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
                UIUtils.alert(this, getString(R.string.cannt_handle_intent_x, getIntent().toString()));
            }
        } catch (Throwable e) {
            DLog.e(e);
            UIUtils.alert(this, getString(R.string.handle_intent_x_error, getIntent().toString() + "\n" + e.getMessage()));
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
            if (intent.getScheme() != null && intent.getData() != null) {
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
     * @param status {@link MenuDef#STATUS_NORMAL}, {@link MenuDef#STATUS_DISABLED}
     */
    public void setMenuStatus(@IdRes int menuResId, int status) {
        if (mToolbar == null) {
            return;
        }
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

    @CallSuper
    @Override
    public boolean onCreateOptionsMenu(Menu container) {
        MenuFactory menuFactory = MenuFactory.getInstance(this);
        List<MenuItemInfo> topMenu = menuFactory.getToolbarIcon();
        for (MenuItemInfo item : topMenu) {
            MenuItem menuItem = container.add(MenuDef.GROUP_TOOLBAR, item.getItemId(), item.getOrder(), item.getTitleResId());
            menuItem.setIcon(MenuManager.makeToolbarNormalIcon(this, item.getIconResId()));
            menuItem.setShowAsAction(MenuItem.SHOW_AS_ACTION_ALWAYS);
        }

        MenuGroup[] values = new MenuGroup[]{MenuGroup.FILE, MenuGroup.EDIT};
        for (MenuGroup group : values) {
            if (group == MenuGroup.TOP) {
                continue;
            }
            SubMenu subMenu = container.addSubMenu(MenuDef.GROUP_TOOLBAR, group.getMenuId(), 0, group.getTitleId());
            subMenu.getItem().setShowAsAction(MenuItem.SHOW_AS_ACTION_ALWAYS | MenuItem.SHOW_AS_ACTION_WITH_TEXT);

            List<MenuItemInfo> items = menuFactory.getMenuItemsWithoutToolbarMenu(group);
            for (MenuItemInfo item : items) {
                MenuItem menuItem = subMenu.add(MenuDef.GROUP_TOOLBAR, item.getItemId(), item.getOrder(), item.getTitleResId());
                menuItem.setIcon(MenuManager.makeMenuNormalIcon(this, item.getIconResId()));
                menuItem.setShowAsAction(MenuItem.SHOW_AS_ACTION_ALWAYS);
            }
        }

        MenuItem menuItem = container.add(MenuDef.GROUP_TOOLBAR, R.id.m_menu, Menu.NONE, getString(R.string.more_menu));
        menuItem.setIcon(MenuManager.makeToolbarNormalIcon(this, R.drawable.ic_more_horiz_white_24dp));
        menuItem.setShowAsAction(MenuItem.SHOW_AS_ACTION_ALWAYS);
        return super.onCreateOptionsMenu(container);
    }

    @CallSuper
    protected void onCreateNavigationMenu(Menu menu) {
        MenuFactory menuFactory = MenuFactory.getInstance(this);
        MenuGroup[] groups = new MenuGroup[]{MenuGroup.VIEW, MenuGroup.OTHER};
        for (MenuGroup group : groups) {
            if (group == MenuGroup.TOP) {
                continue;
            }
            SubMenu subMenu = menu.addSubMenu(group.getTitleId());
            List<MenuItemInfo> items = menuFactory.getMenuItemsWithoutToolbarMenu(group);
            for (MenuItemInfo item : items) {
                MenuItem menuItem = subMenu.add(MenuDef.GROUP_NAVIGATION, item.getItemId(), item.getOrder(), item.getTitleResId());
                menuItem.setIcon(item.getIconResId());
            }
        }
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        return onMenuItemClick(item) || super.onOptionsItemSelected(item);
    }

    @Override
    public boolean onMenuItemClick(MenuItem item) {
        onMenuClick(item.getItemId());
        return true;
    }

    private void onMenuClick(int id) {
        Command.CommandEnum commandEnum;

        closeMenu();

        if (id == R.id.action_new_file) {
            createNewFile();

        } else if (id == R.id.action_open) {
            openFileExplorer();

        } else if (id == R.id.action_goto_line) {
            new GotoLineDialog(this).show();

        } else if (id == R.id.action_file_history) {
            RecentFilesManager rfm = new RecentFilesManager(this);
            rfm.setOnFileItemClickListener(new RecentFilesManager.OnFileItemClickListener() {
                @Override
                public void onClick(String file, String encoding) {
                    openFile(file, encoding, 0);
                }
            });
            rfm.show(this);

        } else if (id == R.id.action_highlight) {
            new LangListDialog(this).show();

        } else if (id == R.id.m_menu) {
            hideSoftInput();

            mDrawerLayout.postDelayed(new Runnable() {
                @Override
                public void run() {
                    mDrawerLayout.openDrawer(GravityCompat.END);
                }
            }, 200);


        } else if (id == R.id.action_save_all) {
            UIUtils.toast(this, R.string.save_all);
            saveAll(0);

        } else if (id == R.id.m_fullscreen) {
            boolean fullscreenMode = mPreferences.isFullScreenMode();
            mPreferences.setFullScreenMode(!fullscreenMode);
            UIUtils.toast(this, fullscreenMode
                    ? R.string.disabled_fullscreen_mode_message
                    : R.string.enable_fullscreen_mode_message);

        } else if (id == R.id.m_readonly) {
            boolean readOnly = !mPreferences.isReadOnly();
            mPreferences.setReadOnly(readOnly);
            doCommandForAllEditor(new Command(Command.CommandEnum.READONLY_MODE));

        } else if (id == R.id.action_encoding) {
            new CharsetsDialog(this).show();

        } else if (id == R.id.action_editor_setting) {
            EditorSettingsActivity.open(this, RC_SETTINGS);

        } else if (id == R.id.action_share) {
            StoreUtil.shareThisApp(this);
        } else if (id == R.id.action_rate) {
            StoreUtil.gotoPlayStore(this, getPackageName());
        } else {
            commandEnum = MenuFactory.getInstance(this).idToCommandEnum(id);
            if (commandEnum != Command.CommandEnum.NONE)
                doCommand(new Command(commandEnum));
        }
    }

    private void openFileExplorer() {
        EditorDelegate editorDelegate = getCurrentEditorDelegate();
        String sourceDir;
        String homeDir = Environment.getExternalStorageDirectory().getAbsolutePath();
        if (BuildConfig.DEBUG) {
            homeDir = getFilesDir().getAbsolutePath();
        }
        if (editorDelegate != null) {
            sourceDir = new File(editorDelegate.getPath()).getParent();
        } else {
            sourceDir = homeDir;
        }
        FileExplorerActivity.startPickFileActivity(this, sourceDir, homeDir, RC_OPEN_FILE);
    }

    public void createNewFile() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            if (ActivityCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE)
                    != PackageManager.PERMISSION_GRANTED) {
                requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE}, RC_PERMISSION_WRITE_STORAGE);
                return;
            }
        }

        String[] fileExtensions = getSupportedFileExtensions();
        EditorDelegate currentEditorDelegate = getCurrentEditorDelegate();
        String path;
        if (currentEditorDelegate != null) {
            path = currentEditorDelegate.getPath();
            if (new File(path).isFile()) {
                path = new File(path).getParent();
            }
        } else {
            path = Environment.getExternalStorageDirectory().getPath();
        }
        DialogNewFile dialog = DialogNewFile.newInstance(fileExtensions, path, new DialogNewFile.OnCreateFileListener() {
            @Override
            public void onFileCreated(@NonNull File file) {
                mTabManager.newTab(file);
            }
        });
        dialog.show(getSupportFragmentManager(), DialogNewFile.class.getSimpleName());
    }

    protected String[] getSupportedFileExtensions() {
        return new String[]{".txt"};
    }

    public void saveAll(final int requestCode) {
        final ProgressDialog dialog = new ProgressDialog(this);
        dialog.setTitle(R.string.saving);
        dialog.setCanceledOnTouchOutside(false);
        dialog.setCancelable(false);
        dialog.show();
        SaveAllTask saveAllTask = new SaveAllTask(this, new SaveListener() {
            @Override
            public void onSavedSuccess() {
                onSaveComplete(requestCode);
                dialog.cancel();
            }

            @Override
            public void onSaveFailed(Exception e) {
                UIUtils.alert(IdeActivity.this, e.getMessage());
                dialog.cancel();
            }
        });
        saveAllTask.execute();
    }

    protected void onSaveComplete(int requestCode) {

    }

    public void closeMenu() {
        if (mDrawerLayout.isDrawerOpen(GravityCompat.START)) {
            mDrawerLayout.closeDrawer(GravityCompat.START);
        }
        if (mDrawerLayout.isDrawerOpen(GravityCompat.END)) {
            mDrawerLayout.closeDrawer(GravityCompat.END);
        }
    }

    private void hideSoftInput() {
        doCommand(new Command(Command.CommandEnum.HIDE_SOFT_INPUT));
    }

    public void doCommandForAllEditor(Command command) {
        for (IEditorDelegate editorDelegate : mTabManager.getEditorPagerAdapter().getAllEditor()) {
            editorDelegate.doCommand(command);
        }
    }

    public void doCommand(Command command) {
        EditorDelegate editorDelegate = getCurrentEditorDelegate();
        if (editorDelegate != null) {
            editorDelegate.doCommand(command);
            if (command.what == Command.CommandEnum.HIGHLIGHT) {
                if (mToolbar != null) {
                    mToolbar.setTitle(editorDelegate.getToolbarText());
                }
            }
        }
    }

    protected EditorDelegate getCurrentEditorDelegate() {
        if (mTabManager == null || mTabManager.getEditorPagerAdapter() == null)
            return null;
        return mTabManager.getEditorPagerAdapter().getCurrentEditorDelegate();
    }

    public void startPickPathActivity(String path, String encoding) {
        FileExplorerActivity.startPickPathActivity(this, path, encoding, RC_SAVE_AS);
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == RC_PERMISSION_WRITE_STORAGE &&
                grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            createNewFile();
        }
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
            case RC_SAVE_AS:
                String file = FileExplorerActivity.getFile(data);
                if (file == null) {
                    return;
                }
                String encoding = FileExplorerActivity.getFileEncoding(data);
                EditorDelegate delegate = getCurrentEditorDelegate();
                if (delegate != null) {
                    delegate.saveInBackground(new File(file), encoding);
                    SQLHelper.getInstance(this).addRecentFile(file, encoding);
                    SQLHelper.getInstance(this).updateRecentFile(file, false);
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

    public void openFile(final String filePath, final String encoding, final int offset) {
        //ensure file exist, can read/write
        if (TextUtils.isEmpty(filePath)) {
            return;
        }
        final File file = new File(filePath);
        if (!file.isFile()) {
            UIUtils.toast(this, R.string.file_not_exists);
            return;
        }
        if (!file.canRead()) {
            UIUtils.alert(this, this.getString(R.string.cannt_read_file, file.getPath()));
            return;
        }

        boolean textFile = false;
        for (Map.Entry<String, Mode> mode : Catalog.modes.entrySet()) {
            if (mode.getValue().accept(file.getPath(), file.getName(), "")) {
                textFile = true;
                break;
            }
        }
        if (!textFile) {
            UIUtils.showConfirmDialog(this, getString(R.string.not_a_text_file, file.getName()),
                    new UIUtils.OnClickCallback() {
                        @Override
                        public void onOkClick() {
                            createNewEditor(file, offset, encoding);
                        }
                    });
        } else {
            createNewEditor(file, offset, encoding);
        }

    }

    private void createNewEditor(File file, int offset, String encoding) {
        if (!mTabManager.newTab(file, offset, encoding)) {
            return;
        }
        SQLHelper.getInstance(IdeActivity.this).addRecentFile(file.getPath(), encoding);
    }

    public void insertText(CharSequence text) {
        if (text == null) {
            return;
        }
        Command c = new Command(Command.CommandEnum.INSERT_TEXT);
        c.object = text;
        doCommand(c);
    }

    public TabManager getTabManager() {
        return mTabManager;
    }

    @Override
    public void onBackPressed() {
        if (closeDrawers()) {
            return;
        }
        if (mTabManager.onDestroy()) {
            if ((System.currentTimeMillis() - mExitTime) > 2000) {
                UIUtils.toast(this, R.string.press_again_will_exit);
                mExitTime = System.currentTimeMillis();
            } else {
                super.onBackPressed();
            }
        }
    }

    protected boolean closeDrawers() {
        if (mDrawerLayout != null) {
            if (mDrawerLayout.isDrawerOpen(Gravity.START)) {
                mDrawerLayout.closeDrawer(Gravity.START);
                return true;
            }
            if (mDrawerLayout.isDrawerOpen(Gravity.END)) {
                mDrawerLayout.closeDrawer(Gravity.END);
                return true;
            }
        }
        return false;
    }

    public String getCurrentLang() {
        EditorDelegate editorDelegate = getCurrentEditorDelegate();
        if (editorDelegate == null)
            return null;

        return editorDelegate.getLang();
    }

    @Override
    public void onEditorViewCreated(IEditorDelegate editorDelegate) {
        editorDelegate.setCodeFormatProvider(getCodeFormatProvider());
    }

    protected void onShowKeyboard() {
        if (mTabLayout != null) {
            mTabLayout.setVisibility(View.GONE);
        }
        if (mTxtDocumentInfo != null) {
            mTxtDocumentInfo.setVisibility(VISIBLE);
        }
    }

    protected void onHideKeyboard() {
        if (mTabLayout != null) {
            mTabLayout.setVisibility(VISIBLE);
        }
        if (mTxtDocumentInfo != null) {
            mTxtDocumentInfo.setVisibility(View.GONE);
        }
    }

    @Override
    protected void onDestroy() {
        mDrawerLayout.getViewTreeObserver()
                .removeGlobalOnLayoutListener(mKeyBoardListener);
        super.onDestroy();
    }

    @Nullable
    protected CodeFormatProvider getCodeFormatProvider() {
        return new CodeFormatProviderImpl(this);
    }

    private class KeyBoardEventListener implements ViewTreeObserver.OnGlobalLayoutListener {
        IdeActivity activity;

        KeyBoardEventListener(IdeActivity activityIde) {
            this.activity = activityIde;
        }

        public void onGlobalLayout() {
            int i = 0;
            int navHeight = this.activity.getResources().getIdentifier("navigation_bar_height", "dimen", "android");
            navHeight = navHeight > 0 ? this.activity.getResources().getDimensionPixelSize(navHeight) : 0;
            int statusBarHeight = this.activity.getResources().getIdentifier("status_bar_height", "dimen", "android");
            if (statusBarHeight > 0) {
                i = this.activity.getResources().getDimensionPixelSize(statusBarHeight);
            }
            Rect rect = new Rect();
            activity.getWindow().getDecorView().getWindowVisibleDisplayFrame(rect);
            if (activity.mDrawerLayout.getRootView().getHeight() - ((navHeight + i) + rect.height()) <= 0) {
                activity.onHideKeyboard();
            } else {
                activity.onShowKeyboard();
            }
        }
    }
}
