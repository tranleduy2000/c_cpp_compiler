package com.duy.ide.code.model;

import android.support.annotation.Nullable;

import com.duy.ide.code.api.CodeFormatter;

import org.json.JSONObject;

public class JsonFormatter implements CodeFormatter {
    @Nullable
    @Override
    public CharSequence format(CharSequence input) {
        try {
            JSONObject jsonObject = new JSONObject(input.toString());
            return jsonObject.toString(1);
        } catch (Exception e) {
        }
        return null;
    }
}
