/*
 *
 *  * Copyright (C) 2006 The Android Open Source Project
 *  *
 *  * Licensed under the Apache License, Version 2.0 (the "License");
 *  * you may not use this file except in compliance with the License.
 *  * You may obtain a copy of the License at
 *  *
 *  *      http://www.apache.org/licenses/LICENSE-2.0
 *  *
 *  * Unless required by applicable law or agreed to in writing, software
 *  * distributed under the License is distributed on an "AS IS" BASIS,
 *  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  * See the License for the specific language governing permissions and
 *  * limitations under the License.
 *
 */

package android.core.view;

import android.content.Context;
import android.text.style.SuggestionSpan;
import android.view.View;
import android.view.inputmethod.InputMethodManager;

import com.jecelyin.common.utils.DLog;
import com.jecelyin.common.utils.ReflectionUtil;


public class InputMethodManagerCompat {
    public static InputMethodManager peekInstance(Context context) {
        return (InputMethodManager) context.getSystemService(Context.INPUT_METHOD_SERVICE);
    }

    public static void notifySuggestionPicked(InputMethodManager imm, SuggestionSpan span, String originalString, int index) {
        try {
            ReflectionUtil.callAny(
                    imm,
                    "notifySuggestionPicked",
                    new Class[]{InputMethodManager.class, SuggestionSpan.class, String.class, int.class},
                    new Object[]{imm, span, originalString, index}
            );
        } catch (Throwable e) {
            DLog.e(e);
        }
//        imm.notifySuggestionPicked(span, originalString, index);
    }

    public static void registerSuggestionSpansForNotification(InputMethodManager imm, SuggestionSpan[] spans) {
        try {
            ReflectionUtil.callAny(imm, "registerSuggestionSpansForNotification", new Class[]{SuggestionSpan[].class}, new Object[]{spans});
        } catch (Throwable e) {
            DLog.e(e);
        }
    }

    public static void setUpdateCursorAnchorInfoMode(InputMethodManager imm, int cursorUpdateMode) {
        try {
            ReflectionUtil.callAny(imm, "setUpdateCursorAnchorInfoMode", new Class[]{int.class}, new Object[]{cursorUpdateMode});
        } catch (Throwable e) {
            DLog.e(e);
        }
//        imm.setUpdateCursorAnchorInfoMode(cursorUpdateMode);
    }

    public static void hideSoftInput(View view) {
        InputMethodManager imm = InputMethodManagerCompat.peekInstance(view.getContext());
        if (imm != null) {
            imm.hideSoftInputFromWindow(view.getWindowToken(), InputMethodManager.HIDE_NOT_ALWAYS);
        }
    }


    public static void showSoftInput(View view) {
        InputMethodManager imm = InputMethodManagerCompat.peekInstance(view.getContext());
        if (imm != null) {
            imm.showSoftInput(view, InputMethodManager.SHOW_FORCED);
        }
    }
}
