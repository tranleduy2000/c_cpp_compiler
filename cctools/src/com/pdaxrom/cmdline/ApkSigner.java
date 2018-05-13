package com.pdaxrom.cmdline;

import kellinwood.security.zipsigner.ZipSigner;

public class ApkSigner {
	public static void main(String[] args) {
		if (args.length != 2) {
			usage();
			return;
		}
		try {
			ZipSigner zipSigner = new ZipSigner();
			zipSigner.setKeymode(ZipSigner.KEY_TESTKEY);
			zipSigner.signZip(args[0], args[1]);
		} catch (Exception e) {
			System.err.println("Error during apk signing: " + e);
		}
	}
	
	private static void usage() {
		System.out.println("Usage: apksigner <input.apk> <output.apk>");
	}
}