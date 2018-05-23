package com.duy.ccppcompiler.diagnostic.ui;

import android.support.annotation.IdRes;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.annotation.StringRes;
import android.support.v4.app.Fragment;
import android.view.View;
import android.view.ViewGroup;

import com.duy.ccppcompiler.R;

class PagerAdapter extends android.support.v4.view.PagerAdapter {
    private Fragment fragment;

    public PagerAdapter(Fragment fragment) {
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