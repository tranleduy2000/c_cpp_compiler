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
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.util.Pair;
import android.support.v4.view.PagerAdapter;
import android.support.v4.view.ViewPager;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.duy.file.explorer.util.FileUtils;
import com.duy.ide.core.api.IdeActivity;
import com.duy.ide.database.RecentFileItem;
import com.duy.ide.database.SQLHelper;
import com.duy.ide.editor.editor.R;
import com.duy.ide.editor.pager.EditorFragmentPagerAdapter;
import com.duy.ide.editor.pager.EditorPageDescriptor;
import com.duy.ide.file.SaveListener;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.common.TabCloseListener;
import com.jecelyin.editor.v2.dialog.SaveConfirmDialog;
import com.duy.ide.editor.EditorDelegate;
import com.duy.ide.editor.IEditorDelegate;
import com.duy.ide.editor.task.SaveAllTask;
import com.ogaclejapan.smarttablayout.SmartTabLayout;

import java.io.File;
import java.util.ArrayList;


public class TabManager implements ViewPager.OnPageChangeListener, SmartTabLayout.TabProvider, ITabManager {
    @NonNull
    private IdeActivity mActivity;
    private EditorFragmentPagerAdapter mPagerAdapter;
    @Nullable
    private TextView mTxtDocumentInfo;
    @Nullable
    private SmartTabLayout mTabLayout;
    private ViewPager mViewPager;

    public TabManager(@NonNull IdeActivity activity, @NonNull ViewPager viewPager) {
        mActivity = activity;
        mViewPager = viewPager;
        mTxtDocumentInfo = mActivity.findViewById(R.id.txt_document_info);
        mTabLayout = mActivity.findViewById(R.id.tab_layout);
        if (mTabLayout != null) {
            mTabLayout.setCustomTabView(this);
        }
    }

    public void createEditor() {
        mPagerAdapter = new EditorFragmentPagerAdapter(mActivity);
        mViewPager.setAdapter(mPagerAdapter);

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
            mViewPager.setOffscreenPageLimit(mPagerAdapter.getCount());
            updateTabList();

            int lastTab = Preferences.getInstance(mActivity).getLastTab();
            setCurrentTab(lastTab);

//            if (descriptors.isEmpty()) {
//                mActivity.createNewFile();
//            }
        }

        mPagerAdapter.registerDataSetObserver(new DataSetObserver() {
            @Override
            public void onChanged() {
                updateTabList();
                updateToolbar();
                mViewPager.setOffscreenPageLimit(mPagerAdapter.getCount());
            }
        });
    }
    @Override
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
    @Override
    public int getTabCount() {
        return mPagerAdapter.getCount();
    }
    @Override
    public int getCurrentTab() {
        return mViewPager.getCurrentItem();
    }
    @Override
    public void setCurrentTab(int index) {
        int tabCount = mViewPager.getAdapter().getCount();
        index = Math.min(Math.max(0, index), tabCount);

        mViewPager.setCurrentItem(index);
        updateToolbar();
    }

    @Override
    public void closeAllTab() {
        while (getTabCount() > 0) {
            closeTab(0);
        }
    }

    @Override
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
        updateToolbar();
    }

    @Override
    public void onPageScrollStateChanged(int state) {

    }

    private void updateTabList() {
        ViewPager pager = mViewPager;

        //can not remove old tab listener
        pager.clearOnPageChangeListeners();

        //auto add page change listener
        if (mTabLayout != null) {
            mTabLayout.setViewPager(pager);
        }
        pager.addOnPageChangeListener(this);
    }

    public void onDocumentChanged() {
        updateTabList();
        updateToolbar();
    }

    private void updateToolbar() {
        if (mTxtDocumentInfo == null) {
            return;
        }
        EditorDelegate delegate = mPagerAdapter.getEditorDelegateAt(getCurrentTab());
        if (delegate == null) {
            mTxtDocumentInfo.setText("");
        } else {
            mTxtDocumentInfo.setText(delegate.getToolbarText());
        }
    }

    public boolean onDestroy() {
        if (mViewPager != null) {
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
            if (FileUtils.isSameFile(file, editFile)) {
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

    /**
     * Auto refresh all view when adapter change, so make position is final not cause any effect
     */
    @Override
    public View createTabView(ViewGroup container, final int position, PagerAdapter adapter) {
        if (adapter instanceof EditorFragmentPagerAdapter) {
            EditorFragmentPagerAdapter editorAdapter = (EditorFragmentPagerAdapter) adapter;
            LayoutInflater layoutInflater = LayoutInflater.from(mActivity);
            View view = layoutInflater.inflate(R.layout.list_item_tab, container, false);
            TextView txtName = view.findViewById(R.id.txt_name);
            final EditorPageDescriptor item = editorAdapter.getItem(position);
            final File file = new File(item.getPath());

            txtName.setText(file.getName());
            txtName.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    mViewPager.setCurrentItem(position);
                }
            });
            txtName.setOnLongClickListener(new View.OnLongClickListener() {
                @Override
                public boolean onLongClick(View v) {
                    Toast.makeText(mActivity,
                            "Path:" + file.getPath() + ", Encoding " + item.getEncoding(),
                            Toast.LENGTH_SHORT)
                            .show();
                    //handled, do not move to editor
                    return true;
                }
            });
            view.findViewById(R.id.btn_close).setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    closeTab(position);
                }
            });
            return view;
        }
        return null;
    }
}
