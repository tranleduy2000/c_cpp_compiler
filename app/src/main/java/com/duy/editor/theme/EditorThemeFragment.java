package com.duy.editor.theme;

import android.os.Bundle;
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
import com.duy.ide.editor.theme.model.EditorTheme;
import com.jecelyin.editor.v2.Preferences;

public class EditorThemeFragment extends Fragment {
    private RecyclerView mRecyclerView;
    private EditorThemeAdapter mEditorThemeAdapter;
    private Preferences mPreferences;

    public static EditorThemeFragment newInstance() {

        Bundle args = new Bundle();

        EditorThemeFragment fragment = new EditorThemeFragment();
        fragment.setArguments(args);
        return fragment;
    }

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
        mEditorThemeAdapter = new EditorThemeAdapter(getContext());
        mEditorThemeAdapter.setOnThemeSelectListener((EditorThemeAdapter.OnThemeSelectListener) getActivity());
        mRecyclerView.setAdapter(mEditorThemeAdapter);
        mRecyclerView.scrollToPosition(findThemeIndex(mPreferences.getEditorTheme()));

    }

    private int findThemeIndex(EditorTheme editorTheme) {
        int position = mEditorThemeAdapter.getPosition(editorTheme);
        if (position < 0) {
            return 0;
        }
        return position;
    }

}
