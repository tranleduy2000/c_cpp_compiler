/*
 * Copyright 2018 Mr Duy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.duy.ccppcompiler.console.services;

import android.content.Context;
import android.media.AudioManager;
import android.util.Log;
import android.view.InputDevice;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.inputmethod.InputMethodManager;

import com.duy.ccppcompiler.console.ConsoleActivity;
import com.termux.terminal.KeyHandler;
import com.termux.terminal.TerminalEmulator;
import com.termux.terminal.TerminalSession;
import com.termux.view.TerminalViewClient;

public final class TermuxViewClient implements TerminalViewClient {

    private static final String TAG = "TermuxViewClient";
    private final ConsoleActivity mActivity;
    /**
     * Keeping track of the special keys acting as Ctrl and Fn for the soft keyboard and other hardware keys.
     */
    private boolean mVirtualControlKeyDown;
    private boolean mVirtualFnKeyDown;

    public TermuxViewClient(ConsoleActivity activity) {
        this.mActivity = activity;
    }

    @Override
    public float onScale(float scale) {
        if (scale < 0.9f || scale > 1.1f) {
            boolean increase = scale > 1.f;
            mActivity.changeFontSize(increase);
            return 1.0f;
        }
        return scale;
    }

    @Override
    public void onSingleTapUp(MotionEvent e) {
        Log.d(TAG, "onSingleTapUp() called with: e = [" + e + "]");
        InputMethodManager mgr = (InputMethodManager) mActivity.getSystemService(Context.INPUT_METHOD_SERVICE);
        mgr.showSoftInput(mActivity.mEmulatorView, InputMethodManager.SHOW_IMPLICIT);
    }

    @Override
    public boolean shouldBackButtonBeMappedToEscape() {
        return false;
    }

    @Override
    public void copyModeChanged(boolean copyMode) {
        // Disable drawer while copying.
    }

    private void log(String log) {
        System.out.println("TermuxViewClient:" + log);
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent e, TerminalSession currentSession) {

        if (handleVirtualKeys(keyCode, e, true))
            return true;

        if (keyCode == KeyEvent.KEYCODE_ENTER && !currentSession.isRunning()) {
            mActivity.finish();
            return true;
        } else if (e.isCtrlPressed() && e.isAltPressed()) {
            // Get the unmodified code point:
            int unicodeChar = e.getUnicodeChar(0);

            if (unicodeChar == 'k'/* keyboard */) {
                InputMethodManager imm = (InputMethodManager) mActivity.getSystemService(Context.INPUT_METHOD_SERVICE);
                imm.toggleSoftInput(InputMethodManager.SHOW_FORCED, 0);
            } else if (unicodeChar == 'm'/* menu */) {
                mActivity.mEmulatorView.showContextMenu();
            } else if (unicodeChar == 'v') {
                mActivity.doPaste();
            } else if (unicodeChar == '+' || e.getUnicodeChar(KeyEvent.META_SHIFT_ON) == '+') {
                // We also check for the shifted char here since shift may be required to produce '+',
                // see https://github.com/termux/termux-api/issues/2
                mActivity.changeFontSize(true);
            } else if (unicodeChar == '-') {
                mActivity.changeFontSize(false);
            } else if (unicodeChar >= '1' && unicodeChar <= '9') {
                int num = unicodeChar - '1';
                TermuxService service = mActivity.mTermService;
            }
            return true;
        }

        return false;

    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent e) {
        return handleVirtualKeys(keyCode, e, false);
    }

    @Override
    public boolean readControlKey() {
        return false;
    }

    @Override
    public boolean readAltKey() {
        return false;
    }

    @Override
    public boolean onCodePoint(final int codePoint, boolean ctrlDown, TerminalSession session) {
        if (mVirtualFnKeyDown) {
            int resultingKeyCode = -1;
            int resultingCodePoint = -1;
            boolean altDown = false;
            int lowerCase = Character.toLowerCase(codePoint);
            switch (lowerCase) {
                // Arrow keys.
                case 'w':
                    resultingKeyCode = KeyEvent.KEYCODE_DPAD_UP;
                    break;
                case 'a':
                    resultingKeyCode = KeyEvent.KEYCODE_DPAD_LEFT;
                    break;
                case 's':
                    resultingKeyCode = KeyEvent.KEYCODE_DPAD_DOWN;
                    break;
                case 'd':
                    resultingKeyCode = KeyEvent.KEYCODE_DPAD_RIGHT;
                    break;

                // Page up and down.
                case 'p':
                    resultingKeyCode = KeyEvent.KEYCODE_PAGE_UP;
                    break;
                case 'n':
                    resultingKeyCode = KeyEvent.KEYCODE_PAGE_DOWN;
                    break;

                // Some special keys:
                case 't':
                    resultingKeyCode = KeyEvent.KEYCODE_TAB;
                    break;
                case 'i':
                    resultingKeyCode = KeyEvent.KEYCODE_INSERT;
                    break;
                case 'h':
                    resultingCodePoint = '~';
                    break;

                // Special characters to input.
                case 'u':
                    resultingCodePoint = '_';
                    break;
                case 'l':
                    resultingCodePoint = '|';
                    break;

                // Function keys.
                case '1':
                case '2':
                case '3':
                case '4':
                case '5':
                case '6':
                case '7':
                case '8':
                case '9':
                    resultingKeyCode = (codePoint - '1') + KeyEvent.KEYCODE_F1;
                    break;
                case '0':
                    resultingKeyCode = KeyEvent.KEYCODE_F10;
                    break;

                // Other special keys.
                case 'e':
                    resultingCodePoint = /*Escape*/ 27;
                    break;
                case '.':
                    resultingCodePoint = /*^.*/ 28;
                    break;

                case 'b': // alt+b, jumping backward in readline.
                case 'f': // alf+f, jumping forward in readline.
                case 'x': // alt+x, common in emacs.
                    resultingCodePoint = lowerCase;
                    altDown = true;
                    break;

                // Volume control.
                case 'v':
                    resultingCodePoint = -1;
                    AudioManager audio = (AudioManager) mActivity.getSystemService(Context.AUDIO_SERVICE);
                    audio.adjustSuggestedStreamVolume(AudioManager.ADJUST_SAME, AudioManager.USE_DEFAULT_STREAM_TYPE, AudioManager.FLAG_SHOW_UI);
                    break;


            }
            if (resultingKeyCode != -1) {
                TerminalEmulator term = session.getEmulator();
                session.write(KeyHandler.getCode(resultingKeyCode, 0, term.isCursorKeysApplicationMode(), term.isKeypadApplicationMode()));
            } else if (resultingCodePoint != -1) {
                session.writeCodePoint(altDown, resultingCodePoint);

                System.out.println("---------------------->input:" + resultingCodePoint);
            }
            return true;
        }

        return false;
    }

    @Override
    public boolean onLongPress(MotionEvent event) {
        return false;
    }

    /**
     * Handle dedicated volume buttons as virtual keys if applicable.
     */
    private boolean handleVirtualKeys(int keyCode, KeyEvent event, boolean down) {
        InputDevice inputDevice = event.getDevice();
        if (inputDevice != null && inputDevice.getKeyboardType() == InputDevice.KEYBOARD_TYPE_ALPHABETIC) {
            // Do not steal dedicated buttons from a full external keyboard.
            return false;
        } else if (keyCode == KeyEvent.KEYCODE_VOLUME_DOWN) {
            mVirtualControlKeyDown = down;
            return true;
        } else if (keyCode == KeyEvent.KEYCODE_VOLUME_UP) {
            mVirtualFnKeyDown = down;
            return true;
        }
        return false;
    }


}
