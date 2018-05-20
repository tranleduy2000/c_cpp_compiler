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

import com.duy.ccppcompiler.packagemanager.RepoParser;
import com.duy.ccppcompiler.packagemanager.model.PackageInfo;
import com.duy.common.DLog;
import com.pdaxrom.utils.XMLParser;

import java.util.ArrayList;
import java.util.List;

import static com.duy.ccppcompiler.packagemanager.RepoUtils.CPU_API;
import static com.duy.ccppcompiler.packagemanager.RepoUtils.replaceMacro;

/**
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

    private String getRepoXmlFromUrl(String url) {
        XMLParser parser = new XMLParser();
        String xml = parser.getXmlFromUrl(url + "/Packages");
        return replaceMacro(xml);
    }

}
