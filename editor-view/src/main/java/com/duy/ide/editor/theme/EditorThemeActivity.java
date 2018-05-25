package com.duy.ide.editor.theme;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.SwitchCompat;
import android.support.v7.widget.Toolbar;
import android.widget.CompoundButton;

import com.duy.ide.editor.editor.R;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.ThemeSupportActivity;

public class EditorThemeActivity extends ThemeSupportActivity {
    private static final String EXTRA_USE_LIGHT_THEME = "EXTRA_USE_LIGHT_THEME";
    private RecyclerView mRecyclerView;
    private ThemeAdapter mThemeAdapter;
    private SwitchCompat mSwitchCompat;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        boolean useLightTheme = getIntent().getBooleanExtra(EXTRA_USE_LIGHT_THEME, true);
        if (useLightTheme){
            setTheme(R.style.LightTheme);
        }else {
            setTheme(R.style.DarkTheme);
        }

        setContentView(R.layout.activity_editor_theme);
        setSupportActionBar((Toolbar) findViewById(R.id.toolbar));
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        setTitle("");

        ThemeLoader.init(this);

        mRecyclerView = findViewById(R.id.recyclerView);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(this));
        mThemeAdapter = new ThemeAdapter(this);
        mRecyclerView.setAdapter(mThemeAdapter);
        mSwitchCompat = findViewById(R.id.switch_theme);
        mSwitchCompat.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                useLightTheme(isChecked);
            }
        });

        Preferences.getInstance(this).registerOnSharedPreferenceChangeListener(this);
    }

    private void useLightTheme(boolean useLightTheme) {
        boolean current = getIntent().getBooleanExtra(EXTRA_USE_LIGHT_THEME, true);
        if (current != useLightTheme) {
            getIntent().putExtra(EXTRA_USE_LIGHT_THEME, useLightTheme);
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
