/*
 * Copyright (C) 2016 Jecelyin Peng <jecelyin@gmail.com>
 *
 * This file is part of 920 Text Editor.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.jecelyin.common.utils;

import android.content.Context;
import android.content.DialogInterface;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.annotation.StringRes;
import android.support.design.widget.TextInputLayout;
import android.support.v7.app.AlertDialog;
import android.text.InputType;
import android.text.TextUtils;
import android.view.LayoutInflater;
import android.view.View;
import android.view.inputmethod.EditorInfo;
import android.widget.EditText;
import android.widget.Toast;

import com.afollestad.materialdialogs.DialogAction;
import com.afollestad.materialdialogs.MaterialDialog;
import com.duy.ide.editor.editor.R;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class UIUtils {
    public static void alert(Context context, String message) {
        alert(context, null, message);
    }

    public static void alert(Context context, String title, String message) {
        MaterialDialog.Builder builder = new MaterialDialog.Builder(context)
                .content(message)
                .positiveText(android.R.string.ok);

        if (!TextUtils.isEmpty(title))
            builder.title(title);

        builder.show();
    }

    public static void toast(Context context, int messageResId) {
        toast(context, context.getString(messageResId));
    }

    public static void toast(Context context, int messageResId, Object... args) {
        toast(context, context.getString(messageResId, args));
    }

    public static void toast(Context context, String message) {
        Toast.makeText(context.getApplicationContext(), message, Toast.LENGTH_SHORT).show();
    }

    public static void toast(Context context, Throwable t) {
        DLog.e(t);
        Toast.makeText(context.getApplicationContext(), t.getMessage(), Toast.LENGTH_LONG).show();
    }

    /**
     * The content type of the text box, whose bits are defined by
     * {@link InputType}.
     *
     * @see InputType
     * @see InputType#TYPE_MASK_CLASS
     * @see InputType#TYPE_MASK_VARIATION
     * @see InputType#TYPE_MASK_FLAGS
     */
    public static void showInputDialog(Context context, @StringRes int titleRes, @StringRes int hintRes,
                                       CharSequence value, int inputType, OnShowInputCallback callback) {
        showInputDialog(context, titleRes != 0 ? context.getString(titleRes) : null,
                hintRes != 0 ? context.getString(hintRes) : null, value, inputType, callback);
    }

    /**
     * @param context  android context
     * @param title    dialog title
     * @param hint     edittext hint
     * @param value    init value for edit text
     * @param callback result callback
     */
    public static void showInputDialog(Context context, CharSequence title,
                                       @Nullable CharSequence hint,
                                       @Nullable CharSequence value, int inputType,
                                       @Nullable final OnShowInputCallback callback) {
        AlertDialog.Builder builder = new AlertDialog.Builder(context);
        builder.setTitle(title);
        View view = LayoutInflater.from(context).inflate(R.layout.dialog_input, null);
        builder.setView(view);
        final EditText editText = view.findViewById(R.id.edit_input);
        editText.setInputType(inputType == 0 ? EditorInfo.TYPE_CLASS_TEXT : inputType);
        if (value != null) {
            editText.setText(value);
            editText.setSelection(value.length());
        }
        if (hint != null) {
            TextInputLayout inputLayout = view.findViewById(R.id.hint);
            inputLayout.setHint(hint);
        }
        builder.setPositiveButton(android.R.string.ok, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                if (callback != null) {
                    callback.onConfirm(editText.getText());
                }
            }
        }).setNegativeButton(android.R.string.cancel, new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.cancel();
            }
        });
        AlertDialog dialog = builder.show();
        dialog.setCanceledOnTouchOutside(false);
        dialog.setCancelable(true);
    }

    public static void showConfirmDialog(Context context, @StringRes int messageRes, final OnClickCallback callback) {
        showConfirmDialog(context, context.getString(messageRes), callback);
    }

    public static void showConfirmDialog(Context context, CharSequence message, final OnClickCallback callback) {
        showConfirmDialog(context, null, message, callback);
    }

    public static void showConfirmDialog(Context context, CharSequence title, CharSequence message, final OnClickCallback callback) {
        showConfirmDialog(context, title, message, callback, context.getString(android.R.string.ok), context.getString(android.R.string.cancel));
    }

    public static void showConfirmDialog(Context context, @StringRes int title, @StringRes int message, final OnClickCallback callback, @StringRes int postiveRes, @StringRes int negativeRes) {
        showConfirmDialog(context, title == 0 ? null : context.getString(title), context.getString(message), callback, context.getString(postiveRes), context.getString(negativeRes));
    }

    public static void showConfirmDialog(Context context, CharSequence title, CharSequence message, final OnClickCallback callback, String postiveStr, String negativeStr) {
        MaterialDialog.Builder dialog = new MaterialDialog.Builder(context)
                .title(title)
                .content(message)
                .positiveText(postiveStr)
                .negativeText(negativeStr)
                .onPositive(new MaterialDialog.SingleButtonCallback() {
                    @Override
                    public void onClick(@NonNull MaterialDialog dialog, @NonNull DialogAction which) {
                        if (callback == null)
                            return;
                        callback.onOkClick();
                    }
                })
                .onNegative(new MaterialDialog.SingleButtonCallback() {
                    @Override
                    public void onClick(@NonNull MaterialDialog dialog, @NonNull DialogAction which) {
                        if (callback == null)
                            return;
                        callback.onCancelClick();
                    }
                });

        MaterialDialog dlg = dialog.show();
        dlg.setCanceledOnTouchOutside(false);
        dlg.setCancelable(true);
    }


    public static abstract class OnClickCallback {
        public abstract void onOkClick();

        public void onCancelClick() {
        }
    }

    public static abstract class OnShowInputCallback {
        public abstract void onConfirm(CharSequence input);
    }
}
