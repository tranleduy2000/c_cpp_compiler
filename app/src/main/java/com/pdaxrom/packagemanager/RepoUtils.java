package com.pdaxrom.packagemanager;

import android.support.annotation.NonNull;
import android.support.annotation.Nullable;

import com.jecelyin.common.utils.DLog;
import com.pdaxrom.utils.XMLParser;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

public class RepoUtils {
    // XML node keys
    public static final String KEY_PACKAGE = "package"; // parent node
    public static final String KEY_NAME = "name";
    public static final String KEY_FILE = "file";
    public static final String KEY_SIZE = "size";
    public static final String KEY_FILESIZE = "filesize";
    public static final String KEY_VERSION = "version";
    public static final String KEY_DESC = "description";
    public static final String KEY_DEPENDS = "depends";
    public static final String KEY_ARCH = "arch";
    public static final String KEY_REPLACES = "replaces";
    public static final String KEY_STATUS = "status";

    private static final String TAG = "RepoUtils";
    /**
     * armeabi-v7a/
     * armeabi/
     * mips/
     * x86/
     */
    private static String buildAbi;
    private static String ndkArch;
    private static int ndkVersion;

    private static boolean DEBUG = false;

    public static void setVersion(String buildAbi, String ndkArch, int ndkVersion) {
        RepoUtils.buildAbi = buildAbi;
        RepoUtils.ndkArch = ndkArch;
        RepoUtils.ndkVersion = ndkVersion;
    }

    @NonNull
    public static List<PackageInfo> getRepoFromUrl(List<String> urls) {
        List<PackageInfo> list = new ArrayList<>();
        for (String url : urls) {
            url = url + "/" + buildAbi;
            list = parseRepoXml(list, getRepoXmlFromUrl(url), url); // getting DOM element
        }
        return list;
    }

    @NonNull
    public static List<PackageInfo> getRepoFromDir(String path) {
        return parseRepoXml(new ArrayList<PackageInfo>(), getRepoXmlFromDir(path), path);
    }

    public static String getRepoXmlFromUrl(String url) {
        XMLParser parser = new XMLParser();
        String xml = parser.getXmlFromUrl(url + "/Packages");
        return replaceMacro(xml);
    }

    public static String getRepoXmlFromDir(String path) {
        File dir = new File(path);
        if (dir.isDirectory()) {
            StringBuilder sb = new StringBuilder();
            sb.append("<repo>").append("\n");

            FilenameFilter filter = new FilenameFilter() {
                public boolean accept(File dir, String name) {
                    String lowercaseName = name.toLowerCase();
                    return lowercaseName.endsWith(".pkgdesc");
                }
            };

            for (String filePath : dir.list(filter)) {
                if (DEBUG) {
                    System.out.println(TAG + " Read file " + filePath);
                }
                File f = new File(path + "/" + filePath);
                try {
                    FileInputStream fin = new FileInputStream(f);
                    BufferedReader reader = new BufferedReader(new InputStreamReader(fin));
                    String line = null;
                    while ((line = reader.readLine()) != null) {
                        sb.append(line).append("\n");
                    }
                    reader.close();
                } catch (IOException e) {
                    System.err.println(TAG + " getRepoXmlFromDir() IO error " + e);
                }
            }
            sb.append("</repo>");
            if (DEBUG) {
                System.out.println(TAG + " installed xml = " + replaceMacro(sb.toString()));
            }
            return replaceMacro(sb.toString());
        }
        return null;
    }

    public static boolean isContainsPackage(List<PackageInfo> repo, String pkg) {
        if (DLog.DEBUG)
            DLog.d(TAG, "isContainsPackage() called with: repo = [" + repo + "], pkg = [" + pkg + "]");
        for (PackageInfo packageInfo : repo) {
            if (packageInfo.getName().equals(pkg)) {
                return true;
            }
        }
        return false;
    }

    public static boolean isContainsPackage(List<PackageInfo> repo, String pkg, String version) {
        for (PackageInfo packageInfo : repo) {
            if (packageInfo.getName().equals(pkg) &&
                    packageInfo.getVersion().equals(version)) {
                return true;
            }
        }
        return false;
    }

    @Nullable
    public static PackageInfo getPackageByName(List<PackageInfo> repo, String pkg) {
        for (PackageInfo packageInfo : repo) {
            if (packageInfo.getName().equals(pkg)) {
                return packageInfo;
            }
        }
        return null;
    }

    public static PackageInfo getPackage(List<PackageInfo> repo, String pkg, String version) {
        for (PackageInfo packageInfo : repo) {
            if (packageInfo.getName().equals(pkg) &&
                    packageInfo.getVersion().equals(version)) {
                return packageInfo;
            }
        }
        return null;
    }

    private static List<PackageInfo> parseRepoXml(List<PackageInfo> list, String repo, String url) {
        if (repo != null) {
            XMLParser parser = new XMLParser();
            Document doc = parser.getDomElement(repo); // getting DOM element
            if (doc == null) {
                return list;
            }
            NodeList nl = doc.getElementsByTagName(KEY_PACKAGE);
            if (list == null) {
                list = new ArrayList<>();
            }

            for (int i = 0; i < nl.getLength(); i++) {
                Element e = (Element) nl.item(i);
                int size;
                int filesize;
                if (DEBUG) {
                    System.out.println(TAG + " pkg [ " + parser.getValue(e, KEY_NAME) + " ][ " + parser.getValue(e, KEY_SIZE) + "]");
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
                if (isContainsPackage(list, parser.getValue(e, KEY_NAME), parser.getValue(e, KEY_VERSION))) {
                    if (DEBUG) {
                        System.out.println(TAG + "skip exists pkg" + parser.getValue(e, KEY_NAME));
                    }
                    continue;
                }
                PackageInfo packageInfo = new PackageInfo(
                        parser.getValue(e, KEY_NAME),
                        parser.getValue(e, KEY_FILE),
                        size /*Integer.valueOf(parser.getValue(e, KEY_SIZE))*/,
                        filesize /*Integer.valueOf(parser.getValue(e, KEY_FILESIZE))*/,
                        parser.getValue(e, KEY_VERSION),
                        parser.getValue(e, KEY_DESC),
                        parser.getValue(e, KEY_DEPENDS),
                        parser.getValue(e, KEY_ARCH),
                        parser.getValue(e, KEY_REPLACES),
                        url);
                list.add(packageInfo);
                if (DEBUG) {
                    System.out.println(TAG + " added pkg = " + packageInfo.getName());
                }
            }
        }
        return list;
    }

    private static String replaceMacro(String str) {
        if (str != null) {
            str = str.replaceAll("\\$\\{HOSTARCH\\}", ndkArch);
            str = str.replaceAll("\\$\\{HOSTNDKARCH\\}", ndkArch);
            str = str.replaceAll("\\$\\{HOSTNDKVERSION\\}", String.valueOf(ndkVersion));
        }
        return str;
    }

    public static List<PackageInfo> checkingForUpdates(List<PackageInfo> availablePackages,
                                                       List<PackageInfo> installedPackages) {
        List<PackageInfo> list = null;

        for (PackageInfo installedPkg : installedPackages) {
            for (PackageInfo pkg : availablePackages) {
                if (installedPkg.getName().equals(pkg.getName())) {
                    if (!installedPkg.getVersion().equals(pkg.getVersion())) {
                        if (list == null) {
                            list = new ArrayList<>();
                        }
                        list.add(pkg);
                    }
                    break;
                }
            }
        }
        return list;
    }

    public static List<PackageInfo> checkingForUpdates(PackagesLists packagesLists) {
        return checkingForUpdates(packagesLists.getAvailablePackages(),
                packagesLists.getInstalledPackages());
    }
}
