package com.duy.ccppcompiler.ide.diagnostic;

import com.duy.ide.diagnostic.Message;
import com.duy.ide.diagnostic.parser.IParser;

import java.io.LineNumberReader;
import java.util.List;

public class GccParser implements IParser {
    private static final String FROM = "from";
    private static final String UNKNOWN_MSG_PREFIX1 = "In file included " + FROM;
    private static final String UNKNOWN_MSG_PREFIX2 = "                 " + FROM;

    private static final char COLON = ':';
    @Override
    public boolean parse(String line, LineNumberReader reader, List<Message> messages) {
        return false;
    }
}
