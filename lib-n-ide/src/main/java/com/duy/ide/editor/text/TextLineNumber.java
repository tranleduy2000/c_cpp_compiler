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

package com.duy.ide.editor.text;

import java.util.ArrayList;
import java.util.List;

public class TextLineNumber {
    private List<LineInfo> mLines = new ArrayList<>();

    public void clear() {
        mLines.clear();
    }

    public void addLine(String text, int baseline) {
        mLines.add(new LineInfo(text, baseline));
    }

    public List<LineInfo> getLines() {
        return mLines;
    }

    public static class LineInfo {
        public final int y;
        public String text;

        LineInfo(String text, int baseline) {
            this.text = text;
            this.y = baseline;
        }

        public int getY() {
            return y;
        }

        public String getText() {
            return text;
        }
    }
}
