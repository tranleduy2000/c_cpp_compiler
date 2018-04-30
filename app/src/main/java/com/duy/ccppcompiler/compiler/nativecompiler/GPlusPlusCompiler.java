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

package com.duy.ccppcompiler.compiler.nativecompiler;

import android.content.Context;

import com.duy.ccppcompiler.compiler.GCCConstants;
import com.duy.ccppcompiler.compiler.INativeCompiler;
import com.duy.ccppcompiler.compiler.shell.ShellResult;
import com.duy.ccppcompiler.compiler.shell.ShellUtils;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by Duy on 25-Apr-18.
 */

public class GPlusPlusCompiler implements INativeCompiler {
    private Context mContext;

    public GPlusPlusCompiler(Context context) {
        this.mContext = context;
    }

    @Override
    public ShellResult compile(File[] sourceFiles) {
        File internalDir = mContext.getFilesDir();
        File gccDir = new File(internalDir, "gcc");

        final String sysPath = System.getenv("PATH");
        final File gccBinDir = new File(gccDir, "bin");
        final File armGccBinDir = new File(gccDir, "arm-linux-androideabi" + File.separator + "bin");

        String compilerPath = new File(gccBinDir, "arm-linux-androideabi-g++").getAbsolutePath();

        List<String> flags = new ArrayList<>();
        for (File sourceFile : sourceFiles) {
            flags.add(sourceFile.getAbsolutePath());
        }
        flags.add("-pie");
        flags.add("-std=c++14");
        flags.add("-lz");
        flags.add("-ldl");
        flags.add("-lm");
        flags.add("-llog");
        flags.add("-lncurses");
        flags.add("-Og");
        flags.add("-o");
        flags.add(internalDir.getAbsolutePath() + File.separator + GCCConstants.TEMP_BINARY_NAME);

        String TEMPEnv = new File(gccDir, GCCConstants.BUILD_DIR).getAbsolutePath();
        String PATHEnv = internalDir.getAbsolutePath() + File.pathSeparator
                + gccBinDir.getAbsolutePath() + File.pathSeparator
                + armGccBinDir.getAbsolutePath() + File.pathSeparator
                + sysPath;


        Map<String, String> envMap = new HashMap<>();
        envMap.put("PATH", PATHEnv);
        envMap.put("TEMP", TEMPEnv);

        return ShellUtils.execCommand(compilerPath, flags, envMap);

    }
}
