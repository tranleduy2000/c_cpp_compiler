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

package com.duy.ccppcompiler.console;

import android.annotation.SuppressLint;
import android.os.Handler;
import android.os.Message;
import android.util.Log;

import com.duy.common.DLog;
import com.pdaxrom.utils.Utils;

import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.UnsupportedEncodingException;
import java.util.Arrays;

import jackpal.androidterm.emulatorview.ColorScheme;
import jackpal.androidterm.emulatorview.TermSession;
import jackpal.androidterm.emulatorview.UpdateCallback;
import jackpal.androidterm.TermSettings;

public class ShellTermSession extends TermSession {
    public static final int PROCESS_EXITED = 1;
    private static final String TAG = "ShellTermSession";
    //** Set to true to force into 80 x 24 for testing with vttest. */
    private static final boolean VTTEST_MODE = false;

    private int mProcId;
    private FileDescriptor mTermFd;
    private Thread mWatcherThread;

    private TermSettings mSettings;
    private String mProcessExitMessage;
    private String mInitialCommand;

    private UpdateCallback mUTF8ModeNotify = new UpdateCallback() {
        public void onUpdate() {
            setPtyUTF8Mode(getUTF8Mode());
        }
    };

    @SuppressLint("HandlerLeak")
    private Handler mMsgHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            if (!isRunning()) {
                return;
            }
            if (msg.what == PROCESS_EXITED) {
                onProcessExit((Integer) msg.obj);
            }
        }
    };

    public ShellTermSession(TermSettings settings, String[] argv, String[] envp, String cwd) {
        super();
        if (DLog.DEBUG) DLog.d(TAG, "argv = " + Arrays.toString(argv));
        if (DLog.DEBUG) DLog.d(TAG, "envp = " + Arrays.toString(envp));
        if (DLog.DEBUG) DLog.d(TAG, "cwd = " + cwd);

        updatePrefs(settings);
        createSubProcess(argv, envp, cwd);

        mWatcherThread = new Thread() {
            @Override
            public void run() {
                Log.i(TAG, "waiting for: " + mProcId);
                int result = Utils.waitFor(mProcId);
                Log.i(TAG, "Subprocess exited: " + result);
                mMsgHandler.sendMessage(mMsgHandler.obtainMessage(PROCESS_EXITED, result));
            }
        };
        mWatcherThread.setName("Process watcher");

    }

    public void updatePrefs(TermSettings settings) {
        mSettings = settings;
        setColorScheme(new ColorScheme(settings.getColorScheme()));
        setDefaultUTF8Mode(settings.defaultToUTF8Mode());
    }

    public int getPid() {
        return mProcId;
    }

    /**
     * @param arg - argument(s)
     * @param env - environment variables
     * @param cwd - current working directory
     */
    private void createSubProcess(String[] arg, String[] env, String cwd) {
        int[] pId = new int[1];
        String cmd = arg[0];
        /* detect shell login */
        if (cmd.startsWith("-")) {
            cmd = cmd.substring(1);
            int lastSlash = cmd.lastIndexOf("/");
            if (lastSlash > 0 && lastSlash < cmd.length() - 1) {
                arg[0] = "-" + cmd.substring(lastSlash + 1);
            }
        }
        mTermFd = Utils.createSubProcess(cwd, cmd, arg, env, pId);
        mProcId = pId[0];
        if (mProcId > 0) {
            setTermIn(new FileInputStream(mTermFd));
            setTermOut(new FileOutputStream(mTermFd));
        }
    }

    @Override
    public void initializeEmulator(int columns, int rows) {
        if (VTTEST_MODE) {
            columns = 80;
            rows = 24;
        }
        super.initializeEmulator(columns, rows);

        setPtyUTF8Mode(getUTF8Mode());
        setUTF8ModeUpdateCallback(mUTF8ModeNotify);

        mWatcherThread.start();
        sendInitialCommand(mInitialCommand);

    }

    private void sendInitialCommand(String initialCommand) {
        if (initialCommand != null && initialCommand.length() > 0) {
            write(initialCommand + '\r');
        }
    }

    @Override
    public void updateSize(int columns, int rows) {
        if (VTTEST_MODE) {
            columns = 80;
            rows = 24;
        }
        // Inform the attached pty of our new size:
        setPtyWindowSize(rows, columns, 0, 0);
        super.updateSize(columns, rows);
    }

    private void onProcessExit(int result) {
        onProcessExit();
    }

    @Override
    public void finish() {
        hangupProcessGroup();
        super.finish();
    }

    /**
     * Send SIGHUP to a process group, SIGHUP notifies a terminal client, that the terminal have been disconnected,
     * and usually results in client's death, unless it's process is a daemon or have been somehow else detached
     * from the terminal (for example, by the "nohup" utility).
     */
    void hangupProcessGroup() {
        Utils.hangupProcessGroup(mProcId);
        Utils.close(mTermFd);
    }

    /* XXX We should really get this ourselves from the resource bundle, but
       we cannot hold a context */
    public void setProcessExitMessage(String message) {
        mProcessExitMessage = message;
    }

    @Override
    protected void onProcessExit() {
        if (mSettings.closeWindowOnProcessExit()) {
            finish();
        } else if (mProcessExitMessage != null) {
            try {
                byte[] msg = ("\r\n[" + mProcessExitMessage + "]").getBytes("UTF-8");
                appendToEmulator(msg, 0, msg.length);
                notifyUpdate();
            } catch (UnsupportedEncodingException e) {
                // Never happens
            }
        }
    }

    /**
     * Set the widow size for a given pty. Allows programs
     * connected to the pty learn how large their screen is.
     */
    void setPtyWindowSize(int row, int col, int xpixel, int ypixel) {
        // If the tty goes away too quickly, this may get called after it's descriptor is closed
        if (!mTermFd.valid())
            return;

        try {
            Utils.setPtyWindowSize(mTermFd, row, col, xpixel, ypixel);
        } catch (Exception e) {
            Log.e("exec", "Failed to set window size: " + e.getMessage());

            if (isFailFast())
                throw new IllegalStateException(e);
        }
    }

    /**
     * Set or clear UTF-8 mode for a given pty.  Used by the terminal driver
     * to implement correct erase behavior in cooked mode (Linux >= 2.6.4).
     */
    void setPtyUTF8Mode(boolean utf8Mode) {
        // If the tty goes away too quickly, this may get called after it's descriptor is closed
        if (!mTermFd.valid())
            return;

        try {
            Utils.setPtyUTF8Mode(mTermFd, utf8Mode);
        } catch (Exception e) {
            Log.e("exec", "Failed to set UTF mode: " + e.getMessage());
            if (isFailFast())
                throw new IllegalStateException(e);
        }
    }


    /**
     * @return true, if failing to operate on file descriptor deserves an exception (never the case for ATE own shell)
     */
    boolean isFailFast() {
        return false;
    }
}
