package com.duy.ide.editor.theme;

import android.content.Context;
import android.content.res.AssetManager;
import android.graphics.Color;

import com.duy.ide.editor.theme.model.EditorTheme;
import com.jecelyin.common.utils.DLog;

import org.apache.commons.io.IOUtils;

import java.io.IOException;
import java.io.InputStream;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Map;
import java.util.Properties;

public class ThemeLoader {
    private static final String TAG = "ThemeLoader";
    private static final HashMap<String, EditorTheme> CACHED = new HashMap<>();
    private static final String ASSET_PATH = "themes/vscode";

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

    public static EditorTheme getTheme(Context context, String name) {
        loadTheme(context, name);
        return CACHED.get(name);
    }

    public static ArrayList<EditorTheme> getAll() {
        ArrayList<EditorTheme> themes = new ArrayList<>();
        for (Map.Entry<String, EditorTheme> entry : CACHED.entrySet()) {
            themes.add(entry.getValue());
        }
        return themes;
    }

    public static EditorTheme loadDefault(Context context) {
        AssetManager assets = context.getAssets();
        return loadFromAsset(assets, "themes/vscode/absent-contrast.json.properties");
    }

    private static void loadTheme(Context context, String theme) {
//        if (CACHED.get(theme) != null) {
//            return;
//        }
        EditorTheme editorTheme = loadFromAsset(context.getAssets(), ASSET_PATH + "/" + theme);
        CACHED.put(theme, editorTheme);
    }

    private static EditorTheme loadFromAsset(AssetManager assets, String propFile) {
        try {
            InputStream input = assets.open(propFile);
            String content = IOUtils.toString(input);
            input.close();

            Properties properties = new Properties();
            properties.load(new StringReader(content));

            return loadTheme(properties);
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

    private static int getColor(Properties properties, ThemeAttr attr, int defaultValue) {
        try {
            String str = properties.getProperty(attr.getKey());
            return Color.parseColor(str);
        } catch (Exception e) {
            if (DLog.DEBUG) DLog.e(TAG, "getColor: ", e);
            return defaultValue;
        }
    }

}

