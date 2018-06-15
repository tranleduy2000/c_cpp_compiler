/*
 * Copyright (C) 2018 Tran Le Duy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package com.duy.ide.file;

import android.content.Context;

import com.jecelyin.common.utils.SysUtils;

import java.io.File;
import java.io.IOException;

/**
 * Created by Duy on 25-Apr-18.
 */

public class FileManager {
    private Context context;

    public FileManager(Context context) {
        this.context = context;
    }

    public File createNewFile(String fileName) {
        File file = new File(getApplicationDir(), fileName);
        file.getParentFile().mkdirs();
        try {
            file.createNewFile();
        } catch (IOException e) {
            e.printStackTrace();
        }
        return null;
    }

    public File getApplicationDir() {
        String path = SysUtils.getAppStoragePath(context);
        return new File(path);
    }
}
