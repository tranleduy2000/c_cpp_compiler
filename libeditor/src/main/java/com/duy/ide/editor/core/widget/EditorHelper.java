/*
 *
 *  * Copyright (C) 2006 The Android Open Source Project
 *  *
 *  * Licensed under the Apache License, Version 2.0 (the "License");
 *  * you may not use this file except in compliance with the License.
 *  * You may obtain a copy of the License at
 *  *
 *  *      http://www.apache.org/licenses/LICENSE-2.0
 *  *
 *  * Unless required by applicable law or agreed to in writing, software
 *  * distributed under the License is distributed on an "AS IS" BASIS,
 *  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  * See the License for the specific language governing permissions and
 *  * limitations under the License.
 *
 */

package com.duy.ide.editor.core.widget;

import android.text.Editable;

import com.duy.ide.editor.core.text.SpannableStringBuilder;
import com.duy.ide.editor.view.IEditAreaView;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class EditorHelper {
    private final IEditAreaView editAreaView;

    public EditorHelper(IEditAreaView editAreaView) {
        this.editAreaView = editAreaView;
    }

    public void duplication() {
        int start = editAreaView.getSelectionStart();
        int end = editAreaView.getSelectionEnd();
        SpannableStringBuilder text = new SpannableStringBuilder();
        int offset;
        Editable mText = editAreaView.getEditableText();
        if (end == start) {//重复行
            int s = start, e = end;
            while (--s >= 0 && mText.charAt(s) != '\n' && mText.charAt(s) != '\r') ;
            if (s < 0) s = 0;
            int length = mText.length();
            while (e < length && mText.charAt(e) != '\n' && mText.charAt(e) != '\r') {
                e++;
            }
            //fix first line nowrap
            if (s == 0)
                text.append('\n');
            text.append(mText.subSequence(s, e));
            offset = e;
        } else {
            //重复选中的文本
            text.append(mText.subSequence(start, end));
            offset = end;
        }
        mText.replace(offset, offset, text, 0, text.length());
    }

    public void convertWrapCharTo(String chars) {
        Editable text = editAreaView.getText();
        int offset = text.length(), len = chars.length();
        while (--offset >= 0) {
            if (text.charAt(offset) == '\n') {
                //\r\n
                if (offset > 0 && text.charAt(offset - 1) == '\r') {
                    text.replace(offset - 1, offset + 1, chars, 0, len);
                    offset--;
                } else {
                    text.replace(offset, offset + 1, chars, 0, len);
                }
            } else if (text.charAt(offset) == '\r') {
                text.replace(offset, offset + 1, chars, 0, len);
            }
        }
    }
}
