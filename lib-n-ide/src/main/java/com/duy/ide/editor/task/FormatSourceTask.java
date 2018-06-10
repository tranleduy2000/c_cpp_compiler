package com.duy.ide.editor.task;

import android.annotation.SuppressLint;
import android.app.ProgressDialog;
import android.content.Context;
import android.os.AsyncTask;
import android.support.annotation.NonNull;
import android.widget.Toast;

import com.duy.ide.code.api.CodeFormatter;
import com.duy.ide.editor.editor.R;
import com.duy.ide.editor.view.IEditAreaView;

public class FormatSourceTask extends AsyncTask<Void, Void, CharSequence> {
    @SuppressLint("StaticFieldLeak")
    public Context mContext;
    private IEditAreaView mEditText;

    private ProgressDialog mProgressDialog;
    private CodeFormatter mFormatter;
    private Exception mException;
    private int mOldSelection;

    public FormatSourceTask(@NonNull Context context,
                            @NonNull IEditAreaView editText,
                            @NonNull CodeFormatter formatter) {
        mContext = context;
        mEditText = editText;
        mFormatter = formatter;
    }

    @Override
    protected void onPreExecute() {
        super.onPreExecute();
        mOldSelection = mEditText.getSelectionStart();

        mProgressDialog = new ProgressDialog(mContext);
        mProgressDialog.setTitle(R.string.formatting);
        mProgressDialog.setCancelable(false);
        mProgressDialog.setCanceledOnTouchOutside(false);
        mProgressDialog.show();
    }

    @Override
    protected CharSequence doInBackground(Void... voids) {
        try {
            String source = mEditText.getText().toString();
            return mFormatter.format(source);
        } catch (Exception e) {
            mException = e;
            return null;
        }
    }

    @Override
    protected void onPostExecute(CharSequence formatted) {
        super.onPostExecute(formatted);
        mProgressDialog.dismiss();
        if (formatted != null) {
            mEditText.setText(formatted);
            if (mEditText.length() <= mOldSelection) {
                mEditText.setSelection(mOldSelection);
            }
            Toast.makeText(mContext, R.string.formated_source, Toast.LENGTH_SHORT).show();
        } else {
            if (mException == null) {
                Toast.makeText(mContext, R.string.can_not_format_source, Toast.LENGTH_SHORT).show();
            } else {
                Toast.makeText(mContext, mException.getMessage(), Toast.LENGTH_SHORT).show();
            }
        }
    }
}
