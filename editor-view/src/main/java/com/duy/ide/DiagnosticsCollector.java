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

import java.util.ArrayList;

/**
 * Provides an easy way to collect diagnostics in a list.
 * <p>
 * Created by Duy on 28-Apr-18.
 */

public class DiagnosticsCollector implements DiagnosticListener {
    private final ArrayList<Diagnostic> diagnostics = new ArrayList<>();
    private boolean ignoreSystemFiles = true;

    public void setIgnoreSystemFiles(boolean ignoreSystemFiles) {
        this.ignoreSystemFiles = ignoreSystemFiles;
    }

    @Override
    public void report(Diagnostic diagnostic) {
        diagnostics.add(diagnostic);
    }

    @Override
    public void clear() {
        diagnostics.clear();
    }

    public ArrayList<Diagnostic> getDiagnostics() {
        return diagnostics;
    }
}
