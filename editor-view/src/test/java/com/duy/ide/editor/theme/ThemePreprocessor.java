package com.duy.ide.editor.theme;

import junit.framework.TestCase;

import org.apache.commons.io.IOUtils;
import org.xml.sax.SAXException;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;

import javax.xml.parsers.ParserConfigurationException;

public class ThemePreprocessor extends TestCase {
    public void test() throws IOException, ParserConfigurationException, SAXException {
        File dir = new File(System.getenv("user.dir"), "colorschemes/jedit");
        File[] files = dir.listFiles();
        for (File file : files) {
            System.out.println("file = " + file);
            //rename it
//            String nameWithExt = file.getName();
//            String name = nameWithExt.substring(0, nameWithExt.lastIndexOf("."));
//            file.renameTo(new File(file.getParent(), name + ".properties"));

            FileInputStream input = new FileInputStream(file);
            String content = IOUtils.toString(input);
            input.close();
            content = content.replace(" style\\:", "");
            content = content.replace("color\\:\\", "");
            content = content.replace("\\#", "#");
            FileOutputStream output = new FileOutputStream(file);
            IOUtils.write(content, output);
            output.close();
        }
    }

    private void convert(File file) throws IOException, SAXException, ParserConfigurationException {

    }
}
