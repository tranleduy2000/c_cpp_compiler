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

package com.duy.ccppcompiler.compiler.diagnostic;

import android.net.Uri;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.Reader;
import java.io.Writer;

/**
 * File abstraction for tools.  In this context, <em>file</em> means
 * an abstraction of regular files and other sources of data.  For
 * example, a file object can be used to represent regular files,
 * memory cache, or data in databases.
 * <p>
 * <p>All methods in this interface might throw a SecurityException if
 * a security exception occurs.
 * <p>
 * <p>Unless explicitly allowed, all methods in this interface might
 * throw a NullPointerException if given a {@code null} argument.
 */
public interface FileObject {

    /**
     * Returns a URI identifying this file object.
     *
     * @return a URI
     */
    Uri toUri();

    /**
     * Gets a user-friendly name for this file object.  The exact
     * value returned is not specified but implementations should take
     * care to preserve names as given by the user.  For example, if
     * the user writes the filename {@code "BobsApp\Test.java"} on
     * the command line, this method should return {@code
     * "BobsApp\Test.java"} whereas the {@linkplain #toUri toUri}
     * method might return {@code
     * file:///C:/Documents%20and%20Settings/UncleBob/BobsApp/Test.java}.
     *
     * @return a user-friendly name
     */
    String getName();

    /**
     * Gets an InputStream for this file object.
     *
     * @return an InputStream
     * @throws IllegalStateException         if this file object was
     *                                       opened for writing and does not support reading
     * @throws UnsupportedOperationException if this kind of file
     *                                       object does not support byte access
     * @throws IOException                   if an I/O error occurred
     */
    InputStream openInputStream() throws IOException;

    /**
     * Gets an OutputStream for this file object.
     *
     * @return an OutputStream
     * @throws IllegalStateException         if this file object was
     *                                       opened for reading and does not support writing
     * @throws UnsupportedOperationException if this kind of
     *                                       file object does not support byte access
     * @throws IOException                   if an I/O error occurred
     */
    OutputStream openOutputStream() throws IOException;

    /**
     * Gets a reader for this object.  The returned reader will
     * replace bytes that cannot be decoded with the default
     * translation character.  In addition, the reader may report a
     * diagnostic unless {@code ignoreEncodingErrors} is true.
     *
     * @param ignoreEncodingErrors ignore encoding errors if true
     * @return a Reader
     * @throws IllegalStateException         if this file object was
     *                                       opened for writing and does not support reading
     * @throws UnsupportedOperationException if this kind of
     *                                       file object does not support character access
     * @throws IOException                   if an I/O error occurred
     */
    Reader openReader(boolean ignoreEncodingErrors) throws IOException;

    /**
     * Gets the character content of this file object, if available.
     * Any byte that cannot be decoded will be replaced by the default
     * translation character.  In addition, a diagnostic may be
     * reported unless {@code ignoreEncodingErrors} is true.
     *
     * @param ignoreEncodingErrors ignore encoding errors if true
     * @return a CharSequence if available; {@code null} otherwise
     * @throws IllegalStateException         if this file object was
     *                                       opened for writing and does not support reading
     * @throws UnsupportedOperationException if this kind of
     *                                       file object does not support character access
     * @throws IOException                   if an I/O error occurred
     */
    CharSequence getCharContent(boolean ignoreEncodingErrors) throws IOException;

    /**
     * Gets a Writer for this file object.
     *
     * @return a Writer
     * @throws IllegalStateException         if this file object was
     *                                       opened for reading and does not support writing
     * @throws UnsupportedOperationException if this kind of
     *                                       file object does not support character access
     * @throws IOException                   if an I/O error occurred
     */
    Writer openWriter() throws IOException;

    /**
     * Gets the time this file object was last modified.  The time is
     * measured in milliseconds since the epoch (00:00:00 GMT, January
     * 1, 1970).
     *
     * @return the time this file object was last modified; or 0 if
     * the file object does not exist, if an I/O error occurred, or if
     * the operation is not supported
     */
    long getLastModified();

    /**
     * Deletes this file object.  In case of errors, returns false.
     *
     * @return true if and only if this file object is successfully
     * deleted; false otherwise
     */
    boolean delete();

}
