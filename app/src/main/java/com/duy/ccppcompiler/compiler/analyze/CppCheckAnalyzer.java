/*
 *  Copyright (C)  2018  Duy Tran Le
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.duy.ccppcompiler.compiler.analyze;

import android.content.Context;
import android.core.widget.EditAreaView;
import android.os.Handler;
import android.support.annotation.NonNull;
import android.text.Editable;
import android.text.TextWatcher;

import com.duy.ccppcompiler.compiler.shell.CommandBuilder;
import com.duy.ccppcompiler.compiler.shell.CommandResult;
import com.duy.ccppcompiler.compiler.shell.Shell;
import com.duy.common.DLog;
import com.jecelyin.editor.v2.editor.EditorDelegate;

import org.apache.commons.io.IOUtils;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.regex.Pattern;

/**
 * https://sourceforge.net/p/cppcheck/wiki/Home/
 */
public class CppCheckAnalyzer implements ICodeAnalyser {
    public static final long DELAY_TIME = 300;
    private static final String TAG = "CppCheckAnalyzer";

    private static final String CPPCHECK_PROGRAM = "cppcheck";
    private static final String TEMPLATE = "--template=\"{file}:{line}:{message}\"";
    private static final Pattern TEMPLATE_PATTERN = Pattern.compile("^(\\S+):([0-9]+):(.*)");

    private final Context mContext;
    private final EditorDelegate mEditorDelegate;
    private final android.os.Handler mHandler = new Handler();
    private final Runnable mAnalyze = new Runnable() {
        @Override
        public void run() {
            try {
                analyzeImpl();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    };

    public CppCheckAnalyzer(@NonNull Context context, @NonNull EditorDelegate delegate) {
        this.mContext = context;
        this.mEditorDelegate = delegate;
        setEditor(delegate.getEditText());
    }

    private void analyzeImpl() throws IOException {
        File file = new File(mContext.getCacheDir(), "cppcheck/tmp/" + mEditorDelegate.getDocument().getFile().getName());
        FileOutputStream output = new FileOutputStream(file);
        IOUtils.write(mEditorDelegate.getEditableText(), output);
        output.close();
        if (DLog.DEBUG) DLog.d(TAG, "analyzeImpl: write tmp file complete");

        CommandBuilder builder = new CommandBuilder(CPPCHECK_PROGRAM);
        builder.addFlags(TEMPLATE);
        builder.addFlags(file.getAbsolutePath());

        String cmd = builder.build();
        CommandResult result = Shell.exec(mContext, file.getParent(), cmd);
        if (DLog.DEBUG) DLog.d(TAG, "result = " + result);
    }

    @Override
    public void analyze(File[] sources) {

    }

    public void setEditor(EditAreaView editor) {
        editor.addTextChangedListener(new TextWatcher() {

            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {

            }

            @Override
            public void afterTextChanged(Editable s) {
                mHandler.removeCallbacks(mAnalyze);
                mHandler.postDelayed(mAnalyze, DELAY_TIME);
            }
        });
    }
}
