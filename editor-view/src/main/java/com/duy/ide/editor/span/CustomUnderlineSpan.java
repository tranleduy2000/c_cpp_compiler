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