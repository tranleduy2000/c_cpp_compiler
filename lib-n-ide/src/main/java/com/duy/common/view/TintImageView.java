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

package com.duy.common.view;

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
