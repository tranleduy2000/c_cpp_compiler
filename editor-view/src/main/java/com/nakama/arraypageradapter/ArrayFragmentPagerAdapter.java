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

import android.annotation.SuppressLint;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;

import java.util.List;

/**
 * ViewPager adapter that handles Fragments and items.
 * You can use this adapter if there is a few number of pages and no need to save states (or implement by yourself).
 * Subclasses of this class just need to implement getFragment() and return a fragment associated with position and item.
 * @param <T> item type
 */
public abstract class ArrayFragmentPagerAdapter<T> extends ArrayPagerAdapter<T> {
    private static final String TAG = "FragmentPagerAdapter";
    private static final boolean DEBUG = false;

    private FragmentManager fragmentManager;
    private FragmentTransaction currentTransaction = null;
    private Fragment currentPrimaryItem = null;


    public ArrayFragmentPagerAdapter(FragmentManager fm) {
        super();
    }

    public ArrayFragmentPagerAdapter(FragmentManager fm, T... items) {
        super(items);
    }

    public ArrayFragmentPagerAdapter(FragmentManager fm, List<T> items) {
        super(items);
        fragmentManager = fm;
    }

    /**
     * Return the Fragment associated with a specified position and item.
     * @param item item of this page.
     * @param position position of this page.
     * @return fragment that represent this page.
     */
    public abstract Fragment getFragment(T item, int position);

    @SuppressLint("CommitTransaction")
    @Override
    public Object instantiateItem(ViewGroup container, int position) {
        if (currentTransaction == null) {
            currentTransaction = fragmentManager.beginTransaction();
        }

        // Do we already have this fragment?
        final IdentifiedItem<T> item = getItemWithId(position);

        // Do we already have this fragment?
        String name = makeFragmentName(item.id);
        Fragment fragment = fragmentManager.findFragmentByTag(name);
        if (fragment != null) {
            if (DEBUG) Log.d(TAG, "Attaching item #" + item + ": f=" + fragment);
            currentTransaction.attach(fragment);
        } else {
            fragment = getFragment(item.item, position);
            if (DEBUG) Log.d(TAG, "Adding item #" + item + ": f=" + fragment);
            currentTransaction.add(container.getId(), fragment,
                    makeFragmentName(item.id));
        }
        if (fragment != currentPrimaryItem) {
            fragment.setMenuVisibility(false);
            fragment.setUserVisibleHint(false);
        }

        return super.instantiateItem(container, position);
    }


    @SuppressLint("CommitTransaction")
    @Override
    public void destroyItem(ViewGroup container, int position, Object object) {
        if (currentTransaction == null) {
            currentTransaction = fragmentManager.beginTransaction();
        }
        String name = makeFragmentName(((IdentifiedItem) object).id);
        Fragment f = fragmentManager.findFragmentByTag(name);
        if (f != null) {
            if (DEBUG) Log.d(TAG, "Detaching item #" + getItemId(position) + ": f=" + object
                    + " v=" + f.getView());
            currentTransaction.detach(f);
        }
    }

    @SuppressLint("CommitTransaction")
    @Override
    public void remove(int position) throws IndexOutOfBoundsException {
        if (currentTransaction == null) {
            currentTransaction = fragmentManager.beginTransaction();
        }
        String name = makeFragmentName(getItemId(position));
        Fragment f = fragmentManager.findFragmentByTag(name);
        if (f != null) {
            if (DEBUG) Log.d(TAG, "Removing item #" + getItemId(position) + ": f=" + f
                    + " v=" + f.getView());
            currentTransaction.remove(f);
        }
        super.remove(position);
    }

    @SuppressLint("CommitTransaction")
    @Override
    public void clear() {
        if (currentTransaction == null) {
            currentTransaction = fragmentManager.beginTransaction();
        }
        List<Fragment> fragments = fragmentManager.getFragments();
        if (fragments != null) {
            for (Fragment fragment : fragments) {
                if (fragment != null) {
                    currentTransaction.remove(fragment);
                }
            }
        }
        super.clear();
    }

    @Override
    public void setPrimaryItem(ViewGroup container, int position, Object object) {
        Fragment fragment = fragmentManager.findFragmentByTag(makeFragmentName(getItemId(position)));
        if (fragment != currentPrimaryItem) {
            if (currentPrimaryItem != null) {
                currentPrimaryItem.setMenuVisibility(false);
                currentPrimaryItem.setUserVisibleHint(false);
            }
            if (fragment != null) {
                fragment.setMenuVisibility(true);
                fragment.setUserVisibleHint(true);
            }
            currentPrimaryItem = fragment;
        }
    }

    @Override
    public void finishUpdate(ViewGroup container) {
        if (currentTransaction != null) {
            currentTransaction.commitAllowingStateLoss();
            currentTransaction = null;
            fragmentManager.executePendingTransactions();
        }
    }

    @Override
    public boolean isViewFromObject(View view, Object object) {
        for (Fragment fragment : fragmentManager.getFragments()) {
            if (fragment != null) {
                View v = fragment.getView();
                if (v != null && v == view && makeFragmentName(((IdentifiedItem) object).id).equals(fragment.getTag())) {
                    return true;
                }
            }
        }
        return false;
    }

    private long getItemId(int position) {
        if (getCount() > position) {
            return getItemWithId(position).id;
        }
        return -1;
    }

    private static String makeFragmentName(long id) {
        return "android:switcher:" + id;
    }

}
