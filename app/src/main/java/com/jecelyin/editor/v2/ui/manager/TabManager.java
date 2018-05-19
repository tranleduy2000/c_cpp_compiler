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
import android.support.annotation.Nullable;
import android.support.v4.util.Pair;
import android.support.v4.view.GravityCompat;
import android.support.v4.view.ViewPager;
import android.view.View;

import com.duy.ccppcompiler.R;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.adapter.EditorFragmentPagerAdapter;
import com.jecelyin.editor.v2.adapter.TabAdapter;
import com.jecelyin.editor.v2.common.TabCloseListener;
import com.duy.editor.EditorActivity;
import com.jecelyin.editor.v2.ui.editor.EditorDelegate;
import com.jecelyin.editor.v2.ui.editor.EditorPageDescriptor;
import com.jecelyin.editor.v2.utils.DBHelper;
import com.yqritc.recyclerviewflexibledivider.HorizontalDividerItemDecoration;

import java.io.File;
import java.util.ArrayList;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class TabManager implements ViewPager.OnPageChangeListener {
    private final EditorActivity mActivity;
    private final TabAdapter mTabAdapter;
    private EditorFragmentPagerAdapter mEditorFragmentPagerAdapter;

    public TabManager(EditorActivity activity) {
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
            case R.id.btn_close:
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
        mEditorFragmentPagerAdapter = new EditorFragmentPagerAdapter(mActivity);
        mActivity.mEditorPager.setAdapter(mEditorFragmentPagerAdapter);

        if (Preferences.getInstance(mActivity).isOpenLastFiles()) {
            ArrayList<DBHelper.RecentFileItem> recentFiles = DBHelper.getInstance(mActivity).getRecentFiles(true);
            ArrayList<EditorPageDescriptor> descriptors = new ArrayList<>();
            File file;
            for (DBHelper.RecentFileItem item : recentFiles) {
                file = new File(item.path);
                if (!(file.isFile() && file.canRead() && file.canWrite())) {
                    DBHelper.getInstance(mActivity).updateRecentFile(item.path, false);
                    continue;
                }
                descriptors.add(new EditorPageDescriptor(file, item.offset, item.encoding));
            }

            mEditorFragmentPagerAdapter.addAll(descriptors);
            mActivity.mEditorPager.setOffscreenPageLimit(mEditorFragmentPagerAdapter.getCount());
            updateTabList();

            int lastTab = Preferences.getInstance(mActivity).getLastTab();
            setCurrentTab(lastTab);

            if (descriptors.isEmpty()) {
                mActivity.createNewFile();
            }
        }

        mEditorFragmentPagerAdapter.registerDataSetObserver(new DataSetObserver() {
            @Override
            public void onChanged() {
                updateTabList();
                updateToolbar();
                mActivity.mEditorPager.setOffscreenPageLimit(mEditorFragmentPagerAdapter.getCount());
            }
        });

        // TODO: 25-Apr-18 show layout create new file
    }

    public void newTab(File file) {
        newTab(file, 0, "UTF-8");
    }

    public boolean newTab(File file, int offset, String encoding) {
        int count = mEditorFragmentPagerAdapter.getCount();
        for (int i = 0; i < count; i++) {
            EditorPageDescriptor descriptor = mEditorFragmentPagerAdapter.getItem(i);
            if (descriptor.getPath() == null)
                continue;
            if (descriptor.getPath().equals(file.getPath())) {
                setCurrentTab(i);
                return false;
            }
        }
        mEditorFragmentPagerAdapter.newEditor(file, offset, encoding);
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
        mEditorFragmentPagerAdapter.removeEditor(position, new TabCloseListener() {
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

    @Override
    public void onPageScrolled(int position, float positionOffset, int positionOffsetPixels) {

    }

    @Override
    public void onPageSelected(int position) {
        mTabAdapter.setCurrentTab(position);
        updateToolbar();
    }

    @Override
    public void onPageScrollStateChanged(int state) {

    }

    private void updateTabList() {
        mTabAdapter.setTabInfoList(mEditorFragmentPagerAdapter.getTabInfoList());
        mTabAdapter.notifyDataSetChanged();
    }

    public void onDocumentChanged() {
        updateTabList();
        updateToolbar();
    }

    private void updateToolbar() {
        EditorDelegate delegate = mEditorFragmentPagerAdapter.getEditorDelegateAt(getCurrentTab());
        if (delegate == null) {
            mActivity.mToolbar.setTitle("");
        } else {
            mActivity.mToolbar.setTitle(delegate.getToolbarText());
        }
    }

    public void onDestroy() {
        if (mActivity.mEditorPager != null) {
            Preferences.getInstance(mActivity).setLastTab(getCurrentTab());
        }
        ArrayList<EditorDelegate> allEditor = mEditorFragmentPagerAdapter.getAllEditor();
        for (EditorDelegate editorDelegate : allEditor) {
            String path = editorDelegate.getPath();
            String encoding = editorDelegate.getEncoding();
            int offset = editorDelegate.getCursorOffset();
            DBHelper.getInstance(mActivity).updateRecentFile(path, encoding, offset);
        }
    }

    public EditorFragmentPagerAdapter getEditorPagerAdapter() {
        return mEditorFragmentPagerAdapter;
    }

    /**
     * Get current editor edit file
     *
     * @return first is index, second is {@link EditorDelegate}, null if not found
     */
    @Nullable
    public Pair<Integer, EditorDelegate> getEditorDelegate(File file) {
        String path = file.getPath();
        ArrayList<EditorDelegate> allEditor = mEditorFragmentPagerAdapter.getAllEditor();
        for (int i = 0, allEditorSize = allEditor.size(); i < allEditorSize; i++) {
            EditorDelegate editorDelegate = allEditor.get(i);
            if (editorDelegate.getPath().equals(path)) {
                return new Pair<>(i, editorDelegate);
            }
        }
        return null;
    }
}
