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

package com.duy.file.explorer.widget;

import android.content.Context;
import android.util.AttributeSet;
import android.widget.Checkable;

import com.duy.ide.editor.editor.R;
import com.makeramen.roundedimageview.RoundedImageView;

/**
 * Created by jecelyin on 16/7/29.
 */

public class IconImageView extends RoundedImageView implements Checkable {
    private int defaultBackgroundColor;
    private int defaultImageResource;
    private boolean checked;

    public IconImageView(Context context) {
        super(context);
    }

    public IconImageView(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public IconImageView(Context context, AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
    }

    public void setDefaultBackgroundColor(int color) {
        super.setBackgroundColor(color);
        defaultBackgroundColor = color;
    }

    public void setDefaultImageResource(int drawable) {
        super.setImageResource(drawable);
        defaultImageResource = drawable;
    }

    public void reset() {
        setBackgroundColor(defaultBackgroundColor);
        setImageResource(defaultImageResource);
    }

    @Override
    public boolean isChecked() {
        return checked;
    }

    @Override
    public void setChecked(boolean checked) {
        this.checked = checked;
        if (checked) {
            setBackgroundColor(getResources().getColor(R.color.item_icon_select_status));
            setImageResource(R.drawable.file_checked);
        } else {
            setBackgroundColor(defaultBackgroundColor);
            setImageResource(defaultImageResource);
        }
    }

    @Override
    public void toggle() {
        checked = !checked;
        setChecked(checked);
    }
}
