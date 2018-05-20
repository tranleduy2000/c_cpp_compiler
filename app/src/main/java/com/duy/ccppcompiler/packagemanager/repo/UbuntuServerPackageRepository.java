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

import android.support.annotation.NonNull;
import android.util.Log;

import com.duy.ccppcompiler.packagemanager.DownloadListener;
import com.duy.ccppcompiler.packagemanager.RepoParser;
import com.duy.ccppcompiler.packagemanager.model.PackageInfo;
import com.duy.common.DLog;
import com.pdaxrom.utils.XMLParser;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.net.MalformedURLException;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.List;

import static com.duy.ccppcompiler.packagemanager.RepoUtils.CPU_API;
import static com.duy.ccppcompiler.packagemanager.RepoUtils.replaceMacro;

/**
 * old cctools repo
 * Created by Duy on 20-May-18.
 */
public class UbuntuServerPackageRepository implements IPackageRepository {
    private static final String TAG = "UbuntuServerPackageRepo";
    private List<String> mUrls;

    public UbuntuServerPackageRepository(List<String> urls) {
        this.mUrls = urls;
    }

    @Override
    @NonNull
    public List<PackageInfo> getPackages() {
        List<PackageInfo> list = new ArrayList<>();
        RepoParser repoParser = new RepoParser();
        for (String url : mUrls) {
            url = url + "/" + CPU_API;
            if (DLog.DEBUG) DLog.d(TAG, "url = " + url);
            list.addAll(repoParser.parseRepoXml(getRepoXmlFromUrl(url), url));
        }
        return list;
    }

    @Override
    public void download(File saveToDir, PackageInfo packageInfo, DownloadListener listener) {
        //download file from url
        File temp = new File(saveToDir, packageInfo.getFileName());
        if (temp.exists()) {
            Log.i(TAG, "Use file " + temp.getAbsolutePath());
            listener.onComplete(temp);
            return;
        }

        URL fromUrl = packageInfo.getURL();
        if (fromUrl == null) {
            listener.onFailure(new MalformedURLException("URL not found"));
            return;
        }

        final String fileName = packageInfo.getFileName();
        DLog.i(TAG, "Downloading file " + fromUrl + "/" + fileName);

        try {
            URL url = new URL(fromUrl + "/" + fileName);
            URLConnection urlConnection = url.openConnection();
            urlConnection.setReadTimeout(3 * 60 * 1000); // timeout 3 minutes
            urlConnection.connect();

            int fileSize = urlConnection.getContentLength();
            InputStream inputStream = urlConnection.getInputStream();
            if (inputStream == null) {
                throw new RuntimeException("stream is null");
            }

            Log.i(TAG, "File is " + temp.getAbsolutePath());
            int totalRead = 0;
            FileOutputStream outputStream = new FileOutputStream(temp);
            byte buf[] = new byte[128 * 1024];
            do {
                int numRead = inputStream.read(buf);
                if (numRead <= 0) {
                    break;
                }
                outputStream.write(buf, 0, numRead);
                totalRead += numRead;
                listener.onProgress(totalRead, fileSize);
            } while (true);

            inputStream.close();
            outputStream.close();
            if (totalRead != fileSize) {
                listener.onFailure(new RuntimeException("Partially downloaded file!"));
            }

        } catch (Exception e) {
            e.printStackTrace();
            temp.delete();
            listener.onFailure(e);
        }

    }

    private String getRepoXmlFromUrl(String url) {
        XMLParser parser = new XMLParser();
        String xml = parser.getXmlFromUrl(url + "/Packages");
        return replaceMacro(xml);
    }

}
