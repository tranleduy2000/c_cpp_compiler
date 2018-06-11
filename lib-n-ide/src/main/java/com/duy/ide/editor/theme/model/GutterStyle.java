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

package com.duy.ide.editor.theme.model;

import android.graphics.Color;

import com.jecelyin.common.utils.DLog;

import java.util.Properties;

public class GutterStyle extends ColorScheme {

    private static final String TAG = "GutterStyle";

    public GutterStyle() {

    }

    private int getColor(Attr attr) {
        return super.getColor(attr.getKey());
    }

    public int getBgColor() {
        return getColor(Attr.VIEW_GUTTER_BG_COLOR);
    }

    public int getCurrentLineColor() {
        return getColor(Attr.VIEW_GUTTER_CURRENT_LINE_COLOR);
    }

    public int getFgColor() {
        return getColor(Attr.VIEW_GUTTER_FG_COLOR);
    }

    public int getFocusBorderColor() {
        return getColor(Attr.VIEW_GUTTER_FOCUS_BORDER_COLOR);
    }

    public int getFoldColor() {
        return getColor(Attr.VIEW_GUTTER_FOLD_COLOR);
    }

    public int getHighlightColor() {
        return getColor(Attr.VIEW_GUTTER_HIGHLIGHT_COLOR);
    }

    public int getMarkerColor() {
        return getColor(Attr.VIEW_GUTTER_MARKER_COLOR);
    }

    public int getNoFocusBorderColor() {
        return getColor(Attr.VIEW_GUTTER_NO_FOCUS_BORDER_COLOR);
    }

    public int getRegisterColor() {
        return getColor(Attr.VIEW_GUTTER_REGISTER_COLOR);
    }

    @Override
    public void load(Properties properties) {
        for (Attr attr : Attr.values()) {
            try {
                String color = properties.getProperty(attr.getKey());
                put(attr.getKey(), Color.parseColor(color));
            } catch (Exception ignored) {
                if (DLog.DEBUG) DLog.w(TAG, "load: can not find attr " + attr.getKey());
            }
        }
    }


    public enum Attr {
        VIEW_GUTTER_BG_COLOR("view.gutter.bgColor"),
        VIEW_GUTTER_CURRENT_LINE_COLOR("view.gutter.currentLineColor"),
        VIEW_GUTTER_FG_COLOR("view.gutter.fgColor"),
        VIEW_GUTTER_FOCUS_BORDER_COLOR("view.gutter.focusBorderColor"),
        VIEW_GUTTER_FOLD_COLOR("view.gutter.foldColor"),
        VIEW_GUTTER_HIGHLIGHT_COLOR("view.gutter.highlightColor"),
        VIEW_GUTTER_MARKER_COLOR("view.gutter.markerColor"),
        VIEW_GUTTER_NO_FOCUS_BORDER_COLOR("view.gutter.noFocusBorderColor"),
        VIEW_GUTTER_REGISTER_COLOR("view.gutter.registerColor"),
        VIEW_GUTTER_STRUCTURE_HIGHLIGHT_COLOR("view.gutter.structureHighlightColor");

        private String key;

        Attr(String key) {
            this.key = key;
        }

        public String getKey() {
            return key;
        }

    }
}
