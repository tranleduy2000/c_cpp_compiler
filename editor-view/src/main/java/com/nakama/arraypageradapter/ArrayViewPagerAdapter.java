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

package com.nakama.arraypageradapter;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.duy.ide.editor.editor.R;

import java.util.List;

/**
 * ViewPager adapter that handles View and items.
 * If you want to use View to draw pages, this adapter is useful.
 * Subclasses of this class just need to implement getView() and return a view associated with position and items.
 *
 * @param <T> item type
 */
public abstract class ArrayViewPagerAdapter<T> extends ArrayPagerAdapter<T> {

    public ArrayViewPagerAdapter() {
        super();
    }

    public ArrayViewPagerAdapter(T... items) {
        super(items);
    }

    public ArrayViewPagerAdapter(List<T> items) {
        super(items);
    }

    /**
     * Return the View associated with a specified position and item.
     *
     * @param inflater  inflater
     * @param container ViewGroup that will be container of the view.
     * @param item      item of this page.
     * @param position  position of this page.
     * @return view of this page.
     */
    public abstract View getView(LayoutInflater inflater, ViewGroup container, T item, int position);

    @Override
    public Object instantiateItem(ViewGroup container, int position) {
        T item = getItem(position);
        View view = getView(LayoutInflater.from(container.getContext()), container, item, position);
        view.setTag(R.id.avpa_view_tag_key, getItemWithId(position));
        container.addView(view);
        return super.instantiateItem(container, position);
    }

    @Override
    public boolean isViewFromObject(View view, Object item) {
        return item.equals(view.getTag(R.id.avpa_view_tag_key));
    }

    @Override
    public void destroyItem(ViewGroup container, int position, Object item) {
        container.removeView(findViewWithTagInViewPager(container, item));
    }

    private View findViewWithTagInViewPager(ViewGroup container, Object item) {
        for (int i = 0; i < container.getChildCount(); i++) {
            View view = container.getChildAt(i);
            Object tag = view.getTag(R.id.avpa_view_tag_key);
            if (item.equals(tag)) {
                return view;
            }
        }
        throw new NullPointerException("view's tag is not found for some reason.");
    }
}
