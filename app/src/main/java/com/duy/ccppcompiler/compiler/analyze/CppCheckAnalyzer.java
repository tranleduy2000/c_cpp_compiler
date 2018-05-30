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
import android.os.Handler;
import android.support.annotation.NonNull;
import android.text.Editable;
import android.text.TextWatcher;

import com.duy.ccppcompiler.compiler.shell.CommandBuilder;
import com.duy.ccppcompiler.compiler.shell.CommandResult;
import com.duy.ccppcompiler.compiler.shell.OutputParser;
import com.duy.ccppcompiler.compiler.shell.Shell;
import com.duy.common.DLog;
import com.duy.ide.diagnostic.Diagnostic;
import com.duy.ide.diagnostic.DiagnosticPresenter;
import com.duy.ide.diagnostic.DiagnosticsCollector;
import com.jecelyin.editor.v2.editor.IEditorDelegate;
import com.jecelyin.editor.v2.io.LocalFileWriter;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
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
    private final IEditorDelegate mEditorDelegate;
    private final android.os.Handler mHandler = new Handler();
    private DiagnosticPresenter mDiagnosticPresenter;
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

    public CppCheckAnalyzer(@NonNull Context context, @NonNull IEditorDelegate delegate, DiagnosticPresenter diagnosticPresenter) {
        mContext = context;
        mEditorDelegate = delegate;
        mDiagnosticPresenter = diagnosticPresenter;
    }

    private void analyzeImpl() throws IOException {
        File originFile = mEditorDelegate.getDocument().getFile();
        File tmpFile = new File(getCppCheckTmpDir(), originFile.getName());

        LocalFileWriter localFileWriter = new LocalFileWriter(tmpFile, "UTF-8");
        localFileWriter.writeToFile(mEditorDelegate.getEditText().getText());

        CommandBuilder builder = new CommandBuilder(CPPCHECK_PROGRAM);
        builder.addFlags(TEMPLATE);
        builder.addFlags(tmpFile.getAbsolutePath());

        String cmd = builder.build();
        CommandResult result = Shell.exec(mContext, tmpFile.getParent(), cmd);
        if (DLog.DEBUG) DLog.d(TAG, "result = " + result);
        String message = result.getMessage().replace(tmpFile.getAbsolutePath(), originFile.getAbsolutePath());

        DiagnosticsCollector diagnosticsCollector = new DiagnosticsCollector();
        OutputParser parser = new OutputParser(diagnosticsCollector);
        parser.parse(message);

        ArrayList<Diagnostic> diagnostics = diagnosticsCollector.getDiagnostics();
        mDiagnosticPresenter.setDiagnostics(diagnostics);
    }

    private File getCppCheckTmpDir() {
        File dir = new File(mContext.getCacheDir(), "cppcheck/tmp");
        if (!dir.exists()) {
            dir.mkdirs();
        }
        return dir;
    }

    @Override
    public void start() {
        mEditorDelegate.getEditText().addTextChangedListener(new TextWatcher() {

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
