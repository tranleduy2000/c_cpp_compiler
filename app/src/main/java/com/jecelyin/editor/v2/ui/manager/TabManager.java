/*
 * Copyright (C) 2016 Jecelyin Peng <jecelyin@gmail.com>
 *
 * This file is part of 920 Text Editor.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.jecelyin.editor.v2.ui.manager;

import android.database.DataSetObserver;
import android.support.v4.view.GravityCompat;
import android.support.v4.view.ViewPager;
import android.view.View;

import com.duy.ccppcompiler.R;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.adapter.EditorPagerAdapter;
import com.jecelyin.editor.v2.adapter.TabAdapter;
import com.jecelyin.editor.v2.common.TabCloseListener;
import com.jecelyin.editor.v2.ui.activities.MainActivity;
import com.jecelyin.editor.v2.ui.editor.EditorDelegate;
import com.jecelyin.editor.v2.utils.DBHelper;
import com.jecelyin.editor.v2.utils.ExtGrep;
import com.jecelyin.editor.v2.view.EditorView;
import com.yqritc.recyclerviewflexibledivider.HorizontalDividerItemDecoration;

import java.io.File;
import java.util.ArrayList;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class TabManager implements ViewPager.OnPageChangeListener {
    private final MainActivity mActivity;
    private final TabAdapter mTabAdapter;
    private EditorPagerAdapter mEditorPagerAdapter;
    private boolean exitApp;

    public TabManager(MainActivity activity) {
        this.mActivity = activity;

        this.mTabAdapter = new TabAdapter();
        mTabAdapter.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onTabMenuViewsClick(v);
            }
        });
        mActivity.getTabRecyclerView().addItemDecoration(new HorizontalDividerItemDecoration.Builder(activity.getContext()).build());
        mActivity.getTabRecyclerView().setAdapter(mTabAdapter);

        initEditor();

        mActivity.mToolbar.setNavigationOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mActivity.mDrawerLayout.openDrawer(GravityCompat.START);
            }
        });
        mActivity.mEditorPager.addOnPageChangeListener(this);
    }

    private void onTabMenuViewsClick(View v) {
        switch (v.getId()) {
            case R.id.close_image_view:
                closeTab((int) v.getTag());
                break;
            default:
                int position = (int) v.getTag();
                mActivity.closeMenu();
                setCurrentTab(position);
                break;
        }
    }

    private void initEditor() {
        mEditorPagerAdapter = new EditorPagerAdapter(mActivity);
        mActivity.mEditorPager.setAdapter(mEditorPagerAdapter);

        if (Preferences.getInstance(mActivity).isOpenLastFiles()) {
            ArrayList<DBHelper.RecentFileItem> recentFiles = DBHelper.getInstance(mActivity).getRecentFiles(true);
            boolean needUpdateRecentFile = false;
            File file;
            for (DBHelper.RecentFileItem item : recentFiles) {
                file = new File(item.path);
                if (!(file.isFile() && file.canRead() && file.canWrite())) {
                    needUpdateRecentFile = true;
                    continue;
                }
                mEditorPagerAdapter.newEditor(false, file, item.offset, item.encoding);
                setCurrentTab(mEditorPagerAdapter.getCount() - 1); //fixme: auto load file, otherwise click other tab will crash by search result
            }
            mEditorPagerAdapter.notifyDataSetChanged();
            updateTabList();

            int lastTab = Preferences.getInstance(mActivity).getLastTab();
            setCurrentTab(lastTab);
        }

        mEditorPagerAdapter.registerDataSetObserver(new DataSetObserver() {
            @Override
            public void onChanged() {
                updateTabList();
                if (!exitApp && mEditorPagerAdapter.getCount() == 0) {
                    newTab();
                }
            }
        });

        if (mEditorPagerAdapter.getCount() == 0)
            mEditorPagerAdapter.newEditor(mActivity.getString(R.string.new_filename, mEditorPagerAdapter.countNoFileEditor() + 1), null);
    }

    public void newTab() {
        mEditorPagerAdapter.newEditor(mActivity.getString(R.string.new_filename, mEditorPagerAdapter.getCount() + 1), null);
        setCurrentTab(mEditorPagerAdapter.getCount() - 1);
    }

    public boolean newTab(CharSequence content) {
        mEditorPagerAdapter.newEditor(mActivity.getString(R.string.new_filename, mEditorPagerAdapter.getCount() + 1), content);
        setCurrentTab(mEditorPagerAdapter.getCount() - 1);
        return true;
    }

    public boolean newTab(ExtGrep grep) {
        mEditorPagerAdapter.newEditor(grep);
        setCurrentTab(mEditorPagerAdapter.getCount() - 1);
        return true;
    }

    public boolean newTab(File file, String encoding) {
        return newTab(file, 0, encoding);
    }

    public boolean newTab(File file, int offset, String encoding) {
        int count = mEditorPagerAdapter.getCount();
        for (int i = 0; i < count; i++) {
            EditorDelegate delegate = mEditorPagerAdapter.getEditorDelegateAt(i);
            if (delegate.getPath() == null)
                continue;
            if (delegate.getPath().equals(file.getPath())) {
                setCurrentTab(i);
                return false;
            }
        }
        mEditorPagerAdapter.newEditor(file, offset, encoding);
        setCurrentTab(count);
        return true;
    }

    public int getTabCount() {
        if (mTabAdapter == null)
            return 0;
        return mTabAdapter.getItemCount();
    }

    public int getCurrentTab() {
        return mActivity.mEditorPager.getCurrentItem();
    }

    public void setCurrentTab(int index) {
        int tabCount = mActivity.mEditorPager.getAdapter().getCount();
        index = Math.min(Math.max(0, index), tabCount);

        mActivity.mEditorPager.setCurrentItem(index);
        mTabAdapter.setCurrentTab(index);
        updateToolbar();
    }

    public void closeTab(int position) {
        mEditorPagerAdapter.removeEditor(position, new TabCloseListener() {
            @Override
            public void onClose(String path, String encoding, int offset) {
                DBHelper.getInstance(mActivity).updateRecentFile(path, false);
                int currentTab = getCurrentTab();
                if (getTabCount() != 0) {
                    setCurrentTab(currentTab);
                }
            }
        });
    }

    public EditorPagerAdapter getEditorPagerAdapter() {
        return mEditorPagerAdapter;
    }

    @Override
    public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {

    }

    @Override
    public void onPageSelected(int position) {
        mTabAdapter.setCurrentTab(position);
    }

    @Override
    public void onPageScrollStateChanged(int state) {

    }

    private void updateTabList() {
        mTabAdapter.setTabInfoList(mEditorPagerAdapter.getTabInfoList());
        mTabAdapter.notifyDataSetChanged();
    }

    public void updateEditorView(int index, EditorView editorView) {
        mEditorPagerAdapter.setEditorView(index, editorView);
    }

    public void onDocumentChanged(int index) {
        updateTabList();
        updateToolbar();
    }

    private void updateToolbar() {
        EditorDelegate delegate = mEditorPagerAdapter.getEditorDelegateAt(getCurrentTab());
        if (delegate == null)
            return;
        mActivity.mToolbar.setTitle(delegate.getToolbarText());
    }

    public boolean closeAllTabAndExitApp() {
        EditorDelegate.setDisableAutoSave(true);
        exitApp = true;
        if (mActivity.mEditorPager != null) {
            Preferences.getInstance(mActivity).setLastTab(getCurrentTab());
        }
        return mEditorPagerAdapter.removeAll(new TabCloseListener() {
            @Override
            public void onClose(String path, String encoding, int offset) {
                DBHelper.getInstance(mActivity).updateRecentFile(path, encoding, offset);
                int count = getTabCount();
                if (count == 0) {
                    mActivity.finish();
                } else {
                    mEditorPagerAdapter.removeAll(this);
                }
            }
        });
    }
}
