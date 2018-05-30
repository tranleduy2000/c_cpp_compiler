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

package com.duy.ccppcompiler.compiler.manager;

import android.app.NativeActivity;
import android.content.Intent;
import android.widget.Toast;

import com.duy.ccppcompiler.compiler.shell.CommandResult;
import com.duy.ccppcompiler.compiler.shell.CompileResult;
import com.duy.ccppcompiler.compiler.shell.NativeActivityCompileResult;
import com.duy.ccppcompiler.console.TermActivity;
import com.duy.ccppcompiler.packagemanager.Environment;
import com.duy.editor.CodeEditorActivity;
import com.pdaxrom.cctools.BuildConstants;
import com.pdaxrom.utils.Utils;

import org.apache.commons.io.IOUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

/**
 * Created by Duy on 25-Apr-18.
 */

public class CompileManager extends CompileManagerImpl {

    public CompileManager(CodeEditorActivity activity) {
        super(activity);
    }

    @Override
    public void onCompileSuccess(CommandResult result) {
        super.onCompileSuccess(result);
        File internalBinary = null;
        if (result instanceof CompileResult) {
            File binFile = ((CompileResult) result).getBinaryFile();
            if (binFile == null) {
                return;
            }

            //copy to internal storage
            try {
                internalBinary = copyToTmpExeDirAndSetExecutable(binFile);

            } catch (Exception e) {
                e.printStackTrace();
                Toast.makeText(getActivity(), String.format("Internal error: %s. Please report bug on GitHub",
                        e.getMessage()), Toast.LENGTH_SHORT).show();
                return;
            }
        }

        if (result instanceof NativeActivityCompileResult) {
            //now run binary file
            Intent intent = new Intent(getActivity(), NativeActivity.class);
            //from jni: main.cpp
            intent.putExtra("nativeApp", internalBinary.getAbsolutePath());
            getActivity().startActivity(intent);

        } else if (result instanceof CompileResult) {

            Intent intent = new Intent(getActivity(), TermActivity.class);
            intent.putExtra(BuildConstants.EXTRA_FILE_NAME, internalBinary.getAbsolutePath());
            intent.putExtra(BuildConstants.EXTRA_WORK_DIR, internalBinary.getParent());
            getActivity().startActivity(intent);
        }
    }

    protected File copyToTmpExeDirAndSetExecutable(File binFile) throws IOException {
        File internalBinary = new File(Environment.getTmpExeDir(getActivity()), binFile.getName());
        FileInputStream inputStream = new FileInputStream(binFile);
        FileOutputStream outputStream = new FileOutputStream(internalBinary);
        IOUtils.copy(inputStream, outputStream);
        inputStream.close();
        outputStream.close();

        //set executable
        Utils.chmod(internalBinary.getAbsolutePath(), 0x1ed/*0775*/);

        return internalBinary;
    }

}
