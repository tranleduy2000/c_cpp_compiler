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

package com.duy.ide;

/**
 * https://gcc.gnu.org/onlinedocs/gcc-7.2.0/gcc/Diagnostic-Message-Formatting-Options.html
 * https://gcc.gnu.org/onlinedocs/gcc-3.3.5/gnat_ug_unx/Output-and-Error-Message-Control.html
 */
public enum Kind {
    /**
     * Problem which prevents the tool's normal completion.
     */
    ERROR("error"),
    /**
     * Problem which does not usually prevent the tool from
     * completing normally.
     */
    WARNING("warning"),
    /**
     * Informative message from the tool.
     */
    NOTE("note"),
    /**
     * Diagnostic which does not fit within the other kinds.
     */
    OTHER("other"),;

    private String name;

    Kind(String name) {
        this.name = name;
    }

    public String getName() {
        return name;
    }
}