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

import android.graphics.Canvas;
import android.graphics.DashPathEffect;
import android.graphics.Paint;
import android.text.style.LineBackgroundSpan;

/**
 * https://stackoverflow.com/questions/29375638/custom-span-underline-text-with-dotted-line-android
 */
public class CustomUnderlineSpan implements LineBackgroundSpan {

    private int color;
    private Paint paint;

    public CustomUnderlineSpan(int underlineColor) {
        color = underlineColor;
        paint = new Paint();
        paint.setColor(color);
        paint.setStrokeWidth(3F);
        paint.setPathEffect(new DashPathEffect(new float[]{20, 20}, 0));
    }

    @Override
    public void drawBackground(Canvas canvas, Paint paint, int left, int right, int top, int baseline,
                               int bottom, CharSequence text, int start, int end, int lnum) {
        canvas.drawLine(left, bottom, right, bottom, this.paint);
    }
}