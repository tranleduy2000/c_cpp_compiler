package com.duy.ide.database;

public class RecentFileItem {
    public long time;
    public String path;
    public String encoding;
    public int offset;
    public boolean isLastOpen;

    public long getTime() {
        return time;
    }

    public void setTime(long time) {
        this.time = time;
    }

    public String getPath() {
        return path;
    }

    public void setPath(String path) {
        this.path = path;
    }

    public String getEncoding() {
        return encoding;
    }

    public void setEncoding(String encoding) {
        this.encoding = encoding;
    }

    public int getOffset() {
        return offset;
    }

    public void setOffset(int offset) {
        this.offset = offset;
    }

    public boolean isLastOpen() {
        return isLastOpen;
    }

    public void setLastOpen(boolean lastOpen) {
        isLastOpen = lastOpen;
    }
}