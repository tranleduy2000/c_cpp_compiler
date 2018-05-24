package org.gjt.sp.jedit.awt;

import android.graphics.Typeface;

public class Font {
    public static final int ITALIC = Typeface.ITALIC;
    public static final int BOLD = Typeface.BOLD;
    private final int style;

    public Font(int style) {
        this.style = style;
    }
}
