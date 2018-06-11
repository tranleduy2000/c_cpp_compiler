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

package com.duy.ide.editor.theme;

import android.annotation.SuppressLint;
import android.content.Context;
import android.content.res.AssetManager;
import android.os.AsyncTask;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v7.widget.DividerItemDecoration;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.text.style.CharacterStyle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.duy.ide.editor.Highlighter;
import com.duy.ide.editor.editor.R;
import com.duy.ide.editor.theme.model.Constants;
import com.duy.ide.editor.theme.model.EditorTheme;
import com.duy.ide.editor.view.IEditAreaView;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.highlight.Buffer;
import com.simplecityapps.recyclerview_fastscroll.views.FastScrollRecyclerView;

import org.apache.commons.io.IOUtils;
import org.gjt.sp.jedit.Catalog;
import org.gjt.sp.jedit.Mode;

import java.io.InputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;

public class EditorThemeFragment extends Fragment {
    private RecyclerView mRecyclerView;
    private EditorThemeAdapter mEditorThemeAdapter;
    private Preferences mPreferences;
    private ProgressBar mProgressBar;
    private LoadThemeTask mLoadThemeTask;

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_editor_theme, container, false);
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        mPreferences = Preferences.getInstance(getContext());
        mProgressBar = view.findViewById(R.id.progress_bar);
        mRecyclerView = view.findViewById(R.id.recyclerView);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(getContext()));
        mRecyclerView.addItemDecoration(new DividerItemDecoration(getContext(), DividerItemDecoration.VERTICAL));

        mEditorThemeAdapter = new EditorThemeAdapter(getContext());
        mEditorThemeAdapter.setOnThemeSelectListener((EditorThemeAdapter.OnThemeSelectListener) getActivity());
        mRecyclerView.setAdapter(mEditorThemeAdapter);

        loadData();
    }

    private void loadData() {
        mLoadThemeTask = new LoadThemeTask(getContext());
        mLoadThemeTask.executeOnExecutor(AsyncTask.THREAD_POOL_EXECUTOR);
    }

    @Override
    public void onDestroyView() {
        if (mLoadThemeTask != null) {
            mLoadThemeTask.cancel(true);
        }
        super.onDestroyView();
    }

    private int findThemeIndex(EditorTheme editorTheme) {
        int position = mEditorThemeAdapter.getPosition(editorTheme);
        if (position < 0) {
            return 0;
        }
        return position;
    }

    public static class EditorThemeAdapter extends RecyclerView.Adapter<EditorThemeAdapter.ViewHolder> implements FastScrollRecyclerView.SectionedAdapter {
        private final ArrayList<EditorTheme> mEditorThemes;
        private Context mContext;
        private OnThemeSelectListener onThemeSelectListener;
        private Mode mLanguage;
        private String mSampleCode;

        EditorThemeAdapter(Context context) {
            mContext = context;
            mEditorThemes = new ArrayList<>();
            resolveLanguage();
        }

        private void resolveLanguage() {
            if (mSampleCode == null) {
                try {
                    String fileName = null;
                    if (mContext.getPackageName().contains("cpp")) {
                        fileName = "templates/cplusplus.template";
                        mLanguage = Catalog.getModeByName("C++");
                    } else if (mContext.getPackageName().contains("java")) {
                        fileName = "templates/java.template";
                        mLanguage = Catalog.getModeByName("Java");
                    } else if (mContext.getPackageName().contains("pascal")) {
                        fileName = "templates/pascal.template";
                        mLanguage = Catalog.getModeByName("Pascal");
                    }
                    InputStream input = mContext.getAssets().open(fileName);
                    mSampleCode = IOUtils.toString(input, "UTF-8");
                    mSampleCode = mSampleCode.replace("\r\n", "\n");
                    mSampleCode = mSampleCode.replace("\r", "\n");
                    input.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }

        int getPosition(EditorTheme editorTheme) {
            return mEditorThemes.indexOf(editorTheme);
        }

        @NonNull
        @Override
        public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
            View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.list_item_theme, parent, false);
            return new ViewHolder(view);
        }

        @SuppressLint("UseSparseArrays")
        @Override
        public void onBindViewHolder(@NonNull final ViewHolder holder, int position) {
            final EditorTheme editorTheme = mEditorThemes.get(position);

            final String title = makeTitle(position, editorTheme);
            holder.mTxtName.setText(title);
            IEditAreaView editorView = holder.mEditorView;

            Buffer buffer = new Buffer();
            Highlighter highlighter = new Highlighter();
            editorView.setTheme(editorTheme);

            String sampleData = getSampleData();

            buffer.setMode(mLanguage, mContext);
            editorView.getEditableText().clearSpans();
            editorView.setText(sampleData);

            buffer.setEditable(editorView.getText());

            buffer.insert(0, sampleData);

            int lineCount = buffer.getLineManager().getLineCount();
            highlighter.highlight(buffer, editorTheme,
                    new HashMap<Integer, ArrayList<? extends CharacterStyle>>(),
                    editorView.getText(),
                    0,
                    lineCount - 1);

            holder.mBtnSelect.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    if (onThemeSelectListener != null) {
                        onThemeSelectListener.onEditorThemeSelected(editorTheme);
                    }
                }
            });
        }

        private String makeTitle(int position, EditorTheme editorTheme) {
            return (position + 1) + ". " + editorTheme.getName();
        }

        @Override
        public int getItemCount() {
            return mEditorThemes.size();
        }


        private String getSampleData() {

            if (mSampleCode == null) {
                mSampleCode = Constants.C_PLUS_PLUS_SAMPLE;
            }
            return mSampleCode;
        }

        @NonNull
        @Override
        public String getSectionName(int position) {
            return mEditorThemes.get(position).getName();
        }

        public void setOnThemeSelectListener(OnThemeSelectListener onThemeSelectListener) {
            this.onThemeSelectListener = onThemeSelectListener;
        }

        public void addTheme(EditorTheme theme) {
            mEditorThemes.add(theme);
            notifyItemInserted(mEditorThemes.size() - 1);
        }

        public interface OnThemeSelectListener {
            void onEditorThemeSelected(EditorTheme theme);
        }

        static class ViewHolder extends RecyclerView.ViewHolder {
            View mBtnSelect;
            IEditAreaView mEditorView;
            TextView mTxtName;

            ViewHolder(View itemView) {
                super(itemView);
                setIsRecyclable(false);
                mEditorView = itemView.findViewById(R.id.editor_view);
                mTxtName = itemView.findViewById(R.id.txt_name);
                mBtnSelect = itemView.findViewById(R.id.btn_select);
            }
        }
    }

    @SuppressLint("StaticFieldLeak")
    private class LoadThemeTask extends AsyncTask<Void, EditorTheme, Void> {
        private static final String TAG = "LoadThemeTask";
        private Context context;
        private AssetManager mAssetManager;

        LoadThemeTask(Context context) {
            this.context = context;
        }

        public Context getContext() {
            return context;
        }

        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            mAssetManager = getContext().getAssets();

            mProgressBar.setVisibility(View.VISIBLE);
            mProgressBar.setIndeterminate(true);
        }

        @Override
        protected Void doInBackground(Void... voids) {
            try {
                String[] list = mAssetManager.list(ThemeLoader.ASSET_PATH);
                Arrays.sort(list);
                for (String name : list) {
                    if (isCancelled()) {
                        return null;
                    }
                    Thread.sleep(1);
                    EditorTheme theme = ThemeLoader.getTheme(context, name);
                    publishProgress(theme);
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
            return null;
        }

        @Override
        protected void onProgressUpdate(EditorTheme... themes) {
            super.onProgressUpdate(themes);
            try {
                mEditorThemeAdapter.addTheme(themes[0]);
            } catch (Exception e) {
            }
        }

        @Override
        protected void onPostExecute(Void aVoid) {
            super.onPostExecute(aVoid);
            if (isCancelled()) {
                return;
            }
            mRecyclerView.scrollToPosition(findThemeIndex(mPreferences.getEditorTheme()));
            mProgressBar.setVisibility(View.GONE);

        }
    }
}
