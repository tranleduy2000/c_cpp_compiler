/**
 * AStyleInterface
 */
public class AStyleInterface {

    public static String format(String code, String opts) {
        return AStyleMain(code, opts);
    }

    public static native String AStyleMain(String text, String opts);
}
