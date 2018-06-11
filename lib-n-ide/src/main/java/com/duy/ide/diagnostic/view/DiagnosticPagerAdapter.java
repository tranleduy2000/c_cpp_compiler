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

package com.duy.ide.diagnostic.view;

import android.support.annotation.IdRes;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.annotation.StringRes;
import android.support.v4.app.Fragment;
import android.view.View;
import android.view.ViewGroup;

import com.duy.ide.editor.editor.R;


class DiagnosticPagerAdapter extends android.support.v4.view.PagerAdapter {
    private Fragment fragment;

    public DiagnosticPagerAdapter(Fragment fragment) {
        this.fragment = fragment;
    }

    @NonNull
    @Override
    public Object instantiateItem(@NonNull ViewGroup collection, int position) {
        return fragment.getView().findViewById(Page.values()[position].layoutId);
    }

    @Nullable
    @Override
    public CharSequence getPageTitle(int position) {
        return fragment.getString(Page.values()[position].titleId);
    }

    @Override
    public int getCount() {
        return Page.values().length;
    }

    @Override
    public boolean isViewFromObject(@NonNull View arg0, @NonNull Object arg1) {
        return arg0 == arg1;
    }

    enum Page {
        DIAGNOSTIC(R.id.diagnostic_list_view, R.string.diagnostic),
        COMPILER_LOG(R.id.compiler_output_container, R.string.compiler_output);

        private final int layoutId;
        private final int titleId;

        Page(@IdRes int layoutId, @StringRes int titleId) {
            this.layoutId = layoutId;
            this.titleId = titleId;
        }
    }

}