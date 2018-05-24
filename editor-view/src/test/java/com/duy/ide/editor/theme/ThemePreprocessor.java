package com.duy.ide.editor.theme;

import junit.framework.TestCase;

import java.io.File;
import java.util.Arrays;

public class ThemePreprocessor extends TestCase {
    public void test() {
        File file = new File(System.getenv("user.dir"), "editor-view/colorschemes/rainglow");
        File[] files = file.listFiles();
        System.out.println(Arrays.toString(files));
    }
}
