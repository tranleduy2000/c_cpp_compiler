/*
 * Copyright 2018 Mr Duy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.gjt.sp.jedit.syntax;

import org.gjt.sp.jedit.Segment;

/**
 * Contains utility functions used by the syntax highlighting code.
 *
 * @author Slava Pestov
 * @version $Id: SyntaxUtilities.java 21831 2012-06-18 22:54:17Z ezust $
 * @since jEdit 4.2pre1
 */
public class SyntaxUtilities {


    /**
     * Checks if a subregion of a <code>Segment</code> is equal to a
     * character array.
     *
     * @param ignoreCase True if case should be ignored, false otherwise
     * @param text       The segment
     * @param offset     The offset into the segment
     * @param match      The character array to match
     * @return true if the subregion of the segment was
     * equals to the character array
     * @since jEdit 4.2pre1
     */
    public static boolean regionMatches(boolean ignoreCase, Segment text,
                                        int offset, char[] match) {
        int length = offset + match.length;
        if (length > text.offset + text.count)
            return false;
        char[] textArray = text.array;
        for (int i = offset, j = 0; i < length; i++, j++) {
            char c1 = textArray[i];
            char c2 = match[j];
            if (ignoreCase) {
                c1 = Character.toUpperCase(c1);
                c2 = Character.toUpperCase(c2);
            }
            if (c1 != c2)
                return false;
        }
        return true;
    }
}
