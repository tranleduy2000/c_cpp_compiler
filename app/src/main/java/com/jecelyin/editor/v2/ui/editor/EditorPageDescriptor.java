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

import android.os.Parcel;
import android.support.annotation.NonNull;

import com.duy.ide.editor.pager.SimplePageDescriptor;

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
        return 0;
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
