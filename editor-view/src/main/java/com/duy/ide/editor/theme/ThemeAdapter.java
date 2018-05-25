package com.duy.ide.editor.theme;

import android.content.Context;
import android.core.text.SpannableStringBuilder;
import android.core.widget.EditAreaView;
import android.support.annotation.NonNull;
import android.support.v7.widget.RecyclerView;
import android.text.style.CharacterStyle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;

import com.duy.ide.editor.editor.R;
import com.duy.ide.editor.theme.model.EditorTheme;
import com.jecelyin.editor.v2.editor.Highlighter;
import com.jecelyin.editor.v2.highlight.Buffer;

import org.gjt.sp.jedit.Catalog;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;

public class ThemeAdapter extends RecyclerView.Adapter<ThemeAdapter.ViewHolder> {
    private final ArrayList<EditorTheme> mEditorThemes;
    private Context mContext;

    public ThemeAdapter(Context context) {
        mContext = context;
        ThemeLoader.init(context);
        mEditorThemes = ThemeLoader.getAll();
        Collections.sort(mEditorThemes, new Comparator<EditorTheme>() {
            @Override
            public int compare(EditorTheme o1, EditorTheme o2) {
                return o1.getName().compareTo(o2.getName());
            }
        });
    }


    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.list_item_theme, parent, false);
        return new ViewHolder(view);
    }

    @Override
    public void onBindViewHolder(@NonNull final ViewHolder holder, int position) {
        final EditorTheme editorTheme = mEditorThemes.get(position);

        holder.mTxtName.setText(editorTheme.getName());
        EditAreaView editorView = holder.mEditorView;

        Buffer buffer = new Buffer();
        Highlighter highlighter = new Highlighter();
        editorView.setTheme(editorTheme);
        editorView.setEnabled(false);
        editorView.setFocusable(false);
        editorView.setFocusableInTouchMode(false);

        buffer.setMode(Catalog.getModeByName("C"), mContext);
        editorView.setText(new SpannableStringBuilder());
        editorView.getText().insert(0, getSampleData());
        buffer.setEditable(editorView.getText());
        buffer.insert(0, getSampleData());

        HashMap<Integer, ArrayList<? extends CharacterStyle>> colorsMap = new HashMap<>();
        int lineCount = buffer.getLineManager().getLineCount();
        highlighter.highlight(buffer, editorTheme, colorsMap, editorView.getText(), 0, lineCount - 1);
    }


    @Override
    public int getItemCount() {
        return mEditorThemes.size();
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

    static class ViewHolder extends RecyclerView.ViewHolder {
        EditAreaView mEditorView;
        TextView mTxtName;

        public ViewHolder(View itemView) {
            super(itemView);
            mEditorView = itemView.findViewById(R.id.editor_view);
            mTxtName = itemView.findViewById(R.id.txt_name);
        }
    }
}
