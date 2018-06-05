/*
 *  Copyright (C)  2018  Duy Tran Le
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.duy.ide.editor.view;

import android.content.Context;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.TypedValue;
import android.view.ScaleGestureDetector;

import com.duy.ide.editor.core.widget.FastScroller;
import com.jecelyin.editor.v2.Preferences;

public class ZoomSupportEditText extends HighlightEditorView {
    /**
     * Helper object that renders and controls the fast scroll thumb.
     */
    @Nullable
    private FastScroller mFastScroller;
    /**
     * When user zoom in/zoom out, disable long click event
     */
    private boolean isTextScaling = false;
    private ScaleGestureDetector mScaleDetector;

    public ZoomSupportEditText(Context context) {
        super(context);
        initView();
    }

    public ZoomSupportEditText(Context context, AttributeSet attrs) {
        super(context, attrs);
    }

    public ZoomSupportEditText(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    private void initView() {
        mScaleDetector = new ScaleGestureDetector(getContext(), new ScaleListener());
//        mFastScroller = new FastScroller(getContext(), this);

    }

    @Override
    public boolean performLongClick() {
        if (isTextScaling) {
            return true;
        }
        return super.performLongClick();
    }


    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        if (mFastScroller != null) {
            mFastScroller.onSizeChanged(w, h, oldw, oldh);
        }
    }

    private class ScaleListener extends ScaleGestureDetector.SimpleOnScaleGestureListener {
        private final float minSize;
        private final float maxSize;

        public ScaleListener() {
            DisplayMetrics metrics = getResources().getDisplayMetrics();
            minSize = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_SP, Preferences.DEF_MIN_FONT_SIZE, metrics);
            maxSize = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_SP, Preferences.DEF_MAX_FONT_SIZE, metrics);
        }

        @Override
        public boolean onScaleBegin(ScaleGestureDetector detector) {
            isTextScaling = true;
            return super.onScaleBegin(detector);
        }

        @Override
        public boolean onScale(ScaleGestureDetector detector) {
            float size = getTextSize() * detector.getScaleFactor();
            size = Math.max(minSize, Math.min(size, maxSize * 2));
            setTextSize(TypedValue.COMPLEX_UNIT_PX, size);

            return true;
        }

        @Override
        public void onScaleEnd(ScaleGestureDetector detector) {
            super.onScaleEnd(detector);
            isTextScaling = false;
        }
    }


}
