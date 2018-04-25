/*
 * Copyright (C) 2016 Jecelyin Peng <jecelyin@gmail.com>
 *
 * This file is part of 920 Text Editor.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.jecelyin.editor.v2.ui.settings;

import android.os.Bundle;
import android.preference.CheckBoxPreference;
import android.preference.EditTextPreference;
import android.preference.ListPreference;
import android.preference.PreferenceFragment;
import android.preference.PreferenceGroup;

import com.duy.ide.editor.editor.R;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.preference.JecListPreference;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class SettingsFragment extends PreferenceFragment implements android.preference.Preference.OnPreferenceClickListener {
    /**
     * A preference value change listener that updates the preference's summary
     * to reflect its new value.
     */
    private static android.preference.Preference.OnPreferenceChangeListener sBindPreferenceSummaryToValueListener = new android.preference.Preference.OnPreferenceChangeListener() {
        @Override
        public boolean onPreferenceChange(android.preference.Preference preference, Object value) {
            if (value == null)
                return true;
            String stringValue = value.toString();
            String key = preference.getKey();

            if (preference instanceof ListPreference) {
                // For list preferences, look up the correct display value in
                // the preference's 'entries' list.
                ListPreference listPreference = (ListPreference) preference;
                int index = listPreference.findIndexOfValue(stringValue);

                // Set the summary to reflect the new value.
                preference.setSummary(
                        index >= 0
                                ? listPreference.getEntries()[index]
                                : null);
            } else if (preference instanceof CheckBoxPreference) {
                ((CheckBoxPreference) preference).setChecked((boolean) value);
            } else if ("pref_highlight_file_size_limit".equals(key)) {
                preference.setSummary(stringValue + " KB");
            } else {
                // For all other preferences, set the summary to the value's
                // simple string representation.
                preference.setSummary(String.valueOf(stringValue));
            }

            return true;
        }
    };

    private static void dependBindPreference(PreferenceGroup pg) {
        int count = pg.getPreferenceCount();
        android.preference.Preference preference;
        String key;
        Object value;

        Preferences prefercence = Preferences.getInstance(pg.getContext());

        for (int i = 0; i < count; i++) {
            preference = pg.getPreference(i);
            key = preference.getKey();

            if (preference instanceof PreferenceGroup) {
                dependBindPreference((PreferenceGroup) preference);
                continue;
            }

            Class<? extends android.preference.Preference> cls = preference.getClass();
            if (cls.equals(android.preference.Preference.class))
                continue;

            value = prefercence.getValue(key);

            if (preference instanceof JecListPreference) {
//                if("pref_font_size".equals(key)) {
//                    new FontSizePreference((JecListPreference)preference);
//                } else if("pref_cursor_width".equals(key)) {
//                    new CursorWidthPreference((JecListPreference)preference);
//                }
            } else if (preference instanceof EditTextPreference) {
                ((EditTextPreference) preference).setText(String.valueOf(value));
            } else if (preference instanceof CheckBoxPreference) {
                ((CheckBoxPreference) preference).setChecked((boolean) value);
            }

            if (!Preferences.KEY_SYMBOL.equals(key))
                bindPreferenceSummaryToValue(preference);
        }
    }

    /**
     * Binds a preference's summary to its value. More specifically, when the
     * preference's value is changed, its summary (line of text below the
     * preference title) is updated to reflect the value. The summary is also
     * immediately updated upon calling this method. The exact display format is
     * dependent on the type of preference.
     *
     * @see #sBindPreferenceSummaryToValueListener
     */
    private static void bindPreferenceSummaryToValue(android.preference.Preference preference) {
        // Set the listener to watch for value changes.
        preference.setOnPreferenceChangeListener(sBindPreferenceSummaryToValueListener);

        String key = preference.getKey();
        Object value = Preferences.getInstance(preference.getContext()).getValue(key);
        sBindPreferenceSummaryToValueListener.onPreferenceChange(preference, value);
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        addPreferencesFromResource(R.xml.preference);

        dependBindPreference(getPreferenceScreen());

        findPreference("pref_about").setOnPreferenceClickListener(this);
        findPreference("pref_translate").setOnPreferenceClickListener(this);
        findPreference("pref_feedback").setOnPreferenceClickListener(this);
    }

    @Override
    public boolean onPreferenceClick(android.preference.Preference preference) {
        return true;
    }

}
