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

package com.duy.utils;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

/**
 * Created by Duy on 25-Apr-18.
 */

public class ZipExtractor {
    public static boolean unzip(InputStream srcIn, File targetDir) {
        ZipInputStream zipInputStream = null;
        try {
            zipInputStream = new ZipInputStream(srcIn);
            ZipEntry zipEntry;
            while ((zipEntry = zipInputStream.getNextEntry()) != null) {
                if (zipEntry.isDirectory()) {
                    File file = new File(targetDir + File.separator + zipEntry.getName());
                    file.mkdir();
                } else {
                    System.out.println("Extract " + zipEntry.getName());
                    byte[] buf = new byte[1024];
                    File outFile = new File(targetDir, zipEntry.getName());
                    FileOutputStream fileOutputStream = new FileOutputStream(outFile);
                    int len;
                    while ((len = zipInputStream.read(buf, 0, buf.length)) != -1) {
                        fileOutputStream.write(buf, 0, len);
                        fileOutputStream.flush();
                    }
                    zipInputStream.closeEntry();
                    fileOutputStream.close();
                }
            }
            try {
                zipInputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
            return true;
        } catch (IOException e) {
            e.printStackTrace();
            return false;
        }
    }
}
