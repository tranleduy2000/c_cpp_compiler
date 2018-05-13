package com.pdaxrom.cctools;

import android.content.SharedPreferences;
import android.os.Bundle;
import android.preference.PreferenceActivity;

public class Preferences extends PreferenceActivity implements SharedPreferences.OnSharedPreferenceChangeListener {
	protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getPreferenceManager().setSharedPreferencesName(CCToolsActivity.SHARED_PREFS_NAME);
        // Load the preferences from an XML resource
        addPreferencesFromResource(R.xml.preferences);
        getPreferenceScreen().getSharedPreferences().registerOnSharedPreferenceChangeListener(this);

    }

	protected void onResume() {
        super.onResume();
    }

    protected void onDestroy() {
        getPreferenceScreen().getSharedPreferences().unregisterOnSharedPreferenceChangeListener(this);    
        super.onDestroy();
    }

	public void onSharedPreferenceChanged(SharedPreferences arg0, String arg1) {
	}
}