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
