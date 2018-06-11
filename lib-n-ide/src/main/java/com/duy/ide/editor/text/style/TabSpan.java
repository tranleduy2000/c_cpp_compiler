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
import android.graphics.Paint;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.text.style.ReplacementSpan;

import com.duy.ide.editor.text.LayoutContext;

public class TabSpan extends ReplacementSpan {
    private final int whiteSpaceColor;
    private final int tabWidth;
    private boolean isShowWhiteSpace;

    public TabSpan(LayoutContext layoutContext, int tabWidth) {
        this.whiteSpaceColor = layoutContext.whiteSpaceColor;
        this.isShowWhiteSpace = layoutContext.isShowWhiteSpace;
        this.tabWidth = tabWidth;
    }

    @Override
    public int getSize(@NonNull Paint paint, CharSequence text, int start, int end, @Nullable Paint.FontMetricsInt fm) {
        return tabWidth;
    }

    @Override
    public void draw(@NonNull Canvas canvas, CharSequence text, int start, int end, float x, int top, int y, int bottom, @NonNull Paint paint) {
        if (isShowWhiteSpace) {
            paint.set(paint);
            paint.setColor(whiteSpaceColor);
            paint.setStrokeWidth(2f);
            paint.setStyle(Paint.Style.FILL);
            canvas.drawLine(x, y, x + tabWidth - tabWidth / 10f, y, paint);
        }
    }
}
