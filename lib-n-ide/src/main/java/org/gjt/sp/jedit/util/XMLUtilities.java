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

package org.gjt.sp.jedit.util;


import android.util.Log;

import com.jecelyin.common.utils.DLog;

import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.SAXParseException;
import org.xml.sax.XMLReader;
import org.xml.sax.helpers.DefaultHandler;
import org.xml.sax.helpers.XMLReaderFactory;

import java.io.BufferedInputStream;
import java.io.Closeable;
import java.io.IOException;
import java.io.InputStream;


/**
 * XML utility methods that only depend on the JDK.
 *
 * @author Marcelo Vanzin
 * @version $Id$
 * @since 4.3pre6
 */
public class XMLUtilities {


    private XMLUtilities() {
    }


    /**
     * Converts <, >, & in the string to their HTML entity
     * equivalents.
     * <p>
     * <p>If <code>xml11</code> is true, then character entities
     * are used to convert illegal XML characters (mainly ASCII
     * control characters).</p>
     *
     * @param str   The string
     * @param xml11 Whether to allow XML 1.1 constructs.
     */
    public static String charsToEntities(String str, boolean xml11) {
        StringBuilder buf = new StringBuilder(str.length());
        for (int i = 0; i < str.length(); i++) {
            char ch = str.charAt(i);

            // control characters, excluding \t, \r and \n
            // See: http://www.w3.org/International/questions/qa-controls
            if (((0x00 <= ch && ch <= 0x1F) || (0x7F <= ch && ch <= 0x9F))
                    && ch != '\r' && ch != '\n' && ch != '\t') {
                if (xml11 && ch != 0x00) {
                    buf.append("&#").append((int) ch).append(';');
                } else {
                    // The character is illegal.
                    // But put a PI instead, to make it
                    // recoverable in certain apps.
                    buf.append("<?illegal-xml-character ")
                            .append((int) ch)
                            .append("?>");
                }
                continue;
            }

            switch (ch) {
                case '<':
                    buf.append("<");
                    break;
                case '>':
                    buf.append(">");
                    break;
                case '&':
                    buf.append("&");
                    break;
                default:
                    buf.append(ch);
                    break;
            }
        }
        return buf.toString();
    }


    /**
     * Convenience method for parsing an XML file. This method will
     * wrap the resource in an InputSource and set the source's
     * systemId to "jedit.jar" (so the source should be able to
     * handle any external entities by itself).
     * <p>
     * <p>SAX Errors are caught and are not propagated to the caller;
     * instead, an error message is printed to jEdit's activity
     * log. So, if you need custom error handling, <b>do not use
     * this method</b>.
     * <p>
     * <p>The given stream is closed before the method returns,
     * regardless whether there were errors or not.</p>
     *
     * @return true if any error occured during parsing, false if success.
     */
    public static boolean parseXML(InputStream in, DefaultHandler handler)
            throws IOException {
        try {
            XMLReader parser = XMLReaderFactory.createXMLReader();
            InputSource isrc = new InputSource(
                    new BufferedInputStream(in));
            isrc.setSystemId("jedit.jar");
            parser.setContentHandler(handler);
            parser.setDTDHandler(handler);
            parser.setEntityResolver(handler);
            parser.setErrorHandler(handler);
            parser.parse(isrc);
        } catch (SAXParseException se) {
            int line = se.getLineNumber();
            DLog.log(Log.ERROR, XMLUtilities.class,
                    "while parsing from " + in + ": SAXParseException: line " + line + ": ", se);
            return true;
        } catch (SAXException e) {
            DLog.log(Log.ERROR, XMLUtilities.class, e);
            return true;
        } finally {
            IOUtilities.closeQuietly((Closeable) in);
        }
        return false;
    }

    /**
     * Tries to find the given systemId in the context of the given
     * class. If the given systemId ends with the given test string,
     * then try to load a resource using the Class's
     * <code>getResourceAsStream()</code> method using the test string
     * as the resource.
     * <p>
     * <p>This is used a lot internally while parsing XML files used
     * by jEdit, but anyone is free to use the method if it sounds
     * usable.</p>
     */
    public static InputSource findEntity(String systemId, String test, Class<?> where) {
        if (systemId != null && systemId.endsWith(test)) {
            try {
                return new InputSource(new BufferedInputStream(
                        where.getResourceAsStream(test)));
            } catch (Exception e) {
                DLog.log(Log.ERROR, XMLUtilities.class,
                        "Error while opening " + test + ':');
                DLog.log(Log.ERROR, XMLUtilities.class, e);
            }
        }

        return null;
    }
}
