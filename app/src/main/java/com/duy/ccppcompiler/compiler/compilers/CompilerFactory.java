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

package com.duy.ccppcompiler.compiler.compilers;

import android.content.Context;
import android.support.annotation.Nullable;

import com.duy.ccppcompiler.compiler.CompileSetting;

import org.gjt.sp.jedit.Catalog;

import java.io.File;

/**
 * Created by Duy on 25-Apr-18.
 */

public class CompilerFactory {
    @Nullable
    public static INativeCompiler getCompilerForFile(Context context, File[] sourceFiles) {
        File file = sourceFiles[0];
        String filePath = file.getAbsolutePath();
        String fileName = file.getName();

        CompilerFactory.CompileType compilerType = CompileType.NONE;
        if (Catalog.getModeByName("C++").acceptFile(filePath, fileName)) {
            compilerType = CompilerFactory.CompileType.G_PLUS_PLUS;

        } else if (Catalog.getModeByName("C").acceptFile(filePath, fileName)) {
            compilerType = CompilerFactory.CompileType.GCC;
        }


        switch (compilerType) {
            case G_PLUS_PLUS:
                return new GPlusPlusCompiler(context, new CompileSetting(context));
            case GCC:
                return new GCCCompiler(context, new CompileSetting(context));
            default:
                return null;
        }
    }

    /**
     * Created by Duy on 25-Apr-18.
     */

    public enum CompileType {
        GCC, G_PLUS_PLUS, NONE
    }
}
