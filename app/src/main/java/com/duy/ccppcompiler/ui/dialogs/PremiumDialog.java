package com.duy.ccppcompiler.ui.dialogs;

import android.content.Context;
import android.content.DialogInterface;
import android.support.v7.app.AlertDialog;

import com.duy.ccppcompiler.R;
import com.jecelyin.editor.v2.dialog.AbstractDialog;

public class PremiumDialog extends AbstractDialog {
    private final DialogInterface.OnClickListener onClickListener;

    public PremiumDialog(Context context, DialogInterface.OnClickListener onClickListener) {
        super(context);
        this.onClickListener = onClickListener;
    }

    @Override
    public void show() {
        AlertDialog.Builder builder = getBuilder();
        builder.setTitle(R.string.title_premium_version);
        builder.setMessage(R.string.message_premium);
        builder.setPositiveButton(R.string.button_purchase, onClickListener);
        builder.setNegativeButton(R.string.cancel, onClickListener);
        builder.create().show();
    }
}
