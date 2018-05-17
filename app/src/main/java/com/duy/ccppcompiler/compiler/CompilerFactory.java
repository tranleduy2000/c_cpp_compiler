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

package com.duy.ccppcompiler.compiler;

import android.content.Context;

import com.duy.ide.compiler.INativeCompiler;

/**
 * Created by Duy on 25-Apr-18.
 */

public class CompilerFactory {
    public static INativeCompiler createCompiler(Context context, CompileType compileType) {
        switch (compileType) {
            case G_PLUS_PLUS:
                return new GPlusPlusCompiler(context);
            case GCC:
                return new GCCCompiler(context);
        }
        return new GCCCompiler(context);
    }

    /**
     * Created by Duy on 25-Apr-18.
     */

    public static enum CompileType {
        GCC,  G_PLUS_PLUS
    }
}
