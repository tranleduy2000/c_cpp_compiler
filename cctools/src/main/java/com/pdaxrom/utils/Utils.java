package com.pdaxrom.utils;

import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

public class Utils {
	static {
		final String[] libpaths = {
			"/data/data/com.pdaxrom.cctools/lib/libmyutils.so",
		};
		boolean found = false;
		for (String libpath: libpaths) {
			if ((new File(libpath)).exists()) {
				//System.out.println(TAG + " Found library " + libpath);
				System.load(libpath);
				found = true;
			}
		}
		if (!found) {
			System.loadLibrary("myutils");
		}
	}

	public native static int chmod(String file, int attr);
	
	public native static int unzip(String file, String to, String logfile);
	
	public native static int unzippedSize(String file);
	
	public native static FileDescriptor createSubProcess(String dir,
				String cmd, String[] args, String[] envVars, int[] processId);
	
	public native static void setPtyWindowSize(FileDescriptor fd,
				int row, int col, int xpixel, int ypixel);
	
	public native static void setPtyUTF8Mode(FileDescriptor fd, boolean utf8Mode);
	
	public native static int waitFor(int processId);
	
	public native static int readByte(FileDescriptor fd);
	
	public native static int writeByte(FileDescriptor fd, int b);
	
	public native static void close(FileDescriptor fd);
	
	public native static void hangupProcessGroup(int processId);
	
	public static void copyDirectory(File sourceLocation , File targetLocation) throws IOException {
	    if (sourceLocation.isDirectory()) {
	        if (!targetLocation.exists()) {
	            targetLocation.mkdir();
	        }

	        String[] children = sourceLocation.list();
	        for (int i=0; i<children.length; i++) {
	            copyDirectory(new File(sourceLocation, children[i]),
	                    new File(targetLocation, children[i]));
	        }
	    } else {
	        InputStream in = new FileInputStream(sourceLocation);
	        OutputStream out = new FileOutputStream(targetLocation);

	        // Copy the bits from instream to outstream
	        byte[] buf = new byte[1024*1024];
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
				for (String temp: files) {
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

	public static String humanReadableByteCount(long bytes, boolean si) {
	    int unit = si ? 1000 : 1024;
	    if (bytes < unit) return bytes + " B";
	    int exp = (int) (Math.log(bytes) / Math.log(unit));
	    String pre = (si ? "kMGTPE" : "KMGTPE").charAt(exp-1) + (si ? "" : "i");
	    return String.format("%.1f %sB", bytes / Math.pow(unit, exp), pre);
	}
	
	public static String getBootClassPath() {
		String classPath = null;
		File dir = new File("/system/framework");
		if (dir.exists() && dir.isDirectory()) {
			FilenameFilter filter = new FilenameFilter() {
				public boolean accept(File dir, String name) {
					String lowercaseName = name.toLowerCase();
					if (lowercaseName.endsWith(".jar")) {
						return true;
					} else {
						return false;
					}
				}
			};

			boolean first = true;
			
			for (String file: dir.list(filter)) {
				if (!first) {
					classPath += ":";
				} else {
					classPath = "";
				}
				classPath += "/system/framework/" + file;
			}
		}
		return classPath;
	}
}
