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

package com.duy.ide.editor;

import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.core.widget.EditAreaView;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.support.annotation.CallSuper;
import android.support.annotation.IdRes;
import android.support.annotation.NonNull;
import android.support.design.widget.NavigationView;
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
import android.view.SubMenu;
import android.view.View;
import android.widget.TextView;

import com.duy.common.StoreUtil;
import com.duy.file.explorer.FileExplorerActivity;
import com.duy.ide.editor.dialogs.DialogNewFile;
import com.duy.ide.editor.editor.BuildConfig;
import com.duy.ide.editor.editor.R;
import com.duy.ide.editor.pager.IEditorPagerAdapter;
import com.duy.ide.filemanager.FileManager;
import com.jecelyin.common.utils.DLog;
import com.jecelyin.common.utils.IOUtils;
import com.jecelyin.common.utils.SysUtils;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.FullScreenActivity;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.common.Command;
import com.jecelyin.editor.v2.dialog.CharsetsDialog;
import com.jecelyin.editor.v2.dialog.GotoLineDialog;
import com.jecelyin.editor.v2.dialog.LangListDialog;
import com.jecelyin.editor.v2.dialog.WrapCharDialog;
import com.jecelyin.editor.v2.editor.Document;
import com.jecelyin.editor.v2.editor.EditorDelegate;
import com.jecelyin.editor.v2.editor.task.SaveAllTask;
import com.jecelyin.editor.v2.manager.MenuManager;
import com.jecelyin.editor.v2.manager.RecentFilesManager;
import com.jecelyin.editor.v2.manager.TabManager;
import com.jecelyin.editor.v2.settings.EditorSettingsActivity;
import com.jecelyin.editor.v2.utils.DBHelper;
import com.jecelyin.editor.v2.widget.SymbolBarLayout;
import com.jecelyin.editor.v2.widget.menu.MenuDef;
import com.jecelyin.editor.v2.widget.menu.MenuFactory;
import com.jecelyin.editor.v2.widget.menu.MenuGroup;
import com.jecelyin.editor.v2.widget.menu.MenuItemInfo;

import org.gjt.sp.jedit.Catalog;
import org.gjt.sp.jedit.Mode;

import java.io.File;
import java.io.InputStream;
import java.util.List;
import java.util.Map;


/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class SimpleEditorActivity extends FullScreenActivity implements MenuItem.OnMenuItemClickListener,
        SharedPreferences.OnSharedPreferenceChangeListener {
    protected static final int RC_OPEN_FILE = 1;
    private static final String TAG = "MainActivity";
    private final static int RC_SAVE = 3;
    private static final int RC_SETTINGS = 5;

    public Toolbar mToolbar;
    public ViewPager mEditorPager;
    public DrawerLayout mDrawerLayout;
    public RecyclerView mTabRecyclerView;
    public TextView mVersionTextView;
    public SymbolBarLayout mSymbolBarLayout;
    protected TabManager mTabManager;
    private Preferences mPreferences;
    private MenuManager mMenuManager;
    private long mExitTime;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_editor);
        mToolbar = findViewById(R.id.toolbar);
        setSupportActionBar(mToolbar);

        MenuManager.init(this);
        mPreferences = Preferences.getInstance(this);

        mEditorPager = findViewById(R.id.view_pager);
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
        super.onSharedPreferenceChanged(sharedPreferences, key);
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
        mTabRecyclerView.setLayoutManager(new LinearLayoutManager(this));
        mDrawerLayout.setEnabled(true);

        initToolbar();

        if (mMenuManager == null) {
            mMenuManager = new MenuManager(this);
            NavigationView rightMenu = findViewById(R.id.menuNavView);
            Menu menu = rightMenu.getMenu();
            onCreateNavigationMenu(menu);
            rightMenu.setNavigationItemSelectedListener(new NavigationView.OnNavigationItemSelectedListener() {
                @Override
                public boolean onNavigationItemSelected(@NonNull MenuItem item) {
                    return onOptionsItemSelected(item);
                }
            });
        }
    }


    private void initToolbar() {
        mToolbar.setNavigationIcon(R.drawable.ic_drawer_raw);
        mToolbar.setNavigationContentDescription(R.string.tab);
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
            SubMenu subMenu = container.addSubMenu(group.getNameResId());
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
            SubMenu subMenu = menu.addSubMenu(group.getNameResId());
            List<MenuItemInfo> items = menuFactory.getMenuItemsWithoutToolbarMenu(group);
            for (MenuItemInfo item : items) {
                MenuItem menuItem = subMenu.add(MenuDef.GROUP_TOOLBAR, item.getItemId(), item.getOrder(), item.getTitleResId());
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

        } else if (id == R.id.m_history) {
            RecentFilesManager rfm = new RecentFilesManager(this);
            rfm.setOnFileItemClickListener(new RecentFilesManager.OnFileItemClickListener() {
                @Override
                public void onClick(String file, String encoding) {
                    openFile(file, encoding, 0);
                }
            });
            rfm.show(this);

        } else if (id == R.id.action_wrap) {
            new WrapCharDialog(this).show();

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
            saveAll();

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
            EditorSettingsActivity.startActivity(this, RC_SETTINGS);

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
//            homeDir = getFilesDir().getAbsolutePath();
        }
        if (editorDelegate != null) {
            sourceDir = new File(editorDelegate.getPath()).getParent();
        } else {
            sourceDir = homeDir;
        }
        FileExplorerActivity.startPickFileActivity(this, sourceDir, homeDir, RC_OPEN_FILE);
    }

    public void createNewFile() {
        String[] fileExtensions = getSupportedFileExtensions();
        EditorDelegate currentEditorDelegate = getCurrentEditorDelegate();
        String path = null;
        if (currentEditorDelegate != null) {
            path = currentEditorDelegate.getPath();
        } else {
            path = Environment.getExternalStorageDirectory().getPath();
        }
        DialogNewFile dialog = new DialogNewFile(this, fileExtensions, path,
                new DialogNewFile.OnCreateFileListener() {
                    @Override
                    public void onFileCreated(@NonNull File file) {
                        mTabManager.newTab(file);
                    }
                });
        dialog.show();
    }

    protected String[] getSupportedFileExtensions() {
        return new String[]{".txt"};
    }

    public void saveAll() {
        SaveAllTask saveAllTask = new SaveAllTask(this, null);
        saveAllTask.execute();
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

    protected EditorDelegate getCurrentEditorDelegate() {
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
                    DBHelper.getInstance(this).addRecentFile(file, encoding);
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
        DBHelper.getInstance(SimpleEditorActivity.this).addRecentFile(file.getPath(), encoding);
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

    public RecyclerView getTabRecyclerView() {
        return mTabRecyclerView;
    }

    @CallSuper
    public void invalidateEditMenu(Document document, EditAreaView mEditText) {
        setMenuStatus(R.id.action_save, document.isChanged() ? MenuDef.STATUS_NORMAL : MenuDef.STATUS_DISABLED);
        setMenuStatus(R.id.action_undo, mEditText != null && mEditText.canUndo() ? MenuDef.STATUS_NORMAL : MenuDef.STATUS_DISABLED);
        setMenuStatus(R.id.action_redo, mEditText != null && mEditText.canRedo() ? MenuDef.STATUS_NORMAL : MenuDef.STATUS_DISABLED);
    }
}
