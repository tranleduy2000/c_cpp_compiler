package com.pdaxrom.pkgmanager;

import java.util.List;

class PackagesLists {
	private List<PackageInfo> availablePackages = null;
	private List<PackageInfo> installedPackages = null;

	PackagesLists() {
		
	}
	
	PackagesLists(List<PackageInfo> availablePackages, List<PackageInfo> installedPackages) {
		this.availablePackages = availablePackages;
		this.installedPackages = installedPackages;
	}
	
	public List<PackageInfo> getInstalledPackages() {
		return installedPackages;
	}
	
	public void setInstalledPackages(List<PackageInfo> installedPackages) {
		this.installedPackages = installedPackages;
	}

	public List<PackageInfo> getAvailablePackages() {
		return availablePackages;
	}

	public void setAvailablePackages(List<PackageInfo> availablePackages) {
		this.availablePackages = availablePackages;
	}	
}
