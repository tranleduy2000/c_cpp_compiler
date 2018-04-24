/*
 * Copyright (C) 2016 Jecelyin Peng <jecelyin@gmail.com>
 *
 * This file is part of 920 Text Editor.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.gjt.sp.jedit;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.Reader;
import java.util.LinkedList;
import java.util.List;
import java.util.Properties;

class PropertyManager {

    private final Properties system = new Properties();
    private final List<Properties> plugins = new LinkedList<Properties>();
    private final Properties site = new Properties();
    private final Properties localization = new Properties();
    private final List<Properties> pluginLocalizations = new LinkedList<Properties>();
    private final Properties user = new Properties();


    private static void loadProps(Properties into, InputStream in)
            throws IOException {
        try {
            into.load(in);
        } finally {
            in.close();
        }
    }


    private static void loadProps(Properties into, Reader in)
            throws IOException {
        try {
            into.load(in);
        } finally {
            in.close();
        }
    }


    Properties getProperties() {
        Properties total = new Properties();
        total.putAll(system);
        for (Properties plugin : plugins)
            total.putAll(plugin);
        total.putAll(site);
        total.putAll(localization);
        for (Properties pluginLocalization : pluginLocalizations)
            total.putAll(pluginLocalization);
        total.putAll(user);
        return total;
    }


    void loadSystemProps(Reader in)
            throws IOException {
        loadProps(system, in);
    }


    void loadSiteProps(InputStream in)
            throws IOException {
        loadProps(site, in);
    }


    void loadLocalizationProps(Reader in)
            throws IOException {
        if (in == null)
            localization.clear();
        else
            loadProps(localization, in);
    }


    void loadUserProps(InputStream in)
            throws IOException {
        loadProps(user, in);
    }


    void saveUserProps(OutputStream out)
            throws IOException {
        user.store(out, "jEdit properties");
    }


    Properties loadPluginProps(InputStream in)
            throws IOException {
        Properties plugin = new Properties();
        loadProps(plugin, in);
        plugins.add(plugin);
        return plugin;
    }


    void addPluginProps(Properties props) {
        plugins.add(props);
    }


    void removePluginProps(Properties props) {
        plugins.remove(props);
    }


    Properties loadPluginLocalizationProps(Reader in)
            throws IOException {
        Properties pluginLocalization = new Properties();
        loadProps(pluginLocalization, in);
        pluginLocalizations.add(pluginLocalization);
        return pluginLocalization;
    }


    void addPluginLocalizationProps(Properties props) {
        pluginLocalizations.add(props);
    }


    void removePluginLocalizationProps(Properties props) {
        pluginLocalizations.remove(props);
    }


    String getProperty(String name) {
        String value = user.getProperty(name);
        if (value != null)
            return value;

        for (Properties pluginLocalization : pluginLocalizations) {
            value = pluginLocalization.getProperty(name);
            if (value != null)
                return value;
        }

        value = localization.getProperty(name);
        if (value != null)
            return value;

        return getDefaultProperty(name);
    }


    void setProperty(String name, String value) {
        String prop = getDefaultProperty(name);

		/* if value is null:
         * - if default is null, unset user prop
		 * - else set user prop to ""
		 * else
		 * - if default equals value, ignore
		 * - if default doesn't equal value, set user
		 */
        if (value == null) {
            if (prop == null || prop.length() == 0)
                user.remove(name);
            else
                user.setProperty(name, "");
        } else {
            if (value.equals(prop))
                user.remove(name);
            else
                user.setProperty(name, value);
        }
    }


    public void setTemporaryProperty(String name, String value) {
        user.remove(name);
        system.setProperty(name, value);
    }


    void unsetProperty(String name) {
        if (getDefaultProperty(name) != null)
            user.setProperty(name, "");
        else
            user.remove(name);
    }


    public void resetProperty(String name) {
        user.remove(name);
    }


    private String getDefaultProperty(String name) {
        String value = site.getProperty(name);
        if (value != null)
            return value;

        for (Properties plugin : plugins) {
            value = plugin.getProperty(name);
            if (value != null)
                return value;
        }

        return system.getProperty(name);
    }


}
