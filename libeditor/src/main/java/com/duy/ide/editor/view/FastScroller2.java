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
import android.content.res.ColorStateList;
import android.content.res.Resources;
import android.content.res.TypedArray;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.drawable.Drawable;
import android.os.Handler;
import android.os.SystemClock;
import android.text.Spannable;
import android.view.MotionEvent;
import android.widget.EditText;
import android.widget.SectionIndexer;

import com.duy.ide.editor.core.text.Selection;
import com.duy.ide.editor.editor.R;
import com.jecelyin.common.utils.DLog;

/**
 * Helper class for AbsListView to draw and control the Fast Scroll thumb
 */
public class FastScroller2 {

    // Scroll thumb not showing
    private static final int STATE_NONE = 0;
    // Not implemented yet - fade-in transition
    //private static final int STATE_ENTER = 1;
    // Scroll thumb visible and moving along with the scrollbar
    private static final int STATE_VISIBLE = 2;
    // Scroll thumb being dragged by user
    private static final int STATE_DRAGGING = 3;
    // Scroll thumb fading out due to inactivity timeout
    private static final int STATE_EXIT = 4;
    // Minimum number of pages to justify showing a fast scroll thumb
    private static int MIN_PAGES = 1;//jecelyin: 超过一页就显示滚动条
    private Drawable mThumbDrawable;

    private int mThumbH;
    private int mThumbW;
    private int mThumbY;

    private EditText mEditText;
    //private boolean mScrollCompleted;
    private int mVisibleItem;
    private Paint mPaint;

    private int mItemCount = -1;
    private boolean mLongList;

    private Object[] mSections;

    private ScrollFade mScrollFade;

    private int mState;

    private Handler mHandler = new Handler();


    private SectionIndexer mSectionIndexer;

    private boolean mChangedBounds;


    public FastScroller2(Context context, EditText textView) {
        mEditText = textView;
        init(context);
    }

    public int getState() {
        return mState;
    }

    public void setState(int state) {
        switch (state) {
            case STATE_NONE:
                mHandler.removeCallbacks(mScrollFade);
                mEditText.invalidate();
                break;
            case STATE_VISIBLE:
                if (mState != STATE_VISIBLE) { // Optimization
                    resetThumbPos();
                }
                // Fall through
            case STATE_DRAGGING:
                mHandler.removeCallbacks(mScrollFade);
                break;
            case STATE_EXIT:
                int viewWidth = mEditText.getWidth();
                mEditText.invalidate(viewWidth - mThumbW, mThumbY, viewWidth, mThumbY + mThumbH);
                break;
        }
        mState = state;
    }

    private void resetThumbPos() {
        final int viewWidth = mEditText.getWidth();
        // Bounds are always top right. Y coordinate get's translated during draw
        //Log.v(TAG, "setBounds resetThumbPos left:"+String.valueOf(viewWidth - mThumbW)+" right:"+String.valueOf(viewWidth)+" button:"+String.valueOf(mThumbH));
        mThumbDrawable.setBounds(viewWidth - mThumbW, 0, viewWidth, mThumbH);
        mThumbDrawable.setAlpha(ScrollFade.ALPHA_MAX);
    }

    private void useThumbDrawable(Context context, Drawable drawable) {
        mThumbDrawable = drawable;
        /*mThumbW = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP,
                48, context.getResources().getDisplayMetrics());
        mThumbH = (int) TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP,
                52, context.getResources().getDisplayMetrics());*/
        mThumbW = context.getResources().getDimensionPixelSize(
                R.dimen.fastscroll_thumb_width);
        mThumbH = context.getResources().getDimensionPixelSize(
                R.dimen.fastscroll_thumb_height);
        mChangedBounds = true;
    }

    private void init(Context context) {
        // Get both the scrollbar states drawables
        final Resources res = context.getResources();
        useThumbDrawable(context, res.getDrawable(R.drawable.scrollbar_handle_accelerated_anim2));
        //mScrollCompleted = true;

        getSectionsFromIndexer();

        mScrollFade = new ScrollFade();
        mPaint = new Paint();
        mPaint.setAntiAlias(true);
        mPaint.setTextAlign(Paint.Align.CENTER);
        TypedArray ta = context.getTheme().obtainStyledAttributes(new int[]{
                android.R.attr.textColorPrimary});
        ColorStateList textColor = ta.getColorStateList(ta.getIndex(0));
        int textColorNormal = textColor.getDefaultColor();
        mPaint.setColor(textColorNormal);
        mPaint.setStyle(Paint.Style.FILL_AND_STROKE);

        mState = STATE_NONE;
    }

    void stop() {
        setState(STATE_NONE);
    }

    boolean isVisible() {
        return !(mState == STATE_NONE);
    }

    public void draw(Canvas canvas) {
        //Log.v(TAG, "draw status "+String.valueOf(mState));
        if (mState == STATE_NONE) {
            // No need to draw anything
            return;
        }

        final int y = mThumbY + mEditText.getScrollY();
        final int viewWidth = mEditText.getWidth();
        final FastScroller2.ScrollFade scrollFade = mScrollFade;
        final int x = mEditText.getScrollX();

        int alpha = -1;
        if (mState == STATE_EXIT) {
            alpha = scrollFade.getAlpha();
            if (alpha < ScrollFade.ALPHA_MAX / 2) {
                mThumbDrawable.setAlpha(alpha * 2);
            }
            int left = viewWidth - (mThumbW * alpha) / ScrollFade.ALPHA_MAX;
            //Log.v(TAG, "setBounds draw left:"+String.valueOf(left)+" right:"+String.valueOf(viewWidth)+" button:"+String.valueOf(mThumbH));
            mThumbDrawable.setBounds(left, 0, viewWidth, mThumbH);
            mChangedBounds = true;
        }
        //Log.v(TAG, "setBounds draw x:"+String.valueOf(x)+" y:"+String.valueOf(y));
        canvas.translate(x, y);
        mThumbDrawable.draw(canvas);
        canvas.translate(-x, -y);

        // If user is dragging the scroll bar, draw the alphabet overlay
        if (alpha == 0) { // Done with exit
            setState(STATE_NONE);
        } else {
            mEditText.invalidate(viewWidth - mThumbW, y, viewWidth, y + mThumbH);
        }
    }

    public void onSizeChanged(int w, int h, int oldw, int oldh) {
        if (mThumbDrawable != null) {
            //Log.v(TAG, "setBounds onSizeChanged left:"+String.valueOf(w - mThumbW)+" right:"+String.valueOf(w)+" button:"+String.valueOf(mThumbH));
            mThumbDrawable.setBounds(w - mThumbW, 0, w, mThumbH);
        }
    }


    public void onScroll(int firstVisibleItem, int visibleItemCount,
                         int totalItemCount) {

        //Log.d(TAG, "onScroll firstVisibleItem:"+String.valueOf(firstVisibleItem)
        //        +" visibleItemCount:"+String.valueOf(visibleItemCount)
        //        +" totalItemCount:"+String.valueOf(totalItemCount)
        //);
        // Are there enough pages to require fast scroll? Recompute only if total count changes
        if (mItemCount != totalItemCount && visibleItemCount > 0) {
            mItemCount = totalItemCount;
            mLongList = mItemCount / visibleItemCount >= MIN_PAGES;
        }
        //Log.d(TAG, "onScroll mLongList:"+String.valueOf(mLongList)+" mState:"+String.valueOf(mState));
        if (!mLongList) {
            if (mState != STATE_NONE) {
                setState(STATE_NONE);
            }
            return;
        }
        if (totalItemCount - visibleItemCount > 0 && mState != STATE_DRAGGING) {
            mThumbY = ((mEditText.getHeight() - mThumbH) * firstVisibleItem)
                    / (totalItemCount - visibleItemCount);
//            DLog.d("FSL onScroll thumbY=" + mThumbY);
            if (mChangedBounds) {
                resetThumbPos();
                mChangedBounds = false;
            }
        }
        //mScrollCompleted = true;
        //Log.d(TAG, "onScroll firstVisibleItem:"+String.valueOf(firstVisibleItem)+" mVisibleItem:"+String.valueOf(mVisibleItem));
        if (firstVisibleItem == mVisibleItem) {
            return;
        }
        mVisibleItem = firstVisibleItem;
        if (mState != STATE_DRAGGING) {
            setState(STATE_VISIBLE);
            mHandler.postDelayed(mScrollFade, 1500);
        }
    }

    SectionIndexer getSectionIndexer() {
        return mSectionIndexer;
    }

    Object[] getSections() {
        if (mSections == null) {
            getSectionsFromIndexer();
        }
        return mSections;
    }

    private void getSectionsFromIndexer() {
        mSectionIndexer = null;
        mSections = new String[]{" "};
    }

    private void scrollTo(float position) {
        int count = mEditText.getLineCount();
        //mScrollCompleted = false;
        int index = (int) (position * count);
        try {
            int offset = mEditText.getLayout().getLineStart(index);
            DLog.d("FSL scrollTo position=" + position + " offset=" + offset + " " + index + "/" + count);
            Selection.setSelection((Spannable) mEditText.getText(), offset, offset);
        } catch (Exception e) {
            DLog.e("FSL scrollTo error", e);
        }

    }

    private void cancelFling() {
        // Cancel the list fling
        MotionEvent cancelFling = MotionEvent.obtain(0, 0, MotionEvent.ACTION_CANCEL, 0, 0, 0);
        mEditText.onTouchEvent(cancelFling);
        cancelFling.recycle();
    }

    public boolean onInterceptTouchEvent(MotionEvent ev) {
        //Log.d(TAG, "onInterceptTouchEvent mState:"+String.valueOf(mState)+" ev.getAction():"+String.valueOf(ev.getAction()));
        if (mState > STATE_NONE && ev.getAction() == MotionEvent.ACTION_DOWN) {
            if (isPointInside(ev.getX(), ev.getY())) {
                setState(STATE_DRAGGING);
                return true;
            }
        }
        return false;
    }

    public boolean onTouchEvent(MotionEvent me) {
        //Log.d(TAG, "onTouchEvent mState:"+String.valueOf(mState));
        if (mState == STATE_NONE) {
            return false;
        }

        final int action = me.getAction();
        //Log.d(TAG, "onTouchEvent action:"+String.valueOf(action));
        if (action == MotionEvent.ACTION_DOWN) {
            if (isPointInside(me.getX(), me.getY())) {
                setState(STATE_DRAGGING);
                if (mSections == null) {
                    getSectionsFromIndexer();
                }
                if (mEditText != null) {
                }

                cancelFling();
                return true;
            }
        } else if (action == MotionEvent.ACTION_UP) {
            if (mState == STATE_DRAGGING) {
                setState(STATE_VISIBLE);
                final Handler handler = mHandler;
                handler.removeCallbacks(mScrollFade);
                handler.postDelayed(mScrollFade, 1000);
                return true;
            }
        } else if (action == MotionEvent.ACTION_MOVE) {
            if (mState == STATE_DRAGGING) {
                final int viewHeight = mEditText.getHeight();
                // Jitter
                int newThumbY = (int) me.getY() - mThumbH / 2;
                if (newThumbY < 0) {
                    newThumbY = 0;
                } else if (newThumbY + mThumbH > viewHeight) {
                    newThumbY = viewHeight - mThumbH;
                }
                //Log.v(TAG, "onTouchEvent: Math.abs(mThumbY - newThumbY): "+String.valueOf(Math.abs(mThumbY - newThumbY)));
                if (Math.abs(mThumbY - newThumbY) < 2) {
                    return true;
                }
                mThumbY = newThumbY;
                DLog.d("FSL onTouchEvent thumbY=" + mThumbY);
                // If the previous scrollTo is still pending
//                if (mScrollCompleted) {
                scrollTo((float) mThumbY / (viewHeight - mThumbH));
//                }
                return true;
            }
        }
        return false;
    }

    boolean isPointInside(float x, float y) {
        int width = mEditText.getWidth();
        final int thumbY = mThumbY;
        DLog.d("FSL isPointInside y=" + y + " thumbY=" + thumbY);
        return x > width - mThumbW && y >= thumbY && y <= thumbY + mThumbH;
    }

    public class ScrollFade implements Runnable {

        static final int ALPHA_MAX = 208;
        static final long FADE_DURATION = 200;
        long mStartTime;
        long mFadeDuration;

        void startFade() {
            mFadeDuration = FADE_DURATION;
            mStartTime = SystemClock.uptimeMillis();
            setState(STATE_EXIT);
        }

        int getAlpha() {
            if (getState() != STATE_EXIT) {
                return ALPHA_MAX;
            }
            int alpha;
            long now = SystemClock.uptimeMillis();
            if (now > mStartTime + mFadeDuration) {
                alpha = 0;
            } else {
                alpha = (int) (ALPHA_MAX - ((now - mStartTime) * ALPHA_MAX) / mFadeDuration);
            }
            return alpha;
        }

        public void run() {
            if (getState() != STATE_EXIT) {
                startFade();
                return;
            }

            if (getAlpha() > 0) {
                mEditText.invalidate();
            } else {
                setState(STATE_NONE);
            }
        }
    }
}