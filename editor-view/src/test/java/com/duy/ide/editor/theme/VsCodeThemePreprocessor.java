package com.duy.ide.editor.theme;

import com.duy.ide.editor.theme.model.EditorTheme;

import junit.framework.TestCase;

import org.apache.commons.io.IOUtils;
import org.json.JSONException;
import org.json.JSONObject;
import org.xml.sax.SAXException;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Properties;
import java.util.Set;

import javax.xml.parsers.ParserConfigurationException;

/**
 * Using JUnit on Android studio faster than create class with main method
 */
public class VsCodeThemePreprocessor extends TestCase {

    public void test() throws IOException, SAXException, JSONException {
        File dir = new File(System.getenv("user.dir"), "colorschemes/vscode");
        File[] files = dir.listFiles();
        for (File file : files) {
            if (file.isDirectory()) {
                continue;
            }
            System.out.println("file = " + file);

            FileInputStream input = new FileInputStream(file);
            String content = IOUtils.toString(input, "UTF-8");
            input.close();
            content = convert(content);

            File outFile = new File(file.getParent(), "gen/" + file.getName());
            outFile.getParentFile().mkdir();
            outFile.createNewFile();
            FileOutputStream output = new FileOutputStream(outFile);
            IOUtils.write(content, output);
            output.close();
        }
    }


    private String convert(String content) throws IOException, JSONException {
        System.out.println("ThemePreprocessor.clean");
        System.out.println("content = " + content);
        JSONObject theme = new JSONObject(content);

        Properties properties = new Properties();
        properties.put(EditorTheme.Attr.SCHEME_NAME.getKey(), theme.getString("name"));
        return propertiesToString(properties);
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


    private void convert(File file) throws IOException, SAXException, ParserConfigurationException {

    }
}
