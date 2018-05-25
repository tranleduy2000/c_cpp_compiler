package com.duy.ide.editor.theme;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.Iterator;
import java.util.function.Consumer;

public class JsonUtils {
    public static JSONObject sortByKey(JSONObject jsonObject, Comparator<String> c) throws JSONException {
        JSONObject sortedJson = new JSONObject();
        Iterator<String> keys = jsonObject.keys();

        final ArrayList<String> keysList = new ArrayList<>();
        keys.forEachRemaining(new Consumer<String>() {
            @Override
            public void accept(String s) {
                keysList.add(s);
            }
        });
        Collections.sort(keysList, c);

        for (String key : keysList) {
            if (jsonObject.get(key) instanceof JSONObject) {
                sortedJson.put(key, sortByKey(jsonObject.getJSONObject(key), c));
            } else {
                sortedJson.put(key, jsonObject.get(key));
            }
        }
        return sortedJson;
    }
}
