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

package com.duy.ccppcompiler.compiler.shell;

import android.content.Context;

import com.duy.ccppcompiler.packagemanager.Environment;
import com.jecelyin.common.utils.DLog;
import com.pdaxrom.utils.Utils;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileDescriptor;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class Shell {

    private static final String TAG = "ShellUtils";

    private Shell() {
    }

    public static CommandResult exec(Context context, String mCommand) {
        return exec(context, Environment.getHomeDir(context), mCommand);
    }

    public static CommandResult exec(Context context, File file) {
        if (!file.canExecute()) {
            Utils.chmod(file.getAbsolutePath(), 0775);
        }
        return exec(context, file.getParent(), file.getPath());
    }

    /**
     * @param context - Android context for build environments variable
     * @param cwd     - Current working directory
     * @param cmd     - Command to be executed
     * @return output from process
     */
    public static CommandResult exec(Context context, String cwd, String cmd) {
        if (DLog.DEBUG) DLog.d(TAG, "mCommand = " + cmd);
        long startTime = System.currentTimeMillis();
        try {
            String[] env = Environment.buildDefaultEnv(context);
            String[] argv = new String[]{"/system/bin/sh"};

            int[] processIds = new int[1];
            FileDescriptor fd = Utils.createSubProcess(cwd, argv[0], argv, env, processIds);
            final int processId = processIds[0];
            if (processId <= 0) {
                return new CommandResult(-1, "Could not create sub process");
            }

            Utils.setPtyUTF8Mode(fd, true);
            Utils.setPtyWindowSize(fd, 128, 1024, 0, 0);

            BufferedReader input = new BufferedReader(new FileReader(fd));
            FileOutputStream output = new FileOutputStream(fd);

            output.write(("export PS1=''\n").getBytes("UTF-8"));
            output.write(("exec " + cmd + "\n").getBytes("UTF-8"));
            output.flush();

            final int[] exitCode = new int[1];
            Thread thread = new Thread(new Runnable() {
                @Override
                public void run() {
                    DLog.d(TAG, "Waiting for hangup session");
                    exitCode[0] = Utils.waitFor(processId);
                    DLog.d(TAG, "Subprocess exited: " + exitCode[0]);
                }
            });
            thread.start();

            //parse output
            StringBuilder message = new StringBuilder();
            int skipLine = 6; //skip echos from two command strings
            final Pattern patClearNewLine = Pattern.compile("(\\x08)\\1+");
            do {
                String errstr;
                try {
                    errstr = input.readLine();
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
                if (skipLine > 0) {
                    skipLine--;
                } else {
                    message.append(errstr).append("\n");
                }
            } while (thread.isAlive());

            if (DLog.DEBUG) DLog.d(TAG, "stdout: \n" + message);

            output.close();
            input.close();

            CommandResult commandResult = new CommandResult(exitCode[0], message.toString());
            long time = System.currentTimeMillis() - startTime;
            if (DLog.DEBUG) DLog.d(TAG, "time = " + time);
            commandResult.setTime(time);
            return commandResult;

        } catch (Throwable ie) {
            ie.printStackTrace();
            return new CommandResult(-1, ie.getMessage());
        }
    }
}