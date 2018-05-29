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

package com.duy.ccppcompiler.compiler;

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
                internalBinary = new File(Environment.getTmpExeDir(getActivity()), binFile.getName());
                FileInputStream inputStream = new FileInputStream(binFile);
                FileOutputStream outputStream = new FileOutputStream(internalBinary);
                IOUtils.copy(inputStream, outputStream);
                inputStream.close();
                outputStream.close();

                //set executable
                Utils.chmod(internalBinary.getAbsolutePath(), 0x1ed/*0775*/);
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

}
