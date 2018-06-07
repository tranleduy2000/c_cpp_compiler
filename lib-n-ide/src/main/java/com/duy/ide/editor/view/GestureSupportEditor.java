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
import android.graphics.Rect;
import android.support.annotation.Nullable;
import android.text.Layout;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.util.TypedValue;
import android.view.GestureDetector;
import android.view.MotionEvent;
import android.view.ScaleGestureDetector;
import android.widget.Scroller;

import com.duy.ide.editor.model.EditorIndex;
import com.jecelyin.editor.v2.Preferences;

public abstract class GestureSupportEditor extends HighlightEditorView
        implements IEditAreaView, GestureDetector.OnGestureListener {

    private final Rect mVisibleRect = new Rect();
    private OnEditorSizeChangedListener mOnEditorSizeChangedListener;
    @Nullable
    private ScaleGestureDetector mScaleDetector;
    @Nullable
    private GestureDetector mGestureDetector;
    @Nullable
    private Scroller mScroller;
    /**
     * When user zoom in/zoom out, disable long click event
     */
    private boolean mIsTextScaling = false;

    public GestureSupportEditor(Context context) {
        this(context, null);
    }

    public GestureSupportEditor(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(context);
    }

    public GestureSupportEditor(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context);
    }

    private void init(Context context) {
        mScaleDetector = new ScaleGestureDetector(getContext(), new ScaleListener());
        mGestureDetector = new GestureDetector(getContext(), this);
        mScroller = new Scroller(getContext());
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        if (mOnEditorSizeChangedListener != null) {
            mOnEditorSizeChangedListener.onEditorSizeChanged(w, h, oldw, oldh);
        }
    }

    public void setOnEditorSizeChangedListener(OnEditorSizeChangedListener onEditorSizeChangedListener) {
        this.mOnEditorSizeChangedListener = onEditorSizeChangedListener;
    }

    @Override
    public EditorIndex getCursorIndex(int realLine, int column) {
        return null;
    }

    @Override
    public int getLineForOffset(int offset) {
        if (getLayout() == null) {
            return -1;
        }
        return getLayout().getLineForOffset(offset);
    }

    @Override
    public boolean performLongClick() {
        if (mIsTextScaling) {
            return true;
        }
        return super.performLongClick();
    }

    @Override
    public boolean onTouchEvent(MotionEvent ev) {
        try {
            if (!hasSelection() && mScaleDetector != null && mPreferences.isTouchScaleTextSize()) {
                mScaleDetector.onTouchEvent(ev);
            }

            if (mGestureDetector != null) {
                mGestureDetector.onTouchEvent(ev);
            }
        } catch (Exception e) {
        }
        return super.onTouchEvent(ev);
    }

    @Override
    public boolean onDown(MotionEvent e) {
        if (mScroller != null) {
            mScroller.forceFinished(true);
        }
        return false;
    }

    @Override
    public void onShowPress(MotionEvent e) {

    }

    @Override
    public boolean onSingleTapUp(MotionEvent e) {
        return false;
    }

    @Override
    public boolean onScroll(MotionEvent e1, MotionEvent e2, float distanceX, float distanceY) {
        return false;
    }

    @Override
    public void onLongPress(MotionEvent e) {

    }

    @Override
    public void computeScroll() {
        if (mScroller != null) {
            if (mScroller.computeScrollOffset()) {
                scrollTo(mScroller.getCurrX(), mScroller.getCurrY());
            }
        } else {
            super.computeScroll();
        }
    }

    /**
     * @see GestureDetector.OnGestureListener#onFling(MotionEvent,
     * MotionEvent, float, float)
     */
    public boolean onFling(MotionEvent e1, MotionEvent e2, float velocityX,
                           float velocityY) {
        Layout layout = getLayout();
        if (mScroller != null && layout != null) {
            int maxX = layout.getWidth() - mVisibleRect.width() - getPaddingRight() - getPaddingLeft();
            int maxY = layout.getHeight() - mVisibleRect.height() - getPaddingBottom() - getPaddingTop();
            mScroller.fling(getScrollX(), getScrollY(), -(int) velocityX, -(int) velocityY,
                    0, maxX, 0, maxY);
        }
        return true;
    }

    @Override
    protected void onLayout(boolean changed, int left, int top, int right, int bottom) {
        super.onLayout(changed, left, top, right, bottom);
        mVisibleRect.set(left, top, right, bottom);
    }

    public interface OnEditorSizeChangedListener {
        void onEditorSizeChanged(int w, int h, int oldw, int oldh);
    }

    private class ScaleListener extends ScaleGestureDetector.SimpleOnScaleGestureListener {
        private final float minSize;
        private final float maxSize;

        ScaleListener() {
            DisplayMetrics metrics = getResources().getDisplayMetrics();
            minSize = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_SP, Preferences.DEF_MIN_FONT_SIZE, metrics);
            maxSize = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_SP, Preferences.DEF_MAX_FONT_SIZE, metrics);
        }

        @Override
        public boolean onScaleBegin(ScaleGestureDetector detector) {
            mIsTextScaling = true;
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
            mIsTextScaling = false;
        }
    }
}
