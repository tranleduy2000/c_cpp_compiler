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

package com.jecelyin.editor.v2.adapter;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.afollestad.materialdialogs.DialogAction;
import com.afollestad.materialdialogs.MaterialDialog;
import com.duy.ide.editor.pager.ArrayPagerAdapter;
import com.duy.ide.editor.pager.PageDescriptor;
import com.jecelyin.editor.v2.common.Command;
import com.jecelyin.editor.v2.common.SaveListener;
import com.jecelyin.editor.v2.common.TabCloseListener;
import com.jecelyin.editor.v2.ui.activities.EditorActivity;
import com.jecelyin.editor.v2.ui.dialog.SaveConfirmDialog;
import com.jecelyin.editor.v2.ui.editor.EditorDelegate;
import com.jecelyin.editor.v2.ui.editor.EditorFragment;

import java.io.File;
import java.util.ArrayList;

/**
 * Created by Duy on 25-Apr-18.
 */

public class EditorFragmentPagerAdapter extends ArrayPagerAdapter<EditorFragment> implements IEditorPagerAdapter {
    private ArrayList<File> files = new ArrayList<>();
    private EditorActivity context;

    public EditorFragmentPagerAdapter(EditorActivity activity) {
        super(activity.getSupportFragmentManager(), null);
        this.context = activity;
    }

    @Override
    protected EditorFragment createFragment(PageDescriptor desc) {
        return null;
    }

    @Override
    public int getCount() {
        return files.size();
    }

    @Override
    public boolean removeAll(TabCloseListener tabCloseListener) {
        return false;
    }

    @Override
    public void newEditor(boolean notify, @NonNull File file, int offset, String encoding) {

    }

    @Override
    public EditorDelegate getCurrentEditorDelegate() {
        return null;
    }

    @Override
    public TabAdapter.TabInfo[] getTabInfoList() {
        return new TabAdapter.TabInfo[0];
    }

    @Override
    public boolean removeEditor(final int position, final TabCloseListener listener) {
        EditorDelegate f = editorDelegates.get(position);

        final String encoding = f.getEncoding();
        final int offset = f.getCursorOffset();
        final String path = f.getPath();

        if (f.isChanged()) {
            new SaveConfirmDialog(context, f.getTitle(), new MaterialDialog.SingleButtonCallback() {
                @Override
                public void onClick(MaterialDialog dialog, DialogAction which) {
                    if (which == DialogAction.POSITIVE) {
                        Command command = new Command(Command.CommandEnum.SAVE);
                        command.object = new SaveListener() {
                            @Override
                            public void onSaved() {
                                removePageAt(position);
                                if (listener != null)
                                    listener.onClose(path, encoding, offset);
                            }
                        };
                        context.doCommand(command);
                    } else if (which == DialogAction.NEGATIVE) {
                        removePageAt(position);
                        if (listener != null)
                            listener.onClose(path, encoding, offset);
                    } else {
                        dialog.dismiss();
                    }
                }
            }).show();
            return false;
        } else {
            removePageAt(position);
            if (listener != null)
                listener.onClose(path, encoding, offset);
            return true;
        }
    }

    private void removePageAt(int position) {
        EditorDelegate delegate = editorDelegates.remove(position);
        delegate.setRemoved();
        notifyDataSetChanged();
    }

    @Nullable
    public EditorDelegate getEditorDelegateAt(int index) {
        EditorFragment fragment = getExistingFragment(index);
        if (fragment != null) {
            return fragment.getEditorDelegate();
        }
        return null;
    }
}
