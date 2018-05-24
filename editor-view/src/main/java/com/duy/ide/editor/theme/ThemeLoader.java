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
import java.util.Properties;
import java.util.WeakHashMap;

public class ThemeLoader {
    private static final String TAG = "ThemeLoader";
    public static WeakHashMap<String, EditorTheme> themes;

    static {
        themes = new WeakHashMap<>();
    }

    public static EditorTheme loadDefault(Context context) {
        AssetManager assets = context.getAssets();
        return loadFromAsset(assets, "colorschemes/Solarized-dark.properties");
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

