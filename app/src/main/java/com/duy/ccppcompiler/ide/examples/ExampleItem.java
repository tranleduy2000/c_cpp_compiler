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

package com.duy.ccppcompiler.ide.examples;

import android.support.annotation.NonNull;

/**
 * Created by Duy on 03-May-18.
 */

public class ExampleItem {
    public static final String TITLE = "title";
    public static final String DESC = "desc";
    public static final String RELATIVE_PATH = "relative_path";

    public String title = "";
    public String relativePath = "";
    public String desc = "";

    public ExampleItem(@NonNull String title, @NonNull String relativePath, @NonNull String desc) {
        this.title = title;
        this.relativePath = relativePath;
        this.desc = desc;
    }

    public ExampleItem() {

    }

    @Override
    public String toString() {
        return "ExampleItem{" +
                "title='" + title + '\'' +
                ", relativePath='" + relativePath + '\'' +
                ", desc='" + desc + '\'' +
                '}';
    }

    @NonNull
    public String getTitle() {
        return title;
    }

    public void setTitle(@NonNull String title) {
        this.title = title;
    }

    @NonNull
    public String getRelativePath() {
        return relativePath;
    }

    public void setRelativePath(@NonNull String relativePath) {
        this.relativePath = relativePath;
    }

    @NonNull
    public String getDesc() {
        return desc;
    }

    public void setDesc(@NonNull String desc) {
        this.desc = desc;
    }

    @Override
    public boolean equals(Object o) {

        if (this == o) return true;
        if (!(o instanceof ExampleItem)) return false;

        ExampleItem item = (ExampleItem) o;

        if (!title.equals(item.title)) return false;
        if (!relativePath.equals(item.relativePath)) return false;
        return desc.equals(item.desc);
    }

    @Override
    public int hashCode() {
        int result = title.hashCode();
        result = 31 * result + relativePath.hashCode();
        result = 31 * result + desc.hashCode();
        return result;
    }
}
