package com.duy.ccppcompiler.compiler;

import android.os.Bundle;
import android.preference.PreferenceFragment;
import android.support.annotation.Nullable;
import android.support.v7.widget.Toolbar;

import com.duy.ccppcompiler.R;
import com.jecelyin.editor.v2.FullScreenActivity;

import static com.duy.common.preferences.PreferencesNative.bindPreferenceSummaryToValue;

public class CompilerSettingActivity extends FullScreenActivity {
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_term_setting);
        Toolbar toolbar = findViewById(com.duy.ide.editor.editor.R.id.toolbar);
        setSupportActionBar(toolbar);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        setTitle(com.duy.ide.editor.editor.R.string.settings);

        // Display the fragment as the main content.
        getFragmentManager()
                .beginTransaction()
                .replace(com.duy.ide.editor.editor.R.id.content, new SettingsFragment())
                .commit();
    }

    public static class SettingsFragment extends PreferenceFragment {
        @Override
        public void onCreate(@Nullable Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            addPreferencesFromResource(R.xml.preferences_compiler);
            bindPreferenceSummaryToValue(findPreference(getString(R.string.pref_key_c_options)));
            bindPreferenceSummaryToValue(findPreference(getString(R.string.pref_key_cxx_options)));
        }
    }
}
