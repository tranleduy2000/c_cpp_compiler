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

import com.duy.ccppcompiler.compiler.ICompileSetting;
import com.duy.ccppcompiler.compiler.shell.ArgumentBuilder;

/**
 * G++ compiler as GCC compiler but it different name, just override {@link #getCompilerProgram()}
 * Created by Duy on 25-Apr-18.
 */
public class GPlusPlusCompiler extends CompilerImpl {
    private static final String G_PLUS_PLUS_COMPILER_NAME = "g++-4.9";
    private ICompileSetting mSetting;

    public GPlusPlusCompiler(Context context, int buildType, ICompileSetting setting) {
        super(context, buildType);
        mSetting = setting;
    }

    @Override
    protected String getCompilerProgram() {
        return G_PLUS_PLUS_COMPILER_NAME;
    }

    @Override
    protected void addUserSettingFlags(ArgumentBuilder args) {
        if (mSetting != null) {
            args.addFlags(mSetting.getCxxFlags());
            args.addFlags(mSetting.getLdFlags());
        }
    }


}
