package com.duy.astyle;

import android.support.test.runner.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;

@RunWith(AndroidJUnit4.class)
public class AStyleInterfaceTest {

    @Test
    public void testCpp() {
        String code = "int Foo(bool isBar){if (isBar)    {    bar();    return 1;    }else    return 0;}";
        String format = AStyleInterface.format(code, "--style=allman");
        System.out.println("format = " + format);
    }
}
