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

import android.os.Handler;
import android.util.Log;

import com.jecelyin.common.utils.DLog;
import com.pdaxrom.utils.Utils;

import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.util.Arrays;

import jackpal.androidterm.emulatorview.TermSession;
import jackpal.androidterm.emulatorview.UpdateCallback;

public class ShellTermSession extends TermSession {
    private static final String TAG = "ShellTermSession";
    private int mProcId;
    private FileDescriptor mFd;
    private Thread mWatcherThread;
    private Handler mMsgHandler;
    private UpdateCallback mUTF8ModeNotify = new UpdateCallback() {
        public void onUpdate() {
            Utils.setPtyUTF8Mode(mFd, getUTF8Mode());
        }
    };

    public ShellTermSession(String[] argv, String envp[], String cwd, Handler handler) {
        super();
        if (DLog.DEBUG) {
            DLog.d(TAG, "ShellTermSession() called with: argv = [" + Arrays.toString(argv) +
                    "], envp = [" + Arrays.toString(envp) + "], cwd = [" + cwd + "], handler = [" + handler + "]");
        }

        mMsgHandler = handler;

        setDefaultUTF8Mode(true);

        createSubProcess(argv, envp, cwd);

        mWatcherThread = new Thread() {
            @Override
            public void run() {
                Log.i(TAG, "waiting for: " + mProcId);
                int result = Utils.waitFor(mProcId);
                Log.i(TAG, "Subprocess exited: " + result);
                //mMsgHandler.sendMessage(mMsgHandler.obtainMessage(PROCESS_EXITED, result));
                mMsgHandler.sendEmptyMessage(123);
            }
        };
        mWatcherThread.setName("Process watcher");
    }

    @Override
    public void finish() {
        Log.i(TAG, "finish()");
        Utils.hangupProcessGroup(mProcId);
        Utils.close(mFd);
        super.finish();
    }

    public void hangup() {
        Utils.hangupProcessGroup(mProcId);
    }

    public int getPid() {
        return mProcId;
    }

    private void createSubProcess(String[] argv, String[] envp, String cwd) {
        int[] pId = new int[1];
        String cmd = argv[0];
        /* detect shell login */
        if (cmd.startsWith("-")) {
            cmd = cmd.substring(1);
            int lastSlash = cmd.lastIndexOf("/");
            if (lastSlash > 0 && lastSlash < cmd.length() - 1) {
                argv[0] = "-" + cmd.substring(lastSlash + 1);
            }
        }
        mFd = Utils.createSubProcess(cwd, cmd, argv, envp, pId);
        mProcId = pId[0];
        if (mProcId > 0) {
            setTermIn(new FileInputStream(mFd));
            setTermOut(new FileOutputStream(mFd));
        }
    }

    @Override
    public void initializeEmulator(int columns, int rows) {
        super.initializeEmulator(columns, rows);

        Utils.setPtyUTF8Mode(mFd, getUTF8Mode());
        setUTF8ModeUpdateCallback(mUTF8ModeNotify);

        mWatcherThread.start();
    }

    @Override
    public void updateSize(int columns, int rows) {
        Utils.setPtyWindowSize(mFd, rows, columns, 0, 0);
        super.updateSize(columns, rows);
    }
}
