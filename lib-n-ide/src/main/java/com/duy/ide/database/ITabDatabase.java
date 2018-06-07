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
