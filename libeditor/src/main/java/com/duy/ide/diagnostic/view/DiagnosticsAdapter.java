/*
 *  Copyright (C)  2018  Duy Tran Le
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.duy.ide.diagnostic.view;

import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.support.annotation.NonNull;
import android.support.annotation.UiThread;
import android.support.v4.content.ContextCompat;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.duy.ide.diagnostic.DiagnosticClickListener;
import com.duy.ide.diagnostic.model.Message;
import com.duy.ide.editor.editor.R;
import com.jecelyin.common.utils.DrawableUtils;

import java.io.File;
import java.util.List;

/**
 * Created by Duy on 28-Apr-18.
 */

public class DiagnosticsAdapter extends RecyclerView.Adapter<DiagnosticsAdapter.ViewHolder> {
    private List<Message> mMessages;
    private Context mContext;
    private DiagnosticClickListener mDiagnosticClickListener;

    DiagnosticsAdapter(List<Message> messages, Context context) {
        this.mMessages = messages;
        this.mContext = context;
    }


    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.list_item_diagnostic, parent, false);
        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull final ViewHolder holder, int position) {
        final Message message = mMessages.get(position);
        if (message.getLineNumber() >= 0) {
            long column = message.getColumn();
            String text = message.getLineNumber() + ":" + (column >= 0 ? column : "");
            holder.txtLineCol.setText(text);
        } else {
            holder.txtLineCol.setText("");
        }

        String path = message.getSourcePath();
        if (path != null) {
            holder.txtFile.setText(new File(path).getName());
        } else {
            holder.txtFile.setText("");
        }

        setIcon(holder, message);

        if (message.getText().isEmpty()) {
            holder.txtMessage.setVisibility(View.GONE);
        } else {
            holder.txtMessage.setVisibility(View.VISIBLE);
            holder.txtMessage.setText(message.getText());
        }

        holder.itemView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (mDiagnosticClickListener != null) {
                    mDiagnosticClickListener.onDiagnosisClick(message, v);
                }
            }
        });
    }

    private void setIcon(ViewHolder holder, Message message) {
        switch (message.getKind()) {
            case ERROR: {
                Drawable drawable = ContextCompat.getDrawable(mContext, R.drawable.baseline_error_24);
                drawable = DrawableUtils.tintDrawable(drawable, Color.RED);
                holder.icon.setImageDrawable(drawable);
                break;
            }
            case WARNING: {
                Drawable drawable = ContextCompat.getDrawable(mContext, R.drawable.baseline_warning_24);
                drawable = DrawableUtils.tintDrawable(drawable, ContextCompat.getColor(mContext, R.color.color_diagnostic_warn));
                holder.icon.setImageDrawable(drawable);
                break;
            }
            default:
                holder.icon.setImageDrawable(null);
                break;
        }
    }

    @Override
    public int getItemCount() {
        return mMessages.size();
    }

    @UiThread
    public void remove(Message message) {
        int i = mMessages.indexOf(message);
        if (i >= 0) {
            mMessages.remove(i);
            notifyItemRemoved(i);
        }
    }

    @UiThread
    public void add(Message message) {
        mMessages.add(message);
        notifyItemInserted(mMessages.size() - 1);
    }

    public void addAll(List<Message> messages) {
        final int oldSize = mMessages.size();
        mMessages.addAll(messages);
        notifyItemRangeInserted(oldSize, messages.size());
    }

    @UiThread
    public void clear() {
        mMessages.clear();
        notifyDataSetChanged();
    }

    public void setDiagnosticClickListener(DiagnosticClickListener diagnosticClickListener) {
        this.mDiagnosticClickListener = diagnosticClickListener;
    }

    public List<Message> getDiagnostics() {
        return mMessages;
    }

    @UiThread
    public void setData(List<Message> messages) {
        mMessages.clear();
        mMessages.addAll(messages);
        notifyDataSetChanged();
    }

    static class ViewHolder extends RecyclerView.ViewHolder {
        TextView txtLineCol, txtMessage, txtFile;
        ImageView icon;

        ViewHolder(View itemView) {
            super(itemView);
            txtLineCol = itemView.findViewById(R.id.txt_line_col);
            txtMessage = itemView.findViewById(R.id.txt_message);
            txtFile = itemView.findViewById(R.id.txt_file);
            icon = itemView.findViewById(R.id.imageview);
        }
    }
}
