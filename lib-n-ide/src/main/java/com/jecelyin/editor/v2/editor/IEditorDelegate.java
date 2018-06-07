package com.jecelyin.editor.v2.editor;

import android.support.annotation.Nullable;

import com.duy.ide.editor.view.IEditAreaView;
import com.jecelyin.editor.v2.common.Command;

public interface IEditorDelegate {

    void saveCurrentFile() throws Exception;

    void saveInBackground();


    IEditAreaView getEditText();

    int getCursorOffset();

    @Nullable
    Document getDocument();

    void formatSource();

    void onDocumentChanged();

    boolean isChanged();

    String getPath();

    String getEncoding();


    void doCommand(Command command);

}
