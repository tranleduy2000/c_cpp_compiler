package com.duy.ccppcompiler.utils;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

/**
 * Created by Duy on 22-Apr-18.
 */

public class IOUtils {

    /**
     * Delete file and all child of file
     *
     * @param file the file to be deleted
     * @return true if delete success
     */
    public static boolean delete(File file) {
        if (file.exists()) {
            if (file.isDirectory()) {
                boolean result = true;
                File[] files = file.listFiles();
                if (files != null) {
                    for (File child : files) {
                        result &= delete(child);
                    }
                }
                result &= file.delete();
                return result;
            } else {
                System.out.println("Delete " + file.getName());
                return file.delete();
            }
        }
        return false;
    }


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

    public static void changeToExecutable(File file) {
        file.setReadable(true);
        file.setWritable(true);
        file.setExecutable(true);
    }
}
