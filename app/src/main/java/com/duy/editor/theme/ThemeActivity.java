package com.duy.editor.theme;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.design.widget.TabLayout;
import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentPagerAdapter;
import android.support.v4.view.ViewPager;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Spinner;
import android.widget.Toast;

import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.ui.dialogs.PremiumDialog;
import com.duy.common.purchase.InAppPurchaseHelper;
import com.duy.common.purchase.Premium;
import com.duy.ide.editor.theme.model.EditorTheme;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.ThemeSupportActivity;

public class ThemeActivity extends ThemeSupportActivity
        implements EditorThemeFragment.EditorThemeAdapter.OnThemeSelectListener, TerminalThemeFragment.TerminalThemeAdapter.OnThemeSelectListener {
    private Preferences mPreferences;
    private InAppPurchaseHelper mInAppPurchaseHelper;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_editor_theme);
        setSupportActionBar((Toolbar) findViewById(R.id.toolbar));
        setTitle("");

        mPreferences = Preferences.getInstance(this);
        mInAppPurchaseHelper = new InAppPurchaseHelper(this);

        Spinner spinner = findViewById(R.id.spinner_themes);
        spinner.setSelection(mPreferences.isUseLightTheme() ? 0 : 1);
        spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                useLightTheme(position == 0);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        mPreferences.registerOnSharedPreferenceChangeListener(this);

        ViewPager viewPager = findViewById(R.id.view_pager);
        viewPager.setAdapter(new FragmentPagerAdapter(getSupportFragmentManager()) {
            @Override
            public Fragment getItem(int position) {
                switch (position) {
                    case 0:
                        return new EditorThemeFragment();
                    case 1:
                        return new TerminalThemeFragment();
                }
                return null;
            }

            @Nullable
            @Override
            public CharSequence getPageTitle(int position) {
                switch (position) {
                    case 0:
                        return getString(R.string.editor);
                    case 1:
                        return getString(R.string.terminal);
                }
                return "";
            }

            @Override
            public int getCount() {
                return 2;
            }
        });
        TabLayout tabLayout = findViewById(R.id.tab_layout);
        tabLayout.setupWithViewPager(viewPager);
    }

    private void useLightTheme(boolean useLightTheme) {
        if (mPreferences.isUseLightTheme() != useLightTheme) {
            mPreferences.setAppTheme(useLightTheme ? 0 : 1);
            recreate();
        }
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {
        super.onSharedPreferenceChanged(sharedPreferences, key);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mPreferences.unregisterOnSharedPreferenceChangeListener(this);
    }

    @Override
    public void onEditorThemeSelected(EditorTheme theme) {
        if (Premium.isPremiumUser(this)) {
            mPreferences.setEditorTheme(theme.getFileName());
            String text = getString(R.string.selected_theme, theme.getName());
            Toast.makeText(this, text, Toast.LENGTH_SHORT).show();
        } else {
            PremiumDialog premiumDialog = new PremiumDialog(this, mInAppPurchaseHelper);
            premiumDialog.show();
        }
    }

    @Override
    public void onTerminalThemeSelected(String name, int index) {
        if (Premium.isPremiumUser(this)) {
            mPreferences.setTerminalTheme(index);
            String text = getString(R.string.selected_theme, name);
            Toast.makeText(this, text, Toast.LENGTH_SHORT).show();
        } else {
            PremiumDialog premiumDialog = new PremiumDialog(this, mInAppPurchaseHelper);
            premiumDialog.show();
        }
    }
}
