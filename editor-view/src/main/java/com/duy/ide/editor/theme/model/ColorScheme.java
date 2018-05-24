package com.duy.ide.editor.theme.model;

import android.graphics.Color;

import java.util.HashMap;
import java.util.Properties;

public abstract class ColorScheme {
    private HashMap<String, Integer> colors = new HashMap<>();

    protected int getColor(String attr) {
        Integer integer = colors.get(attr);
        if (integer == null) {
            return Color.BLACK;
        }
        return integer;
    }

    protected void put(String key, int color) {
        colors.put(key, color);
    }

    public abstract void load(Properties properties);
}
