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

import android.support.annotation.Nullable;

public class CommandResult {

    private String message;
    private int resultCode;
    private long time = 0;

    public CommandResult(int resultCode) {
        this.resultCode = resultCode;
    }

    public CommandResult(int resultCode, @Nullable String message) {
        this.resultCode = resultCode;
        this.message = message;
    }

    public CommandResult(CommandResult commandResult) {
        this(commandResult.getResultCode(), commandResult.getMessage());
    }

    @Override
    public String toString() {
        return "CommandResult{" +
                "message='" + message + '\'' +
                ", resultCode=" + resultCode +
                ", time=" + time +
                '}';
    }

    public long getTime() {
        return time;
    }

    public void setTime(long time) {
        this.time = time;
    }

    public int getResultCode() {
        return resultCode;
    }

    public void setResultCode(int resultCode) {
        this.resultCode = resultCode;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }
}