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

package com.duy.ccppcompiler.compiler.shell;

import android.support.annotation.NonNull;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;

/**
 * Created by Duy on 17-May-18.
 */

public class CommandBuilder {
    private String program;
    private ArrayList<String> flags = new ArrayList<>();

    public CommandBuilder(String program) {
        this.program = program;
    }

    public CommandBuilder() {
        this.program = "";
    }

    public CommandBuilder addFlags(String... flags) {
        this.flags.addAll(Arrays.asList(flags));
        return this;
    }

    public CommandBuilder addFlags(List<String> flags) {
        this.flags.addAll(flags);
        return this;
    }

    public void addFlags(CommandBuilder otherFlags) {
        this.flags.addAll(otherFlags.flags);
    }

    @NonNull
    public String build() {
        StringBuilder cmd = new StringBuilder();
        cmd.append(program);
        if (flags.size() == 0) {
            return cmd.toString();
        }

        for (int i = 0; i < flags.size(); i++) {
            String flag = flags.get(i);
            if (flag != null && !flag.isEmpty()) {
                if (cmd.length() != 0) {
                    cmd.append(" ");
                }
                cmd.append(flag);
            }
        }
        return cmd.toString();
    }


}
