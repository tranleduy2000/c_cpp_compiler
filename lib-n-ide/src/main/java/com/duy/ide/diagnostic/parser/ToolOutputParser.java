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

package com.duy.ide.diagnostic.parser;

import android.support.annotation.NonNull;

import com.duy.ide.diagnostic.model.Message;
import com.duy.ide.diagnostic.model.SourceFilePosition;
import com.duy.ide.diagnostic.util.OutputLineReader;
import com.duy.ide.logging.ILogger;
import com.google.common.collect.ImmutableList;
import com.google.common.collect.Lists;

import java.util.Collections;
import java.util.List;

public class ToolOutputParser {

    @NonNull
    private final List<PatternAwareOutputParser> mParsers;

    @NonNull
    private final ILogger mLogger;

    public ToolOutputParser(@NonNull Iterable<PatternAwareOutputParser> parsers, @NonNull ILogger logger) {
        mParsers = ImmutableList.copyOf(parsers);
        mLogger = logger;
    }

    public ToolOutputParser(@NonNull PatternAwareOutputParser[] parsers, @NonNull ILogger logger) {
        mParsers = ImmutableList.copyOf(parsers);
        mLogger = logger;
    }

    public ToolOutputParser(@NonNull PatternAwareOutputParser parser, @NonNull ILogger logger) {
        mParsers = ImmutableList.of(parser);
        mLogger = logger;
    }

    public List<Message> parseToolOutput(@NonNull String output) {
        OutputLineReader outputReader = new OutputLineReader(output);

        if (outputReader.getLineCount() == 0) {
            return Collections.emptyList();
        }

        List<Message> messages = Lists.newArrayList();
        String line;
        while ((line = outputReader.readLine()) != null) {
            if (line.isEmpty()) {
                continue;
            }
            boolean handled = false;
            for (PatternAwareOutputParser parser : mParsers) {
                try {
                    if (parser.parse(line, outputReader, messages, mLogger)) {
                        handled = true;
                        break;
                    }
                } catch (ParsingFailedException e) {
                    return Collections.emptyList();
                }
            }
            if (handled) {
                int messageCount = messages.size();
                if (messageCount > 0) {
                    Message last = messages.get(messageCount - 1);
                    if (last.getText().contains("Build cancelled")) {
                        // Build was cancelled, just quit. Extra messages are just confusing noise.
                        break;
                    }
                }
            } else {
                // If none of the standard parsers recogni ze the input, include it as info such
                // that users don't miss potentially vital output such as gradle plugin exceptions.
                // If there is predictable useless input we don't want to appear here, add a custom
                // parser to digest it.
                messages.add(new Message(Message.Kind.SIMPLE, line, SourceFilePosition.UNKNOWN));
            }
        }
        return messages;
    }
}
