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
    private String fileName;
    private int size;
    private int filesize;
    private String version;
    private String description;
    private String depends;
    private String arch;
    private String replaces;

    /**
     * <name>build-essential-clang-compact</name>
     * <replaces>build-essential-clang</replaces>
     * <version>1.1</version>
     * <arch>armel</arch>
     * <description>Informational list of clang compact build-essential packages</description>
     * <depends>busybox project-ctl binutils-compact libgcc-compact-dev libstdc++-compact-dev clang make ndk-misc ndk-sysroot-${HOSTNDKARCH}-${HOSTNDKVERSION} cctools-examples</depends>
     * <size></size>
     * <file>build-essential-clang-compact_1.1_all.zip</file>
     * <filesize>601</filesize>
     *
     * @param name     - package name
     * @param fileName - name of file will be store in {@link com.duy.ccppcompiler.packagemanager.repo.LocalPackageRepository}
     * @param version  - current version
     * @param depends  - all package dependencies on this package
     */
    public PackageInfo(String name, String fileName, int size, int filesize,
                       String version, String description, String depends,
                       String arch, String replaces) {
        this.name = name;
        this.fileName = fileName;
        this.size = size;
        this.filesize = filesize;
        this.version = version;
        this.description = description;
        this.depends = depends;
        this.arch = arch;
        this.replaces = replaces;
    }

    public String getName() {
        return name;
    }

    @Override
    public String toString() {
        return "PackageInfo{" +
                "name='" + name + '\'' +
                ", file='" + fileName + '\'' +
                ", size=" + size +
                ", filesize=" + filesize +
                ", version='" + version + '\'' +
                ", description='" + description + '\'' +
                ", depends='" + depends + '\'' +
                ", arch='" + arch + '\'' +
                ", replaces='" + replaces + '\'' +
                '}';
    }

    public String getFileName() {
        return fileName;
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

    public String getArch() {
        return arch;
    }

    public String getReplaces() {
        return replaces;
    }

}
