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

import android.app.Dialog;
import android.content.Context;
import android.support.v7.app.AlertDialog;

import com.duy.ide.core.IdeActivity;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public abstract class AbstractDialog {
    protected final Context context;

    public AbstractDialog(Context context) {
        this.context = context;
    }

    protected void handleDialog(Dialog dlg) {
        dlg.setCanceledOnTouchOutside(false);
        dlg.setCancelable(true);
    }

    protected AlertDialog.Builder getBuilder() {
        return new AlertDialog.Builder(context);
    }

    protected IdeActivity getMainActivity() {
        return (IdeActivity) context;
    }

    public abstract void show();
}
