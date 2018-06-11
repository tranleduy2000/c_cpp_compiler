/*
 * Copyright (C) 2018 Tran Le Duy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package com.duy.file.explorer.util;

import android.webkit.MimeTypeMap;

import com.duy.file.explorer.io.JecFile;

import java.util.HashMap;
import java.util.Map;

public class MimeTypes {

    private Map<String, String> mMimeTypes;
    private Map<String, Integer> mIcons;

    private static MimeTypes instance = null;

    public static MimeTypes getInstance() {
        if(instance == null)
            instance = new MimeTypes();
        return instance;
    }

    public MimeTypes() {
        mMimeTypes = new HashMap<String, String>();
        mIcons = new HashMap<String, Integer>();
    }

    /*
     * I think the type and extension names are switched (type contains .png,
     * extension contains x/y), but maybe it's on purpouse, so I won't change
     * it.
     */
    public void put(String type, String extension, int icon) {
        put(type, extension);
        mIcons.put(extension, icon);
    }

    public void put(String type, String extension) {
        // Convert extensions to lower case letters for easier comparison
        extension = extension.toLowerCase();

        mMimeTypes.put(type, extension);
    }

    /**
     * Gets the extension of a file name, like ".png" or ".jpg".
     * 
     * @param uri
     * @return Extension including the dot("."); "" if there is no extension;
     *         null if uri was null.
     */
    public static String getExtension(String uri) {
        if (uri == null) {
            return null;
        }

        int dot = uri.lastIndexOf(".");
        if (dot >= 0) {
            return uri.substring(dot);
        } else {
            // No extension.
            return "";
        }
    }

    public String getMimeType(String filename) {

        String extension = getExtension(filename);

        // Let's check the official map first. Webkit has a nice
        // extension-to-MIME map.
        // Be sure to remove the first character from the extension, which is
        // the "." character.
        if(extension == null) return null;
        if (extension.length() > 0) {
            String webkitMimeType = MimeTypeMap.getSingleton().getMimeTypeFromExtension(
                    extension.substring(1));

            if (webkitMimeType != null) {
                // Found one. Let's take it!
                return webkitMimeType;
            }
        }

        // Convert extensions to lower case letters for easier comparison
        extension = extension.toLowerCase();

        String mimetype = mMimeTypes.get(extension);

        if (mimetype == null)
            mimetype = "*/*";

        return mimetype;
    }

    public int getIcon(String mimetype) {
        Integer iconResId = mIcons.get(mimetype);
        if (iconResId == null)
            return 0; // Invalid identifier
        return iconResId;
    }

    /**
     * Indicates whether file is considered to be "text".
     *
     * @return {@code true} if file is text, {@code false} if not.
     */
    public boolean isTextFile(JecFile file) {
        return !file.isDirectory() && isMimeText(getMimeType(file.getPath()));
    }

    public static boolean isMimeText(String mime)
    {
        return mime.startsWith("text");
    }

    /**
     * Indicates whether this path is an image.
     *
     * @return {@code true} if file is image, {@code false} if not.
     */
    public boolean isImageFile(JecFile file) {
        return !file.isDirectory() && isImageFile(file.getName());
    }

    /**
     * Indicates whether requested file path is an image. This is done by
     * comparing file extension to a static list of extensions known to be
     * images.
     *
     * @param file File path
     * @return {@code true} if file is image, {@code false} if not.
     */
    public static boolean isImageFile(String file) {
        String ext = file.substring(file.lastIndexOf(".") + 1);
        if (MimeTypes.getInstance().getMimeType(file).startsWith("image/"))
            return true;
        return ext.equalsIgnoreCase("png") || ext.equalsIgnoreCase("jpg")
                || ext.equalsIgnoreCase("jpeg") || ext.equalsIgnoreCase("gif")
                || ext.equalsIgnoreCase("tiff") || ext.equalsIgnoreCase("tif");

    }

    /**
     * Indicates whether current path is an Android App.
     *
     * @return {@code true} if file is an Android App, {@code false} if not.
     */
    public boolean isAPKFile(JecFile file) {
        return !file.isDirectory() && isAPKFile(file.getName());
    }

    /**
     * Indicates whether requested file path is an Android App.
     *
     * @param file File path
     * @return {@code true} if file is Android App, {@code false} if not.
     */
    public static boolean isAPKFile(String file) {
        file = file.substring(file.lastIndexOf("/") + 1);
        if (file.indexOf(".") > -1)
            file = file.substring(file.lastIndexOf(".") + 1);

        return file.equalsIgnoreCase("apk");

    }

    public boolean isArchive(JecFile file) {
        return file.getExtension().equalsIgnoreCase("zip");
    }

    /**
     * Indicates whether requested file path is an image. This is done by
     * checking Mimetype of file via {@link MimeTypes} class, and by comparing
     * file extension to a static list of extensions known to be videos.
     *
     * @return {@code true} if file is a video, {@code false} if not.
     */
    public boolean isVideoFile(JecFile file) {
        return !file.isDirectory() && isVideoFile(file.getName());
    }

    /**
     * Indicates whether requested file path is an image. This is done by
     * checking Mimetype of file via {@link MimeTypes} class, and by comparing
     * file extension to a static list of extensions known to be videos.
     *
     * @param path
     * @return {@code true} if file is video, {@code false} if not.
     */
    public static boolean isVideoFile(String path) {
        if (MimeTypes.getInstance().getMimeType(path).startsWith("video/"))
            return true;

        String ext = path.substring(path.lastIndexOf(".") + 1);
        return ext.equalsIgnoreCase("mp4") || ext.equalsIgnoreCase("3gp")
                || ext.equalsIgnoreCase("avi") || ext.equalsIgnoreCase("webm")
                || ext.equalsIgnoreCase("m4v");
    }

    public boolean isAudioFile(JecFile file) {
        return !file.isDirectory() && isAudioFile(file.getName());
    }

    public static boolean isAudioFile(String path) {
        if (MimeTypes.getInstance().getMimeType(path).startsWith("audio/"))
            return true;
//mp3|wma|wav|aac|ogg|m4a|flac
        String ext = path.substring(path.lastIndexOf(".") + 1);
        return ext.equalsIgnoreCase("mp3") || ext.equalsIgnoreCase("wma") || ext.equalsIgnoreCase("flac")
                || ext.equalsIgnoreCase("wav") || ext.equalsIgnoreCase("aac")
                || ext.equalsIgnoreCase("ogg") || ext.equalsIgnoreCase("m4a");
    }

    public boolean isCodeFile(JecFile file) {
//        return !isDirectory() && ModeProvider.instance.getModeForFile(getAbsolutePath(), null, null) != null;
        return false; // TODO: 16/3/11 add support
    }
}
