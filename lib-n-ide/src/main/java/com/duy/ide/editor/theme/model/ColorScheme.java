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

import java.util.HashMap;
import java.util.Properties;

public abstract class ColorScheme {
    private HashMap<String, Integer> colors = new HashMap<>();

    protected int getColor(String attr) {
        Integer color = colors.get(attr);
        if (color == null) {
            return Color.BLACK;
        }
        if (Color.alpha(color) == 0 && color != Color.TRANSPARENT) {
            return Color.rgb(Color.red(color), Color.green(color), Color.blue(color));
        }
        return color;
    }

    protected void put(String key, int color) {
        colors.put(key, color);
    }

    public abstract void load(Properties properties);
}
