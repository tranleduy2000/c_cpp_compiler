package com.duy.ccppcompiler.compiler;

import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.design.widget.TabLayout;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.support.v7.preference.PreferenceFragmentCompat;
import android.support.v7.widget.Toolbar;

import com.duy.ccppcompiler.R;
import com.jecelyin.editor.v2.ThemeSupportActivity;

import static com.duy.common.preferences.PreferencesSupportV7.bindPreferenceSummaryToValue;

public class CompilerSettingActivity extends ThemeSupportActivity {
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_compiler_setting);
        Toolbar toolbar = findViewById(com.duy.ide.editor.editor.R.id.toolbar);
        setSupportActionBar(toolbar);
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        setTitle(com.duy.ide.editor.editor.R.string.settings);

        ViewPager viewPager = findViewById(R.id.view_pager);
        viewPager.setAdapter(new FragmentPagerAdapter(getSupportFragmentManager()) {
            @Override
            public Fragment getItem(int position) {
                switch (position) {
                    case 0:
                        return new CompilerOptionsFragment();
                    case 1:
                        return new StaticCodeAnalysisSetting();
                    default:
                        return null;
                }
            }

            @Override
            public int getCount() {
                return 2;
            }

            @Nullable
            @Override
            public CharSequence getPageTitle(int position) {
                switch (position) {
                    case 0:
                        return getString(R.string.compiler);
                    case 1:
                        return getString(R.string.code_analysis);
                    default:
                        return null;
                }
            }
        });

        TabLayout tabLayout = findViewById(R.id.tab_layout);
        tabLayout.setupWithViewPager(viewPager);
    }

    public static class CompilerOptionsFragment extends PreferenceFragmentCompat {
        @Override
        public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
            addPreferencesFromResource(R.xml.preferences_compiler);
            bindPreferenceSummaryToValue(findPreference(getString(R.string.pref_option_optimization_level)));
            bindPreferenceSummaryToValue(findPreference(getString(R.string.pref_option_language_standard)));
        }
    }

    public static class StaticCodeAnalysisSetting extends PreferenceFragmentCompat {

        @Override
        public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
            addPreferencesFromResource(R.xml.preferences_cppcheck);
        }
    }
}
