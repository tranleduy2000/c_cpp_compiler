/*
 *  Copyright (C)  2018  Duy Tran Le
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.duy.ide.diagnostic;

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