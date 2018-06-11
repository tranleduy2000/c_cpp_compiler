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

import android.support.annotation.NonNull;

import java.io.File;
import java.io.Serializable;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class FileInfo implements Serializable {

    /**
     * When info isn't accessible (e.g. /persist)
     */
    public boolean readAvailable;

    public String name;

    //These are null if type isn't symlink
    public String linkedPath;
    public File realFile;

    public String permissions;
    public String owner;
    public String group;
    public long size;
    public long lastModified;
    public String directoryFileCount;

    public boolean isSymlink;
    public boolean isDirectory;

    /**
     * Should be used by LsParser only
     */
    public FileInfo(boolean readAvailable, @NonNull String name) {
        this.name = name;
        this.permissions = "";
        this.owner = "";
        this.group = "";
        this.size = -1;
        this.lastModified = -1;
        this.directoryFileCount = "";
        this.readAvailable = readAvailable;
    }

    /**
     * Should be used by FileAdapter
     */
    public FileInfo(FileInfo info) {
        name = info.name;
        permissions = info.permissions;
        owner = info.owner;
        group = info.group;
        size = info.size;
        lastModified = info.lastModified;
        directoryFileCount = info.directoryFileCount;
        readAvailable = info.readAvailable;
        isSymlink = info.isSymlink;
        isDirectory = info.isDirectory;
        realFile = info.realFile;
        linkedPath = info.linkedPath;
    }

    @Override
    public boolean equals(Object o) {
        if (o == this) {
            return true;
        }
        if (o instanceof FileInfo) {
            FileInfo other = (FileInfo) o;
            if (other.readAvailable && readAvailable) {
                try {
                    return (((other.name == null) == (name == null)) ||
                            (other.name != null && other.name.equals(name))) &&
                            other.permissions.equals(permissions) &&
                            other.owner.equals(owner) &&
                            other.group.equals(group) &&
                            other.size == size &&
                            other.lastModified == lastModified &&
                            other.directoryFileCount.equals(directoryFileCount);
                } catch (NullPointerException e) {
                    throw new IllegalStateException("Unexpected null values when comparing FileInfos.\t\n" +
                            "Name: " + other.name + "\t\nPermissions: " + other.permissions + "\t\n" +
                            "Owner: " + other.owner + "\t\nGroup: " + other.group + "\t\nSize: " + other.size + "\t\n" +
                            "Last modified: " + other.lastModified + "\t\nFile count: " + other.directoryFileCount);
                }
            } else if (!other.readAvailable && !readAvailable) {
                return other.name.equals(name);
            }
        }
        return false;
    }

    @NonNull
    @Override
    public String toString() {
        return name;
    }

}
