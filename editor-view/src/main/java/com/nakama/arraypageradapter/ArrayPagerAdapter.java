package com.nakama.arraypageradapter;

import android.support.v4.view.PagerAdapter;
import android.util.SparseBooleanArray;
import android.view.ViewGroup;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * ViewPager adapter that contain items passed from outside.
 * This is base class of other ArrayPagerAdapters in this library.
 *
 * @param <T> item type
 */
public abstract class ArrayPagerAdapter<T> extends PagerAdapter {
    private ArrayList<IdentifiedItem<T>> items;
    private final Object lock = new Object();
    private final IdentifiedItemFactory<T> identifiedItemFactory;

    public ArrayPagerAdapter() {
        this(new ArrayList<T>());
    }

    @SafeVarargs
    public ArrayPagerAdapter(T... items) {
        this(new ArrayList<>(Arrays.asList(items)));
    }

    public ArrayPagerAdapter(List<T> items) {
        identifiedItemFactory = new IdentifiedItemFactory<>(0);
        this.items = identifiedItemFactory.createList(items);
    }


    @Override
    public Object instantiateItem(ViewGroup container, int position) {
        return items.get(position);
    }

    /**
     * Adds the specified item at the end of the array.
     *
     * @param item The item to add at the end of the array.
     */
    public void add(T item) {
        synchronized (lock) {
            items.add(identifiedItemFactory.create(item));
        }
        notifyDataSetChanged();
    }

    /**
     * Adds the specified item at the specified position of the array.
     *
     * @param item The item to add at the specified position of the array.
     */
    public void add(int index, T item) {
        synchronized (lock) {
            items.add(index, identifiedItemFactory.create(item));
        }
        itemPositionChangeChecked = new SparseBooleanArray(this.items.size());
        notifyDataSetChanged();
    }

    /**
     * Adds the specified items at the end of the array.
     *
     * @param items The items to add at the end of the array.
     */
    public void addAll(T... items) {
        synchronized (lock) {
            this.items.addAll(identifiedItemFactory.createList(items));
        }
        itemPositionChangeChecked = new SparseBooleanArray(this.items.size());
        notifyDataSetChanged();
    }

    /**
     * Adds the specified items at the end of the array.
     *
     * @param items The items to add at the end of the array.
     */
    public void addAll(List<T> items) {
        synchronized (lock) {
            this.items.addAll(identifiedItemFactory.createList(items));
        }
        itemPositionChangeChecked = new SparseBooleanArray(this.items.size());
        notifyDataSetChanged();
    }

    /**
     * Removes the specified item from the array.
     *
     * @param position The item position to be removed
     * @return true if this items was modified by this operation, false otherwise.
     * @throws IndexOutOfBoundsException if position < 0 || position >= getCount()
     */
    public void remove(int position) throws IndexOutOfBoundsException {
        synchronized (lock) {
            items.remove(position);
        }
        itemPositionChangeChecked = new SparseBooleanArray(items.size());
        notifyDataSetChanged();
    }

    /**
     * Remove all elements from the list.
     */
    public void clear() {
        synchronized (lock) {
            items.clear();
        }
        itemPositionChangeChecked = new SparseBooleanArray(items.size());
        notifyDataSetChanged();
    }

    /**
     * Get the data item associated with the specified position in the data set.
     *
     * @param position Position of the item whose data we want within the adapter's
     *                 data set.
     * @return The data at the specified position.
     */
    public T getItem(int position) {
        return items.get(position).item;
    }


    /**
     * Returns the position of the specified item in the array.
     *
     * @param item The item to retrieve the position of.
     * @return The position of the specified item.
     */
    public int getPosition(T item) {
        for (int i = 0; i < items.size(); i++) {
            if (items.get(i).item == item) {
                return i;
            }
        }
        return -1;
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getItemPosition(Object item) {
        if (!items.contains(item)) {
            return POSITION_NONE;
        } else if (itemPositionChangeChecked.size() != items.size()) {
            int newPos = items.indexOf(item);
            int ret = itemPositionChangeChecked.get(newPos) ? POSITION_UNCHANGED : newPos;
            itemPositionChangeChecked.put(newPos, true);
            return ret;
        } else {
            return POSITION_UNCHANGED;
        }
    }

    /**
     * {@inheritDoc}
     */
    @Override
    public int getCount() {
        return items.size();
    }

    IdentifiedItem<T> getItemWithId(int position) {
        return items.get(position);
    }

    ArrayList<IdentifiedItem<T>> getItemsWithId() {
        return items;
    }

    ArrayList<T> getItems() {
        ArrayList<T> list = new ArrayList<>();
        for (IdentifiedItem<T> item : items) {
            list.add(item.item);
        }
        return list;
    }

    void setItems(List<T> items) {
        this.items = identifiedItemFactory.createList(items);
        notifyDataSetChanged();
    }

    private SparseBooleanArray itemPositionChangeChecked = new SparseBooleanArray();

    static class IdentifiedItemFactory<T> {
        private long lastId;

        IdentifiedItemFactory(long firstId) {
            lastId = firstId;
        }

        IdentifiedItem<T> create(T item) {
            return new IdentifiedItem(lastId++, item);
        }

        ArrayList<IdentifiedItem<T>> createList(List<T> items) {
            ArrayList<IdentifiedItem<T>> list = new ArrayList();
            for (T item : items) {
                list.add(create(item));
            }
            return list;
        }

        @SafeVarargs
        final ArrayList createList(T... items) {
            return createList(new ArrayList<>(Arrays.asList(items)));
        }
    }

    static class IdentifiedItem<T> {
        long id;
        T item;

        public IdentifiedItem(long id, T item) {
            this.id = id;
            this.item = item;
        }

        @Override
        public String toString() {
            return "IdentifiedItem{" +
                    "id=" + id +
                    ", item=" + item +
                    '}';
        }
    }

}

