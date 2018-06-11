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

package com.duy.ide.editor.content;

import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

public class ClipboardCompat {
    @NonNull
    private Context mContext;
    @Nullable
    private ClipboardManager mClipboardManager;

    public ClipboardCompat(@NonNull Context context) {
        mContext = context;
        mClipboardManager = (ClipboardManager) mContext.getSystemService(Context.CLIPBOARD_SERVICE);
    }

    @Nullable
    public CharSequence getClipboard() {
        if (mClipboardManager == null) {
            return null;
        }
        // Examines the item on the clipboard. If getText() does not return null,
        // the clip item contains the
        // text. Assumes that this application can only handle one item at a time.
        ClipData.Item item = mClipboardManager.getPrimaryClip().getItemAt(0);
        // Gets the clipboard as text.
        return item.getText();
    }

    public void setClipboard(@NonNull CharSequence content) {
        ClipData clipData = ClipData.newPlainText("", content);
        if (mClipboardManager != null) {
            mClipboardManager.setPrimaryClip(clipData);
        }
    }
}
