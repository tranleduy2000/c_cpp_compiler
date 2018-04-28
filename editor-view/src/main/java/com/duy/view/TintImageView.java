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

package com.duy.view;

import android.content.Context;
import android.content.res.ColorStateList;
import android.content.res.TypedArray;
import android.support.v7.widget.AppCompatImageView;
import android.util.AttributeSet;

/**
 * Created by Duy on 28-Apr-18.
 */

public class TintImageView extends AppCompatImageView {
    private ColorStateList colorList;

    public TintImageView(Context context) {
        super(context);
        init(context, null);
    }

    public TintImageView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(context, attrs);
    }

    public TintImageView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context, attrs);
    }

    private void init(Context context, AttributeSet attrs) {
        if (attrs != null) {
            TypedArray arr =
                    context.obtainStyledAttributes(attrs, new int[]{android.R.attr.textColor});
            colorList = arr.getColorStateList(0);
            if (colorList != null) {
                setColorFilter(colorList.getDefaultColor());
            }
            arr.recycle();
        }
    }

    @Override
    public void dispatchDrawableHotspotChanged(float x, float y) {
        super.dispatchDrawableHotspotChanged(x, y);
        clearColorFilter();
        setColorFilter(colorList.getColorForState(getDrawableState(), colorList.getDefaultColor()));
    }

    @Override
    public void refreshDrawableState() {
        super.refreshDrawableState();
    }
}
