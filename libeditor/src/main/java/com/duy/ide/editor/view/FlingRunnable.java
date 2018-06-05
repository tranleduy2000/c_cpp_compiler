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
import android.text.Layout;

import com.duy.ide.editor.core.widget.Scroller;
import com.duy.ide.editor.text.method.Touch;

/**
 * Responsible for fling behavior. Use  {@link #start(int)}  to
 * initiate a fling. Each frame of the fling is handled in {@link #run()}.
 * A FlingRunnable will keep re-posting itself until the fling is done.
 */
public class FlingRunnable implements Runnable {

    static final int TOUCH_MODE_REST = -1;
    static final int TOUCH_MODE_FLING = 3;
    /**
     * Tracks the decay of a fling scroll
     */
    private final Scroller mScroller;
    final private HighlightEditorView mWidget;
    int mTouchMode = TOUCH_MODE_REST;
    /**
     * Y value reported by mScroller on the previous fling
     */
    private int mLastFlingY;
    private int maxY;

    public FlingRunnable(Context context, HighlightEditorView parent) {
        mScroller = new Scroller(context);
        mWidget = parent;
    }

    public void start(int initialVelocity) {
//        int initialX = mWidget.getScrollX(); // initialVelocity < 0 ?
//        // Integer.MAX_VALUE : 0;
//        int initialY = mWidget.getScrollY(); // initialVelocity < 0 ?
//        // Integer.MAX_VALUE : 0;
//        mLastFlingY = initialY;
//        mScroller.fling(initialX, initialY, 0, initialVelocity, 0, Integer.MAX_VALUE, 0, Integer.MAX_VALUE);
        int initialY = mWidget.getScrollY();
        mLastFlingY = initialY;
        maxY = mWidget.getMaxScrollY();
        mScroller.fling(0, initialY, 0, initialVelocity, 0, Integer.MAX_VALUE, 0, Integer.MAX_VALUE);

        mTouchMode = TOUCH_MODE_FLING;

        mWidget.post(this);
    }

    public void endFling() {
        mTouchMode = TOUCH_MODE_REST;

        if (mWidget != null) {
            mWidget.removeCallbacks(this);
//                    mWidget.moveCursorToVisibleOffset();

        }
    }

    public void run() {
        switch (mTouchMode) {
            default:
                return;

            case TOUCH_MODE_FLING: {

                final Scroller scroller = mScroller;
                boolean more = scroller.computeScrollOffset();

//                int x = scroller.getCurrX();
                int x = mWidget.getScrollX();
                int y = scroller.getCurrY();

                Layout layout = mWidget.getLayout();
                if (layout == null)
                    break;

                int delta = mLastFlingY - y;
                if (more && delta != 0 && y < maxY) {
                    Touch.scrollTo(mWidget, layout, x, y);
                    mWidget.invalidate();
                    mLastFlingY = y;
                    mWidget.post(this);
                } else {
                    endFling();
                }
                break;
            }
        }

    }
}
