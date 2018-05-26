package com.duy.ide.editor.theme;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.widget.DividerItemDecoration;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Spinner;

import com.duy.ide.editor.editor.R;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.ThemeSupportActivity;

public class EditorThemeActivity extends ThemeSupportActivity {
    private static final String EXTRA_USE_LIGHT_THEME = "EXTRA_USE_LIGHT_THEME";
    private RecyclerView mRecyclerView;
    private ThemeAdapter mThemeAdapter;
    private Spinner mSpinner;
    private Preferences mPreferences;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_editor_theme);
        setSupportActionBar((Toolbar) findViewById(R.id.toolbar));
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        setTitle("");

        ThemeLoader.init(this);

        mRecyclerView = findViewById(R.id.recyclerView);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(this));
        mRecyclerView.addItemDecoration(new DividerItemDecoration(this, DividerItemDecoration.VERTICAL));
        mThemeAdapter = new ThemeAdapter(this);

        mRecyclerView.setAdapter(mThemeAdapter);
        mPreferences = Preferences.getInstance(this);

        boolean useLightTheme = mPreferences.isUseLightTheme();

        mSpinner = findViewById(R.id.spinner_themes);
        mSpinner.setSelection(useLightTheme ? 0 : 1);
        mSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                useLightTheme(position == 0);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });

        mPreferences.registerOnSharedPreferenceChangeListener(this);
    }

    private void useLightTheme(boolean useLightTheme) {
        if (mPreferences.isUseLightTheme() != useLightTheme) {
            mPreferences.setTheme(useLightTheme ? 0 : 1);
            recreate();
        }
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
