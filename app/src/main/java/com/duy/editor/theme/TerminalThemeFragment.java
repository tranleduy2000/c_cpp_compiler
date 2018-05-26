package com.duy.editor.theme;

import android.content.Context;
import android.os.Bundle;
import android.preference.PreferenceManager;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v7.widget.DividerItemDecoration;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.duy.ccppcompiler.R;
import com.jecelyin.editor.v2.Preferences;
import com.simplecityapps.recyclerview_fastscroll.views.FastScrollRecyclerView;

import java.io.StringBufferInputStream;

import jackpal.androidterm.TermSettings;
import jackpal.androidterm.emulatorview.ColorScheme;
import jackpal.androidterm.emulatorview.EmulatorView;
import jackpal.androidterm.emulatorview.TermSession;

public class TerminalThemeFragment extends Fragment {
    private RecyclerView mRecyclerView;
    private TerminalThemeAdapter mAdapter;
    private Preferences mPreferences;


    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        return inflater.inflate(R.layout.fragment_editor_theme, container, false);
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        super.onViewCreated(view, savedInstanceState);
        mPreferences = Preferences.getInstance(getContext());
        mRecyclerView = view.findViewById(R.id.recyclerView);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(getContext()));
        mRecyclerView.addItemDecoration(new DividerItemDecoration(getContext(), DividerItemDecoration.VERTICAL));
        mAdapter = new TerminalThemeAdapter(getContext());
        mAdapter.setOnThemeSelectListener((TerminalThemeAdapter.OnThemeSelectListener) getActivity());
        mRecyclerView.setAdapter(mAdapter);

    }

    public static class TerminalThemeAdapter extends RecyclerView.Adapter<TerminalThemeAdapter.ViewHolder> implements FastScrollRecyclerView.SectionedAdapter {
        private final String[] mThemes;
        private OnThemeSelectListener onThemeSelectListener;
        private TermSettings mTermSettings;
        private Context mContext;

        public TerminalThemeAdapter(Context context) {
            mContext = context;
            mThemes = context.getResources().getStringArray(R.array.entries_emulator_color_preference);
            mTermSettings = new TermSettings(context.getResources(),
                    PreferenceManager.getDefaultSharedPreferences(context));
        }

        @NonNull
        @Override
        public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
            View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.list_item_terminal, parent, false);
            return new ViewHolder(view);
        }

        @Override
        public void onBindViewHolder(@NonNull final ViewHolder holder, final int position) {
            EmulatorView termView = holder.mTermView;
            TermSession session = new TermSession();
            session.setTermIn(new StringBufferInputStream(mThemes[position]));
            termView.attachSession(session);
            termView.setDensity(mContext.getResources().getDisplayMetrics());
            termView.setTextSize(16);
            termView.setColorScheme(new ColorScheme(TermSettings.COLOR_SCHEMES[position]));

            holder.mBtnSelect.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    if (onThemeSelectListener != null){
                        onThemeSelectListener.onTerminalThemeSelected(mThemes[position],position);
                    }
                }
            });
        }

        private String makeTitle(int position, String name) {
            return (position + 1) + ". " + name;
        }

        @Override
        public int getItemCount() {
            return mThemes.length;
        }


        @NonNull
        @Override
        public String getSectionName(int position) {
            return mThemes[position];
        }

        public void setOnThemeSelectListener(OnThemeSelectListener onThemeSelectListener) {
            this.onThemeSelectListener = onThemeSelectListener;
        }

        public interface OnThemeSelectListener {
            void onTerminalThemeSelected(String mTheme, int index);
        }

        static class ViewHolder extends RecyclerView.ViewHolder {
            View mBtnSelect;
            EmulatorView mTermView;

            ViewHolder(View itemView) {
                super(itemView);
                mTermView = itemView.findViewById(R.id.term_view);
                mBtnSelect = itemView.findViewById(R.id.btn_select);
            }
        }
    }
}
