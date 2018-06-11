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

import java.io.PrintStream;
import java.util.Formatter;


/**
 * An implementation of {@link ILogger} that prints to {@link System#out} and {@link System#err}.
 * <p/>
 */
public class StdLogger implements ILogger {

    private final Level mLevel;

    /**
     * Creates the {@link StdLogger} with a given log {@link Level}.
     *
     * @param level the log Level.
     */
    public StdLogger(@NonNull Level level) {
        if (level == null) {
            throw new IllegalArgumentException("level cannot be null");
        }

        mLevel = level;
    }

    /**
     * Returns the logger's log {@link Level}.
     *
     * @return the log level.
     */
    public Level getLevel() {
        return mLevel;
    }

    /**
     * Prints an error message.
     * <p/>
     * The message will be tagged with "Error" on the output so the caller does not
     * need to put such a prefix in the format string.
     * <p/>
     * The output is done on {@link System#err}.
     * <p/>
     * This is always displayed, independent of the logging {@link Level}.
     *
     * @param t           is an optional {@link Throwable} or {@link Exception}. If non-null, it's
     *                    message will be printed out.
     * @param errorFormat is an optional error format. If non-null, it will be printed
     *                    using a {@link Formatter} with the provided arguments.
     * @param args        provides the arguments for errorFormat.
     */
    @Override
    public void error(@Nullable Throwable t, @Nullable String errorFormat, Object... args) {
        if (errorFormat != null) {
            String msg = String.format("Error: " + errorFormat, args);

            printMessage(msg, System.err);
        }
        if (t != null) {
//            System.err.println(String.format("Error: %1$s", t.getMessage()));
            t.printStackTrace();
        }
    }

    /**
     * Prints a warning message.
     * <p/>
     * The message will be tagged with "Warning" on the output so the caller does not
     * need to put such a prefix in the format string.
     * <p/>
     * The output is done on {@link System#out}.
     * <p/>
     * This is displayed only if the logging {@link Level} is {@link Level#WARNING} or higher.
     *
     * @param warningFormat is a string format to be used with a {@link Formatter}. Cannot be null.
     * @param args          provides the arguments for warningFormat.
     */
    @Override
    public void warning(@NonNull String warningFormat, Object... args) {
        if (mLevel.mLevel > Level.WARNING.mLevel) {
            return;
        }

        String msg = String.format("Warning: " + warningFormat, args);

        printMessage(msg, System.out);
    }

    /**
     * Prints an info message.
     * <p/>
     * The output is done on {@link System#out}.
     * <p/>
     * This is displayed only if the logging {@link Level} is {@link Level#INFO} or higher.
     *
     * @param msgFormat is a string format to be used with a {@link Formatter}. Cannot be null.
     * @param args      provides the arguments for msgFormat.
     */
    @Override
    public void info(@NonNull String msgFormat, Object... args) {
        if (mLevel.mLevel > Level.INFO.mLevel) {
            return;
        }

        String msg = String.format(msgFormat, args);

        printMessage(msg, System.out);
    }

    /**
     * Prints a verbose message.
     * <p/>
     * The output is done on {@link System#out}.
     * <p/>
     * This is displayed only if the logging {@link Level} is {@link Level#VERBOSE} or higher.
     *
     * @param msgFormat is a string format to be used with a {@link Formatter}. Cannot be null.
     * @param args      provides the arguments for msgFormat.
     */
    @Override
    public void verbose(@NonNull String msgFormat, Object... args) {
        if (mLevel.mLevel > Level.VERBOSE.mLevel) {
            return;
        }

        String msg = String.format(msgFormat, args);

        printMessage(msg, System.out);
    }

    private void printMessage(String msg, PrintStream stream) {
        stream.print(msg);
        if (!msg.endsWith("\n")) {
            stream.println();
        }
    }

    public enum Level {
        VERBOSE(0),
        INFO(1),
        WARNING(2),
        ERROR(3);

        private final int mLevel;

        Level(int level) {
            mLevel = level;
        }
    }

}
