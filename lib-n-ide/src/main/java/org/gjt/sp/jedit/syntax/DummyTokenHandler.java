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
 * A dummy token handler that discards tokens.
 *
 * @author Slava Pestov
 * @version $Id: DummyTokenHandler.java 21831 2012-06-18 22:54:17Z ezust $
 * @since jEdit 4.1pre1
 */
public class DummyTokenHandler implements TokenHandler {
    /**
     * To avoid having to create new instances of this class, use
     * this variable. This is allowed because instances of this
     * class do not store any state.
     */
    public static final DummyTokenHandler INSTANCE = new DummyTokenHandler();


    /**
     * Called by the token marker when a syntax token has been parsed.
     *
     * @param seg     The segment containing the text
     * @param id      The token type (one of the constants in the
     *                {@link Token} class).
     * @param offset  The start offset of the token
     * @param length  The number of characters in the token
     * @param context The line context
     * @since jEdit 4.2pre3
     */
    public void handleToken(Segment seg, byte id, int offset, int length,
                            TokenMarker.LineContext context) {
    }


    /**
     * The token handler can compare this object with the object
     * previously given for this line to see if the token type at the end
     * of the line has changed (meaning subsequent lines might need to be
     * retokenized).
     *
     * @since jEdit 4.2pre6
     */
    public void setLineContext(TokenMarker.LineContext lineContext) {
    }
}
