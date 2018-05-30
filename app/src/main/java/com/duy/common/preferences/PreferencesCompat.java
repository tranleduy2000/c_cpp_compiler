/*
 *  Copyright (C)  2018  Duy Tran Le
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.duy.common.preferences;

import android.support.v7.preference.Preference;


public class PreferencesCompat {

    private static final String TAG = "PreferencesCompat";

    /**
     * Binds a preference's summary to its value. More specifically, when the
     * preference's value is changed, its summary (line of text below the
     * preference title) is updated to reflect the value. The summary is also
     * immediately updated upon calling this method. The exact display format is
     * dependent on the type of preference.
     *
     * @see PreferencesSupportV7
     */
    public static void bindPreferenceSummaryToValue(Preference preference) {
        PreferencesSupportV7.bindPreferenceSummaryToValue(preference);
    }

    /**
     * Binds a preference's summary to its value. More specifically, when the
     * preference's value is changed, its summary (line of text below the
     * preference title) is updated to reflect the value. The summary is also
     * immediately updated upon calling this method. The exact display format is
     * dependent on the type of preference.
     *
     * @see PreferencesNative
     */
    public static void bindPreferenceSummaryToValue(Preference preference, Object value) {
        PreferencesSupportV7.bindPreferenceSummaryToValue(preference, value);
    }

    /**
     * Binds a preference's summary to its value. More specifically, when the
     * preference's value is changed, its summary (line of text below the
     * preference title) is updated to reflect the value. The summary is also
     * immediately updated upon calling this method. The exact display format is
     * dependent on the type of preference.
     *
     * @see PreferencesSupportV7
     */
    public static void bindPreferenceSummaryToValue(android.preference.Preference preference) {
        PreferencesNative.bindPreferenceSummaryToValue(preference);
    }

    /**
     * Binds a preference's summary to its value. More specifically, when the
     * preference's value is changed, its summary (line of text below the
     * preference title) is updated to reflect the value. The summary is also
     * immediately updated upon calling this method. The exact display format is
     * dependent on the type of preference.
     *
     * @see PreferencesNative
     */
    public static void bindPreferenceSummaryToValue(android.preference.Preference preference, Object value) {
        PreferencesNative.bindPreferenceSummaryToValue(preference, value);
    }

}