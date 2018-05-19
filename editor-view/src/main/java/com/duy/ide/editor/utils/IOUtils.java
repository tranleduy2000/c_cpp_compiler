package com.duy.ide.editor.utils;

import java.io.File;

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


    /**
     * Create new file, if file exist, do not create file
     */
    public static boolean createNewFile(File file) {
        if (file.exists()) {
            return true;
        }
        try {
            file.getParentFile().mkdirs();
            return file.createNewFile();
        } catch (Exception e) {
            e.printStackTrace();
            return false;
        }
    }

    @SuppressWarnings("ResultOfMethodCallIgnored")
    public static void changeToExecutable(File file) {
        if (file.isDirectory()) {
            File[] files = file.listFiles();
            if (files != null) {
                for (File f : files) {
                    changeToExecutable(f);
                }
            }
        } else {
            file.setExecutable(true);
            file.setReadable(true);
        }
    }
}
