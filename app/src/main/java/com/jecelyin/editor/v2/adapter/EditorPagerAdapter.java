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

package com.jecelyin.editor.v2.adapter;

import android.content.Context;
import android.os.Parcel;
import android.os.Parcelable;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.afollestad.materialdialogs.DialogAction;
import com.afollestad.materialdialogs.MaterialDialog;
import com.duy.ccppcompiler.R;
import com.jecelyin.common.adapter.ViewPagerAdapter;
import com.jecelyin.editor.v2.common.Command;
import com.jecelyin.editor.v2.common.SaveListener;
import com.jecelyin.editor.v2.common.TabCloseListener;
import com.jecelyin.editor.v2.common.TabInfo;
import com.jecelyin.editor.v2.task.ClusterCommand;
import com.jecelyin.editor.v2.ui.activities.MainActivity;
import com.jecelyin.editor.v2.ui.dialog.SaveConfirmDialog;
import com.jecelyin.editor.v2.ui.editor.EditorDelegate;
import com.jecelyin.editor.v2.utils.ExtGrep;
import com.jecelyin.editor.v2.view.EditorView;

import java.io.File;
import java.util.ArrayList;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class EditorPagerAdapter extends ViewPagerAdapter {
    private static final String TAG = "EditorPagerAdapter";
    private final Context context;
    private ArrayList<EditorDelegate> editorDelegates = new ArrayList<>();
    private int currentPosition;

    public EditorPagerAdapter(Context context) {
        this.context = context;
    }

    @Override
    public View getView(int position, ViewGroup pager) {
        EditorDelegate delegate = getEditorDelegateAt(position);
        if (delegate != null && delegate.getEditorView() != null) {
            return delegate.getEditorView();
        }

        EditorView view = (EditorView) LayoutInflater.from(context).inflate(R.layout.editor, pager, false);
        setEditorView(position, view);
        return view;
    }

    @Override
    public int getCount() {
        return editorDelegates.size();
    }


    @Override
    public void setPrimaryItem(ViewGroup container, int position, Object object) {
        super.setPrimaryItem(container, position, object);
        currentPosition = position;
        setEditorView(position, (EditorView) object);
    }

    @Override
    public int getItemPosition(@NonNull Object object) {
        return ((EditorView) object).isRemoved() ? POSITION_NONE : POSITION_UNCHANGED;
    }

    @Override
    public Parcelable saveState() {
        SavedState savedState = new SavedState();
        savedState.states = new EditorDelegate.SavedState[editorDelegates.size()];
        for (int i = editorDelegates.size() - 1; i >= 0; i--) {
            savedState.states[i] = (EditorDelegate.SavedState) editorDelegates.get(i).onSaveInstanceState();
        }
        return savedState;
    }

    @Override
    public void restoreState(Parcelable state, ClassLoader loader) {
        if (!(state instanceof SavedState))
            return;
        EditorDelegate.SavedState[] savedStates = ((SavedState) state).states;
        editorDelegates.clear();
        for (EditorDelegate.SavedState savedState : savedStates) {
            editorDelegates.add(new EditorDelegate(savedState));
        }
        notifyDataSetChanged();
    }

    public void newEditor(@Nullable File file, int offset, String encoding) {
        newEditor(true, file, offset, encoding);
    }

    public void newEditor(boolean notify, @Nullable File file, int offset, String encoding) {
        editorDelegates.add(new EditorDelegate(editorDelegates.size(), file, offset, encoding));
        if (notify)
            notifyDataSetChanged();
    }

    public void newEditor(String title, @Nullable CharSequence content) {
        editorDelegates.add(new EditorDelegate(editorDelegates.size(), title, content));
        notifyDataSetChanged();
    }

    public void newEditor(ExtGrep grep) {
        editorDelegates.add(new EditorDelegate(editorDelegates.size(), context.getString(R.string.find_title, grep.getRegex()), grep));
        notifyDataSetChanged();
    }

    public void setEditorView(int index, EditorView editorView) {
        if (index >= getCount()) {
            return;
        }
        EditorDelegate delegate = editorDelegates.get(index);
        if (delegate != null)
            delegate.setEditorView(editorView);
    }

    public EditorDelegate getCurrentEditorDelegate() {
        if (editorDelegates == null || editorDelegates.isEmpty() || currentPosition >= editorDelegates.size())
            return null;
        return editorDelegates.get(currentPosition);
    }

    public int countNoFileEditor() {
        int count = 0;
        for (EditorDelegate f : editorDelegates) {
            if (f.getPath() == null) {
                count++;
            }
        }
        return count;
    }

    public TabInfo[] getTabInfoList() {
        int size = editorDelegates.size();
        TabInfo[] arr = new TabInfo[size];
        EditorDelegate delegate;
        for (int i = 0; i < size; i++) {
            delegate = editorDelegates.get(i);
            arr[i] = new TabInfo(delegate.getTitle(), delegate.getPath(), delegate.isChanged());
        }

        return arr;
    }

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
                                remove(position);
                                if (listener != null)
                                    listener.onClose(path, encoding, offset);
                            }
                        };
                        ((MainActivity) context).doCommand(command);
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
        EditorDelegate delegate = editorDelegates.remove(position);
        delegate.setRemoved();
        notifyDataSetChanged();
    }

    public ClusterCommand makeClusterCommand() {
        return new ClusterCommand(new ArrayList<>(editorDelegates));
    }

    public boolean removeAll(TabCloseListener tabCloseListener) {
        int position = editorDelegates.size() - 1;
        return position < 0 || removeEditor(position, tabCloseListener);
    }

    @Nullable
    public EditorDelegate getEditorDelegateAt(int index) {
        if (index >= editorDelegates.size())
            return null;
        return editorDelegates.get(index);
    }

    public static class SavedState implements Parcelable {
        public static final Creator<SavedState> CREATOR = new Creator<SavedState>() {
            @Override
            public SavedState createFromParcel(Parcel in) {
                return new SavedState(in);
            }

            @Override
            public SavedState[] newArray(int size) {
                return new SavedState[size];
            }
        };
        EditorDelegate.SavedState[] states;

        protected SavedState() {
        }

        protected SavedState(Parcel in) {
//            states = in.readParcelableArray();
            states = in.createTypedArray(EditorDelegate.SavedState.CREATOR);
        }

        @Override
        public int describeContents() {
            return 0;
        }

        @Override
        public void writeToParcel(Parcel dest, int flags) {
            dest.writeParcelableArray(states, flags);
        }
    }
}
