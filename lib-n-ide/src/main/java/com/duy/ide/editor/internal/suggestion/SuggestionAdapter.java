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

package com.duy.ide.editor.internal.suggestion;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.annotation.UiThread;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.TextView;

import com.duy.ide.code.api.SuggestItem;
import com.duy.ide.editor.editor.R;

import java.util.ArrayList;
import java.util.Collection;
import java.util.List;

public class SuggestionAdapter extends ArrayAdapter<SuggestItem> {

    @NonNull
    private Context mContext;
    @NonNull
    private LayoutInflater mInflater;
    @NonNull
    private List<SuggestItem> mOriginData;
    @NonNull
    private List<SuggestItem> mFilteredData;

    @Nullable
    private AdapterView.OnItemClickListener mListener;

    private Integer mTextColor = null;

    public SuggestionAdapter(@NonNull Context context,
                             @NonNull List<SuggestItem> objects) {
        super(context, -1, objects);
        mInflater = LayoutInflater.from(context);
        mContext = context;
        mOriginData = new ArrayList<>(objects);
        mFilteredData = new ArrayList<>();
    }

    @NonNull
    @Override
    public View getView(final int position, @Nullable View convertView, @NonNull ViewGroup parent) {
        if (convertView == null) {
            convertView = mInflater.inflate(R.layout.list_item_suggest_default, null);
        }

        final SuggestItem item = getItem(position);
        TextView txtName = convertView.findViewById(R.id.txt_name);
        TextView txtReturnType = convertView.findViewById(R.id.txt_return_type);
        TextView txtHeader = convertView.findViewById(R.id.txt_suggest_header);

        if (mTextColor != null){
            txtName.setTextColor(mTextColor);
            txtReturnType.setTextColor(mTextColor);
            txtHeader.setTextColor(mTextColor);
        }
        if (item != null) {
            txtName.setText(ensureNotNull(item.getName()));
            txtReturnType.setText(ensureNotNull(item.getReturnType()));
            txtHeader.setText(ensureNotNull(String.valueOf(item.getTypeHeader())));
        }
        convertView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (mListener != null) {
                    mListener.onItemClick(null, v, position, View.NO_ID);
                }
            }
        });
        return convertView;
    }

    private String ensureNotNull(String value) {
        if (value == null) {
            return "";
        }
        return value;
    }

    @UiThread
    public void clearAllData() {
        super.clear();
        mOriginData.clear();
    }

    @UiThread
    public void addData(@NonNull Collection<? extends SuggestItem> collection) {
        addAll(collection);
        mOriginData.addAll(collection);
    }

    public List<SuggestItem> getAllItems() {
        return mOriginData;
    }

    public void setListener(AdapterView.OnItemClickListener listener) {
        this.mListener = listener;
    }

    public void setData(List<SuggestItem> data) {
        clear();
        addAll(data);
    }

    public void setTextColor(int textColor) {
        this.mTextColor = textColor;
    }
}
