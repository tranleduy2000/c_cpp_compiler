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
import com.duy.ide.compiler.shell.ShellResult;
import com.duy.ide.compiler.shell.ShellUtils;

import java.io.File;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Created by Duy on 25-Apr-18.
 */

public class GCCCompiler implements INativeCompiler {
    private Context mContext;

    public GCCCompiler(Context context) {
        this.mContext = context;
    }

    @Override
    public ShellResult compile(File[] sourceFiles) {
        File internalDir = mContext.getFilesDir();
        File gccDir = new File(internalDir, "gcc");
        final String sysPath = System.getenv("PATH");
        final File gccBinDir = new File(gccDir, "bin");
        final File armGccBinDir = new File(gccDir, "arm-linux-androideabi" + File.separator + "bin");
        final File libExecBinDir = new File(gccDir, "libexec/gcc/arm-linux-androideabi/" + GCCConstants.GCC_VERSION);

        String compilerPath = new File(gccBinDir, "arm-linux-androideabi-gcc").getAbsolutePath();

        List<String> flags = new ArrayList<>();
        for (File source : sourceFiles) {
            flags.add(source.getAbsolutePath());
        }
        flags.add("-pie");
        flags.add("-std=c99");
        flags.add("-lz");//zlib
        flags.add("-ldl");
        flags.add("-lm");//math
        flags.add("-llog");
        flags.add("-lncurses");
        flags.add("-Og");
        flags.add("-o");
        flags.add(new File(internalDir.getAbsolutePath(), GCCConstants.TEMP_BINARY_NAME).getAbsolutePath());

        String TEMPEnv = new File(gccDir, GCCConstants.BUILD_DIR).getAbsolutePath();
        String PATHEnv =
                internalDir.getAbsolutePath() + File.pathSeparator
                        + gccBinDir.getAbsolutePath() + File.pathSeparator
                        + armGccBinDir.getAbsolutePath() + File.pathSeparator
                        + libExecBinDir.getAbsolutePath() + File.pathSeparator
                        + sysPath;

        System.out.println("PATHEnv = " + PATHEnv);
        System.out.println("TEMPEnv = " + TEMPEnv);

        Map<String, String> envMap = new HashMap<>();
        envMap.put("PATH", PATHEnv);
        envMap.put("TEMP", TEMPEnv);
        return ShellUtils.execCommand(compilerPath, flags, envMap);
    }
}
