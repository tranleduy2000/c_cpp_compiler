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

package com.duy.ccppcompiler.packagemanager.repo;

import android.util.Log;

import com.duy.ccppcompiler.packagemanager.IPackageLoadListener;
import com.duy.ccppcompiler.packagemanager.PackageDownloadListener;
import com.duy.ccppcompiler.packagemanager.RepoParser;
import com.duy.ccppcompiler.packagemanager.model.PackageInfo;
import com.duy.common.DLog;
import com.pdaxrom.utils.XMLParser;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.List;

import static com.duy.ccppcompiler.packagemanager.RepoUtils.replaceMacro;
import static com.duy.ccppcompiler.packagemanager.repo.RepoConstants.PACKAGES_INDEX_FILE;

/**
 * old cctools repo
 * Created by Duy on 20-May-18.
 */
public class UbuntuServerPackageRepository extends PackageRepositoryImpl {
    private static final String TAG = "UbuntuServerPackageRepo";
    private final String mUrl;

    public UbuntuServerPackageRepository(String url) {
        if (url == null) {
            throw new NullPointerException();
        }
        mUrl = url;
    }

    @Override
    public void getPackagesInBackground(final IPackageLoadListener listener) {
        Thread thread = new Thread(new Runnable() {
            @Override
            public void run() {
                RepoParser repoParser = new RepoParser();
                if (DLog.DEBUG) DLog.d(TAG, "url = " + mUrl);
                String repoXmlFromUrl = getRepoXmlFromUrl(mUrl);
                List<PackageInfo> list = new ArrayList<>(repoParser.parseRepoXml(repoXmlFromUrl));
                listener.onSuccess(list);
            }
        });
        thread.start();
    }

    @Override
    public void download(File saveToDir, PackageInfo packageInfo, PackageDownloadListener listener) {
        if (DLog.DEBUG) DLog.d(TAG, "download() called with: ");
        if (DLog.DEBUG) DLog.d(TAG, "saveToDir = " + saveToDir);
        if (DLog.DEBUG) DLog.d(TAG, "packageInfo = " + packageInfo);

        //download file from url
        File temp = new File(saveToDir, packageInfo.getFileName());
        if (temp.exists() && temp.length() != 0) {
            Log.i(TAG, "Use file " + temp.getAbsolutePath());
            listener.onDownloadComplete(packageInfo, temp);
            return;
        }

        URL fromUrl = getUrl(packageInfo);
        if (fromUrl == null) {
            listener.onFailure(new MalformedURLException("Download URL not found (" + packageInfo.getFileName() + ")"));
            return;
        }

        DLog.i(TAG, "Downloading file " + fromUrl.toString());

        try {
            URLConnection urlConnection = fromUrl.openConnection();
            urlConnection.setReadTimeout(3 * 60 * 1000); // timeout 3 minutes
            urlConnection.connect();

            int fileSize = urlConnection.getContentLength();
            InputStream input = urlConnection.getInputStream();
            if (input == null) {
                listener.onFailure(new RuntimeException("Can not open input stream for package " + packageInfo.getName()));
                return;
            }

            Log.i(TAG, "File is " + temp.getAbsolutePath());
            int totalRead = 0;
            FileOutputStream output = new FileOutputStream(temp);
            byte buf[] = new byte[128 * 1024];
            do {
                int numRead = input.read(buf);
                if (numRead <= 0) {
                    break;
                }
                output.write(buf, 0, numRead);
                totalRead += numRead;
                listener.onDownloadProgress(packageInfo, totalRead, fileSize);
            } while (true);

            input.close();
            output.close();
            if (totalRead != fileSize) {
                listener.onFailure(new IOException("Partially downloaded file!"));
                return;
            }
            listener.onDownloadComplete(packageInfo, temp);
        } catch (Exception e) {
            e.printStackTrace();
            temp.delete();
            listener.onFailure(e);
        }
    }

    private URL getUrl(PackageInfo packageInfo) {
        try {
            String url = mUrl + "/" + packageInfo.getFileName();
            return new URL(url);
        } catch (Exception e) {
            e.printStackTrace();
            return null;
        }
    }

    private String getRepoXmlFromUrl(String url) {
        XMLParser parser = new XMLParser();
        String xml = parser.getXmlFromUrl(url + "/" + PACKAGES_INDEX_FILE);
        return replaceMacro(xml);
    }

}
