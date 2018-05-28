package com.duy.astyle;

/**
 * AStyleInterface jni
 *
 * @link http://astyle.sourceforge.net/astyle.html#_Options
 * @link http://astyle.sourceforge.net/develop/jni.html
 */
public class AStyleInterface {

    private static final String TAG = "AStyleInterface";

    static {
        System.loadLibrary("astyle");
    }

    /**
     * Error message function for this example.
     */
    private static void error(String message) {
        System.out.println(message);
        System.out.println("The program has terminated!");
        System.exit(1);
    }

    /**
     * Calls the Java AStyleMain function in Artistic Style.
     * The function name is constructed from method names in this program.
     *
     * @param textIn  A string containing the source code to be formatted.
     * @param options A string of options to Artistic Style.
     * @return A String containing the formatted source from Artistic Style.
     */
    private native String AStyleMain(String textIn, String options);

    /**
     * Calls the Java AStyleGetVersion function in Artistic Style.
     * The function name is constructed from method names in this program.
     *
     * @return A String containing the version number of Artistic Style.
     */
    private native String AStyleGetVersion();

    /**
     * Error handler for messages from Artistic Style.
     * This method is called only if there are errors when AStyleMain is called.
     * This is for debugging and there should be no errors when the calling
     * parameters are correct.
     * Changing the method name requires changing Artistic Style.
     * Signature: (ILjava/lang/String;)V.
     *
     * @param errorNumber  The error number from Artistic Style.
     * @param errorMessage The error message from Artistic Style.
     */
    @SuppressWarnings("unused")
    private void ErrorHandler(int errorNumber, String errorMessage) {
        System.out.println("AStyle error "
                + String.valueOf(errorNumber)
                + " - " + errorMessage);
    }

    /**
     * Call the AStyleMain function in Artistic Style.
     *
     * @param textIn  A string containing the source code to be formatted.
     * @param options A string of options to Artistic Style.
     * @return A String containing the formatted source from Artistic Style,
     * or an empty string on error.
     */
    public String formatSource(String textIn, String options) {   // Return the allocated string
        // Memory space is allocated by OnAStyleMemAlloc, a callback function from AStyle
        String textOut = "";
        try {
            textOut = AStyleMain(textIn, options);
        } catch (UnsatisfiedLinkError e) {   //~ System.out.println(e.getMessage());
            error("Cannot call the Java AStyleMain function");
        }
        return textOut;
    }

    /**
     * Call the AStyleGetVersion function in Artistic Style.
     *
     * @return A String containing the version number from Artistic Style.
     */
    public String getVersion() {
        String version = new String();
        try {
            version = AStyleGetVersion();
        } catch (UnsatisfiedLinkError e) {   //~ System.out.println(e.getMessage());
            error("Cannot call the Java AStyleGetVersion function");
        }
        return version;
    }

}
