package com.duy.ide.editor.theme;

import com.duy.ide.editor.theme.model.EditorTheme;
import com.duy.ide.editor.theme.model.GutterStyle;
import com.duy.ide.editor.theme.model.SyntaxStyle;
import com.duy.ide.editor.theme.model.WhiteSpaceStyle;

import junit.framework.TestCase;

import org.apache.commons.io.IOUtils;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.FileFilter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Properties;
import java.util.Set;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Using JUnit on Android studio faster than create class with main method
 */
public class VsCodeThemePreprocessor extends TestCase {
    private static final String TAG = "VsCodeThemePreprocessor";
    private final File mRootDir, mBuildDir;

    public VsCodeThemePreprocessor() {
        mRootDir = new File(System.getenv("user.dir"), "colorschemes/vscode");
        mBuildDir = new File(mRootDir, "build");
        mBuildDir.mkdir();
    }

    public void test() throws IOException, JSONException {
        removeDuplicateKeys();
        convertToJEditSchemes();
        copyToAndroidAssets();
    }

    private void copyToAndroidAssets() throws IOException {
        File outputDir = new File(mBuildDir, "output");
        outputDir.mkdir();
        File[] files = outputDir.listFiles(new FileFilter() {
            @Override
            public boolean accept(File pathname) {
                return pathname.getPath().toLowerCase().endsWith(".properties");
            }
        });
        File assestDir = new File("D:\\github\\CCppCompiler\\editor-view\\src\\main\\assets\\themes\\vscode");
        assestDir.mkdir();
        for (File file : files) {
            FileInputStream input = new FileInputStream(file);
            File outFile = new File(assestDir, file.getName());
            FileOutputStream output = new FileOutputStream(outFile);
            IOUtils.copy(input, output);
            input.close();
            output.close();
        }
    }

    private void removeDuplicateKeys() throws IOException, JSONException {
        File sourceDir = new File(mRootDir, "source");
        sourceDir.mkdir();
        File[] files = sourceDir.listFiles(new FileFilter() {
            @Override
            public boolean accept(File pathname) {
                return pathname.getPath().toLowerCase().endsWith(".json");
            }
        });
        File outDir = new File(mBuildDir, "generated");
        outDir.mkdir();
        for (File file : outDir.listFiles()) {
            file.delete();
        }

        for (File file : files) {
            System.out.println("file = " + file);

            FileInputStream input = new FileInputStream(file);
            String content = IOUtils.toString(input, "UTF-8");
            input.close();
            content = removeDuplicateKeys(content);

//            JSONObject jsonObject = new JSONObject(content);
//            content = jsonObject.toString();

            File outFile = new File(outDir, file.getName());
            FileOutputStream output = new FileOutputStream(outFile);
            IOUtils.write(content, output);
            output.close();
        }
    }

    private void sort(JSONObject jsonObject) {

    }

    private String removeDuplicateKeys(String content) {
        StringBuilder result = new StringBuilder();

        String[] lines = content.split("\n");
        boolean[] keep = new boolean[lines.length];
        Arrays.fill(keep, true);
        HashMap<String, Integer> exist = new HashMap<>();
        Pattern pattern = Pattern.compile("\"([A-Za-z]+)(\\.)([A-Za-z]+)\":");

        for (int i = 0; i < lines.length; i++) {
            String line = lines[i];
            Matcher matcher = pattern.matcher(line);
            if (matcher.find()) {
                String group = matcher.group();
                if (exist.get(group) != null) {
                    keep[exist.get(group)] = false;
                }
                exist.put(group, i);
            }
        }
        for (int i = 0; i < lines.length; i++) {
            String line = lines[i];
            if (keep[i] && !line.isEmpty()) {
                result.append(line).append("\n");
            }
        }
        return result.toString();
    }

    private void convertToJEditSchemes() {
        File outputDir = new File(mBuildDir, "output");
        outputDir.mkdir();
        for (File file : outputDir.listFiles()) {
            file.delete();
        }

        File inputDir = new File(mBuildDir, "generated");
        inputDir.mkdir();
        File[] files = inputDir.listFiles(new FileFilter() {
            @Override
            public boolean accept(File pathname) {
                return pathname.getPath().toLowerCase().endsWith(".json");
            }
        });
        for (File file : files) {
            System.out.println("file = " + file);
            try {

                FileInputStream input = new FileInputStream(file);
                String content = IOUtils.toString(input, "UTF-8");
                input.close();
                content = convert(content);

                File outFile = new File(outputDir, file.getName() + ".properties");
                FileOutputStream output = new FileOutputStream(outFile);
                IOUtils.write(content, output);
                output.close();
            } catch (Exception e) {
                System.err.println("convert failed " + file);
                e.printStackTrace();
            }
        }
    }

    //https://vscode-doc-jp.github.io/docs/getstarted/theme-color-reference.html
    private String convert(String content) throws IOException, JSONException {
        JSONObject theme = new JSONObject(content);
        JSONArray tokenColors = theme.getJSONArray("tokenColors");
        JSONObject colors = theme.getJSONObject("colors");

        Properties properties = new Properties();
        properties.put(EditorTheme.ThemeAttr.SCHEME_NAME.getKey(), theme.getString("name"));
        properties.put(EditorTheme.ThemeAttr.TYPE.getKey(), theme.getString("type"));

        //view.bgColor=#ffffff
        properties.put(EditorTheme.Attr.VIEW_BG_COLOR.getKey(), colors.getString("editor.background"));

        //view.caretColor=#000000
        properties.put(EditorTheme.Attr.VIEW_CARET_COLOR.getKey(), colors.getString("editorCursor.foreground"));

        //view.eolMarkerColor=#ff6600 todo

        //view.fgColor=#000000
        properties.put(EditorTheme.Attr.VIEW_FG_COLOR.getKey(), colors.getString("editor.foreground"));

        //view.gutter.bgColor=#fefffb
        properties.put(GutterStyle.Attr.VIEW_GUTTER_BG_COLOR.getKey(), colors.getString("editorGutter.background"));

        //view.gutter.currentLineColor=#ff0000 todo

        //view.gutter.fgColor=#666666
        properties.put(GutterStyle.Attr.VIEW_GUTTER_FG_COLOR.getKey(), colors.getString("editorLineNumber.foreground"));

        //view.gutter.focusBorderColor=#0099cc todo
        //view.gutter.foldColor=#8eb58e
        properties.put(GutterStyle.Attr.VIEW_GUTTER_FOLD_COLOR.getKey(), colors.getString("editorIndentGuide.background"));
        //view.gutter.highlightColor=#000099 todo
        //view.gutter.markerColor=#deedfd todo
        //view.gutter.noFocusBorderColor=#ffffff todo
        //view.gutter.registerColor=#ffffcc todo
        //view.gutter.structureHighlightColor=#000066 todo

        //view.lineHighlightColor=#e5f9ff
        properties.put(EditorTheme.Attr.VIEW_SELECTION_COLOR.getKey(), colors.getString("editor.lineHighlightBackground"));
        //view.selectionColor=#ccccff
        properties.put(EditorTheme.Attr.VIEW_SELECTION_COLOR.getKey(), colors.getString("editor.selectionBackground"));

        //view.status.background=#ffffff todo
        //view.status.foreground=#000000 todo
        //view.status.memory.background=#666699a todo
        //view.status.memory.foreground=#cccccc todo
        //view.structureHighlightColor=#000000 todo

        //view.style.comment1=color:#009933 style:i
        properties.put(SyntaxStyle.Attr.view_style_comment1.getKey(), parseColor(tokenColors, "scope", "comment", "settings"));
        //view.style.comment2=color:#0099cc style:i
        properties.put(SyntaxStyle.Attr.view_style_comment2.getKey(), parseColor(tokenColors, "scope", "comment", "settings"));
        //view.style.comment3=color:#6600cc
        properties.put(SyntaxStyle.Attr.view_style_comment3.getKey(), parseColor(tokenColors, "scope", "comment", "settings"));
        //view.style.comment4=color:#cc6600
        properties.put(SyntaxStyle.Attr.view_style_comment4.getKey(), parseColor(tokenColors, "scope", "comment", "settings"));

        //view.style.digit=color:#993300
        properties.put(SyntaxStyle.Attr.view_style_digit.getKey(), parseColor(tokenColors, "scope", "constant.numeric", "settings"));

        //view.style.foldLine.0=color:#000000 bgColor:#f5deb8 style:b todo
        //view.style.foldLine.1=color:#000000 bgColor:#d5d5f7 style:b todo
        //view.style.foldLine.2=color:#000000 bgColor:#ffffcc style:b todo
        //view.style.foldLine.3=color:#000000 bgColor:#b0ecc4 style:b todo

        //view.style.function=color:#996600
        properties.put(SyntaxStyle.Attr.view_style_function.getKey(),
                parseColor(tokenColors, "scope", "entity.name.function", "settings"));
        //view.style.invalid=color:#ff6600 bgColor:#ffffcc style:b
        properties.put(SyntaxStyle.Attr.view_style_invalid.getKey(),
                parseColor(tokenColors, "scope", "invalid", "settings"));

        //view.style.keyword1=color:#cc3300 style:b
        properties.put(SyntaxStyle.Attr.view_style_keyword1.getKey(),
                parseColor(tokenColors, "scope", "keyword", "settings"));
        //view.style.keyword2=color:#006699 style:b
        properties.put(SyntaxStyle.Attr.view_style_keyword2.getKey(),
                parseColor(tokenColors, "scope", "keyword", "settings"));
        //view.style.keyword3=color:#660066 style:b todo
        properties.put(SyntaxStyle.Attr.view_style_keyword3.getKey(),
                parseColor(tokenColors, "scope", "keyword", "settings"));
        //view.style.keyword4=color:#66ccff style:b todo
        properties.put(SyntaxStyle.Attr.view_style_keyword4.getKey(),
                parseColor(tokenColors, "scope", "keyword.other", "settings"));


        //view.style.label=color:#990033 style:ib
        properties.put(SyntaxStyle.Attr.view_style_label.getKey(),
                parseColor(tokenColors, "scope", "entity.name.class", "settings"));
        //view.style.literal1=color:#990099
        properties.put(SyntaxStyle.Attr.view_style_literal1.getKey(),
                parseColor(tokenColors, "scope", "string", "settings"));
        //view.style.literal2=color:#522c2c
        properties.put(SyntaxStyle.Attr.view_style_literal2.getKey(),
                parseColor(tokenColors, "scope", "constant.language", "settings"));
        //view.style.literal3=color:#9900cc
        properties.put(SyntaxStyle.Attr.view_style_literal3.getKey(),
                parseColor(tokenColors, "scope", "constant.character", "settings"));
        //view.style.literal4=color:#6600cc
        properties.put(SyntaxStyle.Attr.view_style_literal4.getKey(),
                parseColor(tokenColors, "scope", "constant.character", "settings"));
        //view.style.markup=color:#0099cc
        properties.put(SyntaxStyle.Attr.view_style_markup.getKey(), colors.getString("editorCursor.foreground"));
        //view.style.operator=color:#003399 style:b
        properties.put(SyntaxStyle.Attr.view_style_operator.getKey(),
                parseColor(tokenColors, "scope", "variable.parameter", "settings"));
        //view.wrapGuideColor=#8080ff
        properties.put(EditorTheme.Attr.VIEW_WRAP_GUIDE_COLOR.getKey(), colors.getString("editorIndentGuide.background"));
        //white-space.block-color=#000000
        properties.put(WhiteSpaceStyle.Attr.BLOCK_COLOR.getKey(), colors.get("editorWhitespace.foreground"));
        //white-space.fold-color=#cccccc
        properties.put(WhiteSpaceStyle.Attr.FOLD_COLOR.getKey(), colors.get("editorWhitespace.foreground"));
        //white-space.space-color=#bcbcbc
        properties.put(WhiteSpaceStyle.Attr.SPACE_COLOR.getKey(), colors.get("editorWhitespace.foreground"));
        //white-space.tab-color=#bcbcbc
        properties.put(WhiteSpaceStyle.Attr.TAB_COLOR.getKey(), colors.get("editorWhitespace.foreground"));
        //white-space.whitespace-color=#ff6600
        properties.put(WhiteSpaceStyle.Attr.WHITESPACE_COLOR.getKey(), colors.get("editorWhitespace.foreground"));
        return propertiesToString(properties);
    }

    private String parseColor(JSONArray array, String key, String value, String colorSetKey) throws JSONException {
        for (int i = 0; i < array.length(); i++) {
            JSONObject jsonObject = array.getJSONObject(i);
            if (jsonObject.has(key)) {

                boolean found = false;
                if (jsonObject.get(key) instanceof JSONArray) {
                    JSONArray jsonArray = jsonObject.getJSONArray(key);
                    for (int j = 0; j < jsonArray.length(); j++) {
                        Object o = jsonArray.get(j);
                        if (o instanceof String) {
                            if (((String) o).contentEquals(value)) {
                                found = true;
                                break;
                            }
                        }
                    }
                    if (!found) {
                        continue;
                    }
                } else if (jsonObject.getString(key).contentEquals(value)) {
                    found = true;
                }
                if (found) {
                    jsonObject = jsonObject.getJSONObject(colorSetKey);
                    String fgColor = "";
                    String bgColor = "";
                    String style = "";
                    if (jsonObject.has("foreground")) {
                        fgColor = jsonObject.getString("foreground");
                    }
                    if (jsonObject.has("background")) {
                        bgColor = jsonObject.getString("background");
                    }
                    if (jsonObject.has("fontStyle")) {
                        style = jsonObject.getString("fontStyle");
                    }
                    String result = "color:" + fgColor;
                    if (!bgColor.isEmpty()) {
                        result += " bgColor:" + bgColor;
                    }
                    if (!style.isEmpty()) {
                        result += " style:" + style;
                    }
                    return result;
                }
            }
        }
        throw new RuntimeException("Can not find key = " + key);
    }

    private String propertiesToString(Properties properties) {
        Set<String> strings = properties.stringPropertyNames();
        ArrayList<String> keys = new ArrayList<>(strings);
        Collections.sort(keys, new Comparator<String>() {
            @Override
            public int compare(String o1, String o2) {
                return o1.compareTo(o2);
            }
        });

        StringBuilder builder = new StringBuilder();
        for (String key : keys) {
            builder.append(key).append("=").append(properties.get(key));
            builder.append("\n");
        }
        return builder.toString();
    }


}
