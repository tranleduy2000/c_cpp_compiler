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

package com.duy.text.editor.view;

import android.content.Context;
import android.core.text.LayoutContext;
import android.core.text.TextLineNumber;
import android.core.widget.BaseEditorView;
import android.graphics.Canvas;
import android.support.annotation.Nullable;
import android.text.Editable;
import android.text.TextWatcher;
import android.util.AttributeSet;
import android.view.View;

import java.util.List;

/**
 * Created by Duy on 25-Apr-18.
 */

public class LineNumberView extends View implements TextWatcher {
    @Nullable
    private BaseEditorView editorView;

    public LineNumberView(Context context, @Nullable BaseEditorView editorView) {
        super(context);
    }

    public LineNumberView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
    }

    public LineNumberView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    public void setEditorView(@Nullable BaseEditorView editorView) {
        this.editorView = editorView;
        if (editorView != null) {
            editorView.addTextChangedListener(this);
        }
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);
        if (editorView == null) {
            return;
        }
        drawLineNumber(canvas);
    }

    // TODO: 24-Apr-18 improve calculate padding and recalculate when layout changed
    private void drawLineNumber(Canvas canvas) {
        if (editorView == null) {
            return;
        }
        LayoutContext layoutContext = editorView.getLayoutContext();
        if (!layoutContext.preference.isShowLineNumber()) {
            return;
        }

        int width = getScrollX() + layoutContext.gutterWidth;
        int height = getScrollY() + getHeight();
        canvas.drawRect(getScrollX(), getScrollY(), width, height, layoutContext.gutterBackgroundPaint);
        canvas.drawLine(width, getScrollY(), width, height, layoutContext.linePaint);

        List<TextLineNumber.LineInfo> lines = layoutContext.textLineNumber.getLines();
        for (TextLineNumber.LineInfo line : lines) {
            canvas.drawText(line.text, layoutContext.lineNumberX + getScrollX(), line.y, layoutContext.lineNumberPaint);
        }
    }


    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
        // TODO: 25-Apr-18
    }

    @Override
    public void beforeTextChanged(CharSequence s, int start, int count, int after) {

    }

    @Override
    public void onTextChanged(CharSequence s, int start, int before, int count) {

    }

    @Override
    public void afterTextChanged(Editable s) {
        invalidate();
    }


    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        if (editorView != null) {
            editorView.removeTextChangedListener(this);
        }
    }

}
