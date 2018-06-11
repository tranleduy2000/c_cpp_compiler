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

import android.content.Context;
import android.database.Cursor;
import android.database.DatabaseErrorHandler;
import android.database.sqlite.SQLiteDatabase;
import android.database.sqlite.SQLiteOpenHelper;
import android.text.TextUtils;

import com.google.gson.JsonObject;

import java.util.ArrayList;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class SQLHelper extends SQLiteOpenHelper implements ITabDatabase {
    private static final String DATABASE_NAME = "920-text-editor.db";
    private static final int DATABASE_VERSION = 4; // Version must be >= 1

    public SQLHelper(Context context) {
        this(context, DATABASE_NAME, null, DATABASE_VERSION);
    }

    public SQLHelper(Context context, String name, SQLiteDatabase.CursorFactory factory, int version) {
        this(context, name, factory, version, new DatabaseErrorHandler() {
            @Override
            public void onCorruption(SQLiteDatabase dbObj) {

            }
        });
    }

    public SQLHelper(Context context, String name, SQLiteDatabase.CursorFactory factory, int version, DatabaseErrorHandler errorHandler) {
        super(context, name, factory, version, errorHandler);
    }

    public static ITabDatabase getInstance(Context context) {
//        return new SQLHelper(context.getApplicationContext());
        return new JsonDatabase(context.getApplicationContext());
    }

    @Override
    public void onCreate(SQLiteDatabase db) {
        db.execSQL("CREATE TABLE \"recent_files\" (\n" +
                "\t \"path\" TEXT NOT NULL,\n" +
                "\t \"open_time\" integer,\n" +
                "\t \"encoding\" TEXT,\n" +
                "\t \"offset\" integer,\n" +
                "\t \"last_open\" integer,\n" +
                "\tPRIMARY KEY(\"path\")\n" +
                ")");
        db.execSQL("CREATE INDEX \"open_time\" ON recent_files (\"open_time\" DESC)");

        createFindKeywordsTable(db);
    }

    public void createFindKeywordsTable(SQLiteDatabase db) {
        db.execSQL("CREATE TABLE \"find_keywords\" (\n" +
                "\t \"keyword\" TEXT NOT NULL,\n" +
                "\t \"is_replace\" integer,\n" +
                "\t \"ctime\" integer,\n" +
                "\tPRIMARY KEY(\"keyword\", \"is_replace\")\n" +
                ")");
        db.execSQL("CREATE INDEX \"ctime\" ON find_keywords (\"ctime\" DESC)");
    }

    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        for (int version = oldVersion; version < newVersion; version++) {
            switch (version) {
                case 1:
                    upgradeTo2(db);
                    break;
                case 2:
                    upgradeTo3(db);
                    break;
                case 3:
                    upgradeTo4(db);
                    break;
            }
        }
    }

    private void upgradeTo4(SQLiteDatabase db) {
        createFindKeywordsTable(db);
    }

    private void upgradeTo3(SQLiteDatabase db) {
        db.execSQL("alter table recent_files ADD COLUMN offset integer");
        db.execSQL("alter table recent_files ADD COLUMN last_open integer");
    }

    private void upgradeTo2(SQLiteDatabase db) {
        db.execSQL("alter table recent_files ADD COLUMN encoding TEXT");
    }

    @Override
    public void addRecentFile(String path, String encoding) {
        if (TextUtils.isEmpty(path))
            return;
        SQLiteDatabase db = getWritableDatabase();
        db.execSQL("REPLACE INTO recent_files VALUES (?, ?, ?, ?, ?)", new Object[]{path, System.currentTimeMillis(), encoding, 0, 1});
        db.close();
    }

    @Override
    public void updateRecentFile(String path, boolean lastOpen) {
        SQLiteDatabase db = getWritableDatabase();
        db.execSQL("UPDATE recent_files SET last_open = ? WHERE path = ?", new Object[]{lastOpen ? 1 : 0, path});
        db.close();
    }

    @Override
    public void updateRecentFile(String path, String encoding, int offset) {
        SQLiteDatabase db = getWritableDatabase();
        if (offset >= 0)
            db.execSQL("UPDATE recent_files SET encoding = ?, offset = ? WHERE path = ?", new Object[]{encoding, offset, path});
        else
            db.execSQL("UPDATE recent_files SET encoding = ? WHERE path = ?", new Object[]{encoding, path});
        db.close();
    }

    @Override
    public ArrayList<RecentFileItem> getRecentFiles() {
        return getRecentFiles(false);
    }

    @Override
    public ArrayList<RecentFileItem> getRecentFiles(boolean lastOpenFiles) {
        ArrayList<RecentFileItem> list = new ArrayList<RecentFileItem>(30);
        SQLiteDatabase db = getReadableDatabase();
        Cursor cursor = db.query("recent_files", null, null, null, null, null, lastOpenFiles ? "open_time asc" : "open_time desc", "30");
        RecentFileItem item;
        boolean isLastOpen;
        while (cursor.moveToNext()) {
            isLastOpen = cursor.getInt(4) == 1;

            if (lastOpenFiles && !isLastOpen)
                continue;

            item = new RecentFileItem();
            item.path = cursor.getString(0);
            item.time = cursor.getLong(1);
            item.encoding = cursor.getString(2);
            item.offset = cursor.getInt(3);
            item.isLastOpen = isLastOpen;
            list.add(item);
        }
        cursor.close();
        db.close();
        return list;
    }

    @Override
    public void clearRecentFiles() {
        SQLiteDatabase db = getWritableDatabase();
        db.execSQL("delete from recent_files");
        db.close();
    }

    @Override
    public void clearFindKeywords(boolean isReplace) {
        SQLiteDatabase db = getWritableDatabase();
        db.execSQL("delete from find_keywords where is_replace=" + (isReplace ? "1" : "0"));
        db.close();
    }

    @Override
    public void addFindKeyword(String keyword, boolean isReplace) {
        if (TextUtils.isEmpty(keyword))
            return;
        SQLiteDatabase db = getWritableDatabase();
        db.execSQL("REPLACE INTO find_keywords VALUES (?, ?, ?)", new Object[]{keyword, isReplace ? 1 : 0, System.currentTimeMillis()});
        db.close();
    }

    @Override
    public ArrayList<String> getFindKeywords(boolean isReplace) {
        ArrayList<String> list = new ArrayList<>();
        SQLiteDatabase db = getReadableDatabase();
        Cursor cursor = db.query("find_keywords", new String[]{"keyword"}, "is_replace=?", new String[]{isReplace ? "1" : "0"}, null, null, "ctime desc", "100");

        while (cursor.moveToNext()) {
            list.add(cursor.getString(0));
        }
        cursor.close();
        db.close();
        return list;
    }

}
