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

import android.content.Context;
import android.support.annotation.NonNull;

import com.duy.ccppcompiler.compiler.shell.ShellResult;
import com.jecelyin.common.utils.DLog;
import com.pdaxrom.packagemanager.EnvironmentPath;
import com.pdaxrom.utils.Utils;

import java.io.BufferedReader;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStreamReader;

/**
 * Created by Duy on 18-May-18.
 */
public abstract class NativeCompilerImpl implements INativeCompiler {
    private static final String TAG = "NativeCompilerImpl";

    private int mProcessId;

    @NonNull
    protected ShellResult execCommand(Context context, String mWorkDir, String mTmpDir,
                                      @NonNull String mTmpExeDir, @NonNull String mCommand) {
        if (DLog.DEBUG)
            DLog.d(TAG, "execCommand() called with: context = [" + context + "], mWorkDir = ["
                    + mWorkDir + "], mTmpDir = [" + mTmpDir + "], mTmpExeDir = [" + mTmpExeDir + "], mCommand = [" + mCommand + "]");

        try {
            int mExitCode;
            String[] workingEnv = {
                    "PWD=" + mWorkDir,
                    "TMPDIR=" + mTmpDir,
                    "TEMP=" + mTmpDir,
                    "TMPEXEDIR=" + mTmpExeDir,
            };
            String[] defaultEnv = EnvironmentPath.buildEnv(context);
            workingEnv = EnvironmentPath.join(workingEnv, defaultEnv);

            String[] argv = new String[]{"/system/bin/sh"};
            int[] processIds = new int[1];
            FileDescriptor fileDescriptor = Utils.createSubProcess(mWorkDir, argv[0], argv, workingEnv, processIds);
            mProcessId = processIds[0];
            if (mProcessId > 0) {
                BufferedReader reader = new BufferedReader(new InputStreamReader(new FileInputStream(fileDescriptor)));
                FileOutputStream outputStream = new FileOutputStream(fileDescriptor);

                DLog.d(TAG, "Waiting for hangup session");
                mExitCode = Utils.waitFor(mProcessId);
                DLog.d(TAG, "Subprocess exited: " + mExitCode);

                outputStream.write("export PS1=''\n".getBytes());
                mCommand = "exec " + mCommand + "\n";
                outputStream.write(mCommand.getBytes());

                //use string builder to avoid unnecessary string creation.
                StringBuilder message = new StringBuilder();
                int charsRead = -1;
                char[] chars = new char[100];
                do {
                    charsRead = reader.read(chars, 0, chars.length);
                    //if we have valid chars, append them to end of string.
                    if (charsRead > 0) {
                        message.append(chars, 0, charsRead);
                    }
                } while (charsRead > 0);

                outputStream.close();
                reader.close();

                return new ShellResult(mExitCode, message.toString());
            }
        } catch (Exception ie) {
            ie.printStackTrace();
        }
        return new ShellResult(-1);
    }

    public void destroy() {
        Utils.hangupProcessGroup(mProcessId);
    }
}
