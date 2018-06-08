package com.duy.ide.database;

import android.content.Context;
import android.text.TextUtils;

import org.apache.commons.io.IOUtils;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;

public class JsonDatabase implements ITabDatabase {
    private RecentFileJsonHelper mHelper;
    private Context mContext;
    private String RECENT_FILES_DATABASE_NAME = "recent_files.json";
    private String KEYWORDS_DATABASE_NAME = "keywords.json";

    public JsonDatabase(Context context) {
        mContext = context;
        mHelper = new RecentFileJsonHelper();
    }

    @Override
    public void addRecentFile(String path, String encoding) {
        if (TextUtils.isEmpty(path))
            return;
        try {
            JSONObject database = getRecentFileDatabase();
            JSONObject jsonItem;
            RecentFileItem recentFile;
            if (database.has(path)) {
                jsonItem = database.getJSONObject(path);
                recentFile = mHelper.read(jsonItem);
                recentFile.setPath(path);
            } else {
                jsonItem = new JSONObject();
                recentFile = new RecentFileItem();
                recentFile.setPath(path);
                recentFile.setEncoding(encoding);
                database.put(path, jsonItem);
            }
            mHelper.write(jsonItem, recentFile);
            saveRecentFileDatabase(database);

        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void updateRecentFile(String path, boolean lastOpen) {
        try {
            JSONObject database = getRecentFileDatabase();
            JSONObject jsonItem;
            RecentFileItem recentFile;
            if (database.has(path)) {
                jsonItem = database.getJSONObject(path);
                recentFile = mHelper.read(jsonItem);
                recentFile.setPath(path);
                recentFile.setLastOpen(lastOpen);

                mHelper.write(jsonItem, recentFile);
                saveRecentFileDatabase(database);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void updateRecentFile(String path, String encoding, int offset) {
        try {
            JSONObject database = getRecentFileDatabase();
            JSONObject jsonItem;
            RecentFileItem recentFile;
            if (database.has(path)) {
                jsonItem = database.getJSONObject(path);
                recentFile = mHelper.read(jsonItem);
                recentFile.setPath(path);
                recentFile.setOffset(offset);
                mHelper.write(jsonItem, recentFile);
                saveRecentFileDatabase(database);
            }
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    @Override
    public ArrayList<RecentFileItem> getRecentFiles() {
        return getRecentFiles(false);
    }

    @Override
    public ArrayList<RecentFileItem> getRecentFiles(boolean lastOpenFiles) {
        ArrayList<RecentFileItem> list = new ArrayList<>(30);
        JSONObject db = getRecentFileDatabase();
        Iterator<String> keys = db.keys();
        while (keys.hasNext()) {
            String key = keys.next();
            try {
                JSONObject jsonObject = db.getJSONObject(key);
                RecentFileItem file = mHelper.read(jsonObject);
                if (file.isLastOpen() == lastOpenFiles) {
                    list.add(file);
                }
            } catch (JSONException e) {
                e.printStackTrace();
            }
        }
        return list;
    }

    @Override
    public void clearRecentFiles() {
        saveRecentFileDatabase(new JSONObject());
    }

    @Override
    public void clearFindKeywords(boolean isReplace) {
        try {
            JSONObject database = getKeywordsDatabase();
            database.put("keywords", new JSONArray());
            saveKeywordsDatabase(database);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }

    @Override
    public void addFindKeyword(String keyword, boolean isReplace) {
        try {
            JSONObject database = getKeywordsDatabase();
            JSONArray jsonArray;
            if (database.has("keywords")) {
                jsonArray = database.getJSONArray("keywords");
            } else {
                jsonArray = new JSONArray();
                database.put("keywords", jsonArray);
            }
            JSONObject value = new JSONObject();
            value.put("keyword", keyword);
            value.put("isReplace", isReplace);
            jsonArray.put(value);
            saveKeywordsDatabase(database);
        } catch (JSONException e) {
            e.printStackTrace();
        }
    }


    @Override
    public ArrayList<String> getFindKeywords(boolean isReplace) {
        ArrayList<String> list = new ArrayList<>();
        JSONObject database = getKeywordsDatabase();
        if (!database.has("keywords")) {
            try {
                JSONArray array = database.getJSONArray("keywords");
                for (int i = 0; i < array.length(); i++) {
                    JSONObject item = array.getJSONObject(i);
                    if (item.getBoolean("isReplace") == isReplace) {
                        list.add(item.getString("keyword"));
                    }
                }
            } catch (JSONException e) {
                e.printStackTrace();
            }
        }
        if (list.isEmpty()) {
            list.add("");
        }
        return list;
    }

    private JSONObject getKeywordsDatabase() {
        return readFromFile(KEYWORDS_DATABASE_NAME);
    }

    private void saveKeywordsDatabase(JSONObject database) {
        writeJsonToFile(database, KEYWORDS_DATABASE_NAME);
    }

    private void writeJsonToFile(JSONObject jsonObject, String fileName) {
        try {
            File file = new File(mContext.getFilesDir(),
                    "database" + File.separator + fileName);
            file.getParentFile().mkdirs();
            FileOutputStream output = new FileOutputStream(file);
            IOUtils.write(jsonObject.toString(), output, "UTF-8");
            output.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void saveRecentFileDatabase(JSONObject database) {
        writeJsonToFile(database, RECENT_FILES_DATABASE_NAME);
    }

    public JSONObject readFromFile(String fileName) {
        try {
            File jsonDatabase = new File(mContext.getFilesDir(),
                    "database" + File.separator + fileName);
            jsonDatabase.getParentFile().mkdirs();
            FileInputStream input = new FileInputStream(jsonDatabase);
            final String content = IOUtils.toString(input);
            input.close();
            return new JSONObject(content);
        } catch (IOException e) {
            e.printStackTrace();
        } catch (JSONException e) {
            e.printStackTrace();
        }
        return new JSONObject();
    }

    private JSONObject getRecentFileDatabase() {
        return readFromFile(RECENT_FILES_DATABASE_NAME);
    }

    private class RecentFileJsonHelper {
        public void write(JSONObject json, RecentFileItem item) throws JSONException {
            json.put("time", item.time);
            json.put("path", item.path);
            json.put("encoding", item.encoding);
            json.put("offset", item.offset);
            json.put("isLastOpen", item.isLastOpen);
        }

        public RecentFileItem read(JSONObject json) throws JSONException {
            RecentFileItem item = new RecentFileItem();
            if (json.has("time")) {
                item.time = json.getInt("time");
            }
            if (json.has("path")) {
                item.path = json.getString("path");
            }
            if (json.has("encoding")) {
                item.encoding = json.getString("encoding");
            }
            if (json.has("offset")) {
                item.offset = json.getInt("offset");
            }
            if (json.has("isLastOpen")) {
                item.isLastOpen = json.getBoolean("isLastOpen");
            }
            return item;
        }
    }

}
