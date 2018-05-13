package com.pdaxrom.cctools.sdlplugin;

import java.io.BufferedInputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import android.util.Log;

public class Utils {
	private static final String TAG = "CCTools SDL Utils";
	
	public static final int Lib_SDL			= 0;
	public static final int Lib_SDL_image	= 1;
	public static final int Lib_SDL_mixer	= 2;
	public static final int Lib_SDL_net		= 3;
	public static final int Lib_SDL_ttf		= 4;
	
	static {
		System.loadLibrary("ccsdlplugin");
	}

	public static native int setEnv(String name, String value, boolean overwrite);
	public static native int unSetEnv(String name);
	public static native String getEnv(String name);
	public static native int chDir(String path);
	public static native String getSDLVersion(int lib);
	
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
	
    /*
    public int chmod(File path, int mode) throws Exception {
    	Class fileUtils = Class.forName("android.os.FileUtils");
    	Method setPermissions = fileUtils.getMethod("setPermissions", String.class, int.class, int.class, int.class);
    	return (Integer) setPermissions.invoke(null, path.getAbsolutePath(), mode, -1, -1);
    }
    */
    
	public static boolean unpackZip(InputStream is, String to) {       
	     ZipInputStream zis;
	     try {
	    	 zis = new ZipInputStream(new BufferedInputStream(is));          
	         ZipEntry ze;
	         byte[] buffer = new byte[1024];

	         while ((ze = zis.getNextEntry()) != null) {
	        	 Log.i(TAG, "Unzipping file " + ze.getName());
	        	 
	             if (ze.isDirectory()) {
		                File fmd = new File(to + "/" + ze.getName());
		                fmd.mkdirs();
		                continue;
	             }

	             FileOutputStream fout = new FileOutputStream(to + "/" + ze.getName());

	             for (int count = zis.read(buffer); count > 0; count = zis.read(buffer)) {
	            	 fout.write(buffer, 0, count);
	             }

	             fout.close();               
	             zis.closeEntry();
	         }
	         zis.close();
	     } catch(IOException e) {
	         e.printStackTrace();
	         return false;
	     }

	    return true;
	}
}
