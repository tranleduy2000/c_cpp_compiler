package com.pdaxrom.pkgmanager;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import com.pdaxrom.utils.XMLParser;

public class RepoUtils {
	private static final String TAG = "RepoUtils";
	
    // XML node keys
    static public final String KEY_PACKAGE	= "package"; // parent node
    static public final String KEY_NAME		= "name";
    static public final String KEY_FILE		= "file";
    static public final String KEY_SIZE		= "size";
    static public final String KEY_FILESIZE	= "filesize";
    static public final String KEY_VERSION	= "version";
    static public final String KEY_DESC		= "description";
    static public final String KEY_DEPENDS	= "depends";
    static public final String KEY_ARCH		= "arch";
    static public final String KEY_REPLACES = "replaces";
    static public final String KEY_STATUS	= "status";

    private static String	_buildAbi;
	private static String	_ndkArch;
	private static int		_ndkVersion;

	private static boolean	_debug = false;
	
	public static void enableDebug(boolean debug) {
		_debug = debug;
	}
	
	public static void setVersion(String buildAbi, String ndkArch, int ndkVersion) {
		_buildAbi = buildAbi;
		_ndkArch = ndkArch;
		_ndkVersion = ndkVersion;
	}
	
	public static List<PackageInfo> getRepoFromUrl(String url) {
        return parseRepoXml(null, getRepoXmlFromUrl(url), url); // getting DOM element
	}

	public static List<PackageInfo> getRepoFromUrl(List<String> urls) {
		List<PackageInfo> list = null;
		for (String url: urls) {
			url = url + "/" + _buildAbi;
			list = parseRepoXml(list, getRepoXmlFromUrl(url), url); // getting DOM element
		}
		return list;
	}
	
	public static List<PackageInfo> getRepoFromDir(String path) {
		return parseRepoXml(null, getRepoXmlFromDir(path), path);
	}

	public static List<PackageInfo> getRepoFromDir(List<String> paths) {
		List<PackageInfo> list = null;
		for (String path: paths) {
			list = parseRepoXml(list, getRepoXmlFromDir(path), path);
		}
		return list;
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
					if (lowercaseName.endsWith(".pkgdesc")) {
						return true;
					} else {
						return false;
					}
				}
			};

			for (String filePath: dir.list(filter)) {
				if (_debug) {
					System.out.println(TAG + " Read file " + filePath);
				}
				File f = new File(path + "/" + filePath);
				try {
					FileInputStream fin = new FileInputStream(f);
					BufferedReader reader = new BufferedReader(new InputStreamReader(fin));
					String line = null;
					while((line = reader.readLine()) != null) {
						sb.append(line).append("\n");
					}
					reader.close();
				} catch (IOException e) {
					System.err.println(TAG + " getRepoXmlFromDir() IO error " + e);
				}
			}
			sb.append("</repo>");
			if (_debug) {
				System.out.println(TAG + " installed xml = " + replaceMacro(sb.toString()));
			}
			return replaceMacro(sb.toString());
		}
		return null;
	}
	
    public static boolean isContainsPackage(List<PackageInfo> repo, String pkg) {
    	for (PackageInfo packageInfo: repo) {
    		if (packageInfo.getName().equals(pkg)) {
    			return true;
    		}
    	}
    	return false;
    }

    public static boolean isContainsPackage(List<PackageInfo> repo, String pkg, String version) {
    	for (PackageInfo packageInfo: repo) {
    		if (packageInfo.getName().equals(pkg) &&
    				packageInfo.getVersion().equals(version)) {
    			return true;
    		}
    	}
    	return false;
    }

    public static PackageInfo getPackageByName(List<PackageInfo> repo, String pkg) {
    	for (PackageInfo packageInfo: repo) {
    		if (packageInfo.getName().equals(pkg)) {
    			return packageInfo;
    		}
    	}
    	return null;    	
    }

    public static PackageInfo getPackage(List<PackageInfo> repo, String pkg, String version) {
    	for (PackageInfo packageInfo: repo) {
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
	        	list = new ArrayList<PackageInfo>();
	        }

	    	for (int i = 0; i < nl.getLength(); i++) {
	    		Element e = (Element) nl.item(i);
	    		int size;
	    		int filesize;
	    		if (_debug) {
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
	    			if (_debug) {
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
				if (_debug) {
					System.out.println(TAG + " added pkg = " + packageInfo.getName());
				}
	    	}
		}
		return list;
    }
    
    private static String replaceMacro(String str) {
    	if (str != null) {
    		str = str.replaceAll("\\$\\{HOSTARCH\\}", _ndkArch);
    		str = str.replaceAll("\\$\\{HOSTNDKARCH\\}", _ndkArch);
    		str = str.replaceAll("\\$\\{HOSTNDKVERSION\\}", String.valueOf(_ndkVersion));
    	}
    	return str;
    }

    public static List<PackageInfo> checkingForUpdates(List<PackageInfo> availablePackages,
    		List<PackageInfo> installedPackages) {
    	List<PackageInfo> list = null;
    	
    	for (PackageInfo installedPkg: installedPackages) {
    		for (PackageInfo pkg: availablePackages) {
    			if (installedPkg.getName().equals(pkg.getName())) {
    				if (!installedPkg.getVersion().equals(pkg.getVersion())) {
    					if (list == null) {
    						list = new ArrayList<PackageInfo>();
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
