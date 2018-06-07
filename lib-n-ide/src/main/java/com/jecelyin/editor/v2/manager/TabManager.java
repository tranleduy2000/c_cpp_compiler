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

package com.jecelyin.editor.v2.manager;

import android.content.DialogInterface;
import android.database.DataSetObserver;
import android.support.annotation.Nullable;
import android.support.v4.util.Pair;
import android.support.v4.view.ViewPager;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.View;

import com.duy.ide.core.IdeActivity;
import com.duy.ide.database.RecentFileItem;
import com.duy.ide.database.SQLHelper;
import com.duy.ide.editor.editor.R;
import com.duy.ide.editor.pager.EditorFragmentPagerAdapter;
import com.duy.ide.editor.pager.EditorPageDescriptor;
import com.duy.ide.file.SaveListener;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.adapter.TabAdapter;
import com.jecelyin.editor.v2.common.TabCloseListener;
import com.jecelyin.editor.v2.dialog.SaveConfirmDialog;
import com.jecelyin.editor.v2.editor.Document;
import com.jecelyin.editor.v2.editor.EditorDelegate;
import com.jecelyin.editor.v2.editor.IEditorDelegate;
import com.jecelyin.editor.v2.editor.task.SaveAllTask;
import com.ogaclejapan.smarttablayout.SmartTabLayout;
import com.yqritc.recyclerviewflexibledivider.HorizontalDividerItemDecoration;

import java.io.File;
import java.util.ArrayList;


public class TabManager implements ViewPager.OnPageChangeListener {
    private IdeActivity mActivity;
    private TabAdapter mTabAdapter;
    private EditorFragmentPagerAdapter mPagerAdapter;

    @Nullable
    private RecyclerView mTabView;

    public TabManager(IdeActivity activity) {
        mActivity = activity;

        mTabAdapter = new TabAdapter();
        mTabAdapter.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                onTabMenuViewsClick(v);
            }
        });
        mTabView = mActivity.findViewById(R.id.tabRecyclerView);
        if (mTabView != null) {
            mTabView.setLayoutManager(new LinearLayoutManager(mActivity));
            mTabView.addItemDecoration(new HorizontalDividerItemDecoration.Builder(activity).build());
            mTabView.setAdapter(mTabAdapter);
        }

        initEditor();
        mActivity.mEditorPager.addOnPageChangeListener(this);
        SmartTabLayout tabLayout = mActivity.findViewById(R.id.tab_layout);
        tabLayout.setViewPager(mActivity.mEditorPager);
    }

    private void onTabMenuViewsClick(View v) {
        int i = v.getId();
        if (i == R.id.btn_close) {
            closeTab((int) v.getTag());

        } else {
            int position = (int) v.getTag();
            mActivity.closeMenu();
            setCurrentTab(position);

        }
    }

    private void initEditor() {
        mPagerAdapter = new EditorFragmentPagerAdapter(mActivity);
        mActivity.mEditorPager.setAdapter(mPagerAdapter);

        if (Preferences.getInstance(mActivity).isOpenLastFiles()) {
            ArrayList<RecentFileItem> recentFiles = SQLHelper.getInstance(mActivity).getRecentFiles(true);
            ArrayList<EditorPageDescriptor> descriptors = new ArrayList<>();
            File file;
            for (RecentFileItem item : recentFiles) {
                file = new File(item.path);
                if (!(file.isFile() && file.canRead() && file.canWrite())) {
                    SQLHelper.getInstance(mActivity).updateRecentFile(item.path, false);
                    continue;
                }
                descriptors.add(new EditorPageDescriptor(file, item.offset, item.encoding));
            }

            mPagerAdapter.addAll(descriptors);
            mActivity.mEditorPager.setOffscreenPageLimit(mPagerAdapter.getCount());
            updateTabList();

            int lastTab = Preferences.getInstance(mActivity).getLastTab();
            setCurrentTab(lastTab);

            if (descriptors.isEmpty()) {
                mActivity.createNewFile();
            }
        }

        mPagerAdapter.registerDataSetObserver(new DataSetObserver() {
            @Override
            public void onChanged() {
                updateTabList();
                updateToolbar();
                mActivity.mEditorPager.setOffscreenPageLimit(mPagerAdapter.getCount());
            }
        });

        // TODO: 25-Apr-18 show layout create new file
    }

    public boolean newTab(File file) {
        return newTab(file, 0, "UTF-8");
    }

    /**
     * @return true if new tab added success, otherwise return fable
     */
    public boolean newTab(File file, int offset, String encoding) {
        if (!file.exists() || !file.canRead() || !file.canWrite()) {
            return false;
        }

        int count = mPagerAdapter.getCount();
        for (int i = 0; i < count; i++) {
            EditorPageDescriptor descriptor = mPagerAdapter.getItem(i);
            if (descriptor.getPath() == null)
                continue;
            if (descriptor.getPath().equals(file.getPath())) {
                setCurrentTab(i);
                return false;
            }
        }
        mPagerAdapter.newEditor(file, offset, encoding);
        setCurrentTab(count);
        return true;
    }

    public int getTabCount() {
        if (mTabAdapter == null) {
            return 0;
        }
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
        mPagerAdapter.removeEditor(position, new TabCloseListener() {
            @Override
            public void onClose(String path, String encoding, int offset) {
                SQLHelper.getInstance(mActivity).updateRecentFile(path, false);
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
        mTabAdapter.setTabInfoList(mPagerAdapter.getTabInfoList());
        mTabAdapter.notifyDataSetChanged();
    }

    public void onDocumentChanged(Document mDocument) {
        updateTabList();
        updateToolbar();
    }

    private void updateToolbar() {
        EditorDelegate delegate = mPagerAdapter.getEditorDelegateAt(getCurrentTab());
        if (delegate == null) {
            mActivity.setTitle("");
        } else {
            mActivity.setTitle(delegate.getToolbarText());
        }
    }

    public boolean onDestroy() {
        if (mActivity.mEditorPager != null) {
            Preferences.getInstance(mActivity).setLastTab(getCurrentTab());
        }
        ArrayList<File> needSaveFiles = new ArrayList<>();
        ArrayList<IEditorDelegate> allEditor = mPagerAdapter.getAllEditor();
        for (IEditorDelegate editorDelegate : allEditor) {
            String path = editorDelegate.getPath();
            String encoding = editorDelegate.getEncoding();
            int offset = editorDelegate.getCursorOffset();
            if (editorDelegate.isChanged()) {
                needSaveFiles.add(editorDelegate.getDocument().getFile());
            }
            SQLHelper.getInstance(mActivity).updateRecentFile(path, encoding, offset);
        }

        if (needSaveFiles.isEmpty()) {
            return true;
        } else {

            StringBuilder fileName = new StringBuilder("(");
            for (int i = 0; i < needSaveFiles.size(); i++) {
                File needSaveFile = needSaveFiles.get(i);
                fileName.append(needSaveFile.getName());
                if (i != needSaveFiles.size() - 1) {
                    fileName.append(", ");
                }
            }
            fileName.append(")");

            SaveConfirmDialog saveConfirmDialog = new SaveConfirmDialog(mActivity, fileName.toString(),
                    new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            dialog.cancel();
                            switch (which) {
                                case DialogInterface.BUTTON_POSITIVE:
                                    SaveAllTask saveAllTask = new SaveAllTask(mActivity, new SaveListener() {
                                        @Override
                                        public void onSavedSuccess() {
                                            mActivity.finish();
                                        }

                                        @Override
                                        public void onSaveFailed(Exception e) {

                                        }
                                    });
                                    dialog.dismiss();
                                    saveAllTask.execute();
                                    break;
                                case DialogInterface.BUTTON_NEGATIVE:
                                    dialog.cancel();
                                    mActivity.finish();
                                    break;
                                case DialogInterface.BUTTON_NEUTRAL:
                                    dialog.cancel();
                                    break;
                            }

                        }
                    });
            saveConfirmDialog.show();
            return false;
        }
    }

    public EditorFragmentPagerAdapter getEditorPagerAdapter() {
        return mPagerAdapter;
    }

    /**
     * Get current editor edit file
     *
     * @return first is index, second is {@link EditorDelegate}, null if not found
     */
    @Nullable
    public Pair<Integer, IEditorDelegate> getEditorDelegate(File file) {
        ArrayList<IEditorDelegate> allEditor = mPagerAdapter.getAllEditor();
        for (int i = 0, allEditorSize = allEditor.size(); i < allEditorSize; i++) {
            IEditorDelegate delegate = allEditor.get(i);
            File editFile = delegate.getDocument().getFile();
            if (editFile.equals(file)) {
                return new Pair<>(i, delegate);
            }
        }

        //file editor with name only, if has more editor, return null
        Pair<Integer, IEditorDelegate> result = null;
        for (int i = 0, allEditorSize = allEditor.size(); i < allEditorSize; i++) {
            IEditorDelegate delegate = allEditor.get(i);
            File editFile = delegate.getDocument().getFile();
            if (editFile.getName().equals(file.getPath())) {
                if (result != null) {
                    return null;
                }
                result = new Pair<>(i, delegate);
            }
        }
        return result;
    }
}
