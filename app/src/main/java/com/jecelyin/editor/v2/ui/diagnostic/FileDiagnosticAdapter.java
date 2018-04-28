package com.jecelyin.editor.v2.ui.diagnostic;

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

public class FileDiagnosticAdapter extends RecyclerView.Adapter<FileDiagnosticAdapter.ViewHolder> {
    private List<Diagnostic<File>> diagnostics;
    private Context context;

    public FileDiagnosticAdapter(List<Diagnostic<File>> diagnostics, Context context) {
        this.diagnostics = diagnostics;
        this.context = context;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.list_item_diagnostic, parent, false);
        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        Diagnostic<File> diagnostic = diagnostics.get(position);
        if (diagnostic.getLineNumber() != Diagnostic.NOPOS) {
            holder.txtLine.setText(String.valueOf(diagnostic.getLineNumber()));
            if (diagnostic.getColumnNumber() != Diagnostic.NOPOS) {
                holder.txtCol.setText(String.valueOf(diagnostic.getColumnNumber()));
            } else {
                holder.txtCol.getEditableText().clear();
            }
        } else {
            holder.txtLine.getEditableText().clear();
            holder.txtCol.getEditableText().clear();
        }

        holder.txtFile.setText(diagnostic.getSource().getName());
        holder.txtMessage.setText(diagnostic.getMessage(context));
    }

    @Override
    public int getItemCount() {
        return diagnostics.size();
    }

    static class ViewHolder extends RecyclerView.ViewHolder {
        TextView txtLine, txtCol, txtMessage, txtFile;
        View root;

        ViewHolder(View itemView) {
            super(itemView);
            txtLine = itemView.findViewById(R.id.txt_line);
            txtCol = itemView.findViewById(R.id.txt_col);
            txtMessage = itemView.findViewById(R.id.txt_message);
            txtFile = itemView.findViewById(R.id.txt_file);
            root = itemView;
        }
    }
}
