package com.duy.astyle;

import android.support.test.runner.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;

import static junit.framework.Assert.assertTrue;

@RunWith(AndroidJUnit4.class)
public class AStyleInterfaceTest {

    @Test
    public void testCpp() {
        // options to pass to AStyle
        // mode=java is required for Java files
        String options = "-A2tOP";

        String code = getSample();

        System.out.println("before format");
        System.out.println(code);

        AStyleInterface astyle = new AStyleInterface();
        String formatted = astyle.formatSource(code, options);

        System.out.println("formatted = \n");
        System.out.println(formatted);

        assertTrue(formatted.length() != 0);
    }

    public String getSample() {
        return "#include <iostream>\n" +
                "using namespace std;\n" +
                "\n" +
                "int main() \n" +
                "{" +
                "cout << \"Size of char: \" << sizeof(char) << \" byte\" << endl;" +
                "cout << \"Size of int: \" << sizeof(int) << \" bytes\" << endl;" +
                "cout << \"Size of float: \" << sizeof(float) << \" bytes\" << endl;" +
                "cout << \"Size of double: \" << sizeof(double) << \" bytes\" << endl;" +
                "return 0;\n" +
                "}";
    }
}
