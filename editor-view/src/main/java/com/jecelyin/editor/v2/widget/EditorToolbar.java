/*
 * Copyright (C) 2016 Jecelyin Peng <jecelyin@gmail.com>
 *
 * This file is part of 920 Text Editor.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.jecelyin.editor.v2.widget;

import android.content.Context;
import android.content.res.Resources;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.support.v7.widget.Toolbar;
import android.text.TextPaint;
import android.util.AttributeSet;
import android.util.TypedValue;

import com.jecelyin.common.utils.SysUtils;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class EditorToolbar extends Toolbar {
    private TextPaint mTitlePaint;
    private CharSequence title;

    public EditorToolbar(Context context) {
        super(context);
        init(context);
    }

    public EditorToolbar(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(context);
    }

    public EditorToolbar(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);

        init(context);
    }

    private void init(Context context) {
        setWillNotDraw(false);

        // Get the secondary text color of the theme
        TypedValue typedValue = new TypedValue();
        Resources.Theme theme = context.getTheme();
        theme.resolveAttribute(android.R.attr.textColorSecondary, typedValue, true);
        TypedArray arr = context.obtainStyledAttributes(typedValue.data, new int[]{
                android.R.attr.textColorSecondary});
        int textColor = arr.getColor(0, Color.WHITE);
        arr.recycle();

        mTitlePaint = new TextPaint();
        mTitlePaint.setColor(textColor);
        mTitlePaint.setTextSize(SysUtils.spToPixels(getContext(), 10));
        mTitlePaint.setTextAlign(Paint.Align.LEFT);
        mTitlePaint.setAntiAlias(true);
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    }

    @Override
    public void setTitle(CharSequence title) {
        this.title = title;
        invalidate();
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        if (title == null) {
            return;
        }
        Paint.FontMetrics fontMetrics = mTitlePaint.getFontMetrics();
        int x = SysUtils.dpAsPixels(getContext(), 16);
        int y = (int) (getHeight() - (fontMetrics.bottom)) - SysUtils.dpAsPixels(getContext(), 2);
        canvas.drawText(title, 0, title.length(), x, y, mTitlePaint);
    }
}
