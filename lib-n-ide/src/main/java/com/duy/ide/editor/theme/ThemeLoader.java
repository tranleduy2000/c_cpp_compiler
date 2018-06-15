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

package com.duy.ide.editor.theme;

import android.content.Context;
import android.content.res.AssetManager;

import com.duy.common.DLog;
import com.duy.ide.editor.theme.model.EditorTheme;

import org.apache.commons.io.IOUtils;

import java.io.IOException;
import java.io.InputStream;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Properties;

public class ThemeLoader {
    public static final String ASSET_PATH = "themes/vscode";
    private static final String DEFAULT_EDITOR_THEME_LIGHT = "github-light.json.properties";
    private static final HashMap<String, EditorTheme> CACHED = new HashMap<>();
    private static final String TAG = "ThemeLoader";

    public static void init(Context context) {
        try {
            String[] themes = context.getAssets().list(ASSET_PATH);
            for (String theme : themes) {
                loadTheme(context, theme);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    public static EditorTheme getTheme(Context context, String fileName) {
        return loadTheme(context, fileName);
    }

    public static ArrayList<EditorTheme> getAll(Context context) {
        ArrayList<EditorTheme> themes = new ArrayList<>();
        try {
            String[] names = context.getAssets().list(ASSET_PATH);
            for (String name : names) {
                EditorTheme theme = loadTheme(context, name);
                themes.add(theme);
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return themes;
    }

    public static EditorTheme loadDefault(Context context) {
        return loadTheme(context, DEFAULT_EDITOR_THEME_LIGHT);
    }

    private static EditorTheme loadTheme(Context context, String fileName) {
        if (CACHED.get(fileName) != null) {
            return CACHED.get(fileName);
        }
        EditorTheme editorTheme = loadFromAsset(context.getAssets(), fileName);
        CACHED.put(fileName, editorTheme);
        return editorTheme;
    }

    private static EditorTheme loadFromAsset(AssetManager assets, String fileName) {
        try {
            InputStream input = assets.open(ASSET_PATH + "/" + fileName);
            String content = IOUtils.toString(input);
            input.close();

            Properties properties = new Properties();
            properties.load(new StringReader(content));

            EditorTheme editorTheme = loadTheme(properties);
            editorTheme.setFileName(fileName);
            return editorTheme;
        } catch (IOException e) {
            if (DLog.DEBUG) DLog.w(TAG, "loadFromAsset: Can not load theme " + fileName);
        }

        return null;
    }

    private static EditorTheme loadTheme(Properties properties) {
        EditorTheme editorTheme = new EditorTheme();
        editorTheme.load(properties);
        return editorTheme;
    }

}

