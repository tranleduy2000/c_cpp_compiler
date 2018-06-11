/*
 * Copyright (C) 2018 Tran Le Duy
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package com.duy.ide.editor.theme;

public enum ThemeAttr {

    console_bgColor("console.bgColor"),
    console_errorColor("console.errorColor"),
    console_font("console.font"),
    console_fontsize("console.fontsize"),
    console_fontstyle("console.fontstyle"),
    console_infoColor("console.infoColor"),
    console_plainColor("console.plainColor"),
    console_warningColor("console.warningColor"),
    error_list_errorColor("error-list.errorColor"),
    error_list_warningColor("error-list.warningColor"),
    jdiff_changed_color("jdiff.changed-color"),
    jdiff_deleted_color("jdiff.deleted-color"),
    jdiff_highlight_changed_color("jdiff.highlight-changed-color"),
    jdiff_highlight_deleted_color("jdiff.highlight-deleted-color"),
    jdiff_highlight_inserted_color("jdiff.highlight-inserted-color"),
    jdiff_inserted_color("jdiff.inserted-color"),
    jdiff_invalid_color("jdiff.invalid-color"),
    jdiff_overview_changed_color("jdiff.overview-changed-color"),
    jdiff_overview_deleted_color("jdiff.overview-deleted-color"),
    jdiff_overview_inserted_color("jdiff.overview-inserted-color"),
    jdiff_overview_invalid_color("jdiff.overview-invalid-color"),
    jdiff_selected_highlight_changed_color("jdiff.selected-highlight-changed-color"),
    jdiff_selected_highlight_deleted_color("jdiff.selected-highlight-deleted-color"),
    jdiff_selected_highlight_inserted_color("jdiff.selected-highlight-inserted-color"),
    tasklist_highlight_color("tasklist.highlight.color"),

    view_status_background("view.status.background"),
    view_status_foreground("view.status.foreground"),
    view_status_memory_background("view.status.memory.background"),
    view_status_memory_foreground("view.status.memory.foreground"),
   ;


    private String key;

    ThemeAttr(String key) {
        this.key = key;
    }

    public String getKey() {
        return key;
    }
}