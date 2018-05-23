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

package com.jecelyin.editor.v2.editor;

import android.annotation.SuppressLint;
import android.content.Context;
import android.core.text.SpannableStringBuilder;
import android.os.AsyncTask;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.annotation.WorkerThread;
import android.text.Editable;
import android.text.Spannable;
import android.text.TextWatcher;
import android.text.style.ForegroundColorSpan;

import com.duy.ide.editor.editor.R;
import com.duy.ide.editor.span.ErrorSpan;
import com.duy.ide.filemanager.ReadFileListener;
import com.duy.ide.filemanager.SaveListener;
import com.jecelyin.common.utils.DLog;
import com.jecelyin.common.utils.StringUtils;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.editor.task.SaveTask;
import com.jecelyin.editor.v2.highlight.Buffer;
import com.jecelyin.editor.v2.highlight.HighlightInfo;
import com.jecelyin.editor.v2.io.FileReader;
import com.jecelyin.editor.v2.io.LocalFileWriter;

import org.gjt.sp.jedit.Catalog;
import org.gjt.sp.jedit.ColorSchemeLoader;
import org.gjt.sp.jedit.LineManager;
import org.gjt.sp.jedit.Mode;
import org.gjt.sp.jedit.syntax.DefaultTokenHandler;
import org.gjt.sp.jedit.syntax.ModeProvider;
import org.gjt.sp.jedit.syntax.SyntaxStyle;
import org.gjt.sp.jedit.syntax.Token;

import java.io.File;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.HashMap;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class Document implements ReadFileListener, TextWatcher {
    private static final String TAG = "Document";
    public static SyntaxStyle[] styles;
    private final EditorDelegate mEditorDelegate;
    private final Context mContext;
    private final Preferences mPreferences;
    private final Buffer mBuffer;
    @SuppressLint("UseSparseArrays")
    private final HashMap<Integer, ArrayList<ForegroundColorSpan>> mColorSpanMap = new HashMap<>();
    private int mLineCount;
    private String mEncoding = "UTF-8";
    private byte[] mSourceMD5;
    private int mSourceLength;
    private String mModeName;
    @NonNull
    private File mFile;

    Document(Context context, EditorDelegate editorDelegate, @NonNull File currentFile) {
        mEditorDelegate = editorDelegate;
        mContext = context;
        mFile = currentFile;
        mPreferences = Preferences.getInstance(context);
        mBuffer = new Buffer(context);
        editorDelegate.mEditText.addTextChangedListener(this);
    }

    /**
     * Returns the md5sum for given string. Or dummy byte array on error
     * Suppress NoSuchAlgorithmException because MD5 algorithm always present in JRE
     *
     * @param charSequence Given string
     * @return md5 sum of given string
     */
    private static byte[] md5(CharSequence charSequence) {
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

    void onSaveInstanceState(EditorDelegate.SavedState ss) {
        ss.modeName = mModeName;
        ss.lineNumber = mLineCount;
        ss.textMd5 = mSourceMD5;
        ss.textLength = mSourceLength;
        ss.encoding = mEncoding;
        ss.file = mFile;
    }

    void onRestoreInstanceState(EditorDelegate.SavedState ss) {
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
        Editable text = fileReader.getBuffer();
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
        if (mEditorDelegate == null || mEditorDelegate.mEditText == null)
            return;
        if (!ok) {
            mEditorDelegate.onLoadFinish();
            UIUtils.alert(mContext, mContext.getString(R.string.read_file_exception));
            return;
        }

        mEditorDelegate.mEditText.setLineNumber(mLineCount);
        mEditorDelegate.mEditText.setText(spannableStringBuilder);
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
        {
            ErrorSpan[] spans = editableText.getSpans(lineStartOffset, lineEndOffset, ErrorSpan.class);
            for (ErrorSpan span : spans) {
                editableText.removeSpan(span);
            }
        }

        highlight(editableText, startLine, endLine);
    }

    @Override
    public void afterTextChanged(Editable s) {

    }

    void setMode(String name) {
        mModeName = name;

        mBuffer.setMode(Catalog.getModeByName(name), mContext);
        mEditorDelegate.getEditableText().clearSpans();

        highlight(mEditorDelegate.getEditableText());
    }

    String getModeName() {
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
        if (false) {
            mEditorDelegate.onDocumentChanged();
        }
    }

    public boolean isChanged() {
        if (mSourceMD5 == null) {
            return mEditorDelegate.getText().length() != 0;
        }
        if (mSourceLength != mEditorDelegate.getEditableText().length())
            return true;

        byte[] curMD5 = md5(mEditorDelegate.getEditableText());

        return !StringUtils.isEqual(mSourceMD5, curMD5);
    }

    private void highlight(Spannable spannableStringBuilder) {
        highlight(spannableStringBuilder, 0, mLineCount - 1);
    }

    private void highlight(Spannable spannableStringBuilder, int startLine, int endLine) {
        if (!mBuffer.isCanHighlight())
            return;

        DefaultTokenHandler tokenHandler;
        if (styles == null) {
            styles = ColorSchemeLoader.loadStyles(mContext);
        }
        ArrayList<HighlightInfo> mergerArray;

        for (int i = startLine; i <= endLine; i++) {
            tokenHandler = new DefaultTokenHandler();
            mBuffer.markTokens(i, tokenHandler);
            Token token = tokenHandler.getTokens();

            mergerArray = new ArrayList<>();
            collectToken(mBuffer, i, token, mergerArray);
            addTokenSpans(spannableStringBuilder, i, mergerArray);
        }
    }

    private void addTokenSpans(Spannable spannableStringBuilder, int line, ArrayList<HighlightInfo> mergerArray) {
        ForegroundColorSpan fcs;

        ArrayList<ForegroundColorSpan> oldSpans = mColorSpanMap.remove(line);
        if (oldSpans != null) {
            for (ForegroundColorSpan span : oldSpans) {
                spannableStringBuilder.removeSpan(span);
            }
        }

        int length = spannableStringBuilder.length();

        ArrayList<ForegroundColorSpan> spans = new ArrayList<>(mergerArray.size());
        for (HighlightInfo hi : mergerArray) {
            if (hi.endOffset > length) {
                DLog.e("assert hi.endOffset %d > maxLength %d", hi.endOffset, length);
                hi.endOffset = length;
            }
            if (hi.startOffset >= hi.endOffset) {
                DLog.e("hi.startOffset %d >= hi.endOffset %d", hi.startOffset, hi.endOffset);
                continue;
            }
            fcs = new ForegroundColorSpan(hi.color);
            spannableStringBuilder.setSpan(fcs, hi.startOffset, hi.endOffset, SpannableStringBuilder.SPAN_EXCLUSIVE_EXCLUSIVE);
            spans.add(fcs);
        }
        mColorSpanMap.put(line, spans);
    }

    private void collectToken(Buffer buffer, int lineNumber, Token token,
                              ArrayList<HighlightInfo> mergerArray) {

        int lineStartOffset = buffer.getLineManager().getLineStartOffset(lineNumber);

        HighlightInfo hi;
        while (token.id != Token.END) {
            int startIndex = lineStartOffset + token.offset;
            int endIndex = lineStartOffset + token.offset + token.length;
            SyntaxStyle style = styles[token.id];
            token = token.next;

            if (style == null)
                continue;

            int color = style.getForegroundColor();

            if (mergerArray.isEmpty()) {
                mergerArray.add(new HighlightInfo(startIndex, endIndex, color));
            } else {
                hi = mergerArray.get(mergerArray.size() - 1);
                if (hi.color == color && hi.endOffset == startIndex) {
                    hi.endOffset = endIndex;
                } else {
                    mergerArray.add(new HighlightInfo(startIndex, endIndex, color));
                }
            }
        }


    }

    public byte[] getMd5() {
        return mSourceMD5;
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
