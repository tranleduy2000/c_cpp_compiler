/*
 * Copyright (C) 2018 Tran Le Duy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package com.duy.ide.editor.text;

import org.mozilla.intl.chardet.nsDetector;
import org.mozilla.intl.chardet.nsICharsetDetectionObserver;
import org.mozilla.intl.chardet.nsPSMDetector;

import java.io.BufferedInputStream;
import java.util.ArrayList;
import java.util.List;

public class CharsetDetector {

    public static String detect(BufferedInputStream bufferedInputStream) throws Exception {

        nsDetector det = new nsDetector(nsPSMDetector.ALL);

        // Set an observer...
        // The Notify() will be called when a matching charset is found.
        final List<String> charsets = new ArrayList<>();
        det.Init(new nsICharsetDetectionObserver() {
            public void Notify(String charset) {
                charsets.add(charset);
            }
        });

        byte[] buf = new byte[1024];
        int len;
        boolean done = false;
        boolean isAscii = true;

        while ((len = bufferedInputStream.read(buf, 0, buf.length)) != -1) {

            // Check if the stream is only ascii.
            if (isAscii)
                isAscii = det.isAscii(buf, len);

            // DoIt if non-ascii and not done yet.
            if (!isAscii && !done)
                done = det.DoIt(buf, len, false);
        }
        det.DataEnd();

        String encoding = charsets.isEmpty() ? "UTF-8" : charsets.get(0);
        if ("GB2312".equals(encoding)) {
            encoding = "GBK";
        }
        return encoding;
    }
}
