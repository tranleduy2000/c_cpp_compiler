package com.duy.ccppcompiler.ide.diagnostic;

import com.duy.ide.diagnostic.Diagnostic;
import com.duy.ide.diagnostic.parser.IParser;

import java.io.LineNumberReader;
import java.util.List;

public class GccParser implements IParser {

    @Override
    public boolean parse(String line, LineNumberReader reader, List<Diagnostic> diagnostics) {
        return false;
    }
}
