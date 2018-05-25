package com.duy.ide.editor.theme;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;

import com.duy.ide.editor.editor.R;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.ThemeSupportActivity;

public class EditorThemeActivity extends ThemeSupportActivity {
    private RecyclerView mRecyclerView;
    private ThemeAdapter mThemeAdapter;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_editor_theme);
        setSupportActionBar((Toolbar) findViewById(R.id.toolbar));
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        ThemeLoader.init(this);

        mRecyclerView = findViewById(R.id.recyclerView);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(this));
        mThemeAdapter = new ThemeAdapter(this);
        mRecyclerView.setAdapter(mThemeAdapter);

        Preferences.getInstance(this).registerOnSharedPreferenceChangeListener(this);
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {
        if (key.equals(getString(R.string.pref_current_theme))) {

        } else {
            super.onSharedPreferenceChanged(sharedPreferences, key);
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        Preferences.getInstance(this).unregisterOnSharedPreferenceChangeListener(this);
    }
}
