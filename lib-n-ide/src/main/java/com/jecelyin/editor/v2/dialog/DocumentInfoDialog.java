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

package com.jecelyin.editor.v2.dialog;

import android.content.Context;
import android.support.v7.app.AlertDialog;
import android.view.LayoutInflater;
import android.view.View;
import android.widget.TextView;

import com.duy.ide.editor.editor.R;
import com.duy.ide.editor.view.IEditAreaView;
import com.duy.ide.editor.Document;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class DocumentInfoDialog extends AbstractDialog {
    private CharSequence path;
    private IEditAreaView editAreaView;
    private Document document;

    public DocumentInfoDialog(Context context) {
        super(context);
    }

    public void setPath(CharSequence path) {
        this.path = path;
    }

    public void setEditAreaView(IEditAreaView editAreaView) {
        this.editAreaView = editAreaView;
    }

    public void setDocument(Document document) {
        this.document = document;
    }

    @Override
    public void show() {
        Matcher matcher = Pattern.compile("[a-zA-Z]+").matcher(editAreaView.getText());
        int wordCount = 0;
        while (matcher.find()) wordCount++;

        View view = LayoutInflater.from(context).inflate(R.layout.dialog_document_info, null);
        ViewHolder viewHolder = new ViewHolder(view);
        viewHolder.mPathTextView.setText(context.getString(R.string.path_x, path == null ? "" : path));
        viewHolder.mCharCountTextView.setText(context.getString(R.string.char_x, editAreaView.getText().length()));
        viewHolder.mWordCountTextView.setText(context.getString(R.string.word_x, wordCount));
        viewHolder.mEncodingTextView.setText(context.getString(R.string.encoding_x, document.getEncoding()));
        viewHolder.mLineCountTextView.setText(context.getString(R.string.line_number_x, document.getLineCount()));

        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle(R.string.document_info)
                .setView(view)
                .setPositiveButton(R.string.close, null);
        AlertDialog alertDialog = builder.create();
        alertDialog.setCanceledOnTouchOutside(false);
        alertDialog.setCancelable(true);
        alertDialog.show();
    }

    /**
     * This class contains all butterknife-injected Views & Layouts from layout file 'document_info.xml'
     * for easy to all layout elements.
     *
     * @author ButterKnifeZelezny, plugin for Android Studio by Inmite Developers (http://inmite.github.io)
     */
    static class ViewHolder {
        TextView mPathTextView;
        TextView mEncodingTextView;
        TextView mWordCountTextView;
        TextView mCharCountTextView;
        TextView mLineCountTextView;

        ViewHolder(View view) {
            mPathTextView = view.findViewById(R.id.path_textView);
            mEncodingTextView = view.findViewById(R.id.encoding_textView);
            mWordCountTextView = view.findViewById(R.id.word_count_textView);
            mCharCountTextView = view.findViewById(R.id.char_count_textView);
            mLineCountTextView = view.findViewById(R.id.line_count_textView);
        }
    }
}
