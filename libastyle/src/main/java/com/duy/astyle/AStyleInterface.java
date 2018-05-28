package com.duy.astyle;

import android.util.Log;

/**
 * AStyleInterface jni
 *
 * @link http://astyle.sourceforge.net/develop/jni.html
 */
public class AStyleInterface {

    private static final String TAG = "AStyleInterface";

    static {
        System.loadLibrary("astyle");
    }

    /**
     * This function is called to get the Artistic Style version number.
     *
     * @return A Java String containing the Artistic Style version number.
     */
    public static native String AStyleGetVersion();

    /**
     * This function is called to format the source code.
     *
     * @param textIn  - A string containing the source file to be formatted
     * @param options A Java String containing the formatting options. They should be in the same
     *                format as in the default options file. The options may be set apart by
     *                new-lines, commas, tabs or spaces. The long options do not need the "--" prefix.
     *                Comments may be used but they must be terminated by a new-line "\n" character.
     *                <p>
     *                If the file is not a C/C++ file, the file mode option "mode=java" or "mode=cs"
     *                must be included.
     *                Otherwise the default mode of C/C++ is used.
     * @return If the function succeeds, the return value is a Java String containing the formatted
     * source code.
     * If the function fails, the return value is an empty Java String. The returned String should
     * be checked for a length of zero to determine if an error occurred. Before the source is
     * returned, an error message will be sent to the ErrorHandler method.
     * <p>
     * The function will NOT fail for an invalid option in the formatting options. In this case,
     * an error message is sent to the error handling function and the formatted source code is
     * returned without using the invalid option.
     */
    private native String AStyleMain(String textIn, String options);


    public String format(String code, String opts) {
        return AStyleMain(code, opts);
    }

    /**
     * ErrorHandler is called by the Artistic Style static library when an error occurs. It should
     * display an error message and then either abort or continue the program depending on the error.
     *
     * @param errorNumber  The error number as defined by Artistic Style.
     * @param errorMessage A Java String containing the error message as defined by Artistic Style.
     */
    @SuppressWarnings("unused")
    private void ErrorHandler(int errorNumber, String errorMessage) {
        Log.d(TAG, "ErrorHandler() called with: errorNumber = [" + errorNumber + "], errorMessage = [" + errorMessage + "]");
    }
}
