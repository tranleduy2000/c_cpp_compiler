/*
 * Copyright 2018 Mr Duy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package jackpal.androidterm.compat;

import android.view.MenuItem;

/**
 * Definitions related to android.view.MenuItem
 */
public class MenuItemCompat {
    public static final int SHOW_AS_ACTION_NEVER = 0;
    public static final int SHOW_AS_ACTION_IF_ROOM = 1;
    public static final int SHOW_AS_ACTION_ALWAYS = 2;
    public static final int SHOW_AS_ACTION_WITH_TEXT = 4;

    public static void setShowAsAction(MenuItem item, int actionEnum) {
        if (AndroidCompat.SDK >= 11) {
            Api11OrLater.setShowAsAction(item, actionEnum);
        }
    }

    private static class Api11OrLater {
        public static void setShowAsAction(MenuItem item, int actionEnum) {
            item.setShowAsAction(actionEnum);
        }
    }
}
