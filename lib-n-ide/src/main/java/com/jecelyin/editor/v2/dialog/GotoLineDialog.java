/*
 * Copyright 2018 Mr Duy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.jecelyin.editor.v2.dialog;

import android.content.Context;
import android.view.inputmethod.EditorInfo;

import com.duy.ide.editor.Document;
import com.duy.ide.editor.EditorDelegate;
import com.duy.ide.editor.editor.R;
import com.jecelyin.common.utils.DLog;
import com.jecelyin.common.utils.StringUtils;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.common.Command;

import org.gjt.sp.jedit.LineManager;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class GotoLineDialog extends AbstractDialog {
    private final EditorDelegate editorDelegate;

    public GotoLineDialog(Context context, EditorDelegate editorDelegate) {
        super(context);
        this.editorDelegate = editorDelegate;
    }

    @Override
    public void show() {
        int lineCount = -1;
        if (editorDelegate != null) {
            Document document = editorDelegate.getDocument();
            if (document != null) {
                LineManager lineManager = document.getBuffer().getLineManager();
                lineCount = lineManager.getLineCount();
            }
        }
        UIUtils.showInputDialog(
                context,
                context.getString(R.string.goto_line),
                lineCount >= 1 ? ("1-" + lineCount) : null,
                null, EditorInfo.TYPE_CLASS_NUMBER,
                new UIUtils.OnShowInputCallback() {
                    @Override
                    public void onConfirm(CharSequence input) {
                        try {
                            int line = StringUtils.toInt(input.toString());
                            Command command = new Command(Command.CommandEnum.GOTO_INDEX);
                            command.args.putInt("line", line);
                            getMainActivity().doCommand(command);
                        } catch (Exception e) {
                            DLog.e(e);
                        }
                    }
                });
    }
}
