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


import android.content.Context;
import android.support.annotation.Nullable;

import com.duy.ide.file.model.AssetFile;
import com.duy.ide.file.model.IFileObject;
import com.jecelyin.common.utils.DLog;

import org.gjt.sp.jedit.Catalog;
import org.gjt.sp.jedit.Mode;
import org.gjt.sp.jedit.util.IOUtilities;
import org.xml.sax.InputSource;
import org.xml.sax.SAXException;
import org.xml.sax.XMLReader;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Collections;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import javax.xml.parsers.ParserConfigurationException;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;


/**
 * This class works like a singleton, the instance is initialized by jEdit.
 *
 * @author Matthieu Casanova
 * @version $Id: Buffer.java 8190 2006-12-07 07:58:34Z kpouer $
 * @since jEdit 4.3pre10
 */
public class ModeProvider {
    private static final String TAG = "ModeProvider";
    public static ModeProvider instance = new ModeProvider();
    private final LinkedHashMap<String, Mode> modes;

    public ModeProvider() {
        modes = Catalog.modes;
    }

    public static ModeProvider getInstance() {
        return instance;
    }

    public void removeAll() {
        modes.clear();
    }

    /**
     * Returns the edit mode with the specified name.
     *
     * @param name The edit mode
     * @since jEdit 4.3pre10
     */
    @Nullable
    public Mode getMode(String name) {
        Mode mode = modes.get(name);
        if (mode != null) {
            return mode;
        }
        for (Map.Entry<String, Mode> entry : modes.entrySet()) {
            if (entry.getKey().equalsIgnoreCase(name)) {
                return entry.getValue();
            }
        }
        return null;
    }

    /**
     * Get the appropriate mode that must be used for the file
     *
     * @param filename  the filename
     * @param firstLine the first line of the file
     * @return the edit mode, or null if no mode match the file
     * @since jEdit 4.3pre12
     */
    public Mode getModeForFile(String filename, String firstLine) {
        return getModeForFile(null, filename, firstLine);
    }

    /**
     * Get the appropriate mode that must be used for the file
     *
     * @param filepath  the filepath, can be {@code null}
     * @param filename  the filename, can be {@code null}
     * @param firstLine the first line of the file
     * @return the edit mode, or null if no mode match the file
     * @since jEdit 4.5pre1
     */
    public Mode getModeForFile(@Nullable String filepath, @Nullable String filename, String firstLine) {
        if (filepath != null && filepath.endsWith(".gz"))
            filepath = filepath.substring(0, filepath.length() - 3);
        if (filename != null && filename.endsWith(".gz"))
            filename = filename.substring(0, filename.length() - 3);

        List<Mode> acceptable = new ArrayList<Mode>(1);
        for (Mode mode : modes.values()) {
            if (mode.accept(filepath, filename, firstLine)) {
                acceptable.add(mode);
            }
        }
        if (acceptable.size() == 1) {
            return acceptable.get(0);
        }
        if (acceptable.size() > 1) {
            // The check should be in reverse order so that
            // modes from the user catalog get checked first!
            Collections.reverse(acceptable);

            // the very most acceptable mode is one whose file
            // name doesn't only match the file name as regular
            // expression but which is identical
            for (Mode mode : acceptable) {
                if (mode.acceptIdentical(filepath, filename)) {
                    return mode;
                }
            }

            // most acceptable is a mode that matches both the
            // filepath and the first line glob
            for (Mode mode : acceptable) {
                if (mode.acceptFile(filepath, filename) &&
                        mode.acceptFirstLine(firstLine)) {
                    return mode;
                }
            }
            // next best is filepath match
            for (Mode mode : acceptable) {
                if (mode.acceptFile(filepath, filename)) {
                    return mode;
                }
            }
            // all acceptable choices are by first line glob, and
            // they all match, so just return the first one.
            return acceptable.get(0);
        }
        // no matching mode found for this file
        return null;
    }

    /**
     * Returns an array of installed edit modes.
     *
     * @since jEdit 4.3pre10
     */
    public Mode[] getModes() {
        return modes.values().toArray(new Mode[modes.size()]);
    }

    /**
     * Do not call this method. It is only public so that classes
     * in the org.gjt.sp.jedit.syntax package can access it.
     *
     * @param mode The edit mode
     *             see org.gjt.sp.jedit.jEdit#reloadModes reloadModes
     * @since jEdit 4.3pre10
     */
    public void addMode(Mode mode) {
        String name = mode.getName();

        // The removal makes the "insertion order" in modes
        // (LinkedHashMap) follow the order of addMode() calls.
        modes.remove(name);

        modes.put(name, mode);
    }

    public void loadMode(Mode mode, XModeHandler handler, IFileObject file) {
        XMLReader parser = null;
        try {
            SAXParserFactory saxParserFactory = SAXParserFactory.newInstance();
            SAXParser newSAXParser = saxParserFactory.newSAXParser();
            parser = newSAXParser.getXMLReader();
        } catch (SAXException saxe) {
            if (DLog.DEBUG) DLog.e(TAG, "loadMode: ", saxe);
            return;
        } catch (ParserConfigurationException e) {
            e.printStackTrace();
        }
        mode.setTokenMarker(handler.getTokenMarker());

        InputStream grammar = null;
        try {
            grammar = new BufferedInputStream(file.openInputStream());
        } catch (IOException ignored) {
        }

        try {
            InputSource isrc = new InputSource(
                    new BufferedInputStream(file.openInputStream()));
            isrc.setSystemId("jedit.jar");
            parser.setContentHandler(handler);
            parser.setDTDHandler(handler);
            parser.setEntityResolver(handler);
            parser.setErrorHandler(handler);
            parser.parse(isrc);
        } catch (Throwable e) {
            if (DLog.DEBUG) DLog.e(TAG, "loadMode: ", e);
        } finally {
            IOUtilities.closeQuietly(grammar);
        }
    }

    public void loadMode(Mode mode, Context context) {
        XModeHandler handler = new XModeHandler(mode.getName(), context) {
            @Override
            public void error(String what, Object subst) {
                if (DLog.DEBUG) DLog.e(TAG, "error: ", subst);
            }

            @Override
            public TokenMarker getTokenMarker(String modeName) {
                Mode mode = getMode(modeName);
                if (mode == null)
                    return null;
                else
                    return mode.getTokenMarker(getContext());
            }
        };

        AssetFile file = new AssetFile(context.getAssets(), "syntax/" + mode.getFile());
        loadMode(mode, handler, file);
    }

}
