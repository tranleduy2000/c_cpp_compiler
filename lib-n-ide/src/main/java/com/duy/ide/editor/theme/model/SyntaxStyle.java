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

import org.gjt.sp.jedit.awt.Font;

/**
 * A simple text style class. It can specify the color, italic flag,
 * and bold flag of a run of text.
 *
 * @author Slava Pestov
 * @version $Id: SyntaxStyle.java 21831 2012-06-18 22:54:17Z ezust $
 */
public class SyntaxStyle {
    private int fgColor;
    /**
     * Background color can be null
     */
    private int bgColor;
    private Font font;

    public SyntaxStyle(int fgColor, int bgColor, Font font) {
        this.fgColor = fgColor;
        this.bgColor = bgColor;
        this.font = font;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof SyntaxStyle)) return false;

        SyntaxStyle that = (SyntaxStyle) o;

        if (fgColor != that.fgColor) return false;
        if (bgColor != that.bgColor) return false;
        return getFont() != null ? getFont().equals(that.getFont()) : that.getFont() == null;
    }

    @Override
    public int hashCode() {
        int result = fgColor;
        result = 31 * result + bgColor;
        result = 31 * result + (getFont() != null ? getFont().hashCode() : 0);
        return result;
    }

    /**
     * Returns the text color.
     */
    public int getForegroundColor() {
        return fgColor;
    }

    /**
     * Returns the background color.
     */
    public int getBackgroundColor() {
        return bgColor;
    }

    /**
     * Returns the style font.
     */
    public Font getFont() {
        return font;
    }

    public enum Attr {
        view_style_comment1("view.style.comment1"),
        view_style_comment2("view.style.comment2"),
        view_style_comment3("view.style.comment3"),
        view_style_comment4("view.style.comment4"),
        view_style_digit("view.style.digit"),
        view_style_foldLine_0("view.style.foldLine.0"),
        view_style_foldLine_1("view.style.foldLine.1"),
        view_style_foldLine_2("view.style.foldLine.2"),
        view_style_foldLine_3("view.style.foldLine.3"),
        view_style_function("view.style.function"),
        view_style_invalid("view.style.invalid"),
        view_style_keyword1("view.style.keyword1"),
        view_style_keyword2("view.style.keyword2"),
        view_style_keyword3("view.style.keyword3"),
        view_style_keyword4("view.style.keyword4"),
        view_style_label("view.style.label"),
        view_style_literal1("view.style.literal1"),
        view_style_literal2("view.style.literal2"),
        view_style_literal3("view.style.literal3"),
        view_style_literal4("view.style.literal4"),
        view_style_markup("view.style.markup"),
        view_style_operator("view.style.operator");

        private String key;

        Attr(String key) {
            this.key = key;
        }

        public String getKey() {
            return key;
        }

    }
}
