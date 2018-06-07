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

/**
 * Created by nakama on 2016/04/01.
 */

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.os.Parcelable;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentTransaction;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;

import com.duy.ide.editor.editor.R;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * ViewPager adapter that handles data and fragments' states.
 * If there are large number of pages or need to save fragments' and items' state, it is recommended to use this adapter.
 * Subclasses of this class just need to implement getFragment() and return a fragment associated with position and item.
 * Be careful: Item type T must be implementation of Serializable or Parcelable.
 *
 * @param <T> item type : Serializable or Parcelable
 */
public abstract class ArrayFragmentStatePagerAdapter<T> extends ArrayPagerAdapter<T> {
    private static final String TAG = "StatePagerAdapter";
    private static final boolean DEBUG = false;

    private final FragmentManager mFragmentManager;
    private FragmentTransaction mCurTransaction = null;
    private ItemsStateHandler mItemsStateHandler;
    private ArrayList<Fragment.SavedState> mSavedState = new ArrayList<>();
    private ArrayList<Fragment> mFragments = new ArrayList<>();
    private Fragment mCurrentPrimaryItem = null;
    private boolean mTypeChecked = false;

    public ArrayFragmentStatePagerAdapter(FragmentManager fm) {
        super();
        mFragmentManager = fm;
    }

    public ArrayFragmentStatePagerAdapter(FragmentManager fm, T... items) {
        super(new ArrayList(Arrays.asList(items)));
        mFragmentManager = fm;
    }

    public ArrayFragmentStatePagerAdapter(FragmentManager fm, List<T> items) {
        super(items);
        checkItemTypeIsSupported();
        mFragmentManager = fm;
    }

    /**
     * Return the Fragment associated with a specified position and item.
     *
     * @param item     item of this page.
     * @param position position of this page.
     * @return fragment that represent this page.
     */
    public abstract Fragment getFragment(T item, int position);

    public Fragment getCurrentFragment() {
        return mCurrentPrimaryItem;
    }

    @Nullable
    public Fragment getExistingFragment(int position) {
        if (position >= 0 && position < mFragments.size()) {
            return mFragments.get(position);
        }
        return null;
    }


    /**
     * {@inheritDoc}
     */
    @Override
    public void add(T item) {
        super.add(item);
        checkItemTypeIsSupported();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void add(int index, T item) {
        mFragments.add(index, null);
        mSavedState.add(index, null);
        super.add(index, item);
        checkItemTypeIsSupported();
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public void addAll(T... items) {
        super.addAll(items);
        checkItemTypeIsSupported();
    }

    /**
     * {@inheritDoc}
     */
    @SuppressLint("CommitTransaction")
    @Override
    public void clear() {
        if (mCurTransaction == null) {
            mCurTransaction = mFragmentManager.beginTransaction();
        }
        mSavedState.clear();
        mFragments.clear();
        super.clear();
    }

    /**
     * {@inheritDoc}
     */
    @SuppressLint("CommitTransaction")
    @Override
    public void remove(int position) {
        if (mCurTransaction == null) {
            mCurTransaction = mFragmentManager.beginTransaction();
        }
        while (mSavedState.size() <= position) {
            mSavedState.add(null);
        }
        while (mFragments.size() <= position) {
            mFragments.add(null);
        }
        mSavedState.remove(position);
        Fragment f = mFragments.get(position);
        mFragments.remove(position);
        if (f != null) {
            if (DEBUG) Log.d(TAG, "Removing item #" + position + ": f=" + f
                    + " v=" + f.getView());
            mCurTransaction.remove(f);
        }
        super.remove(position);
    }

    @Override
    public void startUpdate(ViewGroup container) {
    }

    @SuppressLint("CommitTransaction")
    @Override
    public Object instantiateItem(ViewGroup container, int position) {
        // If we already have this item instantiated, there is nothing
        // to do.  This can happen when we are restoring the entire pager
        // from its saved state, where the fragment manager has already
        // taken care of restoring the fragments we previously had instantiated.
        if (mFragments.size() > position) {
            Fragment f = mFragments.get(position);
            if (f != null) {
                return super.instantiateItem(container, position);
            }
        }

        if (mCurTransaction == null) {
            mCurTransaction = mFragmentManager.beginTransaction();
        }

        Fragment fragment = getFragment(getItem(position), position);
        if (DEBUG) Log.d(TAG, "Adding item #" + position + ": f=" + fragment);
        if (mSavedState.size() > position) {
            Fragment.SavedState fss = mSavedState.get(position);
            if (fss != null) {
                fragment.setInitialSavedState(fss);
            }
        }
        while (mFragments.size() <= position) {
            mFragments.add(null);
        }
        fragment.setMenuVisibility(false);
        fragment.setUserVisibleHint(false);
        mFragments.set(position, fragment);
        mCurTransaction.add(container.getId(), fragment);

        return super.instantiateItem(container, position);
    }

    @SuppressLint("CommitTransaction")
    @Override
    public void destroyItem(ViewGroup container, int position, Object object) {

        if (mCurTransaction == null) {
            mCurTransaction = mFragmentManager.beginTransaction();
        }
        while (mSavedState.size() <= position) {
            mSavedState.add(null);
        }
        while (mFragments.size() <= position) {
            mFragments.add(null);
        }
        Fragment f = findFragmentByItem(object);
        if (f != null) {
            // if mFragments does not contains f,
            // there is no need to save f because it is removed by user.
            if (mFragments.contains(f)) {
                // fragment is destroyed temporary.
                mSavedState.set(position, f.isAdded()
                        ? mFragmentManager.saveFragmentInstanceState(f) : null);
                mFragments.set(position, null);
            }
            mCurTransaction.remove(f);
        }
    }

    @Override
    public void setPrimaryItem(ViewGroup container, int position, Object object) {
        if (mFragments.size() <= position) return;
        Fragment fragment = mFragments.get(position);
        if (fragment != mCurrentPrimaryItem) {
            if (mCurrentPrimaryItem != null) {
                mCurrentPrimaryItem.setMenuVisibility(false);
                mCurrentPrimaryItem.setUserVisibleHint(false);
            }
            if (fragment != null) {
                fragment.setMenuVisibility(true);
                fragment.setUserVisibleHint(true);
            }
            mCurrentPrimaryItem = fragment;
        }
    }

    @Override
    public void finishUpdate(ViewGroup container) {
        setTags(getItemsWithId());
        if (mCurTransaction != null) {
            mCurTransaction.commitAllowingStateLoss();
            mCurTransaction = null;
            mFragmentManager.executePendingTransactions();
        }
    }

    @Override
    public boolean isViewFromObject(View view, Object object) {
        return object == view.getTag(R.id.avpa_view_tag_key);
    }

    @Override
    public Parcelable saveState() {
        Bundle state = new Bundle();
        if (mSavedState.size() > 0) {
            Fragment.SavedState[] fss = new Fragment.SavedState[mSavedState.size()];
            mSavedState.toArray(fss);
            state.putParcelableArray("states", fss);
        }
        for (int i = 0; i < mFragments.size(); i++) {
            Fragment f = mFragments.get(i);
            if (f != null && f.isAdded()) {
                String key = "f" + i;
                mFragmentManager.putFragment(state, key, f);
            }
        }
        if (mItemsStateHandler != null) {
            mItemsStateHandler.saveItems(state, getItems());
        }
        return state;
    }

    @Override
    public void restoreState(Parcelable state, ClassLoader loader) {
        if (state != null) {
            Bundle bundle = (Bundle) state;
            bundle.setClassLoader(loader);
            Parcelable[] fss = bundle.getParcelableArray("states");
            mSavedState.clear();
            mFragments.clear();
            if (fss != null) {
                for (Parcelable fs : fss) {
                    mSavedState.add((Fragment.SavedState) fs);
                }
            }
            if (mItemsStateHandler != null) {
                setItems(mItemsStateHandler.restoreItems(bundle));
            }
            Iterable<String> keys = bundle.keySet();
            for (String key : keys) {
                if (key.startsWith("f")) {
                    int index = Integer.parseInt(key.substring(1));
                    Fragment f = mFragmentManager.getFragment(bundle, key);
                    if (f != null) {
                        while (mFragments.size() <= index) {
                            mFragments.add(null);
                        }
                        f.setMenuVisibility(false);
                        mFragments.set(index, f);
                    } else {
                        Log.w(TAG, "Bad fragment at key " + key);
                    }
                }
            }
        }
    }

    private void checkItemTypeIsSupported() {
        if (!mTypeChecked) {
            if (getItems() != null && getItems().size() > 0) {
                Object item = getItems().get(0);
                mTypeChecked = true;
                if (getItems() instanceof ArrayList) {
                    if (item instanceof Serializable) {
                        mItemsStateHandler = new SerializableItemStateHandler();
                    } else if (item instanceof Parcelable) {
                        mItemsStateHandler = new ParcelableItemStateHandler();
                    } else {
                        throw new IllegalArgumentException("Item of " + TAG + " must be subclass of Serializable or Parcelable.");
                    }
                } else {
                    throw new IllegalArgumentException("Items of " + TAG + " list must be subclass of ArrayList.");
                }
            }
        }
    }

    private Fragment findFragmentByItem(Object item) {
        List<Fragment> fragments = mFragmentManager.getFragments();
        if (fragments == null) return null;
        for (Fragment fragment : fragments) {
            if (fragment != null) {
                View view = fragment.getView();
                if (view != null && item == view.getTag(R.id.avpa_view_tag_key)) {
                    return fragment;
                }
            }
        }
        return null;
    }

    private void setTags(List<IdentifiedItem<T>> items) {
        final int max = Math.min(items.size(), mFragments.size());
        for (int i = 0; i < max; i++) {
            Fragment fragment = mFragments.get(i);
            if (fragment != null && fragment.getView() != null) {
                fragment.getView().setTag(R.id.avpa_view_tag_key, items.get(i));
            }
        }
    }

    interface ItemsStateHandler<T> {
        String SAVE_KEY = "apa_items_key";

        void saveItems(Bundle state, ArrayList<T> items);

        ArrayList<T> restoreItems(Bundle bundle);
    }

    static class SerializableItemStateHandler implements ItemsStateHandler<Serializable> {

        @Override
        public void saveItems(Bundle state, ArrayList<Serializable> items) {
            state.putSerializable(SAVE_KEY, items);
        }

        @Override
        public ArrayList<Serializable> restoreItems(Bundle bundle) {
            return (ArrayList<Serializable>) bundle.getSerializable(SAVE_KEY);
        }
    }

    static class ParcelableItemStateHandler implements ItemsStateHandler<Parcelable> {

        @Override
        public void saveItems(Bundle state, ArrayList<Parcelable> items) {
            state.putParcelableArrayList(SAVE_KEY, items);
        }

        @Override
        public ArrayList<Parcelable> restoreItems(Bundle bundle) {
            return bundle.getParcelableArrayList(SAVE_KEY);
        }
    }
}
