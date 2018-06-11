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

package com.duy.ide.editor.internal.suggestion;

public class Editor {
    private final String text;
    private final int cursor;

    public Editor(String text, int cursor) {
        this.text = text;
        this.cursor = cursor;
    }

    public String getText() {
        return text;
    }

    public int getCursor() {
        return cursor;
    }

    public int length() {
        return text.length();
    }

    @Override
    public String toString() {
        return "Editor{" +
                "text='" + text + '\'' +
                ", cursor=" + cursor +
                '}';
    }
}
