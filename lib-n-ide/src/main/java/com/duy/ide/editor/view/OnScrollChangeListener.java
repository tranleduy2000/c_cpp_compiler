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

package com.duy.ide.editor.view;

import android.view.View;

/**
 * Interface definition for a callback to be invoked when the scroll
 * X or Y positions of a view change.
 * <p>
 * <b>Note:</b> Some views handle scrolling independently from View and may
 * have their own separate listeners for scroll-type events. For example,
 * {@link android.widget.ListView ListView} allows clients to register an
 * {@link android.widget.ListView#setOnScrollListener(android.widget.AbsListView.OnScrollListener) AbsListView.OnScrollListener}
 * to listen for changes in list scroll position.
 * <p>
 * see #setOnScrollChangeListener(View.OnScrollChangeListener)
 */
public interface OnScrollChangeListener {
    /**
     * Called when the scroll position of a view changes.
     *
     * @param v          The view whose scroll position has changed.
     * @param scrollX    Current horizontal scroll origin.
     * @param scrollY    Current vertical scroll origin.
     * @param oldScrollX Previous horizontal scroll origin.
     * @param oldScrollY Previous vertical scroll origin.
     */
    void onScrollChange(View v, int scrollX, int scrollY, int oldScrollX, int oldScrollY);
}
