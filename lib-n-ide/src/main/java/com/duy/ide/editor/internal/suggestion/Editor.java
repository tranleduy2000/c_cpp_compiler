package com.duy.ide.editor.internal.suggestion;

public class Editor {
    private final String text;
    private final int cursor;

    public Editor(String text, int cursor) {
        this.text = text;
        this.cursor = cursor;
    }

    public String getText() {
        return text;
    }

    public int getCursor() {
        return cursor;
    }

    public int length() {
        return text.length();
    }

    @Override
    public String toString() {
        return "Editor{" +
                "text='" + text + '\'' +
                ", cursor=" + cursor +
                '}';
    }
}
