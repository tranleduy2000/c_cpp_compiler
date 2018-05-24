package com.duy.ide.editor.theme;

import java.util.EnumMap;

public class Theme {
    private EnumMap<ThemeAttr, Integer> colors = new EnumMap<ThemeAttr, Integer>(ThemeAttr.class);

    public Theme() {

    }

    public void set(ThemeAttr attr, int color) {
        colors.put(attr, color);
    }

    public Integer get(ThemeAttr attr) {
        return colors.get(attr);
    }

}
