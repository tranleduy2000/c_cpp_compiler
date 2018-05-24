/*
 * SyntaxUtilities.java - Syntax and styles utility utility functions
 * :tabSize=4:indentSize=4:noTabs=false:
 * :folding=explicit:collapseFolds=1:
 *
 * Copyright (C) 1999, 2008 Matthieu Casanova, Slava Pestov
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

package org.gjt.sp.jedit.util;


//{{{ Imports

import android.graphics.Color;

import com.jecelyin.common.utils.DLog;

import org.gjt.sp.jedit.awt.Font;
import org.gjt.sp.jedit.syntax.SyntaxStyle;
import org.gjt.sp.jedit.syntax.Token;

import java.util.Locale;
import java.util.Properties;
import java.util.StringTokenizer;
//}}}

/**
 * Syntax utilities that depends on JDK only and syntax package.
 *
 * @author Matthieu Casanova
 * @version $Id: StandardUtilities.java 9871 2007-06-28 16:33:20Z Vampire0 $
 * @since 4.3pre13
 */
public class SyntaxUtilities {


    private static final String TAG = "SyntaxUtilities";


    private SyntaxUtilities() {
    }

    /**
     * Converts a color object to its hex value. The hex value
     * prefixed is with `#', for example `#ff0088'.
     *
     * @param c The color object
     * @since jEdit 4.3pre13
     */
    public static String getColorHexString(int c) {
        String colString = Integer.toHexString(c & 0xffffff);
        return "#000000".substring(0, 7 - colString.length()).concat(colString);
    }

    /**
     * @since jEdit 4.3pre13
     */
    public static Integer parseColor(String name, Integer defaultColor) {
        if (name == null || name.length() == 0)
            return defaultColor;

        name = name.trim();
        if (name.charAt(0) == '#') {
            try {
                return Color.parseColor(name);
            } catch (NumberFormatException nf) {
                return defaultColor;
            }
        } else if ("red".equals(name))
            return Color.RED;
        else if ("green".equals(name))
            return Color.GREEN;
        else if ("blue".equals(name))
            return Color.BLACK;
        else if ("yellow".equals(name))
            return Color.YELLOW;
//        else if ("orange".equals(name))
//            return Color.orange;
        else if ("white".equals(name))
            return Color.WHITE;
        else if ("lightGray".equals(name))
            return Color.LTGRAY;
        else if ("gray".equals(name))
            return Color.GRAY;
        else if ("darkGray".equals(name))
            return Color.DKGRAY;
        else if ("black".equals(name))
            return Color.BLACK;
        else if ("cyan".equals(name))
            return Color.CYAN;
        else if ("magenta".equals(name))
            return Color.MAGENTA;
//        else if ("pink".equals(name))
//            return Color.pink;
        else
            return defaultColor;
    }

    /**
     * Converts a style string to a style object.
     *
     * @param str            The style string
     * @param color          If false, the styles will be monochrome
     * @param defaultFgColor Default foreground color (if not specified in style string)
     * @throws IllegalArgumentException if the style is invalid
     * @since jEdit 4.3pre17
     */
    public static SyntaxStyle parseStyle(String str, boolean color, int defaultFgColor)
            throws IllegalArgumentException {
        int fgColor = defaultFgColor;
        Integer bgColor = null;
        boolean italic = false;
        boolean bold = false;
        StringTokenizer st = new StringTokenizer(str);
        while (st.hasMoreTokens()) {
            String s = st.nextToken();
            if (s.startsWith("color:")) {
                if (color) {
                    String substring = s.substring(6);
                    fgColor = parseColor(substring, Color.BLACK);
                }
            } else if (s.startsWith("bgColor:")) {
                if (color) {
                    bgColor = parseColor(s.substring(8), null);
                }
            } else if (s.startsWith("style:")) {
                for (int i = 6; i < s.length(); i++) {
                    if (s.charAt(i) == 'i')
                        italic = true;
                    else if (s.charAt(i) == 'b')
                        bold = true;
                    else
                        throw new IllegalArgumentException(
                                "Invalid style: " + s);
                }
            } else
                throw new IllegalArgumentException(
                        "Invalid directive: " + s);
        }
        return new SyntaxStyle(fgColor, bgColor,
                new Font((italic ? Font.ITALIC : 0) | (bold ? Font.BOLD : 0)));
    }

    /**
     * Converts a style string to a style object.
     *
     * @param str   The style string
     * @param color If false, the styles will be monochrome
     * @throws IllegalArgumentException if the style is invalid
     * @since jEdit 4.3pre13
     */
    public static SyntaxStyle parseStyle(String str, boolean color)
            throws IllegalArgumentException {
        return parseStyle(str, color, Color.BLACK);
    }

    /**
     * Loads the syntax styles from the properties, giving them the specified
     * base font family and size.
     *
     * @since jEdit 4.3pre13
     */
    public static SyntaxStyle[] loadStyles(Properties properties) {
        return loadStyles(properties, true);
    }

    /**
     * Loads the syntax styles from the properties, giving them the specified
     * base font family and size.
     *
     * @param color If false, the styles will be monochrome
     * @since jEdit 4.3pre13
     */
    public static SyntaxStyle[] loadStyles(Properties properties, boolean color) {
        SyntaxStyle[] styles = new SyntaxStyle[Token.ID_COUNT];

        // start at 1 not 0 to skip Token.NULL
        for (int i = 1; i < styles.length; i++) {
            try {
                String styleName = "view.style." + Token.tokenToString((byte) i).toLowerCase(Locale.ENGLISH);
                styles[i] = parseStyle(properties.getProperty(styleName), color);
            } catch (Exception e) {
                if (DLog.DEBUG) DLog.e(TAG, "loadStyles: ", e);
            }
        }

        return styles;
    }
}