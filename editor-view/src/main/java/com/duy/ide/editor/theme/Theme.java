package com.duy.ide.editor.theme;

import java.util.EnumMap;

public class Theme {
    private EnumMap<Attr, Integer> colors = new EnumMap<Attr, Integer>(Attr.class);

    public Theme() {

    }

    public void set(Attr attr, int color) {
        colors.put(attr, color);
    }

    public Integer get(Attr attr) {
        return colors.get(attr);
    }

    public enum Attr {
        NAME,
        VIEW_FG_COLOR,
        VIEW_BG_COLOR,
        VIEW_CARET_COLOR,
        VIEW_SELECTION_COLOR,
        VIEW_EOL_MARKER_COLOR,
        VIEW_LINE_HIGHLIGHT_COLOR,

        VIEW_STYLE_COMMENT_1,
        VIEW_STYLE_LITERAL_1,
        VIEW_STYLE_LABEL,
        VIEW_STYLE_DIGIT,
        VIEW_STYLE_KEYWORD_1,
        VIEW_STYLE_KEYWORD_2,
        VIEW_STYLE_KEYWORD_3,
        VIEW_STYLE_KEYWORD_4,
        VIEW_STYLE_OPERATOR,
        VIEW_STYLE_FUNCTION,
        VIEW_STYLE_LITERAL_3,
        VIEW_STYLE_MARKUP
    }
}
