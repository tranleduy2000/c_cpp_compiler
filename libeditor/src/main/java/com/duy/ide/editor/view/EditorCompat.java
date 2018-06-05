/*
 *  Copyright (C)  2018  Duy Tran Le
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.duy.ide.editor.view;

import android.text.Editable;

import com.duy.ide.editor.core.text.SpannableStringBuilder;

public class EditorCompat {
    public static void gotoTop(IEditAreaView editView) {
        editView.setSelection(0);
    }

    public static void gotoEnd(IEditAreaView editView) {
        editView.setSelection(editView.length());
    }

    public static void duplicateSelection(IEditAreaView editAreaView) {

        int start = editAreaView.getSelectionStart();
        int end = editAreaView.getSelectionEnd();
        SpannableStringBuilder text = new SpannableStringBuilder();
        int offset;
        Editable mText = editAreaView.getEditableText();
        if (end == start) {
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
            text.append(mText.subSequence(start, end));
            offset = end;
        }
        mText.replace(offset, offset, text, 0, text.length());
    }
}
