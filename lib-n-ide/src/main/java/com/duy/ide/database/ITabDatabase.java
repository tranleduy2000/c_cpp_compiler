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

package com.duy.ide.database;

import java.util.ArrayList;

public interface ITabDatabase {
    void addRecentFile(String path, String encoding);

    void updateRecentFile(String path, boolean lastOpen);

    void updateRecentFile(String path, String encoding, int offset);

    ArrayList<RecentFileItem> getRecentFiles();

    ArrayList<RecentFileItem> getRecentFiles(boolean lastOpenFiles);

    void clearRecentFiles();

    void clearFindKeywords(boolean isReplace);

    void addFindKeyword(String keyword, boolean isReplace);

    ArrayList<String> getFindKeywords(boolean isReplace);
}
