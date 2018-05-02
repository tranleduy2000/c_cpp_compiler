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

package com.duy.ccppcompiler.ui.examples;

import com.duy.common.DLog;

import junit.framework.TestCase;

import java.io.FileInputStream;
import java.util.ArrayList;

/**
 * Created by Duy on 03-May-18.
 */
public class ExampleParserTest extends TestCase {

    public void testParse() throws Exception {
        try {
            ExampleParser exampleParser = new ExampleParser();
            ArrayList<ExampleItem> result =
                    exampleParser.parse(new FileInputStream(".\\app\\src\\main\\assets\\example\\cpp\\index.xml"));
            for (ExampleItem item : result) {
                System.out.println("item = " + item);
            }
        } catch (Exception e) {
            fail();
        }
    }

    @Override
    protected void setUp() throws Exception {
        super.setUp();
        DLog.ANDROID = false;
    }
}