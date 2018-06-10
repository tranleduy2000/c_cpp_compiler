package com.duy.ide.editor.internal.suggestion;

import android.content.Context;
import android.support.annotation.LayoutRes;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.annotation.UiThread;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.Filter;
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

    private int mResourceID;
    @Nullable
    private OnSuggestItemClickListener mListener;

    private Filter mSuggestionFilter = new Filter() {
        @Override
        public CharSequence convertResultToString(Object value) {
            if (value == null) {
                return "";
            }
            return ((SuggestItem) value).getInsertText();
        }

        @Override
        protected FilterResults performFiltering(CharSequence constraint) {
            FilterResults filterResults = new FilterResults();
            mFilteredData.clear();
            if (constraint != null) {
                for (SuggestItem item : mOriginData) {
//                    if (item.compareTo(constraint.toString()) == 0) {
                    mFilteredData.add(item);
//                    }
                }
                filterResults.values = mFilteredData;
                filterResults.count = mFilteredData.size();
            }
            return filterResults;
        }

        @Override
        @SuppressWarnings("unchecked")
        @UiThread
        protected void publishResults(CharSequence constraint, FilterResults results) {
            ArrayList<SuggestItem> filteredList = (ArrayList<SuggestItem>) results.values;
            clear();
            if (filteredList != null && filteredList.size() > 0) {
                addAll(filteredList);
            }
            notifyDataSetChanged();
        }
    };


    public SuggestionAdapter(@NonNull Context context, @LayoutRes int resource,
                             @NonNull List<SuggestItem> objects) {
        super(context, resource, objects);
        mInflater = LayoutInflater.from(context);
        mContext = context;
        mOriginData = new ArrayList<>(objects);
        mFilteredData = new ArrayList<>();
        mResourceID = resource;
    }

    @NonNull
    @Override
    public View getView(int position, @Nullable View convertView, @NonNull ViewGroup parent) {
        if (convertView == null) {
            convertView = mInflater.inflate(mResourceID, null);
        }

        final SuggestItem item = getItem(position);
        TextView txtName = convertView.findViewById(R.id.txt_name);
        TextView txtReturnType = convertView.findViewById(R.id.txt_return_type);
        TextView txtHeader = convertView.findViewById(R.id.txt_suggest_header);

        if (item != null) {
            txtName.setText(ensureNotNull(item.getName()));
            txtReturnType.setText(ensureNotNull(item.getReturnType()));
            txtHeader.setText(ensureNotNull(String.valueOf(item.getTypeHeader())));
        }
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
        notifyDataSetChanged();
    }

    @UiThread
    public void addData(@NonNull Collection<? extends SuggestItem> collection) {
        addAll(collection);
        mOriginData.addAll(collection);
        notifyDataSetChanged();
    }

    @NonNull
    @Override
    public Filter getFilter() {
        return mSuggestionFilter;
    }

    public List<SuggestItem> getAllItems() {
        return mOriginData;
    }

    public void setListener(OnSuggestItemClickListener listener) {
        this.mListener = listener;
    }

}
