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

package com.duy.ccppcompiler.compiler.diagnostic.parser;

import junit.framework.TestCase;

import java.util.regex.Matcher;

import static com.duy.ccppcompiler.compiler.diagnostic.OutputParser.DIAGNOSTICS_PATTERN;

/**
 * Created by Duy on 28-Apr-18.
 */
public class OutputFormatPatternsTest extends TestCase {

    public void testError() {
        Matcher matcher = DIAGNOSTICS_PATTERN.matcher("D:\\Data\\Downloads\\c\\file.c:21:57: error: 'firstNber' undeclared (first use in this function)\n" +
                "       printf(\"\\nAfter swapping, firstNumber = %.2lf\\n\", firstNber);\n" +
                "                                                         ^");
        assertTrue(matcher.find());
        for (int i = 0; i <= matcher.groupCount(); i++) {
            System.out.println("group " + i + ": " + matcher.group(i));
        }
    }

}