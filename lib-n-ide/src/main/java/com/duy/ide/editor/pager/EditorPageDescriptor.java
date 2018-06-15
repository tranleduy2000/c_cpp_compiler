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

package com.duy.ide.editor.pager;

import android.os.Parcel;
import android.support.annotation.NonNull;

import com.commonsware.cwac.pager.SimplePageDescriptor;

import java.io.File;

/**
 * Created by Duy on 25-Apr-18.
 */

public class EditorPageDescriptor extends SimplePageDescriptor {
    public static final Creator<EditorPageDescriptor> CREATOR = new Creator<EditorPageDescriptor>() {
        @Override
        public EditorPageDescriptor createFromParcel(Parcel in) {
            return new EditorPageDescriptor(in);
        }

        @Override
        public EditorPageDescriptor[] newArray(int size) {
            return new EditorPageDescriptor[size];
        }
    };

    @NonNull
    private final File file;
    private final int cursorOffset;
    private final String encoding;

    public EditorPageDescriptor(@NonNull File file, int cursorOffset, String encoding) {
        super(file.getPath(), file.getName());
        this.file = file;
        this.cursorOffset = cursorOffset;
        this.encoding = encoding;
    }

    private EditorPageDescriptor(Parcel in) {
        super(in);
        file = (File) in.readSerializable();
        cursorOffset = in.readInt();
        encoding = in.readString();
    }


    @Override
    public int describeContents() {
        return super.describeContents();
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        super.writeToParcel(dest, flags);
        dest.writeSerializable(file);
        dest.writeInt(cursorOffset);
        dest.writeString(encoding);
    }

    @NonNull
    public File getFile() {
        return file;
    }

    public int getCursorOffset() {
        return cursorOffset;
    }

    public String getEncoding() {
        return encoding;
    }

    public String getPath() {
        return file.getPath();
    }
}
