package com.duy.ide.editor.theme;

import android.content.Context;
import android.core.text.SpannableStringBuilder;
import android.core.widget.EditAreaView;
import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.duy.ide.editor.editor.R;
import com.duy.ide.editor.theme.model.EditorTheme;

import java.util.ArrayList;

public class ThemeAdapter extends RecyclerView.Adapter<ThemeAdapter.ViewHolder> {
    private final ArrayList<EditorTheme> mEditorThemes;
    private Context mContext;

    public ThemeAdapter(Context context) {
        mContext = context;
        ThemeLoader.init(context);
        mEditorThemes = ThemeLoader.getAll();
    }

    private void initData() {
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.list_item_theme, parent, false);
        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        holder.mEditorView.setTheme(mEditorThemes.get(position));
        holder.mEditorView.setText(new SpannableStringBuilder(getSampleData()));
    }



    @Override
    public int getItemCount() {
        return mEditorThemes.size();
    }

    static class ViewHolder extends RecyclerView.ViewHolder {
        EditAreaView mEditorView;

        public ViewHolder(View itemView) {
            super(itemView);
            mEditorView = itemView.findViewById(R.id.editor_view);
        }
    }


    private CharSequence getSampleData() {
        return "// C Program to Access Elements of an Array Using Pointer\n" +
                "\n" +
                "// This program declares the array of five element and the elements of that array are accessed using pointer.\n" +
                "\n" +
                "#include <stdio.h>\n" +
                "\n" +
                "int main() {\n" +
                "    int data[5], i;\n" +
                "    printf(\"Enter elements: \");\n" +
                "\n" +
                "    for (i = 0; i < 5; ++i)\n" +
                "        scanf(\"%d\", data + i);\n" +
                "\n" +
                "    printf(\"You entered: \\n\");\n" +
                "    for (i = 0; i < 5; ++i)\n" +
                "        printf(\"%d\\n\", *(data + i));\n" +
                "\n" +
                "    return 0;\n" +
                "}";
    }
}
