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

package com.duy.ide.compiler.shell;

import com.jecelyin.common.utils.DLog;

import java.io.BufferedReader;
import java.io.DataOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.List;
import java.util.Map;


public class ShellUtils {

    private static final String COMMAND_SU = "su";
    private static final String COMMAND_SH = "sh";
    private static final String COMMAND_EXIT = "exit\n";
    private static final String COMMAND_LINE_END = "\n";
    private static final String TAG = "ShellUtils";


    private ShellUtils() {
        throw new AssertionError();
    }

    public static boolean checkRootPermission() {
        return execCommand(true, false).resultCode == 0;
    }

    public static ShellResult execCommand(String command) {
        return execCommand(new String[]{command}, false, true);
    }

    public static ShellResult execCommand(List<String> commands, boolean isRoot) {
        return execCommand(commands == null ? null : commands.toArray(new String[]{}), isRoot, true);
    }

    public static ShellResult execCommand(String[] commands, boolean isRoot) {
        return execCommand(commands, isRoot, true);
    }

    private static ShellResult execCommand(boolean isRoot, boolean isNeedResultMsg) {
        return execCommand(new String[]{"echo root"}, isRoot, isNeedResultMsg);
    }

    public static ShellResult execCommand(List<String> commands, boolean isRoot, boolean isNeedResultMsg) {
        return execCommand(commands == null ? null : commands.toArray(new String[]{}), isRoot, isNeedResultMsg);
    }

    private static ShellResult execCommand(String[] commands, boolean isRoot, boolean isNeedResultMsg) {
        int result = -1;
        if (commands == null || commands.length == 0) {
            return new ShellResult(result, null);
        }

        Process process = null;
        BufferedReader successResult = null;
        //BufferedReader errorResult = null;
        StringBuilder msg = null;
        // StringBuilder errorMsg = null;

        DataOutputStream os = null;
        try {
            process = Runtime.getRuntime().exec(isRoot ? COMMAND_SU : COMMAND_SH, null);
            os = new DataOutputStream(process.getOutputStream());
            for (String command : commands) {
                if (command == null) {
                    continue;
                }
                // donnot use os.writeBytes(commmand), avoid chinese charset error
                os.write(command.getBytes());
                os.writeBytes(COMMAND_LINE_END);
                os.flush();
            }
            os.writeBytes(COMMAND_EXIT);
            os.flush();

            result = process.waitFor();
            // get command result
            if (isNeedResultMsg) {
                msg = new StringBuilder();
                //errorMsg = new StringBuilder();
                successResult = new BufferedReader(new InputStreamReader(process.getInputStream()));
                //errorResult = new BufferedReader(new InputStreamReader(process.getErrorStream()));
                String s = null;
                while ((s = successResult.readLine()) != null) {
                    msg.append(s);
                    msg.append("\n");
                }

            }
        } catch (Exception e) {
            e.printStackTrace();
            if (msg != null)
                msg.append(e.toString());
        } finally {
            try {
                if (os != null) {
                    os.close();
                }
                if (successResult != null) {
                    successResult.close();
                }

            } catch (IOException e) {
                e.printStackTrace();
            }

            if (process != null) {
                process.destroy();
            }
        }
        return new ShellResult(result, msg == null ? null : msg.toString());
    }

    public static ShellResult execCommand(String command, List<String> args, Map<String, String> env) {
        int result = -1;
        if (command == null || command.length() == 0) {
            return new ShellResult(result, null);
        }
        long timeStart = System.currentTimeMillis();

        Process process = null;
        BufferedReader reader = null;
        StringBuilder message = null;
        try {
            ProcessBuilder builder = new ProcessBuilder(command);

            List<String> argsList = builder.command();
            argsList.addAll(args);

            builder.redirectErrorStream(true);//合并输入流和错误流

            Map<String, String> map = builder.environment();
            //map.clear();
            for (Map.Entry<String, String> entry : env.entrySet()) {
                map.put(entry.getKey(), entry.getValue());
            }
            process = builder.start();
            result = process.waitFor();


            message = new StringBuilder();
            reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            String line;
            while ((line = reader.readLine()) != null) {
                message.append(line);
                message.append("\n");
            }
        } catch (Exception e) {
            if (DLog.DEBUG) DLog.e(TAG, "execCommand: ", e);
            if (message != null) {
                message.append(e.toString());
            }
        } finally {
            try {
                if (reader != null) {
                    reader.close();
                }
            } catch (IOException e) {
                if (DLog.DEBUG) DLog.e(TAG, "execCommand: ", e);
            }

            if (process != null) {
                process.destroy();
            }
        }
        long timeEnd = System.currentTimeMillis();
        ShellResult shellResult = new ShellResult(result, message == null ? "" : message.toString());
        shellResult.setTime(timeEnd - timeStart);
        return shellResult;
    }

}