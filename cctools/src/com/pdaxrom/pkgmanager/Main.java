package com.pdaxrom.pkgmanager;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.File;
import java.io.FileDescriptor;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import com.pdaxrom.utils.Utils;

class Main {
    static final int sdk2ndk_arm[] = {
	/*   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23  */
	-1, -1, -1,  3,  4,  5,  5,  5,  8,  9,  9,  9,  9, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, -1
    };
    static final int sdk2ndk_mips[] = {
	/*   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1,  9,  9, -1, -1, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, -1
    };
    static final int sdk2ndk_x86[] = {
	/*   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1,  9,  9, -1, -1, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, -1
    };

    static private int ndkVersion;
    static private int sdkVersion;
    static private String ndkArch;
    static private String cpuAbi;

    static private String URL = "http://mirror.cctools.info/packages";
    
    private static final String toolchainDir = "/data/data/com.pdaxrom.cctools/root";
    private static String sdCardDir;
    private static String filesDir;
    private static final String PKGS_LISTS_DIR	= "/installed/";

    private static PackagesLists packagesLists = new PackagesLists();

    public static void main(String[] args) {
    	setupVersion();
    	sdCardDir = getEnv(toolchainDir + "/cctools", "EXTERNAL_STORAGE");
    	if (!new File(sdCardDir).exists()) {
    		System.err.println("Error: Can not find internal SD card path.");
    		return;
    	}
    	sdCardDir = sdCardDir + "/CCTools";
    	filesDir = sdCardDir + "/backup";
    	
    	packagesLists.setInstalledPackages(RepoUtils.getRepoFromDir(toolchainDir + "/" + PKGS_LISTS_DIR));
		packagesLists.setAvailablePackages(RepoUtils.getRepoFromUrl(getReposList()));
		
		List<PackageInfo> packagesForUpdate = null;
		
		if (packagesLists.getAvailablePackages() != null &&
				packagesLists.getInstalledPackages() != null) {
			packagesForUpdate = RepoUtils.checkingForUpdates(packagesLists);
		}

    	if (args.length == 0) {
        	System.out.println("SDK " + sdkVersion + 
        						", NKD " + ndkVersion + 
        						", Arch " + ndkArch + 
        						", CPU ABI " + cpuAbi);
        	List<String> list = getReposList();
        	if (list != null) {
        		for (String url: list) {
        			System.out.println("Repo " + url);
        		}
        	}

        	if (packagesForUpdate != null) {
        		System.out.println();
        		System.out.println("Available update for: ");
        		for (PackageInfo pkg: packagesForUpdate) {
        			System.out.print(pkg.getName() + " ");
        		}
        		System.out.println();
        	}
        	
    		return;
    	}

		if (args[0].equals("list")) {
			if (packagesLists.getAvailablePackages() != null) {
				for (PackageInfo pkg: packagesLists.getAvailablePackages()) {
					System.out.println(pkg.getName() + " - " + pkg.getDescription());
				}
			}
		} else if (args[0].equals("installed")) {
			if (packagesLists.getInstalledPackages() != null) {
				for (PackageInfo pkg: packagesLists.getInstalledPackages()) {
					System.out.println(pkg.getName() + " - " + pkg.getDescription());
				}
			}
		} else if (args[0].equals("search")) {
			if (packagesLists.getAvailablePackages() != null) {
				int i;
				for (i = 1; i < args.length; i++) {
					boolean found = false;
					for (PackageInfo pkg: packagesLists.getAvailablePackages()) {
						if (pkg.getName().equals(args[i])) {
							System.out.println(pkg.getName() + " - " + pkg.getDescription());
							found = true;
							break;
						}
					}
					if (!found) {
						System.out.println(args[i] + " not found");
					}
				}
			}
		} else if (args[0].equals("install")) {
			if (packagesLists.getAvailablePackages() != null) {
				int i;
	        	final InstallPackageInfo installInfo = new InstallPackageInfo();

				for (i = 1; i < args.length; i++) {
					if (RepoUtils.isContainsPackage(packagesLists.getAvailablePackages(), args[i])) {
						installInfo.addPackage(packagesLists, args[i]);
					} else {						
						System.out.println(args[i] + " not found");
					}
				}
				if (!installPackage(installInfo)) {
					System.err.println("Errors during installation.");
				}
			}
		} else if (args[0].equals("upgrade")) {
			if (packagesForUpdate != null) {
	        	final InstallPackageInfo updateInfo = new InstallPackageInfo();

				for (PackageInfo pkg: packagesForUpdate) {
					updateInfo.addPackage(packagesLists, pkg.getName());
				}
				if (!installPackage(updateInfo)) {
					System.err.println("Errors during upgrade.");
				}				
			}
		} else if (args[0].equals("show")) {
			if (packagesLists.getAvailablePackages() != null) {
				if (args.length > 1) {
					int i;
					for (i = 1; i < args.length; i++) {
						PackageInfo pkg = RepoUtils.getPackageByName(packagesLists.getAvailablePackages(), args[i]);
						if (pkg != null) {
							System.out.println("Name:           " + pkg.getName());
							System.out.println("Version:        " + pkg.getVersion());
							System.out.println("Arch:           " + pkg.getArch());
							System.out.println("Depends:        " + pkg.getDepends());
							System.out.println("Replaces:       " + pkg.getReplaces());
							System.out.println("File:           " + pkg.getFile());
							System.out.println("File size:      " + pkg.getFileSize());
							System.out.println("Installed size: " + pkg.getSize());
							System.out.println("  " + pkg.getDescription());
							System.out.println();
						}
					}
				}
			}
		} else if (args[0].equals("uninstall")) {
			if (packagesLists.getInstalledPackages() != null) {
				if (args.length > 1) {
					int i;
					for (i = 1; i < args.length; i++) {
						if (RepoUtils.isContainsPackage(packagesLists.getInstalledPackages(), args[i])) {
							uninstallPackage(args[i]);
						}
					}
				}
			}
		} else {
			System.out.println("CCTools console package manager");
			System.out.println();
			System.out.println("Usage:");
			System.out.println("pkgman                                     - summary about system and updates");
			System.out.println("pkgman list                                - show all packages");
			System.out.println("pkgman installed                           - show installed packages");
			System.out.println("pkgman search    <package> [<package> ...] - search package(s)");
			System.out.println("pkgman show      <package> [<package> ...] - show package(s) info");
			System.out.println("pkgman install   <package> [<package> ...] - install package(s)");
			System.out.println("pkgman uninstall <package> [<package> ...] - uninstall package(s)");
			System.out.println("pkgman upgrade                             - upgrade package(s)");
			System.out.println();
		}
    }

    private static void setupVersion() {
    	cpuAbi = getProperties("ro.product.cpu.abi");
        sdkVersion = Integer.parseInt(getProperties("ro.build.version.sdk"));
        ndkVersion = -1;
        ndkArch = "all";
        if (cpuAbi.startsWith("arm")) {
        	ndkArch = "armel";
        	if (sdk2ndk_arm.length > sdkVersion) {
        		ndkVersion = sdk2ndk_arm[sdkVersion];
        	} else {
        		ndkVersion = sdk2ndk_arm[sdk2ndk_arm.length -1];
        	}
        } else if (cpuAbi.startsWith("mips")) {
        	ndkArch = "mipsel";
        	if (sdk2ndk_mips.length > sdkVersion) {
        		ndkVersion = sdk2ndk_mips[sdkVersion];
        	} else {
        		ndkVersion = sdk2ndk_mips[sdk2ndk_mips.length -1];
        	}
        } else {
        	ndkArch = "i686";
        	if (sdk2ndk_x86.length > sdkVersion) {
        		ndkVersion = sdk2ndk_x86[sdkVersion];
        	} else {
        		ndkVersion = sdk2ndk_x86[sdk2ndk_x86.length -1];
        	}
        }

        RepoUtils.setVersion(cpuAbi, ndkArch, ndkVersion);
    }

    private static String getProperties(String prop) {
    	String retval = null;
        try {
        	String line;
            Process p = Runtime.getRuntime().exec("getprop");
            BufferedReader input = new BufferedReader(new InputStreamReader(p.getInputStream()));
            while ((line = input.readLine()) != null) {
            	Matcher m = Pattern.compile("\\[" + prop + "\\].*\\[(.+?)\\]").matcher(line);
            	while(m.find()) {
            		retval = m.group(1);
            	}
            }
            input.close();
        } catch (Exception err) {
        	err.printStackTrace();
        }
        return retval;
    }
    
	private static boolean downloadAndUnpack(String file, String from, String to, String log) {
		File temp = new File(filesDir + "/" + file);
		if (!temp.exists()) {
			try {
				int totalread = 0;
				System.out.println("Downloading file " + from + "/" + file);
				URL url = new URL(from + "/" + file);
				URLConnection cn = url.openConnection();
				cn.setReadTimeout(3 * 60 * 1000); // timeout 3 minutes
				cn.connect();
				int file_size = cn.getContentLength();
				if (sdkVersion >= 9) {
					File partition = new File(filesDir);
					if (partition.getUsableSpace() < file_size) {
						temp.delete();
						System.err.println("Can't download, need " + 
								Utils.humanReadableByteCount(file_size - partition.getUsableSpace(), false) + 
								" on SD");
						return false;
					}
				}
				InputStream stream = cn.getInputStream();
				if (stream == null) {
					throw new RuntimeException("stream is null");
				}
				FileOutputStream out = new FileOutputStream(temp);
				byte buf[] = new byte[128 * 1024];
				do {
					int numread = stream.read(buf);
					if (numread <= 0) {
						break;
					}
					out.write(buf, 0, numread);
					totalread += numread;
					System.out.print("Received " + totalread + " from " + file_size + "\r");
				} while (true);
				stream.close();
				out.close();
				if (totalread != file_size) {
					throw new RuntimeException("Partially downloaded file!");
				}
			} catch (Exception e) {
				temp.delete();
				System.err.println("Error downloading file " + file);
				return false;
			}
		} else {
			System.out.println("Cached file " + temp.getAbsolutePath());
		}

		String tempPath = temp.getAbsolutePath();
		//System.out.println("Unpacking file " + tempPath + " to " + to);
		String logFile = log;
		try {
			int need_mem = Utils.unzippedSize(tempPath);
			if (need_mem < 0) {
				throw new RuntimeException("bad archive");
			}
			
			File partition = new File(to);			
			if (partition.getUsableSpace() < need_mem) {
				System.err.println("Need " + 
					Utils.humanReadableByteCount(need_mem, false) + 
					" but " + 
					Utils.humanReadableByteCount(partition.getUsableSpace(), false) + 
					" available");
				return false;
			}
			if (logFile == null) {
				logFile = toolchainDir + PKGS_LISTS_DIR + file + ".list";
			}
			if (Utils.unzip(tempPath, to, logFile) != 0) {
				if ((new File(logFile)).exists()) {
					(new File(logFile)).delete();
				}
				throw new RuntimeException("bad archive");
			}
		} catch (Exception e) {
			temp.delete();
			System.out.println("Corrupted archive, restart application and try install again");
			return false;
		}
		return true;
	}

    private static boolean installPackage(InstallPackageInfo info) {
    	List<String> postinstList = new ArrayList<String>();
    	for (PackageInfo packageInfo: info.getPackagesList()) {
    		if ((new File(toolchainDir + "/" + PKGS_LISTS_DIR + "/" 
    				+ packageInfo.getName() + ".pkgdesc")).exists()) {
    			PackageInfo oldPackage = RepoUtils.getPackageByName(packagesLists.getInstalledPackages(),
    																packageInfo.getName());
    			if (packageInfo.getVersion().equals(oldPackage.getVersion())) {
    				System.out.println("Package " + packageInfo.getName() + " already installed.");
    				continue;
    			} else {
    				uninstallPackage(packageInfo.getName());
    				if ((new File(filesDir + "/" + oldPackage.getFile())).exists()) {
    					(new File(filesDir + "/" + oldPackage.getFile())).delete();
    				}
    			}
    		}
    		
    		if (RepoUtils.isContainsPackage(packagesLists.getInstalledPackages(), packageInfo.getReplaces())) {
    			System.out.println("Replace package " + packageInfo.getReplaces());
    			PackageInfo oldPackage = RepoUtils.getPackageByName(packagesLists.getInstalledPackages(),
						packageInfo.getReplaces());
				uninstallPackage(oldPackage.getName());
				if ((new File(filesDir + "/" + oldPackage.getFile())).exists()) {
					(new File(filesDir + "/" + oldPackage.getFile())).delete();
				}
    		}
    		
    		System.out.println("Installing package " + packageInfo.getName());

    		if (!downloadAndUnpack(packageInfo.getFile(), packageInfo.getUrl(), toolchainDir, 
					toolchainDir + "/" + PKGS_LISTS_DIR + "/" + packageInfo.getName() + ".list")) {
				return false;
			}
			// Move package info files from root directory
			String[] infoFiles = {"pkgdesc", "postinst", "prerm"};
			for (String infoFile: infoFiles) {
				if ((new File(toolchainDir + "/" + infoFile)).exists()) {
					String infoFilePath = toolchainDir + "/" + PKGS_LISTS_DIR  + "/" 
							+ packageInfo.getName() + "." + infoFile;
					try {
						Utils.copyDirectory(new File(toolchainDir + "/" + infoFile),
											new File(infoFilePath));
						if (infoFile.equals("postinst")) {
							postinstList.add(packageInfo.getName());
						}
					} catch (IOException e) {
						System.err.println("Can't copy " + infoFile + ": " + e);
					}
					(new File(toolchainDir + "/" + infoFile)).delete();
				}				
			}
    	}
    
    	// Move Examples to sd card
		if ((new File(toolchainDir + "/cctools/Examples")).exists()) {
			try {
				Utils.copyDirectory(new File(toolchainDir + "/cctools/Examples"),
									new File(sdCardDir + "/Examples"));
				Utils.deleteDirectory(new File(toolchainDir + "/cctools/Examples"));
			} catch (IOException e) {
				System.err.println("Can't copy examples directory " + e);
			}
		}

		//Execute postinst scripts
		for (String name: postinstList) {
			String postinstFile = toolchainDir + "/" + PKGS_LISTS_DIR + "/" + name + ".postinst";
			System.out.println("Execute postinst file " + postinstFile);
			Utils.chmod(postinstFile, 0x1ed);
			system(postinstFile);
			(new File(postinstFile)).delete();
		}
		
    	return true;
    }

	private static boolean uninstallPackage(String name) {
		if (name != null) {
			System.out.println("Uninstalling package " + name);
			String prermFile = toolchainDir + "/" + PKGS_LISTS_DIR + "/" + name + ".prerm";
			if ((new File(prermFile)).exists()) {
				System.out.println("Execute prerm script " + prermFile);
				Utils.chmod(prermFile, 0x1ed);
				system(prermFile);
				(new File(prermFile)).delete();
			}
			String descFile = toolchainDir + "/" + PKGS_LISTS_DIR + "/" + name + ".pkgdesc";
			if ((new File(descFile)).exists()) {
				(new File(descFile)).delete();
			}
			String logFile = toolchainDir + "/" + PKGS_LISTS_DIR + "/" + name + ".list";
			if (!(new File(logFile)).exists()) {
				return false;
			}
			try {
				FileInputStream fin = new FileInputStream(logFile);
				DataInputStream in = new DataInputStream(fin);
				BufferedReader reader = new BufferedReader(new InputStreamReader(in));
				String line = "";
				while((line = reader.readLine()) != null) {
					(new File(toolchainDir + "/" + line)).delete();
				}
				in.close();
				(new File(logFile)).delete();
			} catch (Exception e) {
				System.err.println("Error during remove files " + e);
			}
			
		}
		return true;
	}

	private static List<String> getReposList() {
		List<String> list = null;
		String reposListFile = toolchainDir + "/cctools/etc/repos.list"; 
		if (new File(reposListFile).exists()) {
			try {
				FileInputStream fin = new FileInputStream(reposListFile);
				DataInputStream in = new DataInputStream(fin);
				BufferedReader reader = new BufferedReader(new InputStreamReader(in));
				String line = "";
				while ((line = reader.readLine()) != null) {
					if (list == null) {
						list = new ArrayList<String>();
					}
					line = line.trim();
					if (line.length() > 0) {
						if (line.startsWith("#") || line.startsWith(";")) {
							continue;
						}
						list.add(line);
					}
				}
				in.close();
			} catch (Exception e) {
				System.err.println("Error read repos list: " + e);
			}
		}

		if (list == null) {
			list = new ArrayList<String>();
			list.add(URL);
		}
		return list;
	}

	private static void system(String cmdline) {
		String cctoolsDir = toolchainDir + "/cctools";
		String bootClassPath = getEnv(cctoolsDir, "BOOTCLASSPATH");
		if (bootClassPath == null) {
			bootClassPath = "/system/framework/core.jar:/system/framework/ext.jar:/system/framework/framework.jar:/system/framework/android.policy.jar:/system/framework/services.jar"; 
		}
		String[] envp = {
//				"TMPDIR=" + Environment.getExternalStorageDirectory().getPath(),
				"PATH=" + cctoolsDir + "/bin:" + cctoolsDir + "/sbin:/sbin:/vendor/bin:/system/sbin:/system/bin:/system/xbin",
				"ANDROID_ASSETS=/system/app",
				"ANDROID_BOOTLOGO=1",				
				"ANDROID_DATA=" + cctoolsDir + "/var/dalvik",
				"ANDROID_PROPERTY_WORKSPACE=" + getEnv(cctoolsDir, "ANDROID_PROPERTY_WORKSPACE"),
				"ANDROID_ROOT=/system",
				"BOOTCLASSPATH=" + bootClassPath,
				"CCTOOLSDIR=" + cctoolsDir,
//				"CCTOOLSRES=" + getPackageResourcePath(),
				"LD_LIBRARY_PATH=" + cctoolsDir + "/lib",
				"HOME=" + cctoolsDir + "/home",
//				"SHELL=" + getShell(),
				"TERM=xterm",
				"PS1=$ ",
//				"SDDIR=" + sdCardDir,
//				"EXTERNAL_STORAGE=" + Environment.getExternalStorageDirectory().getPath(),
				};
		try {
			System.out.println("exec cmd " + cmdline + ", cctoolsdir " + cctoolsDir);
			Process p = Runtime.getRuntime().exec(cmdline, envp);
			p.waitFor();
		} catch (Exception e) {
			System.err.println("Exec exception " + e);
		}		
	}
	
	private static String getEnv(String baseDir, String variable) {
		String ret = null;
		String[] envp = {
//				"TMPDIR=" + Environment.getExternalStorageDirectory().getPath(),
				"PATH=" + baseDir + "/bin:" + baseDir + "/sbin:/sbin:/vendor/bin:/system/sbin:/system/bin:/system/xbin",
				"ANDROID_ASSETS=/system/app",
				"ANDROID_BOOTLOGO=1",				
				"ANDROID_DATA=" + baseDir + "/var/dalvik",
				"ANDROID_ROOT=/system",
				"CCTOOLSDIR=" + baseDir,
//				"CCTOOLSRES=" + getPackageResourcePath(),
				"LD_LIBRARY_PATH=" + baseDir + "/lib:/system/lib:/vendor/lib",
				"HOME=" + baseDir + "/home",
//				"SHELL=" + getShell(),
				"TERM=xterm",
				"PS1=$ ",
//				"SDDIR=" + sdCardDir,
//				"EXTERNAL_STORAGE=" + Environment.getExternalStorageDirectory().getPath(),
				};
		String[] argv = { "/system/bin/sh", "-c", "set"};
		int[] pId = new int[1];
		FileDescriptor fd = Utils.createSubProcess(baseDir, argv[0], argv, envp, pId);
		FileInputStream fis = new FileInputStream(fd);
		DataInputStream in = new DataInputStream(fis);
		BufferedReader reader = new BufferedReader(new InputStreamReader(in));
		String line = "";
		try {
			while((line = reader.readLine()) != null) {
				if (line.startsWith(variable + "=")) {
					if (line.indexOf("=") != -1) {
						ret = line.substring(line.indexOf("=") + 1);
						break;
					}
				}
			}
			in.close();
			Utils.waitFor(pId[0]);
		} catch (Exception e) {
			System.err.println("exception " + e);
		}
		return ret;
	}

}
