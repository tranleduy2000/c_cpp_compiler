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

package com.jecelyin.editor.v2.ui.editor;

import android.content.Context;
import android.content.DialogInterface;
import android.content.res.TypedArray;
import android.core.widget.BaseEditorView;
import android.core.widget.EditAreaView;
import android.graphics.Color;
import android.os.Parcel;
import android.os.Parcelable;
import android.support.annotation.Nullable;
import android.support.v7.app.AlertDialog;
import android.text.Editable;
import android.text.Spanned;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.text.style.BackgroundColorSpan;
import android.view.ActionMode;
import android.view.Menu;
import android.view.MenuItem;

import com.duy.ccppcompiler.R;
import com.jecelyin.common.utils.DLog;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.common.Command;
import com.jecelyin.editor.v2.common.SaveListener;
import com.jecelyin.editor.v2.ui.activities.EditorActivity;
import com.jecelyin.editor.v2.ui.dialog.DocumentInfoDialog;
import com.jecelyin.editor.v2.ui.dialog.FinderDialog;
import com.jecelyin.editor.v2.ui.widget.menu.MenuDef;
import com.jecelyin.editor.v2.view.EditorView;

import org.gjt.sp.jedit.Catalog;
import org.gjt.sp.jedit.Mode;
import org.gjt.sp.jedit.syntax.ModeProvider;

import java.io.File;
import java.util.Locale;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class EditorDelegate implements TextWatcher {
    public final static String KEY_CLUSTER = "is_cluster";
    private static boolean disableAutoSave = false;
    public EditAreaView mEditText;
    private Context context;
    private EditorView mEditorView;
    private Document mDocument;
    private SavedState savedState;
    private int orientation;
    private boolean loaded = true;
    private int findResultsKeywordColor;

    public EditorDelegate(SavedState ss) {
        savedState = ss;
    }

    public EditorDelegate(@Nullable File file, int offset, String encoding) {
        savedState = new SavedState();
        savedState.file = file;
        savedState.offset = offset;
        savedState.encoding = encoding;
        if (savedState.file != null) {
            savedState.title = savedState.file.getName();
        }
    }

    public static void setDisableAutoSave() {
        disableAutoSave = true;
    }

    private void init() {
        if (mDocument != null)
            return;

        TypedArray a = context.obtainStyledAttributes(new int[]{
                R.attr.findResultsKeyword,
        });
        findResultsKeywordColor = a.getColor(0, Color.BLACK);
        a.recycle();

        mDocument = new Document(context, this);
        mEditText.setReadOnly(Preferences.getInstance(context).isReadOnly());
        mEditText.setCustomSelectionActionModeCallback(new EditorSelectionActionModeCallback());

        if (savedState.editorState != null) {
            mDocument.onRestoreInstanceState(savedState);
            mEditText.onRestoreInstanceState(savedState.editorState);
        } else if (savedState.file != null) {
            mDocument.loadFile(savedState.file, savedState.encoding);
        } else if (!TextUtils.isEmpty(savedState.content)) {
            mEditText.setText(savedState.content);
        }

        mEditText.addTextChangedListener(this);

        noticeDocumentChanged();

        if (savedState.object != null) {
            EditorObjectProcessor.process(savedState.object, this);
        }
    }

    public void onLoadStart() {
        loaded = false;
        mEditText.setEnabled(false);
        mEditorView.setLoading(true);
    }

    public void onLoadFinish() {
        mEditorView.setLoading(false);
        mEditText.setEnabled(true);
        mEditText.post(new Runnable() {
            @Override
            public void run() {
                if (savedState.offset < mEditText.getText().length())
                    mEditText.setSelection(savedState.offset);
            }
        });

        noticeDocumentChanged();
        loaded = true;
    }

    public Context getContext() {
        return context;
    }

    public EditorActivity getMainActivity() {
        return (EditorActivity) context;
    }

    public String getTitle() {
        return savedState.title;
    }

    public String getPath() {
        return mDocument == null ? (savedState.file == null ? null : savedState.file.getPath()) : mDocument.getPath();
    }

    public String getEncoding() {
        return mDocument == null ? null : mDocument.getEncoding();
    }

    public String getText() {
        return mEditText.getText().toString();
    }

    public Editable getEditableText() {
        return mEditText.getText();
    }

    public EditorView getEditorView() {
        return mEditorView;
    }

    public void setEditorView(EditorView editorView) {
        context = editorView.getContext();
        this.mEditorView = editorView;
        this.mEditText = editorView.getEditText();
        this.orientation = context.getResources().getConfiguration().orientation;
        init();
    }

    public CharSequence getSelectedText() {
        return mEditText.hasSelection() ? mEditText.getEditableText().subSequence(mEditText.getSelectionStart(), mEditText.getSelectionEnd()) : "";
    }

    public boolean isChanged() {
        return mDocument != null && mDocument.isChanged();
    }

    public CharSequence getToolbarText() {
        String encode = mDocument == null ? "UTF-8" : mDocument.getEncoding();
        String fileMode = mDocument == null || mDocument.getModeName() == null ? "" : mDocument.getModeName();
        String title = getTitle();
        String changed = isChanged() ? "*" : "";
        String cursor = "";
        if (mEditText != null && mEditText.getLayout() != null && getCursorOffset() >= 0) {
            int cursorOffset = getCursorOffset();
            int line = mEditText.getLayout().getLineForOffset(cursorOffset);
            cursor += line + ":" + cursorOffset;
        }
        return String.format(Locale.US, "%s%s  \t|\t  %s \t %s \t %s", changed, title, encode, fileMode, cursor);
    }

    public void startSaveFileSelectorActivity() {
        getMainActivity().startPickPathActivity(mDocument.getPath(), mDocument.getEncoding());
    }

    public void saveTo(File file, String encoding) {
        mDocument.saveTo(file, encoding == null ? mDocument.getEncoding() : encoding);
    }

    public void addHightlight(int start, int end) {
        mEditText.getText().setSpan(new BackgroundColorSpan(findResultsKeywordColor), start, end, Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
        mEditText.setSelection(end, end);
    }

    public int getCursorOffset() {
        if (mEditText == null)
            return -1;
        return mEditText.getSelectionEnd();
    }

    public boolean doCommand(Command command) {
        if (mEditText == null)
            return false;
        boolean readonly = Preferences.getInstance(context).isReadOnly();
        switch (command.what) {
            case HIDE_SOFT_INPUT:
                mEditText.hideSoftInput();
                break;
            case SHOW_SOFT_INPUT:
                mEditText.showSoftInput();
                break;
            case UNDO:
                if (!readonly)
                    mEditText.undo();
                break;
            case REDO:
                if (!readonly)
                    mEditText.redo();
                break;
            case CUT:
                if (!readonly)
                    return mEditText.cut();
            case COPY:
                return mEditText.copy();
            case PASTE:
                if (!readonly)
                    return mEditText.paste();
            case SELECT_ALL:
                return mEditText.selectAll();
            case DUPLICATION:
                if (!readonly)
                    mEditText.duplication();
                break;
            case CONVERT_WRAP_CHAR:
                if (!readonly)
                    mEditText.convertWrapCharTo((String) command.object);
                break;
            case GOTO_LINE:
                mEditText.gotoLine(command.args.getInt("line"));
                break;
            case GOTO_TOP:
                mEditText.gotoTop();
                break;
            case GOTO_END:
                mEditText.gotoEnd();
                break;
            case DOC_INFO:
                DocumentInfoDialog documentInfoDialog = new DocumentInfoDialog(context);
                documentInfoDialog.setDocument(mDocument);
                documentInfoDialog.setEditAreaView(mEditText);
                documentInfoDialog.setPath(mDocument.getPath());
                documentInfoDialog.show();
                break;
            case READONLY_MODE:
                Preferences preferences = Preferences.getInstance(context);
                boolean readOnly = preferences.isReadOnly();
                mEditText.setReadOnly(readOnly);
                ((EditorActivity) context).doNextCommand();
                break;
            case SAVE:
                if (!readonly)
                    mDocument.save(command.args.getBoolean(KEY_CLUSTER, false), (SaveListener) command.object);
                break;
            case SAVE_AS:
                mDocument.saveAs();
                break;
            case FIND:
                FinderDialog.showFindDialog(this);
                break;
            case HIGHLIGHT:
                String scope = (String) command.object;
                if (scope == null) {
                    Mode mode;
                    String firstLine = getEditableText().subSequence(0, Math.min(80, getEditableText().length())).toString();
                    if (TextUtils.isEmpty(mDocument.getPath()) || TextUtils.isEmpty(firstLine)) {
                        mode = ModeProvider.instance.getMode(Catalog.DEFAULT_MODE_NAME);
                    } else {
                        mode = ModeProvider.instance.getModeForFile(mDocument.getPath(), null, firstLine);
                    }

                    if (mode == null) {
                        mode = ModeProvider.instance.getMode(Catalog.DEFAULT_MODE_NAME);
                    }

                    scope = mode.getName();
                }
                mDocument.setMode(scope);
                ((EditorActivity) context).doNextCommand();
                break;
            case INSERT_TEXT:
                if (!readonly) {
                    int selStart = mEditText.getSelectionStart();
                    int selEnd = mEditText.getSelectionEnd();
                    if (selStart == -1 || selEnd == -1) {
                        mEditText.getText().insert(0, (CharSequence) command.object);
                    } else {
                        mEditText.getText().replace(selStart, selEnd, (CharSequence) command.object);
                    }
                }
                break;
            case RELOAD_WITH_ENCODING:
                reOpenWithEncoding((String) command.object);
                break;
            case FORWARD:
                mEditText.forwardLocation();
                break;
            case BACK:
                mEditText.backLocation();
                break;
        }
        return true;
    }

    private void reOpenWithEncoding(final String encoding) {
        final File file = mDocument.getFile();
        if (file == null) {
            UIUtils.toast(context, R.string.please_save_as_file_first);
            return;
        }
        if (mDocument.isChanged()) {
            new AlertDialog.Builder(context)
                    .setTitle(R.string.document_changed)
                    .setMessage(R.string.give_up_document_changed_message)
                    .setPositiveButton(R.string.cancel, new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            dialog.cancel();
                        }
                    })
                    .setNegativeButton(R.string.ok, new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {
                            dialog.cancel();
                            mDocument.loadFile(file, encoding);
                        }
                    })
                    .create()
                    .show();
            return;
        }
        mDocument.loadFile(file, encoding);
    }

    void noticeDocumentChanged() {
        File file = mDocument.getFile();
        if (file != null) {
            savedState.title = file.getName();
        }

        noticeMenuChanged();
    }

    public void setRemoved() {
        if (mEditorView == null) {
            return;
        }
        mEditorView.setRemoved();
        mDocument = null;
    }

    private void noticeMenuChanged() {
        EditorActivity editorActivity = (EditorActivity) this.context;
        editorActivity.setMenuStatus(R.id.m_save, isChanged() ? MenuDef.STATUS_NORMAL : MenuDef.STATUS_DISABLED);
        editorActivity.setMenuStatus(R.id.m_undo, mEditText != null && mEditText.canUndo() ? MenuDef.STATUS_NORMAL : MenuDef.STATUS_DISABLED);
        editorActivity.setMenuStatus(R.id.m_redo, mEditText != null && mEditText.canRedo() ? MenuDef.STATUS_NORMAL : MenuDef.STATUS_DISABLED);
        ((EditorActivity) context).getTabManager().onDocumentChanged();
    }

    @Override
    public void beforeTextChanged(CharSequence s, int start, int count, int after) {

    }

    @Override
    public void onTextChanged(CharSequence s, int start, int before, int count) {

    }

    @Override
    public void afterTextChanged(Editable s) {
        if (loaded)
            noticeMenuChanged();
    }

    public String getLang() {
        if (mDocument == null)
            return null;
        return mDocument.getModeName();
    }

    private void convertSelectedText(int id) {
        if (mEditText == null || !mEditText.hasSelection())
            return;

        int start = mEditText.getSelectionStart();
        int end = mEditText.getSelectionEnd();

        String selectedText = getEditableText().subSequence(start, end).toString();

        switch (id) {
            case R.id.m_convert_to_uppercase:
                selectedText = selectedText.toUpperCase();
                break;
            case R.id.m_convert_to_lowercase:
                selectedText = selectedText.toLowerCase();
                break;
        }
        getEditableText().replace(start, end, selectedText);
    }

    public Parcelable onSaveInstanceState() {
        SavedState ss = savedState;
        if (mDocument != null) {
            mDocument.onSaveInstanceState(ss);
        }
        if (mEditText != null) {
            mEditText.setFreezesText(true);
            ss.editorState = (BaseEditorView.SavedState) mEditText.onSaveInstanceState();
        }

        if (loaded && !disableAutoSave && mDocument != null && mDocument.getFile() != null && Preferences.getInstance(context).isAutoSave()) {
            int newOrientation = context.getResources().getConfiguration().orientation;
            if (orientation != newOrientation) {
                DLog.d("current is screen orientation, discard auto save!");
                orientation = newOrientation;
            } else {
                mDocument.save();
            }
        }

        return ss;
    }

    private static class Arguments {
        CharSequence content;
        Parcelable object;
    }

    public static class SavedState extends Arguments implements Parcelable {
        public static final Creator<SavedState> CREATOR = new Creator<SavedState>() {
            @Override
            public SavedState createFromParcel(Parcel source) {
                return new SavedState(source);
            }

            @Override
            public SavedState[] newArray(int size) {
                return new SavedState[size];
            }
        };
        int offset;
        int lineNumber;
        @Nullable
        File file;
        String title;
        String encoding;
        String modeName;
        BaseEditorView.SavedState editorState;
        byte[] textMd5;
        boolean root;
        File rootFile;
        int textLength;

        SavedState() {
        }

        SavedState(Parcel in) {
            this.offset = in.readInt();
            this.lineNumber = in.readInt();
            String file, rootFile;
            file = in.readString();
            rootFile = in.readString();
            this.file = TextUtils.isEmpty(file) ? null : new File(file);
            this.rootFile = TextUtils.isEmpty(rootFile) ? null : new File(rootFile);
            this.root = in.readInt() == 1;
            this.title = in.readString();
            this.encoding = in.readString();
            this.modeName = in.readString();
            int hasState = in.readInt();
            if (hasState == 1)
                this.editorState = in.readParcelable(BaseEditorView.SavedState.class.getClassLoader());
            this.textMd5 = in.createByteArray();
            this.textLength = in.readInt();
        }

        @Override
        public int describeContents() {
            return 0;
        }

        @Override
        public void writeToParcel(Parcel dest, int flags) {
            dest.writeInt(this.offset);
            dest.writeInt(this.lineNumber);
            dest.writeString(this.file == null ? null : this.file.getPath());
            dest.writeString(this.rootFile == null ? null : this.rootFile.getPath());
            dest.writeInt(root ? 1 : 0);
            dest.writeString(this.title);
            dest.writeString(this.encoding);
            dest.writeString(this.modeName);
            dest.writeInt(this.editorState == null ? 0 : 1);
            if (this.editorState != null)
                dest.writeParcelable(this.editorState, flags);
            dest.writeByteArray(this.textMd5);
            dest.writeInt(textLength);
        }
    }

    private class EditorSelectionActionModeCallback implements ActionMode.Callback {

        @Override
        public boolean onCreateActionMode(ActionMode mode, Menu menu) {
            final TypedArray styledAttributes = context.obtainStyledAttributes(
                    R.styleable.SelectionModeDrawables);

            boolean readOnly = Preferences.getInstance(context).isReadOnly();
            boolean selected = mEditText.hasSelection();
            if (selected) {
                menu.add(0, R.id.m_find_replace, 0, R.string.find).
                        setIcon(R.drawable.ic_find_replace_white_24dp).
                        setAlphabeticShortcut('f').
                        setShowAsAction(MenuItem.SHOW_AS_ACTION_ALWAYS | MenuItem.SHOW_AS_ACTION_WITH_TEXT);

                if (!readOnly) {
                    menu.add(0, R.id.m_convert_to_uppercase, 0, R.string.convert_to_uppercase)
                            .setIcon(R.drawable.m_uppercase)
                            .setAlphabeticShortcut('U')
                            .setShowAsAction(MenuItem.SHOW_AS_ACTION_IF_ROOM | MenuItem.SHOW_AS_ACTION_WITH_TEXT);

                    menu.add(0, R.id.m_convert_to_lowercase, 0, R.string.convert_to_lowercase)
                            .setIcon(R.drawable.m_lowercase)
                            .setAlphabeticShortcut('L')
                            .setShowAsAction(MenuItem.SHOW_AS_ACTION_IF_ROOM | MenuItem.SHOW_AS_ACTION_WITH_TEXT);
                }
            }

            if (!readOnly) {
                menu.add(0, R.id.m_duplication, 0, selected ? R.string.duplication_text : R.string.duplication_line)
                        .setIcon(R.drawable.ic_control_point_duplicate_white_24dp)
                        .setAlphabeticShortcut('L')
                        .setShowAsAction(MenuItem.SHOW_AS_ACTION_IF_ROOM | MenuItem.SHOW_AS_ACTION_WITH_TEXT);
            }

            styledAttributes.recycle();
            return true;
        }

        @Override
        public boolean onPrepareActionMode(ActionMode mode, Menu menu) {
            return false;
        }

        @Override
        public boolean onActionItemClicked(ActionMode mode, MenuItem item) {
            switch (item.getItemId()) {
                case R.id.m_find_replace:
                    doCommand(new Command(Command.CommandEnum.FIND));
                    return true;
                case R.id.m_convert_to_uppercase:
                case R.id.m_convert_to_lowercase:
                    convertSelectedText(item.getItemId());
                    return true;
                case R.id.m_duplication:
                    mEditText.duplication();
                    return true;
            }
            return false;
        }

        @Override
        public void onDestroyActionMode(ActionMode mode) {

        }
    }

}
