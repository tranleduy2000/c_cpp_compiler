package com.duy.ide.editor.theme;

import junit.framework.TestCase;

import org.apache.commons.io.IOUtils;
import org.xml.sax.SAXException;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.StringReader;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Properties;
import java.util.Set;

import javax.xml.parsers.ParserConfigurationException;

public class ThemePreprocessor extends TestCase {
    public void test() throws IOException, ParserConfigurationException, SAXException {
        File dir = new File(System.getenv("user.dir"), "colorschemes/v2");
        File[] files = dir.listFiles();
        for (File file : files) {
            System.out.println("file = " + file);

            FileInputStream input = new FileInputStream(file);
            String content = IOUtils.toString(input, "UTF-8");
            input.close();
            content = clean(content);

            File outFile = new File(file.getParent(), "gen/" + file.getName());
            outFile.getParentFile().mkdir();
            outFile.createNewFile();
            FileOutputStream output = new FileOutputStream(outFile);
            IOUtils.write(content, output);
            output.close();
        }
    }

    private String clean(String content) throws IOException {
        System.out.println("ThemePreprocessor.clean");
        System.out.println("content = " + content);
        Properties properties = new Properties();
        properties.load(new StringReader(content));

        Set<String> strings = properties.stringPropertyNames();
        System.out.println("strings = " + strings);
        ArrayList<String> list = new ArrayList<>(strings);
        Collections.sort(list, new Comparator<String>() {
            @Override
            public int compare(String o1, String o2) {
                return o1.compareTo(o2);
            }
        });

        StringBuilder builder = new StringBuilder();
        for (String s : list) {
            builder.append(s).append("=").append(properties.get(s));
            builder.append("\n");
        }

        return builder.toString();
    }

    private void convert(File file) throws IOException, SAXException, ParserConfigurationException {

    }
}
