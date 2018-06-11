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
