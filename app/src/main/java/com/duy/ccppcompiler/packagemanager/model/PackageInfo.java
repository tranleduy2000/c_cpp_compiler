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

package com.duy.ccppcompiler.packagemanager.model;

public class PackageInfo {
    private String name;
    private String file;
    private int size;
    private int filesize;
    private String version;
    private String description;
    private String depends;
    private String arch;
    private String replaces;
    private String url;

    public PackageInfo(String name, String file, int size, int filesize,
                       String version, String description, String depends,
                       String arch, String replaces, String url) {
        this.name = name;
        this.file = file;
        this.size = size;
        this.filesize = filesize;
        this.version = version;
        this.description = description;
        this.depends = depends;
        this.arch = arch;
        this.replaces = replaces;
        this.url = url;
    }

    public String getName() {
        return name;
    }

    public String getFile() {
        return file;
    }

    public int getSize() {
        return size;
    }

    public int getFileSize() {
        return filesize;
    }

    public String getVersion() {
        return version;
    }

    public String getDescription() {
        return description;
    }

    public String getDepends() {
        return depends;
    }

    String getArch() {
        return arch;
    }

    public String getReplaces() {
        return replaces;
    }

    public String getUrl() {
        return url;
    }
}
