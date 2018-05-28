package com.pdaxrom.utils;

import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class Utils {
    private static final String TAG = "Utils";

    static {
        System.loadLibrary("myutils");
    }

    public native static int chmod(String file, int attr);

    public native static int unzip(String file, String to, String logfile);

    public native static int unzippedSize(String file);

    /**
     * @param cwd     - current working directory
     * @param cmd     - command
     * @param args    - arguments
     * @param envVars - environment variables
     */
    public native static FileDescriptor createSubProcess(String cwd,
                                                         String cmd, String[] args, String[] envVars, int[] processId);

    public native static void setPtyWindowSize(FileDescriptor fd,
                                               int row, int col, int xpixel, int ypixel);

    public native static void setPtyUTF8Mode(FileDescriptor fd, boolean utf8Mode);

    public native static int waitFor(int processId);

    public native static int readByte(FileDescriptor fd);

    public native static int writeByte(FileDescriptor fd, int b);

    public native static void close(FileDescriptor fd);

    public native static void hangupProcessGroup(int processId);

    public static void copyDirectory(File sourceLocation, File targetLocation) throws IOException {
        if (sourceLocation.isDirectory()) {
            if (!targetLocation.exists()) {
                targetLocation.mkdir();
            }

            String[] children = sourceLocation.list();
            for (int i = 0; i < children.length; i++) {
                copyDirectory(new File(sourceLocation, children[i]),
                        new File(targetLocation, children[i]));
            }
        } else {
            InputStream in = new FileInputStream(sourceLocation);
            OutputStream out = new FileOutputStream(targetLocation);

            // Copy the bits from instream to outstream
            byte[] buf = new byte[1024 * 1024];
            int len;
            while ((len = in.read(buf)) > 0) {
                out.write(buf, 0, len);
            }
            in.close();
            out.close();
        }
    }

    public static void deleteDirectory(File file) {
        if (file.isDirectory()) {
            if (file.list().length == 0) {
                file.delete();
            } else {
                String files[] = file.list();
                for (String temp : files) {
                    File fileDelete = new File(file, temp);
                    deleteDirectory(fileDelete);
                }
                if (file.list().length == 0) {
                    file.delete();
                }
            }
        } else {
            file.delete();
        }
    }

    /**
     * https://stackoverflow.com/questions/3758606/how-to-convert-byte-size-into-human-readable-format-in-java
     */
    public static String humanReadableByteCount(long bytes, boolean si) {
        int unit = si ? 1000 : 1024;
        if (bytes < unit) return bytes + " B";
        int exp = (int) (Math.log(bytes) / Math.log(unit));
        String pre = (si ? "kMGTPE" : "KMGTPE").charAt(exp - 1) + (si ? "" : "i");
        return String.format("%.1f %sB", bytes / Math.pow(unit, exp), pre);
    }

}
