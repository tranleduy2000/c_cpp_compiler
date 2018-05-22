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

import com.duy.ccppcompiler.compiler.shell.CommandResult;
import com.duy.ccppcompiler.compiler.shell.ShellUtils;
import com.jecelyin.common.utils.DLog;

/**
 * Created by Duy on 18-May-18.
 */
public abstract class NativeCompilerImpl<T extends CommandResult> implements INativeCompiler<T> {
    private static final String TAG = "NativeCompilerImpl";

    @NonNull
    protected CommandResult execCommand(@NonNull Context context, @NonNull String mWorkDir,
                                        @NonNull String mCommand) {
        if (DLog.DEBUG) DLog.d(TAG, "mWorkDir = " + mWorkDir);
        if (DLog.DEBUG) {
            String[] split = mCommand.split("\\s+");
            for (String s : split) {
                if (DLog.DEBUG) DLog.w(TAG, s);
            }
        }
        return ShellUtils.execCommand(context, mWorkDir, mCommand);
    }

    @Override
    public void hangup() {

    }
}
