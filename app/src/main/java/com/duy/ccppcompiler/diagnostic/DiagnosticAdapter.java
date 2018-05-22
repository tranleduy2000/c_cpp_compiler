package com.duy.ccppcompiler.diagnostic;

import android.content.Context;
import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.compiler.diagnostic.Diagnostic;

import java.io.File;
import java.util.List;

/**
 * Created by Duy on 28-Apr-18.
 */

public class DiagnosticAdapter extends RecyclerView.Adapter<DiagnosticAdapter.ViewHolder> {
    private List<Diagnostic> mDiagnostics;
    private Context mContext;
    private DiagnosticClickListener mDiagnosticClickListener;

    DiagnosticAdapter(List<Diagnostic> diagnostics, Context context) {
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
            String text = diagnostic.getLineNumber() + ":" + diagnostic.getColumnNumber();
            holder.txtLineCol.setText(text);
        } else {
            holder.txtLineCol.setText("");
        }
        holder.btnFixIt.setVisibility(diagnostic.getSuggestion() != null ?
                View.VISIBLE : View.GONE);
        holder.btnFixIt.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (mDiagnosticClickListener != null) {
                    mDiagnosticClickListener.onSuggestionClick(v, diagnostic, diagnostic.getSuggestion());
                }
            }
        });

        File source = diagnostic.getSourceFile();
        if (source != null) {
            holder.txtFile.setText(source.getName());
        } else {
            holder.txtFile.setText("");
        }

        holder.txtMessage.setText(diagnostic.getMessage(mContext));
        holder.root.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                if (mDiagnosticClickListener != null) {
                    mDiagnosticClickListener.onDiagnosisClick(diagnostic, v);
                }
            }
        });
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
        notifyItemRangeRemoved(0, mDiagnostics.size() - 1);
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
        View root;
        View btnFixIt;

        ViewHolder(View itemView) {
            super(itemView);
            txtLineCol = itemView.findViewById(R.id.txt_line_col);
            txtMessage = itemView.findViewById(R.id.txt_message);
            txtFile = itemView.findViewById(R.id.txt_file);
            btnFixIt = itemView.findViewById(R.id.btn_fix_it);
            root = itemView;
        }
    }
}
