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

package com.duy.ccppcompiler.compiler.analyze;

import android.support.annotation.NonNull;

import com.duy.ide.diagnostic.model.Message;
import com.duy.ide.diagnostic.model.SourceFilePosition;
import com.duy.ide.diagnostic.model.SourcePosition;
import com.duy.ide.diagnostic.parser.PatternAwareOutputParser;
import com.duy.ide.diagnostic.util.OutputLineReader;
import com.duy.ide.logging.ILogger;

import java.io.File;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * https://gcc.gnu.org/onlinedocs/gcc-3.3.5/gnat_ug_unx/Output-and-Error-Message-Control.html
 * <p>
 * https://gcc.gnu.org/onlinedocs/gcc-7.2.0/gcc/Diagnostic-Message-Formatting-Options.html
 * <p>
 * <p>
 * <p>
 * Created by Duy on 28-Apr-18.
 */

public class CppCheckOutputParser implements PatternAwareOutputParser {

    public static final String TEMPLATE = "--template=\"analysis:{file}:{line}:{severity}:{message}\"";
    private static final Pattern PATTERN = Pattern.compile("^analysis:(\\S+):([0-9]+):([^:]+):(.*)");

    public CppCheckOutputParser() {
    }

    @Override
    public boolean parse(@NonNull String line, @NonNull OutputLineReader reader, @NonNull List<Message> messages,
                         @NonNull ILogger logger) {
        Matcher matcher = PATTERN.matcher(line);
        try {

            if (matcher.find()) {
                File file = new File(matcher.group(1));
                int lineNumber = Integer.parseInt(matcher.group(2));
                Message.Kind kind = Message.Kind.ERROR;
                String text = matcher.group(4);
                Message message = new Message(kind, text,
                        new SourceFilePosition(file, new SourcePosition(lineNumber - 1, -1, -1)));
                messages.add(message);
                return true;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        return false;
    }
}
