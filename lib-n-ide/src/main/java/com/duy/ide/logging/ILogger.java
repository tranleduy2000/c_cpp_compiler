/*
 * Copyright (C) 2018 Tran Le Duy
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package com.duy.ide.logging;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.annotation.WorkerThread;

/**
 * Interface used to display warnings/errors while parsing the SDK content.
 * <p/>
 * There are a few default implementations available:
 * <ul>
 * <li> {@link NullLogger} is an implementation that does <em>nothing</em> with the log.
 * Useful for limited cases where you need to call a class that requires a non-null logging
 * yet the calling code does not have any mean of reporting logs itself. It can be
 * acceptable for use as a temporary implementation but most of the time that means the caller
 * code needs to be reworked to take a logger object from its own caller.
 * </li>
 * <li> {@link StdLogger} is an implementation that dumps the log to {@link System#out} or
 * {@link System#err}. This is useful for unit tests or code that does not have any GUI.
 * GUI based apps based should not use it and should provide a better way to report to the user.
 * </li>
 * </ul>
 */
public interface ILogger {

    /**
     * Prints an error message.
     *
     * @param t         is an optional {@link Throwable} or {@link Exception}. If non-null, its
     *                  message will be printed out.
     * @param msgFormat is an optional error format. If non-null, it will be printed
     *                  using a {@link Formatter} with the provided arguments.
     * @param args      provides the arguments for errorFormat.
     */
    @WorkerThread
    void error(@Nullable Throwable t, @Nullable String msgFormat, Object... args);

    /**
     * Prints a warning message.
     *
     * @param msgFormat is a string format to be used with a {@link Formatter}. Cannot be null.
     * @param args      provides the arguments for warningFormat.
     */
    @WorkerThread
    void warning(@NonNull String msgFormat, Object... args);

    /**
     * Prints an information message.
     *
     * @param msgFormat is a string format to be used with a {@link Formatter}. Cannot be null.
     * @param args      provides the arguments for msgFormat.
     */
    @WorkerThread
    void info(@NonNull String msgFormat, Object... args);

    /**
     * Prints a verbose message.
     *
     * @param msgFormat is a string format to be used with a {@link Formatter}. Cannot be null.
     * @param args      provides the arguments for msgFormat.
     */
    @WorkerThread
    void verbose(@NonNull String msgFormat, Object... args);

}
