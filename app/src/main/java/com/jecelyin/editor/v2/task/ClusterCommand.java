/*
 * Copyright (C) 2016 Jecelyin Peng <jecelyin@gmail.com>
 *
 * This file is part of 920 Text Editor.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.jecelyin.editor.v2.task;

import android.support.annotation.Nullable;

import com.jecelyin.editor.v2.common.Command;
import com.jecelyin.editor.v2.ui.editor.EditorDelegate;

import java.util.ArrayList;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class ClusterCommand {
    @Nullable
    private ArrayList<EditorDelegate> delegates;
    private Command command;

    public ClusterCommand(@Nullable ArrayList<EditorDelegate> delegates) {
        this.delegates = delegates;
    }

    public void setCommand(Command command) {
        this.command = command;
    }

    public void doNextCommand() {
        if (delegates == null || delegates.size() == 0) {
            return;
        }
        EditorDelegate delegate = delegates.remove(0);
        if (delegate == null || !delegate.doCommand(command)) {
            doNextCommand();
        }
    }

    public boolean hasNextCommand() {
        return delegates != null && delegates.size() > 0;
    }
}
