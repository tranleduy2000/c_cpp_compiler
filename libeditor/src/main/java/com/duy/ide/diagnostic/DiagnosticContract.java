/*
 *  Copyright (C)  2018  Duy Tran Le
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.duy.ide.diagnostic;

import android.support.annotation.MainThread;

import com.duy.ide.diagnostic.model.Message;
import com.duy.ide.diagnostic.parser.PatternAwareOutputParser;
import com.duy.ide.diagnostic.suggestion.ISuggestion;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by Duy on 28-Apr-18.
 */

public class DiagnosticContract {
    public interface View {
        void showDiagnostic(List<Message> messages);

        void showLog(CharSequence log);

        void remove(Message message);

        void add(Message message);

        void clear();

        void setPresenter(Presenter presenter);

    }

    public interface Presenter {
        @MainThread
        void onDiagnosticClick(android.view.View view, Message message);

        @MainThread
        void onSuggestionClick(Message message, ISuggestion suggestion);

        @MainThread
        void showPanel();

        @MainThread
        void hidePanel();

        @MainThread
        void setDiagnostics(ArrayList<Message> messages);

        @MainThread
        void log(String string);

        @MainThread
        void clear();

        void setOutputParser(PatternAwareOutputParser... parsers);
    }
}
