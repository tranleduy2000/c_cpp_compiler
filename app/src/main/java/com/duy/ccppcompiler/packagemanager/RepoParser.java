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

package com.duy.ccppcompiler.packagemanager;

import android.net.Uri;
import android.support.annotation.NonNull;

import com.duy.ccppcompiler.packagemanager.model.PackageInfo;
import com.duy.common.DLog;
import com.pdaxrom.utils.XMLParser;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import java.util.ArrayList;
import java.util.List;

import static com.duy.ccppcompiler.packagemanager.RepoUtils.isContainsPackage;

/**
 * Created by Duy on 20-May-18.
 */

public class RepoParser {
    // XML node keys
    public static final String KEY_PACKAGE = "package"; // parent node
    public static final String KEY_PACKAGE_NAME = "name";
    public static final String KEY_LOCAL_FILE_NAME = "file";
    public static final String KEY_SIZE = "size";
    public static final String KEY_FILESIZE = "filesize";
    public static final String KEY_VERSION = "version";
    public static final String KEY_DESC = "description";
    public static final String KEY_DEPENDS = "depends";
    public static final String KEY_ARCH = "arch";
    public static final String KEY_REPLACES = "replaces";
    public static final String KEY_STATUS = "status";

    private static final String TAG = "RepoParser";
    private static final boolean DEBUG = DLog.DEBUG;

    @NonNull
    public List<PackageInfo> parseRepoXml(String repo, String url) {
        ArrayList<PackageInfo> list = new ArrayList<>();
        if (repo != null) {
            XMLParser parser = new XMLParser();
            Document doc = parser.getDomElement(repo); // getting DOM element
            if (doc == null) {
                return list;
            }
            NodeList nl = doc.getElementsByTagName(KEY_PACKAGE);

            for (int i = 0; i < nl.getLength(); i++) {
                Element e = (Element) nl.item(i);
                int size;
                int filesize;
                if (DEBUG) {
                    System.out.println(TAG + " pkg [ " + parser.getValue(e, KEY_PACKAGE_NAME) + " ][ " + parser.getValue(e, KEY_SIZE) + "]");
                }
                if (parser.getValue(e, KEY_SIZE).length() > 0) {
                    size = Integer.valueOf(parser.getValue(e, KEY_SIZE).replaceAll("@SIZE@", "0"));
                } else {
                    size = 0;
                }
                if (parser.getValue(e, KEY_FILESIZE).length() > 0) {
                    filesize = Integer.valueOf(parser.getValue(e, KEY_FILESIZE).replaceAll("@SIZE@", "0"));
                } else {
                    // old format of packages not included unpacked size
                    filesize = size;
                }
                if (isContainsPackage(list, parser.getValue(e, KEY_PACKAGE_NAME), parser.getValue(e, KEY_VERSION))) {
                    if (DEBUG) {
                        System.out.println(TAG + "skip exists pkg" + parser.getValue(e, KEY_PACKAGE_NAME));
                    }
                    continue;
                }
                PackageInfo packageInfo = new PackageInfo(
                        parser.getValue(e, KEY_PACKAGE_NAME),
                        parser.getValue(e, KEY_LOCAL_FILE_NAME),
                        size /*Integer.valueOf(parser.getValue(e, KEY_SIZE))*/,
                        filesize /*Integer.valueOf(parser.getValue(e, KEY_FILESIZE))*/,
                        parser.getValue(e, KEY_VERSION),
                        parser.getValue(e, KEY_DESC),
                        parser.getValue(e, KEY_DEPENDS),
                        parser.getValue(e, KEY_ARCH),
                        parser.getValue(e, KEY_REPLACES),
                        Uri.parse(url));
                list.add(packageInfo);
                if (DEBUG) {
                    System.out.println(TAG + " added pkg = " + packageInfo.getName());
                }
            }
        }
        return list;
    }
}
