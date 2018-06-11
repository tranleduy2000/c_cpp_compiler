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

package com.duy.ide.editor.text.style;

import android.os.Parcel;
import android.os.Parcelable;
import android.text.TextPaint;
import android.text.style.CharacterStyle;
import android.text.style.UpdateAppearance;

/**
 * Created by Duy on 25-Apr-18.
 */

public class ErrorSpan extends CharacterStyle implements UpdateAppearance, Parcelable {

    public static final Creator<ErrorSpan> CREATOR = new Creator<ErrorSpan>() {
        @Override
        public ErrorSpan createFromParcel(Parcel in) {
            return new ErrorSpan(in);
        }

        @Override
        public ErrorSpan[] newArray(int size) {
            return new ErrorSpan[size];
        }
    };
    private int mColor;


    public ErrorSpan(int color) {
        mColor = color;
    }

    private ErrorSpan(Parcel in) {
        mColor = in.readInt();
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeInt(mColor);
    }

    @Override
    public void updateDrawState(TextPaint paint) {
        paint.setColor(mColor);
        paint.setUnderlineText(true);
        paint.setFakeBoldText(true);
    }
}
