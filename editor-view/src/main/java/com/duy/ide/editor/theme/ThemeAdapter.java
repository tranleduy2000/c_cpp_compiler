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
import com.simplecityapps.recyclerview_fastscroll.views.FastScrollRecyclerView;

import org.gjt.sp.jedit.Catalog;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;

public class ThemeAdapter extends RecyclerView.Adapter<ThemeAdapter.ViewHolder> implements FastScrollRecyclerView.SectionedAdapter {
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

        String title = makeTitle(position, editorTheme);
        holder.mTxtName.setText(title);
        EditAreaView editorView = holder.mEditorView;

        Buffer buffer = new Buffer();
        Highlighter highlighter = new Highlighter();
        editorView.setTheme(editorTheme);
        editorView.setEnabled(false);
        editorView.setFocusable(false);
        editorView.setFocusableInTouchMode(false);

        buffer.setMode(Catalog.getModeByName("C++"), mContext);
        editorView.setText(new SpannableStringBuilder());
        editorView.getText().insert(0, getSampleData());
        buffer.setEditable(editorView.getText());
        buffer.insert(0, getSampleData());

        HashMap<Integer, ArrayList<? extends CharacterStyle>> colorsMap = new HashMap<>();
        int lineCount = buffer.getLineManager().getLineCount();
        highlighter.highlight(buffer, editorTheme, colorsMap, editorView.getText(), 0, lineCount - 1);
    }

    private String makeTitle(int position, EditorTheme editorTheme) {
        StringBuilder builder = new StringBuilder(String.valueOf(position + 1));
        builder.append(". ");
        builder.append(editorTheme.getName());
        for (int i = 0; i < builder.length(); i++) {
            if (builder.charAt(i) == ' ' && i + 1 < builder.length()) {
                builder.setCharAt(i + 1, Character.toUpperCase(builder.charAt(i + 1)));
            }
        }
        return builder.toString();
    }

    private String refine(String name) {
        StringBuilder builder = new StringBuilder(name);
        builder.insert(9, " ");
        for (int i = 0; i < builder.length(); i++) {
            if (builder.charAt(i) == ' ' && i + 1 < builder.length()) {
                builder.setCharAt(i + 1, Character.toUpperCase(builder.charAt(i + 1)));
            }
        }
        return builder.toString();
    }


    @Override
    public int getItemCount() {
        return mEditorThemes.size();
    }

    private CharSequence getSampleData() {
        return "// C++ Program to Find Largest Element of an Array\n" +
                "\n" +
                "// This program takes n number of element from user (where, n is specified by user) and stores data in an array. Then, this program displays the largest element of that array using loops.\n" +
                "\n" +
                "#include <iostream>\n" +
                "\n" +
                "using namespace std;\n" +
                "\n" +
                "int main() {\n" +
                "    int i, n;\n" +
                "    float arr[100];\n" +
                "\n" +
                "    cout << \"Enter total number of elements(1 to 100): \";\n" +
                "    cin >> n;\n" +
                "    cout << endl;\n" +
                "\n" +
                "    // Store number entered by the user\n" +
                "    for (i = 0; i < n; ++i) {\n" +
                "        cout << \"Enter Number \" << i + 1 << \" : \";\n" +
                "        cin >> arr[i];\n" +
                "    }\n" +
                "\n" +
                "    // Loop to store largest number to arr[0]\n" +
                "    for (i = 1; i < n; ++i) {\n" +
                "        // Change < to > if you want to find the smallest element\n" +
                "        if (arr[0] < arr[i])\n" +
                "            arr[0] = arr[i];\n" +
                "    }\n" +
                "    cout << \"Largest element = \" << arr[0];\n" +
                "\n" +
                "    return 0;\n" +
                "}";
    }

    @NonNull
    @Override
    public String getSectionName(int position) {
        return mEditorThemes.get(position).getName();
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
