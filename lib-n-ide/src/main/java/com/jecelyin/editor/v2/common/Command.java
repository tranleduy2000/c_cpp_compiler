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

package com.jecelyin.editor.v2.common;

import android.os.Bundle;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class Command {
    public final Bundle args = new Bundle();
    public final CommandEnum what;
    public Object object;

    public Command(CommandEnum what) {
        this.what = what;
    }

    public enum CommandEnum {
        NONE,
        HIDE_SOFT_INPUT,
        SHOW_SOFT_INPUT,
        SAVE,
        SAVE_AS,
        OPEN,
        REDO,
        UNDO,
        CUT,
        COPY,
        PASTE,
        SELECT_ALL,
        DUPLICATION,
        GOTO_INDEX,
        FIND,
        GOTO_TOP,
        GOTO_END,
        DOC_INFO,
        READONLY_MODE,
        HIGHLIGHT,
        COMMENT,
        INSERT_TEXT,
        RELOAD_WITH_ENCODING,
        REQUEST_FOCUS,
        HIGHLIGHT_ERROR,
        SHARE_CODE,
        FORMAT_SOURCE,
        CLEAR_ERROR,
        CLEAR_WARNING,
        REFRESH_THEME
    }
}
