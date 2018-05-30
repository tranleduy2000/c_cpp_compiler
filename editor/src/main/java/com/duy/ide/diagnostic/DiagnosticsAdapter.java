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

package com.duy.ide.diagnostic;

import android.content.Context;
import android.graphics.Color;
import android.graphics.drawable.Drawable;
import android.support.annotation.NonNull;
import android.support.v4.content.ContextCompat;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ImageView;
import android.widget.TextView;

import com.duy.ide.editor.editor.R;
import com.jecelyin.common.utils.DrawableUtils;

import java.io.File;
import java.util.List;

/**
 * Created by Duy on 28-Apr-18.
 */

public class DiagnosticsAdapter extends RecyclerView.Adapter<DiagnosticsAdapter.ViewHolder> {
    private List<Diagnostic> mDiagnostics;
    private Context mContext;
    private DiagnosticClickListener mDiagnosticClickListener;

    DiagnosticsAdapter(List<Diagnostic> diagnostics, Context context) {
        this.mDiagnostics = diagnostics;
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
        final Diagnostic diagnostic = mDiagnostics.get(position);
        if (diagnostic.getLineNumber() != Diagnostic.NOPOS) {
            long columnNumber = diagnostic.getColumnNumber();
            String text = diagnostic.getLineNumber() + ":"
                    + (columnNumber != Diagnostic.NOPOS ? columnNumber : "");
            holder.txtLineCol.setText(text);
        } else {
            holder.txtLineCol.setText("");
        }

        String path = diagnostic.getSourceFile();
        if (path != null) {
            holder.txtFile.setText(new File(path).getName());
        } else {
            holder.txtFile.setText("");
        }


        if (diagnostic.getMessage(mContext).isEmpty()) {
            holder.txtMessage.setVisibility(View.GONE);
        } else {
            holder.txtMessage.setVisibility(View.VISIBLE);
            holder.txtMessage.setText(diagnostic.getMessage(mContext));
        }

        holder.itemView.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (mDiagnosticClickListener != null) {
                    mDiagnosticClickListener.onDiagnosisClick(diagnostic, v);
                }
            }
        });
    }

    private void setIcon(ViewHolder holder, Diagnostic diagnostic) {
        switch (diagnostic.getKind()) {
            case ERROR: {
                Drawable drawable = ContextCompat.getDrawable(mContext, R.drawable.baseline_error_24);
                drawable = DrawableUtils.tintDrawable(drawable, Color.RED);
                holder.icon.setImageDrawable(drawable);
                break;
            }
            case WARNING: {
                Drawable drawable = ContextCompat.getDrawable(mContext, R.drawable.baseline_warning_24);
                drawable = DrawableUtils.tintDrawable(drawable, Color.YELLOW);
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
        return mDiagnostics.size();
    }

    public void remove(Diagnostic diagnostic) {
        int i = mDiagnostics.indexOf(diagnostic);
        if (i >= 0) {
            mDiagnostics.remove(i);
            notifyItemRemoved(i);
        }
    }

    public void add(Diagnostic diagnostic) {
        mDiagnostics.add(diagnostic);
        notifyItemInserted(mDiagnostics.size() - 1);
    }

    public void clear() {
        mDiagnostics.clear();
        notifyDataSetChanged();
    }

    public void setDiagnosticClickListener(DiagnosticClickListener diagnosticClickListener) {
        this.mDiagnosticClickListener = diagnosticClickListener;
    }

    public List<Diagnostic> getDiagnostics() {
        return mDiagnostics;
    }

    public void setData(List<Diagnostic> diagnostics) {
        mDiagnostics.clear();
        mDiagnostics.addAll(diagnostics);
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
