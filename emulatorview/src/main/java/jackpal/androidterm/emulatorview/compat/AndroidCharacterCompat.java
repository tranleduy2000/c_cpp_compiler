package jackpal.androidterm.emulatorview.compat;

import android.text.AndroidCharacter;

/**
 * Definitions related to android.text.AndroidCharacter
 */
public class AndroidCharacterCompat {
    public static final int EAST_ASIAN_WIDTH_FULL_WIDTH = 3;
    public static final int EAST_ASIAN_WIDTH_WIDE = 5;

    public static int getEastAsianWidth(char c) {
        return AndroidCharacter.getEastAsianWidth(c);
    }

}
