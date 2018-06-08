package com.jecelyin.editor.v2.manager;

import java.io.File;

interface ITabManager {
    boolean newTab(File file);

    int getTabCount();

    int getCurrentTab();

    void setCurrentTab(int index);

    void closeAllTab();

    void closeTab(int position);
}
