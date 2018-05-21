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

package com.duy.ide.editor.pager;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;

import com.jecelyin.editor.v2.adapter.TabAdapter;
import com.jecelyin.editor.v2.common.TabCloseListener;
import com.duy.ide.editor.SimpleEditorActivity;
import com.jecelyin.editor.v2.editor.EditorDelegate;
import com.jecelyin.editor.v2.editor.EditorFragment;
import com.nakama.arraypageradapter.ArrayFragmentStatePagerAdapter;

import java.io.File;
import java.util.ArrayList;

/**
 * Created by Duy on 25-Apr-18.
 */

public class EditorFragmentPagerAdapter extends ArrayFragmentStatePagerAdapter<EditorPageDescriptor> implements IEditorPagerAdapter {
    private static final String TAG = "EditorFragmentPagerAdap";
    private SimpleEditorActivity context;

    public EditorFragmentPagerAdapter(SimpleEditorActivity activity) {
        super(activity.getSupportFragmentManager());
        this.context = activity;
    }

    @Override
    public Fragment getFragment(EditorPageDescriptor item, int position) {
        return EditorFragment.newInstance(item);
    }

    @Override
    public void add(EditorPageDescriptor item) {
        super.add(item);
    }

    @Override
    public void removeAll(TabCloseListener tabCloseListener) {
        while (getCount() > 0) {
            removeEditor(0, tabCloseListener);
        }
    }

    @Override
    public void newEditor(@NonNull File file, int offset, String encoding) {
        add(new EditorPageDescriptor(file, offset, encoding));
    }

    @Nullable
    @Override
    public EditorDelegate getCurrentEditorDelegate() {
        EditorFragment fragment = (EditorFragment) getCurrentFragment();
        if (fragment != null) {
            return fragment.getEditorDelegate();
        }
        return null;
    }

    @Override
    public TabAdapter.TabInfo[] getTabInfoList() {
        int size = getCount();
        TabAdapter.TabInfo[] arr = new TabAdapter.TabInfo[size];
        for (int i = 0; i < size; i++) {
            EditorDelegate editorDelegate = getEditorDelegateAt(i);
            if (editorDelegate != null) {
                boolean changed = editorDelegate.isChanged();
                arr[i] = new TabAdapter.TabInfo(editorDelegate.getTitle(), editorDelegate.getPath(), changed);
            } else {
                EditorPageDescriptor pageDescriptor = getItem(i);
                arr[i] = new TabAdapter.TabInfo(pageDescriptor.getTitle(), pageDescriptor.getPath(), false);
            }
        }

        return arr;
    }

    @Override
    public void removeEditor(final int position, final TabCloseListener listener) {
        EditorDelegate delegate = getEditorDelegateAt(position);
        if (delegate == null) {
            //not init
            return;
        }

        final String encoding = delegate.getEncoding();
        final int offset = delegate.getCursorOffset();
        final String path = delegate.getPath();
        //no need save file, all file will be auto save when EditorFragment destroy
        // if (delegate.isChanged()) {
        //     delegate.save(true);
        // }
        remove(position);
        if (listener != null) {
            listener.onClose(path, encoding, offset);
        }
    }

    public ArrayList<EditorDelegate> getAllEditor() {
        ArrayList<EditorDelegate> delegates = new ArrayList<>();
        for (int i = 0; i < getCount(); i++) {
            delegates.add(getEditorDelegateAt(i));
        }
        return delegates;
    }

    @Nullable
    @Override
    public CharSequence getPageTitle(int position) {
        return getItem(position).getTitle();
    }

    @Nullable
    public EditorDelegate getEditorDelegateAt(int index) {
        EditorFragment fragment = (EditorFragment) super.getExistingFragment(index);
        if (fragment != null) {
            return fragment.getEditorDelegate();
        }
        return null;
    }

    @Override
    public void updateDescriptor(String file, String encoding) {

    }
}
