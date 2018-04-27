package jackpal.androidterm.emulatorview.compat;

import android.content.Context;

public class ClipboardManagerCompatFactory {

    private ClipboardManagerCompatFactory() {
        /* singleton */
    }

    public static ClipboardManagerCompat getManager(Context context) {
        if (Build.VERSION.SDK_INT < 11) {
            return new ClipboardManagerCompatV1(context);
        } else {
            return new ClipboardManagerCompatV11(context);
        }
    }
}
