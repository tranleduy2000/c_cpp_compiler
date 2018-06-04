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

package com.jecelyin.editor.v2.widget.menu;


import android.support.annotation.DrawableRes;
import android.support.annotation.IdRes;

import com.duy.ide.editor.editor.R;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public enum MenuGroup {
    //Run, edit
    TOP(0, 0, 0),
    FILE(R.string.file, R.id.menu_file, R.drawable.ic_folder_open_white_24dp),
    EDIT(R.string.edit, R.id.menu_edit, 0),
    VIEW(R.string.view, R.id.menu_view, 0),
    OTHER(R.string.other, R.id.menu_other, 0);

    private int titleId;
    private int menuId;
    private int iconId;

    MenuGroup(int titleId, @IdRes int menuId, @DrawableRes int iconId) {
        this.titleId = titleId;
        this.menuId = menuId;
        this.iconId = iconId;
    }

    public int getTitleId() {
        return titleId;
    }

    public int getMenuId() {
        return menuId;
    }

    public int getIconId() {
        return iconId;
    }
}
