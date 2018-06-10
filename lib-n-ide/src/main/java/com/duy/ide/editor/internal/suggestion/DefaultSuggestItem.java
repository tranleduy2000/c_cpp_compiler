package com.duy.ide.editor.internal.suggestion;

import com.duy.ide.editor.view.IEditAreaView;

public class DefaultSuggestItem implements com.duy.ide.code.api.SuggestItem {
    private String name;
    private String desc;
    private String returnType;
    private String insertText;
    private int priority;

    public DefaultSuggestItem() {

    }

    public DefaultSuggestItem(String name, String desc, String returnType,
                              String insertText, int priority) {
        this.name = name;
        this.desc = desc;
        this.returnType = returnType;
        this.insertText = insertText;

        this.priority = priority;
    }

    @Override
    public String getName() {
        return name;
    }

    @Override
    public String getDescription() {
        return desc;
    }

    @Override
    public String getReturnType() {
        return returnType;
    }

    @Override
    public String getInsertText() {
        return insertText;
    }

    @Override
    public int getSuggestionPriority() {
        return priority;
    }

    @Override
    public char getTypeHeader() {
        return 0;
    }

    @Override
    public void onSelectThis(IEditAreaView editAreaView) {

    }
}
