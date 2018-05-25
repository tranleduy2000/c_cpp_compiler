package com.duy.ide.editor.theme.model;

import android.graphics.Color;

import com.duy.common.DLog;

import java.util.Properties;

public class WhiteSpaceStyle extends ColorScheme {

    private static final String TAG = "WhiteSpaceStyle";

    public WhiteSpaceStyle() {

    }


    public int getBlock() {
        return getColor(Attr.BLOCK_COLOR.getKey());
    }

    public int getFold() {
        return getColor(Attr.FOLD_COLOR.getKey());

    }

    public int getSpace() {
        return getColor(Attr.SPACE_COLOR.getKey());

    }

    public int getTab() {
        return getColor(Attr.TAB_COLOR.getKey());

    }

    public int getWhitespace() {
        return getColor(Attr.WHITESPACE_COLOR.getKey());

    }

    @Override
    public void load(Properties properties) {
        for (Attr attr : Attr.values()) {
            try {
                String color = properties.getProperty(attr.getKey());
                put(attr.getKey(), Color.parseColor(color));
            } catch (Exception ignored) {
                if (DLog.DEBUG) DLog.w(TAG, "load: failed " + attr.key);
            }
        }
    }

    public enum Attr {
        BLOCK_COLOR("white-space.block-color"),
        FOLD_COLOR("white-space.fold-color"),
        SPACE_COLOR("white-space.space-color"),
        TAB_COLOR("white-space.tab-color"),
        WHITESPACE_COLOR("white-space.whitespace-color");

        private String key;

        Attr(String key) {
            this.key = key;
        }

        public String getKey() {
            return key;
        }
    }
}
