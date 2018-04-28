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

package com.duy.ccppcompiler.console.services;

import android.annotation.SuppressLint;
import android.app.Service;
import android.content.Intent;
import android.net.Uri;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;

import com.termux.terminal.EmulatorDebug;
import com.termux.terminal.TerminalSession;
import com.termux.terminal.TerminalSession.SessionChangedCallback;

import java.io.File;

public final class TermuxService extends Service implements SessionChangedCallback {

    /**
     * Intent action to launch a new terminal session. Executed from TermuxWidgetProvider.
     */
    public static final String ACTION_EXECUTE = "com.termux.service_execute";

    private static final String EXTRA_ARGUMENTS = "com.termux.execute.arguments";
    private static final String EXTRA_CURRENT_WORKING_DIRECTORY = "com.termux.execute.cwd";
    private static final String ACTION_STOP_SERVICE = "com.termux.service_stop";
    private static final String EXTRA_EXECUTE_IN_BACKGROUND = "com.termux.execute.background";


    private final IBinder mBinder = new LocalBinder();
    public SessionChangedCallback mSessionChangeCallback;
    private TerminalSession mTerminalSessions = null;

    @Override
    public void onCreate() {
    }

    @SuppressLint("Wakelock")
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {

        String action = intent.getAction();
        if (ACTION_STOP_SERVICE.equals(action)) {
            mTerminalSessions.finishIfRunning();
            stopSelf();
        } else if (ACTION_EXECUTE.equals(action)) {
            Uri executableUri = intent.getData();
            String executablePath = (executableUri == null ? null : executableUri.getPath());

            String[] arguments = (executableUri == null ? null : intent.getStringArrayExtra(EXTRA_ARGUMENTS));
            String cwd = intent.getStringExtra(EXTRA_CURRENT_WORKING_DIRECTORY);

            if (intent.getBooleanExtra(EXTRA_EXECUTE_IN_BACKGROUND, false)) {
                BackgroundJob task = new BackgroundJob(cwd, executablePath, arguments, this);
            } else {
                mTerminalSessions = createTermSession(executablePath, arguments, cwd);//true使用系统环境变量

                // Transform executable path to session name, e.g. "/bin/do-something.sh" => "do something.sh".
                if (executablePath != null) {
                    int lastSlash = executablePath.lastIndexOf('/');
                    String name = (lastSlash == -1) ? executablePath : executablePath.substring(lastSlash + 1);
                    name = name.replace('-', ' ');
                    mTerminalSessions.mSessionName = name;
                }

                // Make the newly created session the current one to be displayed:

                // Launch the main Termux app, which will now show the current session:
            }
        } else if (action != null) {
            Log.e(EmulatorDebug.LOG_TAG, "Unknown TermuxService action: '" + action + "'");
        }


        // If this service really do get killed, there is no point restarting it automatically - let the user do on next
        // start of {@link Term):
        return Service.START_NOT_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }


    public TerminalSession getTermSession() {
        return mTerminalSessions;
    }

    @Override
    public void onDestroy() {
        stopForeground(true);
        //System.out.println("TermuxService:onDestroy");
        mTerminalSessions.finishIfRunning();

    }

    private TerminalSession createTermSession(String executablePath, String[] arguments, String cwd) {
        File f = new File(EnvironmentVariable.getHomePath(this));
        if (!f.exists())
            f.mkdirs();

        if (cwd == null)
            cwd = EnvironmentVariable.getHomePath(this);

        String[] env = BackgroundJob.buildEnvironment(true, cwd, this);
        boolean isLoginShell = false;

        if (executablePath == null) {
            for (String shellBinary : new String[]{"login", "bash", "zsh"}) {
                File shellFile = new File(EnvironmentVariable.getPrefixPath(this) + "/bin/" + shellBinary);
                if (shellFile.canExecute()) {
                    executablePath = shellFile.getAbsolutePath();
                    break;
                }
            }

            if (executablePath == null) {
                // Fall back to system shell as last resort:
                executablePath = "/system/bin/sh";
            }
            isLoginShell = true;
        }

        String[] processArgs = BackgroundJob.setupProcessArgs(executablePath, arguments, this);
        executablePath = processArgs[0];
        int lastSlashIndex = executablePath.lastIndexOf('/');
        String processName = (isLoginShell ? "-" : "") +
                (lastSlashIndex == -1 ? executablePath : executablePath.substring(lastSlashIndex + 1));

        String[] args = new String[processArgs.length];
        args[0] = processName;
        if (processArgs.length > 1)
            System.arraycopy(processArgs, 1, args, 1, processArgs.length - 1);

        return new TerminalSession(executablePath, cwd, args, env, this);
    }

    @Override
    public void onTitleChanged(TerminalSession changedSession) {
        if (mSessionChangeCallback != null) mSessionChangeCallback.onTitleChanged(changedSession);
    }

    @Override
    public void onSessionFinished(final TerminalSession finishedSession) {
        if (mSessionChangeCallback != null)
            mSessionChangeCallback.onSessionFinished(finishedSession);
    }

    @Override
    public void onTextChanged(TerminalSession changedSession) {
        if (mSessionChangeCallback != null) mSessionChangeCallback.onTextChanged(changedSession);
    }

    @Override
    public void onClipboardText(TerminalSession session, String text) {
        if (mSessionChangeCallback != null) mSessionChangeCallback.onClipboardText(session, text);
    }

    @Override
    public void onBell(TerminalSession session) {
        if (mSessionChangeCallback != null) mSessionChangeCallback.onBell(session);
    }

    @Override
    public void onColorsChanged(TerminalSession session) {
        if (mSessionChangeCallback != null) mSessionChangeCallback.onColorsChanged(session);
    }

    public void onBackgroundJobExited(final BackgroundJob task) {
        stopSelf();
    }

    /**
     * This service is only bound from inside the same process and never uses IPC.
     */
    public class LocalBinder extends Binder {
        public final TermuxService service = TermuxService.this;
    }
}
