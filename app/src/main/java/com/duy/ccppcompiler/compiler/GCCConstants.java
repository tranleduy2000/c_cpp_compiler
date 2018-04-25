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

/**
 * Created by Duy on 22-Apr-18.
 */

public class GCCConstants {
    public static final String GCC_DIR_NAME = "gcc";
    public static final String GCC_VERSION = "7.2.0";
    public static final String GCC_ASSET_FILE = "gcc.zip";

    public static final String TEMP_FILE_NAME = "temp.c";
    public static final String TEMP_BINARY_NAME = "temp";
    public static final String BUILD_DIR = "tmpdir";

    public static final String INDENT_FILE_NAME = "indent.c";
    public static final String INDENT_ARGS = "-nbap -bli0 -i2 -l79 -ts2 -ncs -npcs -npsl -fca -lc79 -fc1 -ts1 -ce -br -cdw -brs -brf";
}
