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

package com.duy.file.explorer.adapter;

import android.content.Context;
import android.content.res.Resources;
import android.databinding.DataBindingUtil;
import android.graphics.Color;
import android.graphics.PorterDuff;
import android.graphics.drawable.Drawable;
import android.support.annotation.NonNull;
import android.support.v4.content.ContextCompat;
import android.support.v7.widget.RecyclerView;
import android.text.TextUtils;
import android.util.SparseIntArray;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.duy.file.explorer.io.JecFile;
import com.duy.file.explorer.model.FileItemModel;
import com.duy.file.explorer.util.MimeTypes;
import com.duy.file.explorer.util.OnCheckedChangeListener;
import com.duy.ide.editor.editor.R;
import com.duy.ide.editor.editor.databinding.FileListItemBinding;
import com.jecelyin.common.adapter.BindingViewHolder;
import com.jecelyin.common.listeners.OnItemClickListener;
import com.jecelyin.common.utils.StringUtils;
import com.simplecityapps.recyclerview_fastscroll.views.FastScrollRecyclerView;

import java.text.SimpleDateFormat;
import java.util.Calendar;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class FileListItemAdapter extends RecyclerView.Adapter<BindingViewHolder<FileListItemBinding>> implements FastScrollRecyclerView.SectionedAdapter {
    private final String year;
    private final SparseIntArray checkedArray;
    private JecFile[] data;
    private OnCheckedChangeListener onCheckedChangeListener;
    private OnItemClickListener onItemClickListener;
    private JecFile[] mOriginalValues;
    private int itemCount;
    private Context context;

    public FileListItemAdapter(Context context) {
        this.context = context;
        year = String.valueOf(Calendar.getInstance().get(Calendar.YEAR));
        checkedArray = new SparseIntArray();
    }

    public void setData(JecFile[] data) {
        this.data = data;
        itemCount = data.length;
        mOriginalValues = data.clone();
        notifyDataSetChanged();
    }

    public void filter(CharSequence filterText) {
        if (mOriginalValues == null)
            return;

        if (TextUtils.isEmpty(filterText)) {
            data = mOriginalValues;
            itemCount = mOriginalValues.length;
            notifyDataSetChanged();
            return;
        }

        data = new JecFile[mOriginalValues.length];

        filterText = filterText.toString().toLowerCase();
        int index = 0;
        for (JecFile path : mOriginalValues) {
            if (path.getName().toLowerCase().contains(filterText)) {
                data[index++] = path;
            }
        }
        itemCount = index;
        notifyDataSetChanged();
    }

    @NonNull
    @Override
    public String getSectionName(int position) {
        JecFile file = getItem(position);
        char c = file.getName().charAt(0);

        if ((c >= '0' && c <= '9')
                || (c >= 'a' && c <= 'z')
                || (c >= 'A' && c <= 'Z')
                ) {
            return String.valueOf(c);
        }

        return "#";
    }

    public JecFile getItem(int position) {
        return data[position];
    }

    public void setOnCheckedChangeListener(OnCheckedChangeListener onCheckedChangeListener) {
        this.onCheckedChangeListener = onCheckedChangeListener;
    }

    public void setOnItemClickListener(OnItemClickListener onItemClickListener) {
        this.onItemClickListener = onItemClickListener;
    }

    @Override
    public int getItemCount() {
        return itemCount;
    }

    @Override
    public BindingViewHolder<FileListItemBinding> onCreateViewHolder(ViewGroup parent, int viewType) {
        FileListItemBinding binding = DataBindingUtil.inflate(LayoutInflater.from(parent.getContext()), R.layout.file_list_item, parent, false);

        return new BindingViewHolder<>(binding);
    }

    @Override
    public void onBindViewHolder(final BindingViewHolder<FileListItemBinding> holder, final int position) {
        JecFile path = data[position];

        MimeTypes mimeTypes = MimeTypes.getInstance();
        Resources res = holder.itemView.getResources();
        int color, icon;
        if (path.isDirectory()) {
            color = R.color.type_folder;
            icon = R.drawable.file_type_folder;

        } else if (mimeTypes.isImageFile(path)) {
            color = R.color.type_media;
            icon = R.drawable.file_type_image;

        } else if (mimeTypes.isVideoFile(path)) {
            color = R.color.type_media;
            icon = R.drawable.file_type_video;

        } else if (mimeTypes.isAudioFile(path)) {
            color = R.color.type_media;
            icon = R.drawable.file_type_audio;

        } else if (mimeTypes.isAPKFile(path)) {
            color = R.color.type_apk;
            icon = R.drawable.file_type_apk;

        } else if (mimeTypes.isArchive(path)) {
            color = R.color.type_archive;
            icon = R.drawable.file_type_archive;

        } else if (mimeTypes.isCodeFile(path)) {
            color = R.color.type_code;
            icon = R.drawable.file_type_code;

        } else {
            color = R.color.type_code;
            String extension = path.getExtension();
            switch (extension.toLowerCase()) {
                default:
                    if (mimeTypes.isTextFile(path)) {
                        color = R.color.type_text;
                        icon = R.drawable.file_type_text;
                    } else {
                        color = R.color.type_file;
                        icon = R.drawable.file_type_file;
                    }
                    break;
            }
        }

        final FileListItemBinding binding = holder.getBinding();
        binding.iconImageView.setDefaultImageResource(icon);
        binding.iconImageView.setDefaultBackgroundColor(res.getColor(color));

        FileItemModel item = new FileItemModel();
        item.setName(path.getName());
        item.setExt(icon == 0 && color == R.color.type_file ? path.getExtension() : "");
        item.setDate(getDate(path.lastModified()));
        item.setSecondLine(path.isFile() ? StringUtils.formatSize(path.length()) : "");
        binding.setItem(item);

        binding.iconImageView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                toggleChecked(position, binding);
            }
        });
        holder.itemView.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View v) {
                toggleChecked(position, binding);
                return true;
            }
        });
        holder.itemView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (checkedArray != null && checkedArray.size() > 0) {
                    toggleChecked(position, binding);
                    return;
                }
                if (onItemClickListener != null)
                    onItemClickListener.onItemClick(position, v);
            }
        });

        boolean isChecked = isChecked(position);
        setViewCheckedStatus(isChecked, binding);
    }

    private void setViewCheckedStatus(boolean isChecked, FileListItemBinding binding) {
        binding.iconImageView.setChecked(isChecked);

        if (!isChecked) {
            binding.getRoot().setSelected(false);
            binding.extTextView.setVisibility(View.VISIBLE);
        } else {
            binding.getRoot().setSelected(true);
            binding.extTextView.setVisibility(View.INVISIBLE);
        }
    }

    private void toggleChecked(int position, FileListItemBinding binding) {
        boolean isChecked = isChecked(position);
        if (isChecked) {
            checkedArray.delete(position);
        } else {
            checkedArray.put(position, 1);
        }

        setViewCheckedStatus(!isChecked, binding);

        if (onCheckedChangeListener != null) {
            onCheckedChangeListener.onCheckedChanged(getItem(position), position, !isChecked);
            onCheckedChangeListener.onCheckedChanged(checkedArray.size());
        }

    }

    public boolean isChecked(int position) {
        return checkedArray.get(position) == 1;
    }

    private String getDate(long f) {
        SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd HH:mm");
        String date = (sdf.format(f));
        if (date.substring(0, 4).equals(year))
            date = date.substring(5);
        return date;
    }

    public void checkAll(boolean checked) {
        if (checked) {
            int count = getItemCount();
            for (int i = 0; i < count; i++) {
                checkedArray.put(i, 1);
            }
        } else {
            checkedArray.clear();
        }

        if (onCheckedChangeListener != null) {
            onCheckedChangeListener.onCheckedChanged(checkedArray.size());
        }

        notifyDataSetChanged();
    }

}
