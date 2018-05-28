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

package com.duy.ide.editor.span;

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

    protected ErrorSpan(Parcel in) {
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
//        TextPaintCompat.setUnderlineText(paint, mColor, 4);
    }
}
