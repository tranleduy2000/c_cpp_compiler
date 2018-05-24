package com.duy.ide.editor.theme;

import java.util.WeakHashMap;

public class ThemeLoader {
    public static WeakHashMap<String, Theme> themes;

    static {
        themes = new WeakHashMap<>();
    }

}
