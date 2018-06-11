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

import android.support.v7.widget.RecyclerView;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.duy.ide.editor.editor.R;
import com.duy.file.explorer.io.JecFile;
import com.jecelyin.common.listeners.OnItemClickListener;

import java.util.ArrayList;
import java.util.Collections;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class PathButtonAdapter extends RecyclerView.Adapter<PathButtonAdapter.ViewHolder> {
    private ArrayList<JecFile> pathList;
    private OnItemClickListener onItemClickListener;

    public JecFile getItem(int position) {
        return pathList.get(position);
    }

    public void setOnItemClickListener(OnItemClickListener onItemClickListener) {
        this.onItemClickListener = onItemClickListener;
    }

    @Override
    public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.path_button_item, parent, false);

        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(ViewHolder holder, final int position) {
        JecFile path = pathList.get(position);
        String name = path.getName();
        if ("/".equals(name) || TextUtils.isEmpty(name))
            name = holder.textView.getContext().getString(R.string.root_path);
        holder.textView.setText(name);
        holder.itemView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (onItemClickListener != null) {
                    onItemClickListener.onItemClick(position, v);
                }
            }
        });
    }

    @Override
    public int getItemCount() {
        return pathList == null ? 0 : pathList.size();
    }

    public void setPath(JecFile path) {
        if (pathList == null)
            pathList = new ArrayList<>();
        else
            pathList.clear();

        for (; path != null; ) {
            pathList.add(path);
            path = path.getParentFile();
        }

        Collections.reverse(pathList);
        notifyDataSetChanged();
    }

    static class ViewHolder extends RecyclerView.ViewHolder {
        TextView textView;

        public ViewHolder(View itemView) {
            super(itemView);

            textView = itemView.findViewById(R.id.textView);
        }
    }
}
