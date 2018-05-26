package com.duy.ide.editor.theme;

import android.content.Context;
import android.content.res.AssetManager;

import com.duy.ide.editor.theme.model.EditorTheme;

import org.apache.commons.io.IOUtils;

import java.io.IOException;
import java.io.InputStream;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Properties;

public class ThemeLoader {
    private static final String ASSET_PATH = "themes/vscode";
    private static final String DEFAULT_EDITOR_THEME_LIGHT = "bold-light.json.properties";
    private static final HashMap<String, EditorTheme> CACHED = new HashMap<>();

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
            e.printStackTrace();
        }

        return null;
    }


    private static EditorTheme loadTheme(Properties properties) {
        EditorTheme editorTheme = new EditorTheme();
        editorTheme.load(properties);
        return editorTheme;
    }

}

