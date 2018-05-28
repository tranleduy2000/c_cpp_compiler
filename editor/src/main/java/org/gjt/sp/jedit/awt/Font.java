package org.gjt.sp.jedit.awt;

import android.graphics.Typeface;

public class Font {
    public static final int ITALIC = Typeface.ITALIC;
    public static final int BOLD = Typeface.BOLD;
    public static final int NORMAL = Typeface.NORMAL;
    private int style;

    public Font(int style) {
        this.style = style;
    }

    public int getStyle() {
        return style;
    }
}
