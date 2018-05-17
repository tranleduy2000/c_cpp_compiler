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

package com.duy.ccppcompiler.compiler.compilers;

import android.content.Context;
import android.support.annotation.NonNull;

import com.duy.ccppcompiler.compiler.shell.CompileResult;
import com.jecelyin.common.utils.DLog;
import com.pdaxrom.packagemanager.EnvironmentPath;
import com.pdaxrom.utils.Utils;

import java.io.BufferedReader;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Created by Duy on 18-May-18.
 */
public abstract class NativeCompilerImpl implements INativeCompiler {
    private static final String TAG = "NativeCompilerImpl";

    private int mProcessId;
    private int mExitCode;

    @NonNull
    protected CompileResult execCommand(Context context, String mWorkDir, String mTmpDir,
                                        @NonNull String mTmpExeDir, @NonNull String mCommand) {
        if (DLog.DEBUG) DLog.d(TAG, "mWorkDir = " + mWorkDir);
        if (DLog.DEBUG) DLog.d(TAG, "mTmpDir = " + mTmpDir);
        if (DLog.DEBUG) DLog.d(TAG, "mTmpExeDir = " + mTmpExeDir);
        if (DLog.DEBUG) DLog.d(TAG, "mCommand = " + mCommand);

        long startTime = System.currentTimeMillis();
        try {
            String[] env = {"PWD=" + mWorkDir, "TMPDIR=" + mTmpDir, "TEMP=" + mTmpDir, "TMPEXEDIR=" + mTmpExeDir,};
            env = EnvironmentPath.join(env, EnvironmentPath.buildEnv(context));

            String[] argv = new String[]{"/system/bin/sh"};
            int[] processIds = new int[1];
            FileDescriptor fileDescriptor = Utils.createSubProcess(mWorkDir, argv[0], argv, env, processIds);
            mProcessId = processIds[0];
            if (mProcessId > 0) {
                Utils.setPtyUTF8Mode(fileDescriptor, true);
                Utils.setPtyWindowSize(fileDescriptor, 64, 128, 0, 0);
                BufferedReader processOut = new BufferedReader(new InputStreamReader(new FileInputStream(fileDescriptor)));
                FileOutputStream outputStream = new FileOutputStream(fileDescriptor);

                Thread execThread = new Thread() {
                    @Override
                    public void run() {
                        DLog.d(TAG, "Waiting for hangup session");
                        mExitCode = Utils.waitFor(mProcessId);
                        DLog.d(TAG, "Subprocess exited: " + mExitCode);
                    }
                };
                execThread.start();

                outputStream.write("export PS1=''\n".getBytes());
                mCommand = "exec " + mCommand + "\n";
                outputStream.write(mCommand.getBytes());


                //parse output
                StringBuilder message = new StringBuilder();
                final Pattern patClearNewLine = Pattern.compile("(\\x08)\\1+");
                do {
                    String errstr;
                    try {
                        errstr = processOut.readLine();
                        // remove escape sequence
                        errstr = errstr.replaceAll("\u001b\\[([0-9]|;)*m", "");
                        // remove clearing new line
                        Matcher m = patClearNewLine.matcher(errstr);
                        if (m.find()) {
                            int length = m.end() - m.start();
                            if (m.start() > length) {
                                errstr = errstr.substring(0, m.start() - length) + errstr.substring(m.end());
                            }
                        }
                    } catch (IOException e) {
                        break;
                    }
                    message.append(errstr).append("\n");
                } while (execThread.isAlive());

                if (DLog.DEBUG) DLog.d(TAG, "stdout: " + message);


                outputStream.close();
                processOut.close();

                CompileResult compileResult = new CompileResult(mExitCode, message.toString());
                long time = System.currentTimeMillis() - startTime;
                compileResult.setTime(time);
                return compileResult;
            }
        } catch (Exception ie) {
            ie.printStackTrace();
            return new CompileResult(-1, ie.getMessage());
        }
        return new CompileResult(-1, "");
    }

    public void destroy() {
        Utils.hangupProcessGroup(mProcessId);
    }
}
