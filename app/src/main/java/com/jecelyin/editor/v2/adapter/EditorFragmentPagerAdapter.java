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
import com.jecelyin.editor.v2.common.Command;
import com.jecelyin.editor.v2.common.SaveListener;
import com.jecelyin.editor.v2.common.TabCloseListener;
import com.jecelyin.editor.v2.task.ClusterCommand;
import com.jecelyin.editor.v2.ui.activities.EditorActivity;
import com.jecelyin.editor.v2.ui.dialog.SaveConfirmDialog;
import com.jecelyin.editor.v2.ui.editor.EditorDelegate;
import com.jecelyin.editor.v2.ui.editor.EditorFragment;
import com.jecelyin.editor.v2.ui.editor.EditorPageDescriptor;

import java.io.File;
import java.util.ArrayList;

/**
 * Created by Duy on 25-Apr-18.
 */

public class EditorFragmentPagerAdapter extends EditorFragmentStatePagerAdapter<EditorFragment> implements IEditorPagerAdapter {
    private EditorActivity context;
    private ArrayList<EditorPageDescriptor> descriptors = new ArrayList<>();

    public EditorFragmentPagerAdapter(EditorActivity activity) {
        super(activity.getSupportFragmentManager());
        this.context = activity;
    }

    @Override
    public EditorFragment getItem(int position) {
        return EditorFragment.newInstance(descriptors.get(position));
    }

    @Override
    public int getCount() {
        return descriptors.size();
    }

    @Override
    public boolean removeAll(TabCloseListener tabCloseListener) {
        return false;
    }

    @Override
    public void newEditor(boolean notify, @NonNull File file, int offset, String encoding) {
        descriptors.add(new EditorPageDescriptor(file, offset, encoding));
        if (notify) {
            notifyDataSetChanged();
        }
    }

    @Nullable
    @Override
    public EditorDelegate getCurrentEditorDelegate() {
        EditorFragment fragment = getCurrentFragment();
        if (fragment != null) {
            return fragment.getEditorDelegate();
        }
        return null;
    }

    @Override
    public TabAdapter.TabInfo[] getTabInfoList() {
        int size = descriptors.size();
        TabAdapter.TabInfo[] arr = new TabAdapter.TabInfo[size];
        for (int i = 0; i < size; i++) {
            EditorPageDescriptor pageDescriptor = descriptors.get(i);
            EditorDelegate delegate = getEditorDelegateAt(i);
            boolean changed = delegate != null && delegate.isChanged();
            arr[i] = new TabAdapter.TabInfo(pageDescriptor.getTitle(), pageDescriptor.getPath(), changed);
        }

        return arr;
    }

    @Override
    public boolean removeEditor(final int position, final TabCloseListener listener) {
        EditorDelegate delegate = getEditorDelegateAt(position);
        if (delegate == null) {
            return false;
        }

        final String encoding = delegate.getEncoding();
        final int offset = delegate.getCursorOffset();
        final String path = delegate.getPath();

        if (delegate.isChanged()) {
            new SaveConfirmDialog(context, delegate.getTitle(), new MaterialDialog.SingleButtonCallback() {
                @Override
                public void onClick(MaterialDialog dialog, DialogAction which) {
                    if (which == DialogAction.POSITIVE) {
                        Command command = new Command(Command.CommandEnum.SAVE);
                        command.object = new SaveListener() {
                            @Override
                            public void onSaved() {
                                remove(position);
                                if (listener != null)
                                    listener.onClose(path, encoding, offset);
                            }
                        };
                        context.doCommand(command);
                    } else if (which == DialogAction.NEGATIVE) {
                        remove(position);
                        if (listener != null)
                            listener.onClose(path, encoding, offset);
                    } else {
                        dialog.dismiss();
                    }
                }
            }).show();
            return false;
        } else {
            remove(position);
            if (listener != null)
                listener.onClose(path, encoding, offset);
            return true;
        }
    }

    private void remove(int position) {
        descriptors.remove(position);
        notifyDataSetChanged();
    }

    public ArrayList<EditorDelegate> getAllEditor() {
        ArrayList<EditorDelegate> delegates = new ArrayList<>();
        for (int i = 0; i < getCount(); i++) {
            delegates.add(getEditorDelegateAt(i));
        }
        return delegates;
    }

    @Override
    public ClusterCommand makeClusterCommand() {
        return new ClusterCommand(getAllEditor());
    }

    @Nullable
    public EditorDelegate getEditorDelegateAt(int index) {
        EditorFragment fragment = super.getExistingFragment(index);
        if (fragment != null) {
            return fragment.getEditorDelegate();
        }
        return null;
    }
}
