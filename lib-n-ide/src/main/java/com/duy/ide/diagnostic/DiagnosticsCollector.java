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

package com.duy.ide.diagnostic;

import com.duy.ide.diagnostic.model.Message;

import java.util.ArrayList;

/**
 * Provides an easy way to collect diagnostics in a list.
 * <p>
 * Created by Duy on 28-Apr-18.
 */

public class DiagnosticsCollector implements DiagnosticListener {
    private final ArrayList<Message> messages = new ArrayList<>();

    @Override
    public void report(Message message) {
        messages.add(message);
    }

    @Override
    public void clear() {
        messages.clear();
    }

    public ArrayList<Message> getMessages() {
        return messages;
    }
}
