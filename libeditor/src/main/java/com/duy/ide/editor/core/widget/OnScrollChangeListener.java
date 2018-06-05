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

package com.duy.ide.editor.core.widget;

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
