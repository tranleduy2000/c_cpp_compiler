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

package com.jecelyin.editor.v2.ui.editor;

import android.content.Context;
import android.core.text.SpannableStringBuilder;
import android.os.AsyncTask;
import android.text.Editable;
import android.text.Spannable;
import android.text.TextWatcher;
import android.text.style.ForegroundColorSpan;

import com.duy.ccppcompiler.R;
import com.duy.ide.filemanager.ReadFileListener;
import com.duy.ide.filemanager.SaveListener;
import com.jecelyin.common.utils.DLog;
import com.jecelyin.common.utils.StringUtils;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.highlight.Buffer;
import com.jecelyin.editor.v2.highlight.HighlightInfo;
import com.jecelyin.editor.v2.io.FileReader;
import com.jecelyin.editor.v2.task.SaveTask;

import org.gjt.sp.jedit.Catalog;
import org.gjt.sp.jedit.LineManager;
import org.gjt.sp.jedit.Mode;
import org.gjt.sp.jedit.StyleLoader;
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
    private final SaveTask mSaveTask;
    private final Preferences mPreferences;
    private final Buffer mBuffer;
    private final HashMap<Integer, ArrayList<ForegroundColorSpan>> mColorSpanMap;

    private int lineCount;
    private String encoding = "UTF-8";
    private byte[] srcMD5;
    private int srcLength;

    private String modeName;
    private File file;

    public Document(Context context, EditorDelegate editorDelegate) {
        mEditorDelegate = editorDelegate;
        mContext = context;
        mPreferences = Preferences.getInstance(context);

        mBuffer = new Buffer(context);
        mColorSpanMap = new HashMap<>();
        mSaveTask = new SaveTask(context, editorDelegate, this);
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

    public void onSaveInstanceState(EditorDelegate.SavedState ss) {
        ss.modeName = modeName;
        ss.lineNumber = lineCount;
        ss.textMd5 = srcMD5;
        ss.textLength = srcLength;
        ss.encoding = encoding;
        ss.file = file;
    }

    public void onRestoreInstanceState(EditorDelegate.SavedState ss) {
        if (ss.modeName != null) {
            setMode(ss.modeName);
        }
        if (ss.lineNumber > 0) {
            lineCount = ss.lineNumber;
        }
        srcMD5 = ss.textMd5;
        srcLength = ss.textLength;
        encoding = ss.encoding;
        file = ss.file;
    }

    public void loadFile(File file, String encodingName) {
        if (!file.isFile() || !file.exists()) {
            UIUtils.alert(mContext, mContext.getString(R.string.cannt_access_file, file.getPath()));
            return;
        }
        if (!file.canRead()) {
            UIUtils.alert(mContext, mContext.getString(R.string.cannt_read_file, file.getPath()));
            return;
        }
        this.file = file;
        FileReader reader = new FileReader(file, encodingName);
        new ReadFileTask(reader, this).execute();
    }

    @Override
    public void onStart() {
        mEditorDelegate.onLoadStart();
    }

    @Override
    public SpannableStringBuilder onAsyncReaded(FileReader fileReader, boolean ok) {
        Editable text = fileReader.getBuffer();
        Mode mode = ModeProvider.instance.getModeForFile(file == null ? null : file.getPath(), null, text.subSequence(0, Math.min(80, text.length())).toString());
        if (mode == null) {
            mode = ModeProvider.instance.getMode(Catalog.DEFAULT_MODE_NAME);
        }
        modeName = mode.getName();
        mBuffer.setMode(mode, mContext);

        lineCount = fileReader.getLineNumber();
        encoding = fileReader.getEncoding();

        srcMD5 = md5(text);
        srcLength = text.length();

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

        mEditorDelegate.mEditText.setLineNumber(lineCount);
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

        lineCount = mBuffer.getLineManager().getLineCount();

        if (!mPreferences.isHighlight() || editableText.length() > mPreferences.getHighlightSizeLimit())
            return;

        LineManager lineManager = mBuffer.getLineManager();
        int startLine = lineManager.getLineOfOffset(start);
        int endLine = lineManager.getLineOfOffset(start + count);
        int lineStartOffset = lineManager.getLineStartOffset(startLine);
        int lineEndOffset = lineManager.getLineEndOffset(endLine);

        boolean canHighlight = mBuffer.isCanHighlight();
        if (startLine == 0 && !canHighlight) {
            Mode mode = ModeProvider.instance.getModeForFile(file == null ? null : file.getPath(), null, s.subSequence(0, Math.min(80, s.length())).toString());
            if (mode != null)
                modeName = mode.getName();
            mBuffer.setMode(mode, mContext);
        }

        if (!canHighlight)
            return;

        ForegroundColorSpan[] spans = editableText.getSpans(lineStartOffset, lineEndOffset, ForegroundColorSpan.class);
        for (ForegroundColorSpan span : spans) {
            editableText.removeSpan(span);
        }

        highlight(editableText, startLine, endLine);
    }

    @Override
    public void afterTextChanged(Editable s) {

    }

    public void setMode(String name) {
        modeName = name;

        mBuffer.setMode(Catalog.getModeByName(name), mContext);
        mEditorDelegate.getEditableText().clearSpans();

        highlight(mEditorDelegate.getEditableText());
    }

    public String getModeName() {
        return modeName;
    }

    public Buffer getBuffer() {
        return mBuffer;
    }

    public File getFile() {
        return file;
    }

    public String getPath() {
        return file == null ? null : file.getPath();
    }

    public int getLineCount() {
        return lineCount;
    }

    public String getEncoding() {
        return encoding;
    }

    public void save() {
        save(false, null);
    }

    public void save(boolean isCluster, SaveListener listener) {
        if (DLog.DEBUG) {
            DLog.d(TAG, "save() called with: isCluster = [" + isCluster + "], listener = [" + listener + "]");
        }
        if (mSaveTask.isWriting()) {
            UIUtils.toast(mContext, R.string.writing);
            return;
        }
        if (isCluster && file == null) {
            listener.onSaved();
            UIUtils.toast(mContext, R.string.save_all_without_new_document_message);
            return;
        }
        mSaveTask.save(isCluster, listener);
    }

    public void saveAs() {
        mEditorDelegate.startSaveFileSelectorActivity();
    }

    void saveTo(File file, String encoding) {
        if (DLog.DEBUG)
            DLog.d(TAG, "saveTo() called with: file = [" + file + "], encoding = [" + encoding + "]");
        mSaveTask.saveTo(file, encoding);
    }

    public void onSaveSuccess(File file, String encoding) {
        this.file = file;
        this.encoding = encoding;
        srcMD5 = md5(mEditorDelegate.getText());
        srcLength = mEditorDelegate.getText().length();
        mEditorDelegate.noticeDocumentChanged();
    }

    public boolean isChanged() {
        if (srcMD5 == null) {
            return mEditorDelegate.getText().length() != 0;
        }
        if (srcLength != mEditorDelegate.getEditableText().length())
            return true;

        byte[] curMD5 = md5(mEditorDelegate.getEditableText());

        return !StringUtils.isEqual(srcMD5, curMD5);
    }

    private void highlight(Spannable spannableStringBuilder) {
        highlight(spannableStringBuilder, 0, lineCount - 1);
    }

    private void highlight(Spannable spannableStringBuilder, int startLine, int endLine) {
        if (!mBuffer.isCanHighlight())
            return;
        DefaultTokenHandler tokenHandler;
        if (styles == null) {
            styles = StyleLoader.loadStyles(mContext);
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

    private final static class ReadFileTask extends AsyncTask<File, Void, SpannableStringBuilder> {
        private final ReadFileListener listener;
        private final FileReader fileReader;

        public ReadFileTask(FileReader reader, ReadFileListener listener) {
            this.fileReader = reader;
            this.listener = listener;
        }

        @Override
        protected void onPreExecute() {
            listener.onStart();
        }

        @Override
        protected SpannableStringBuilder doInBackground(File... params) {
            if (!fileReader.read())
                return null;

            return listener.onAsyncReaded(fileReader, true);
        }

        @Override
        protected void onPostExecute(SpannableStringBuilder spannableStringBuilder) {
            listener.onDone(spannableStringBuilder, spannableStringBuilder != null);
        }
    }
}
