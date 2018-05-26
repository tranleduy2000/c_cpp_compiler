package com.duy.editor;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.design.widget.Snackbar;
import android.support.v7.widget.DividerItemDecoration;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.view.View;
import android.widget.AdapterView;
import android.widget.Spinner;

import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.ui.dialogs.PremiumDialog;
import com.duy.common.purchase.InAppPurchaseHelper;
import com.duy.common.purchase.Premium;
import com.duy.ide.editor.theme.ThemeAdapter;
import com.duy.ide.editor.theme.model.EditorTheme;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.ThemeSupportActivity;

public class EditorThemeActivity extends ThemeSupportActivity implements ThemeAdapter.OnThemeSelectListener {
    private RecyclerView mRecyclerView;
    private ThemeAdapter mThemeAdapter;
    private Spinner mSpinner;
    private Preferences mPreferences;
    private InAppPurchaseHelper mInAppPurchaseHelper;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_editor_theme);
        setSupportActionBar((Toolbar) findViewById(R.id.toolbar));
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        setTitle("");
        mPreferences = Preferences.getInstance(this);

        mRecyclerView = findViewById(R.id.recyclerView);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(this));
        mRecyclerView.addItemDecoration(new DividerItemDecoration(this, DividerItemDecoration.VERTICAL));
        mThemeAdapter = new ThemeAdapter(this);
        mThemeAdapter.setOnThemeSelectListener(this);
        mRecyclerView.setAdapter(mThemeAdapter);
        mRecyclerView.scrollToPosition(findThemeIndex(mPreferences.getEditorTheme()));

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

        mInAppPurchaseHelper = new InAppPurchaseHelper(this);
        mPreferences.registerOnSharedPreferenceChangeListener(this);
    }

    private int findThemeIndex(EditorTheme editorTheme) {
        int position = mThemeAdapter.getPosition(editorTheme);
        if (position < 0) {
            return 0;
        }
        return position;
    }


    private void useLightTheme(boolean useLightTheme) {
        if (mPreferences.isUseLightTheme() != useLightTheme) {
            mPreferences.setTheme(useLightTheme ? 0 : 1);
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
    public void onThemeSelected(EditorTheme theme) {
        if (Premium.isPremiumUser(this)) {
            mPreferences.setEditorTheme(theme.getFileName());
            String text = getString(R.string.selected_theme, theme.getName());
            Snackbar.make(mRecyclerView, text, Snackbar.LENGTH_SHORT).show();
        } else {
            PremiumDialog premiumDialog = new PremiumDialog(this, mInAppPurchaseHelper);
            premiumDialog.show();
        }
    }
}
