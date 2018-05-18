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

import com.duy.ccppcompiler.compiler.shell.CommandResult;
import com.duy.ccppcompiler.compiler.shell.ShellUtils;
import com.pdaxrom.packagemanager.EnvironmentPath;

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
    public CommandResult compile(File[] sourceFiles) {
        File internalDir = mContext.getFilesDir();
        File gccDir = new File(internalDir, "gcc");

        String exec = "g++-4.9";
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

        File tmpDir = new File(gccDir, GCCConstants.BUILD_DIR);
        tmpDir.mkdirs();
        String TEMPEnv = tmpDir.getAbsolutePath();

        Map<String, String> envMap = new HashMap<>();
        envMap.put("TEMP", TEMPEnv);
        String[] envp = EnvironmentPath.buildDefaultEnv(mContext);
        for (String s : envp) {
            String[] split = s.split("=");
            envMap.put(split[0], split[1]);
        }
        return ShellUtils.execCommand(exec, flags, envMap);

    }
}
