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
import android.support.annotation.NonNull;
import android.support.annotation.WorkerThread;

import com.duy.ide.diagnostic.model.Message;
import com.duy.ide.diagnostic.parser.PatternAwareOutputParser;
import com.duy.ide.logging.ILogger;

import java.io.OutputStream;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by Duy on 28-Apr-18.
 */
@SuppressWarnings("unused")
public class DiagnosticContract {
    public static final int DIAGNOSTIC = 0;
    public static final int COMPILER_LOG = 1;

    public interface View {
        @WorkerThread
        void showDiagnostic(List<Message> messages);

        @WorkerThread
        void printMessage(String log);

        @WorkerThread
        void printError(String log);

        @WorkerThread
        void removeMessage(Message message);

        @WorkerThread
        void addMessage(Message message);

        @WorkerThread
        void addMessage(List<Message> messages);

        @WorkerThread
        void clearAll();

        void setPresenter(Presenter presenter);

        @MainThread
        void setCurrentItem(int index);
    }

    public interface Presenter extends ILogger {
        @MainThread
        void onDiagnosticClick(android.view.View view, Message message);

        @MainThread
        void showPanel();

        @MainThread
        void hidePanel();

        @MainThread
        void setMessages(ArrayList<Message> messages);

        @WorkerThread
        void onNewMessage(String string);

        @MainThread
        void clear();

        @MainThread
        void setCurrentItem(int index);

        void setOutputParser(@NonNull PatternAwareOutputParser... parsers);

        @NonNull
        OutputStream getStandardOutput();

        @NonNull
        OutputStream getErrorOutput();
    }
}
