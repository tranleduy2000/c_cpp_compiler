package com.pdaxrom.cmdline;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.OutputStreamWriter;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import com.pdaxrom.utils.XMLParser;

public class AProjectHelper {
	public final static int PROJECT_PACKAGE		= 1;
	public final static int PROJECT_VERSIONCODE = 2;
	public final static int PROJECT_VERSIONNAME = 3;
	public final static int PROJECT_TARGETSDK	= 4;
	public final static int PROJECT_MINSDK		= 5;
	public final static int PROJECT_APPNAME		= 6;
	public final static int PROJECT_ACTIVITIES  = 7;
	
	public static void main(String[] args) {
		int argc = 0;
		while (argc < args.length) {
			if (args[argc].equals("BuildConfig") &&
					(argc + 1) < args.length) {
				argc++;
				boolean result = false;
				if (args[argc].equals("debug")) {
					result = genBuildConfig(".", true);
				} else if (args[argc].equals("release")) {
					result = genBuildConfig(".", false);
				} else {
					System.err.println("ERROR: debug or release keywords are accepted only.");
				}
				if (result == false) {
					System.exit(1);
				}
			} else if (args[argc].equals("Package")) {
				String str = getManifestInfo(".", PROJECT_PACKAGE);
				if (str == null) {
					str = "";
				}
				System.out.println(str);
				return;
			} else if (args[argc].equals("VersionCode")) {
				String str = getManifestInfo(".", PROJECT_VERSIONCODE);
				if (str == null) {
					str = "";
				}
				System.out.println(str);
				return;
			} else if (args[argc].equals("VersionName")) {
				String str = getManifestInfo(".", PROJECT_VERSIONNAME);
				if (str == null) {
					str = "";
				}
				System.out.println(str);
				return;
			} else if (args[argc].equals("TargetSDK")) {
				String str = getManifestInfo(".", PROJECT_TARGETSDK);
				if (str == null) {
					str = "";
				}
				System.out.println(str);
				return;
			} else if (args[argc].equals("MinSDK")) {
				String str = getManifestInfo(".", PROJECT_MINSDK);
				if (str == null) {
					str = "";
				}
				System.out.println(str);
				return;
			} else if (args[argc].equals("AppName")) {
				String str = getManifestInfo(".", PROJECT_APPNAME);
				if (str == null) {
					str = "";
				}
				System.out.println(str);
				return;
			} else if (args[argc].equals("Activities")) {
				String str = getManifestInfo(".", PROJECT_ACTIVITIES);
				if (str == null) {
					str = "";
				}
				System.out.println(str);
				return;
			} else if (args[argc].equals("help")) {
				System.out.println("Usage:");
				System.out.println("aproject-helper help");
				System.out.println("aproject-helper BuildConfig debug|release");
				System.out.println("aproject-helper Package");
				System.out.println("aproject-helper VersionCode");
				System.out.println("aproject-helper VersionName");
				System.out.println("aproject-helper TargetSDK");
				System.out.println("aproject-helper MinSDK");
				System.out.println("aproject-helper AppName");
				System.out.println("aproject-helper Activities");
				return;
			}
			argc++;
		}
	}
	
	/**
	 * Generate BuildConfig file for project
	 * @param path project path
	 * @param debug project build mode
	 * @return false if error
	 */
	public static boolean genBuildConfig(String path, boolean debug) {
		String project = getManifestInfo(path, PROJECT_PACKAGE);
		if (project != null) {
			String dir = path + "/gen/" + project.replace(".", "/");
			if (!new File(dir).exists()) {
				new File(dir).mkdirs();
			}
			try {
				FileOutputStream fos = new FileOutputStream(dir + "/BuildConfig.java");
				BufferedWriter writer = new BufferedWriter(new OutputStreamWriter(fos));
				writer.write("/** Automatically generated file. DO NOT MODIFY */\n");
				writer.newLine();
				writer.write("package " + project + ";\n");
				writer.newLine();
				writer.write("public final class BuildConfig {\n");
				writer.write("\tpublic final static boolean DEBUG = " + (debug?"true":"false") + ";\n");
				
				writer.write("}\n");
				writer.close();
				return true;
			} catch (Exception e) {
				System.err.println("Cannot write BuildConfig.java " + e);
			}
			
		} else {
			System.err.println("Cannot find project package info!");
		}
		
		return false;
	}
	
	/**
	 * Return info from Android Manifest
	 * @param path project path
	 * @param request
	 * @return null if error
	 */
	public static String getManifestInfo(String path, int request) {
		XMLParser xmlParser = new XMLParser();
		String xml = xmlParser.getXmlFromFile(path + "/AndroidManifest.xml");
		if (xml == null) {
			System.err.println("ERROR: No AndroidManifest.xml found!");
			return null;
		}
		
		Document doc = xmlParser.getDomElement(xml);
		if (doc == null) {
			System.err.println("ERROR: Bad xml file!");
			return null;
		}
		
		NodeList nl = doc.getElementsByTagName("manifest");
		
    	Element e = (Element) nl.item(0);
    	if (e == null) {
    		return null;
    	}
		switch(request) {
		case PROJECT_PACKAGE:
			return e.getAttribute("package");
		case PROJECT_VERSIONCODE:
			return e.getAttribute("android:versionCode");
		case PROJECT_VERSIONNAME:
			return e.getAttribute("android:versionName");
		case PROJECT_TARGETSDK:
	    	e = (Element) e.getElementsByTagName("uses-sdk").item(0);
	    	if (e != null) {
	    		return e.getAttribute("android:targetSdkVersion");
	    	}
	    	break;
		case PROJECT_MINSDK:
	    	e = (Element) e.getElementsByTagName("uses-sdk").item(0);
	    	if (e != null) {
	    		return e.getAttribute("android:minSdkVersion");
	    	}
	    	break;
		case PROJECT_APPNAME:
			e = (Element) e.getElementsByTagName("application").item(0);
			if (e != null) {
				return e.getAttribute("android:name");
			}
			break;
		case PROJECT_ACTIVITIES:
			e = (Element) e.getElementsByTagName("application").item(0);
			if (e != null) {
				String activities = "";
				NodeList al = e.getElementsByTagName("activity");
				for (int i = 0; i < al.getLength(); i++) {
					String name = ((Element) al.item(i)).getAttribute("android:name");
					if (name != null) {
						if (i > 0) {
							activities += " ";
						}
						activities += name;
					}
				}
				return activities;
			}
			break;
		}

		return null;
	}
}
