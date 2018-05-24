package com.duy.ide.editor.theme.model;

import android.graphics.Color;

import com.duy.ide.editor.theme.ThemeAttr;

import java.util.Properties;


public class EditorTheme extends ColorScheme {
    private GutterStyle gutterStyle = new GutterStyle();
    private WhiteSpaceStyle whiteSpaceStyle = new WhiteSpaceStyle();
    private SyntaxStyle[] syntaxStyles;
    private String name;

    public int getLineHighlightColor() {
        return getColor(Attr.VIEW_LINE_HIGHLIGHT_COLOR);
    }


    public int getSelectionColor() {
        return getColor(Attr.VIEW_SELECTION_COLOR);
    }


    public int getBgColor() {
        return getColor(Attr.VIEW_BG_COLOR);

    }


    public int getCaretColor() {
        return getColor(Attr.VIEW_CARET_COLOR);

    }

    public int getEolMarkerColor() {
        return getColor(Attr.VIEW_EOL_MARKER_COLOR);

    }

    public int getFgColor() {
        return getColor(Attr.VIEW_FG_COLOR);

    }

    public int getWrapGuideColor() {
        return getColor(Attr.VIEW_WRAP_GUIDE_COLOR);

    }

    private int getColor(Attr attr) {
        return super.getColor(attr.getKey());
    }


    public GutterStyle getGutterStyle() {
        return gutterStyle;
    }


    public WhiteSpaceStyle getWhiteSpaceStyle() {
        return whiteSpaceStyle;
    }

    @Override
    public void load(Properties properties) {
        for (Attr attr : Attr.values()) {
            try {
                String color = properties.getProperty(attr.getKey());
                put(attr.getKey(), Color.parseColor(color));
            } catch (Exception ignored) {
                ignored.printStackTrace();
            }
        }
        name = properties.getProperty(ThemeAttr.scheme_name.getKey());
        gutterStyle.load(properties);
        whiteSpaceStyle.load(properties);
    }

    private enum Attr {
        VIEW_BG_COLOR("view.bgColor"),
        VIEW_CARET_COLOR("view.caretColor"),
        VIEW_EOL_MARKER_COLOR("view.eolMarkerColor"),
        VIEW_FG_COLOR("view.fgColor"),
        VIEW_LINE_HIGHLIGHT_COLOR("view.lineHighlightColor"),
        VIEW_SELECTION_COLOR("view.selectionColor"),
        VIEW_STRUCTURE_HIGHLIGHT_COLOR("view.structureHighlightColor"),
        VIEW_WRAP_GUIDE_COLOR("view.wrapGuideColor");

        private String key;

        Attr(String key) {
            this.key = key;
        }

        public String getKey() {
            return key;
        }
    }
}
