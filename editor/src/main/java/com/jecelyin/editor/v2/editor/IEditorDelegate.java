package com.jecelyin.editor.v2.editor;

import android.core.widget.EditAreaView;
import android.support.annotation.Nullable;

import com.jecelyin.editor.v2.common.Command;

public interface IEditorDelegate {
    //MARK: File interface

    void saveCurrentFile() throws Exception;

    void saveInBackground();

    //MARK: View interface

    EditAreaView getEditText();

    int getCursorOffset();


    //MAKR: Content interface

    @Nullable
    Document getDocument();

    void onDocumentChanged();

    boolean isChanged();

    String getPath();

    String getEncoding();


    void doCommand(Command command);

}
