/*
 * Copyright (C) 2018 Tran Le Duy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package com.duy.ide.editor;

import android.annotation.SuppressLint;
import android.content.Context;
import android.os.AsyncTask;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.annotation.WorkerThread;
import android.text.Editable;
import android.text.SpannableStringBuilder;
import android.text.TextWatcher;
import android.text.style.CharacterStyle;
import android.text.style.ForegroundColorSpan;

import com.duy.ide.editor.editor.R;
import com.duy.ide.editor.task.SaveTask;
import com.duy.ide.editor.theme.model.EditorTheme;
import com.duy.ide.editor.view.IEditAreaView;
import com.duy.ide.file.ReadFileListener;
import com.duy.ide.file.SaveListener;
import com.jecelyin.common.utils.DLog;
import com.jecelyin.common.utils.StringUtils;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.highlight.Buffer;
import com.jecelyin.editor.v2.io.FileReader;
import com.jecelyin.editor.v2.io.LocalFileWriter;

import org.gjt.sp.jedit.Catalog;
import org.gjt.sp.jedit.LineManager;
import org.gjt.sp.jedit.Mode;
import org.gjt.sp.jedit.syntax.ModeProvider;

import java.io.File;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.HashMap;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class Document implements ReadFileListener, TextWatcher {
    private final EditorDelegate mEditorDelegate;
    private final Context mContext;
    private final Preferences mPreferences;
    private final Buffer mBuffer;
    @SuppressLint("UseSparseArrays")
    private final HashMap<Integer, ArrayList<? extends CharacterStyle>> mColorSpanMap = new HashMap<>();

    private int mLineCount;
    private String mEncoding = "UTF-8";
    @Nullable
    private byte[] mSourceMD5;
    private int mSourceLength;
    private String mModeName;
    @NonNull
    private File mFile;
    private Highlighter mHighlighter;


    Document(@NonNull Context context, @NonNull EditorDelegate editorDelegate, @NonNull File currentFile) {
        mEditorDelegate = editorDelegate;
        mContext = context;
        mFile = currentFile;
        mPreferences = Preferences.getInstance(context);
        mBuffer = new Buffer();
        mHighlighter = new Highlighter();
        editorDelegate.getEditText().addTextChangedListener(this);
    }

    /**
     * Returns the md5sum for given string. Or dummy byte array on error
     * Suppress NoSuchAlgorithmException because MD5 algorithm always present in JRE
     *
     * @param charSequence Given string
     * @return md5 sum of given string
     */
    private static byte[] md5(@NonNull CharSequence charSequence) {
        try {
            MessageDigest digest = MessageDigest.getInstance("MD5");
            byte[] ba = new byte[2];
            for (int i = 0, n = charSequence.length(); i < n; i++) {
                char cp = charSequence.charAt(i);
                ba[0] = (byte) (cp & 0xff);
                ba[1] = (byte) (cp >> 8 & 0xff);
                digest.update(ba);
            }
            return digest.digest();
        } catch (NoSuchAlgorithmException e) {
            DLog.e("Can't Calculate MD5 hash!", e);
            return charSequence.toString().getBytes();
        }
    }


    void onSaveInstanceState(@NonNull EditorDelegate.SavedState ss) {
        ss.modeName = mModeName;
        ss.lineNumber = mLineCount;
        ss.textMd5 = mSourceMD5;
        ss.textLength = mSourceLength;
        ss.encoding = mEncoding;
        ss.file = mFile;
    }

    void onRestoreInstanceState(@NonNull EditorDelegate.SavedState ss) {
        if (ss.modeName != null) {
            setMode(ss.modeName);
        }
        if (ss.lineNumber > 0) {
            mLineCount = ss.lineNumber;
        }
        mSourceMD5 = ss.textMd5;
        mSourceLength = ss.textLength;
        mEncoding = ss.encoding;
        mFile = ss.file;
    }

    void loadFile(File file, @Nullable String encodingName) {
        if (!file.isFile() || !file.exists()) {
            UIUtils.alert(mContext, mContext.getString(R.string.cannt_access_file, file.getPath()));
            return;
        }
        if (!file.canRead()) {
            UIUtils.alert(mContext, mContext.getString(R.string.cannt_read_file, file.getPath()));
            return;
        }
        mFile = file;
        FileReader reader = new FileReader(mFile, encodingName);
        new ReadFileTask(reader, this).execute();
    }

    @Override
    public void onStart() {
        mEditorDelegate.onLoadStart();
    }

    @Override
    public SpannableStringBuilder onAsyncReaded(FileReader fileReader, boolean ok) {
        CharSequence text = fileReader.getBuffer();
        String firstLine = text.subSequence(0, Math.min(80, text.length())).toString();

        ModeProvider modeProvider = ModeProvider.getInstance();
        Mode mode = modeProvider.getModeForFile(mFile.getPath(), mFile.getName(), firstLine);
        if (mode == null) {
            mode = modeProvider.getMode(Catalog.DEFAULT_MODE_NAME);
        }

        mModeName = mode.getName();
        mBuffer.setMode(mode, mContext);

        mLineCount = fileReader.getLineCount();
        mEncoding = fileReader.getEncoding();

        mSourceMD5 = md5(text);
        mSourceLength = text.length();

        return (SpannableStringBuilder) text;

    }

    @Override
    public void onDone(SpannableStringBuilder spannableStringBuilder, boolean ok) {
        if (mEditorDelegate == null || mEditorDelegate.getEditText() == null)
            return;
        if (!ok) {
            mEditorDelegate.onLoadFinish();
            UIUtils.alert(mContext, mContext.getString(R.string.read_file_exception));
            return;
        }

        mEditorDelegate.getEditText().setInitLineNumber(mLineCount);
        mEditorDelegate.getEditText().disableUndoRedoFilter();
        mEditorDelegate.getEditText().setText(spannableStringBuilder);
        mEditorDelegate.getEditText().enableUndoRedoFilter();
        mEditorDelegate.onLoadFinish();

    }

    @Override
    public void beforeTextChanged(CharSequence s, int start, int count, int after) {

    }

    @Override
    public void onTextChanged(CharSequence s, int start, int before, int count) {

        Editable editableText = mEditorDelegate.getEditableText();
        mBuffer.setEditable(editableText);

        if (before > 0) {
            mBuffer.remove(start, before);
        }
        if (count > 0) {
            mBuffer.insert(start, s.subSequence(start, start + count));
        }

        mLineCount = mBuffer.getLineManager().getLineCount();

        if (editableText.length() > mPreferences.getHighlightSizeLimit())
            return;

        LineManager lineManager = mBuffer.getLineManager();
        int startLine = lineManager.getLineOfOffset(start);
        int endLine = lineManager.getLineOfOffset(start + count);
        int lineStartOffset = lineManager.getLineStartOffset(startLine);
        int lineEndOffset = lineManager.getLineEndOffset(endLine);

        boolean canHighlight = mBuffer.isCanHighlight();
        if (startLine == 0 && !canHighlight) {
            ModeProvider modeProvider = ModeProvider.getInstance();
            String firstLine = s.subSequence(0, Math.min(80, s.length())).toString();
            Mode mode = modeProvider.getModeForFile(mFile.getPath(), mFile.getName(), firstLine);
            if (mode != null) {
                mModeName = mode.getName();
            }
            mBuffer.setMode(mode, mContext);
        }

        if (!canHighlight) {
            return;
        }
        {
            ForegroundColorSpan[] spans = editableText.getSpans(lineStartOffset, lineEndOffset, ForegroundColorSpan.class);
            for (ForegroundColorSpan span : spans) {
                editableText.removeSpan(span);
            }
        }
        EditorTheme editorTheme = mEditorDelegate.getEditText().getEditorTheme();
        mHighlighter.highlight(mBuffer, editorTheme, mColorSpanMap, editableText, startLine, endLine);
    }

    @Override
    public void afterTextChanged(Editable s) {

    }

    void setMode(String name) {
        mModeName = name;

        mBuffer.setMode(Catalog.getModeByName(name), mContext);
        mEditorDelegate.getEditableText().clearSpans();

        highlightSyntax(mEditorDelegate.getEditText());
    }

    public String getModeName() {
        return mModeName;
    }

    @NonNull
    public File getFile() {
        return mFile;
    }

    public String getPath() {
        return mFile.getPath();
    }

    public int getLineCount() {
        return mLineCount;
    }

    public String getEncoding() {
        return mEncoding;
    }

    @WorkerThread
    public void writeToFile(File file, String encoding) throws Exception {
        LocalFileWriter writer = new LocalFileWriter(file, encoding);
        writer.writeToFile(mEditorDelegate.getEditableText());

        onSaveSuccess(file, encoding);
    }

    /**
     * Write current content to new file and set new file to edit
     *
     * @param file - file to write
     */
    protected void saveInBackground(final File file, final String encoding, SaveListener listener) {
        SaveTask saveTask = new SaveTask(file, encoding, this, listener);
        saveTask.execute();
    }

    private void onSaveSuccess(File newFile, String encoding) {
        mFile = newFile;
        mEncoding = encoding;
        mSourceMD5 = md5(mEditorDelegate.getText());
        mSourceLength = mEditorDelegate.getText().length();
    }

    public boolean isChanged() {
        if (mSourceMD5 == null) {
            return mEditorDelegate.getText().length() != 0;
        }
        if (mSourceLength != mEditorDelegate.getEditableText().length()) {
            return true;
        }

        byte[] curMD5 = md5(mEditorDelegate.getEditableText());

        return !StringUtils.isEqual(mSourceMD5, curMD5);
    }

    private void highlightSyntax(IEditAreaView editAreaView) {
        EditorTheme editorTheme = mEditorDelegate.getEditText().getEditorTheme();
        Editable editableText = editAreaView.getEditableText();
        try {
            mHighlighter.highlight(mBuffer, editorTheme, mColorSpanMap, editableText, 0, mLineCount - 1);
        } catch (Exception e) {
            //should not happened
            e.printStackTrace();
        }
    }

    public void highlightWarn(int startLine, int endLine) {
        EditorTheme editorTheme = mEditorDelegate.getEditText().getEditorTheme();
        Editable editableText = mEditorDelegate.getEditableText();
        try {
            mHighlighter.highlightWarn(mBuffer, editorTheme, mColorSpanMap, editableText, startLine, endLine, false);
        } catch (Exception e) {
            //should not happened
            e.printStackTrace();
        }
    }

    public void highlightError(int startLine, int endLine) {
        EditorTheme editorTheme = mEditorDelegate.getEditText().getEditorTheme();
        Editable editableText = mEditorDelegate.getEditableText();
        try {
            mHighlighter.highlightError(
                    mBuffer,
                    editorTheme,
                    mColorSpanMap,
                    editableText,
                    startLine - 1 /*line start at 0*/,
                    endLine - 1 /*line start at 0*/,
                    false);
        } catch (Exception e) {
            //should not happened
            e.printStackTrace();
        }
    }

    public byte[] getMd5() {
        return mSourceMD5;
    }

    public Buffer getBuffer() {
        return mBuffer;
    }

    private final static class ReadFileTask extends AsyncTask<File, Void, SpannableStringBuilder> {
        private final ReadFileListener listener;
        private final FileReader fileReader;

        ReadFileTask(FileReader reader, ReadFileListener listener) {
            this.fileReader = reader;
            this.listener = listener;
        }

        @Override
        protected void onPreExecute() {
            listener.onStart();
        }

        @Override
        protected SpannableStringBuilder doInBackground(File... params) {
            if (!fileReader.read()) {
                return null;
            }

            return listener.onAsyncReaded(fileReader, true);
        }

        @Override
        protected void onPostExecute(SpannableStringBuilder spannableStringBuilder) {
            listener.onDone(spannableStringBuilder, spannableStringBuilder != null);
        }
    }
}
