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

package com.duy.ccppcompiler.console;

import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.content.res.Configuration;
import android.content.res.Resources;
import android.net.Uri;
import android.os.Bundle;
import android.os.IBinder;
import android.preference.PreferenceManager;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AlertDialog;
import android.support.v7.widget.Toolbar;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.ContextMenu;
import android.view.GestureDetector;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.FrameLayout;
import android.widget.Toast;

import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.pkgmanager.Environment;
import com.jecelyin.editor.v2.ThemeSupportActivity;
import com.pdaxrom.cctools.BuildConstants;

import java.util.Arrays;
import java.util.List;

import jackpal.androidterm.TermPreferencesActivity;
import jackpal.androidterm.TermSettings;
import jackpal.androidterm.TermView;
import jackpal.androidterm.emulatorview.EmulatorView;
import jackpal.androidterm.emulatorview.TermSession;
import jackpal.androidterm.emulatorview.compat.ClipboardManagerCompat;
import jackpal.androidterm.emulatorview.compat.ClipboardManagerCompatFactory;
import jackpal.androidterm.emulatorview.compat.KeycodeConstants;

public class TermActivity extends ThemeSupportActivity implements SharedPreferences.OnSharedPreferenceChangeListener, TermSession.FinishCallback, ShellTermSession.OnProcessExitListener {
    private static final String TAG = "TermActivity";
    private final static int SELECT_TEXT_ID = 0;
    private final static int COPY_ALL_ID = 1;
    private final static int PASTE_ID = 2;
    private final static int SEND_CONTROL_KEY_ID = 3;
    private final static int SEND_FN_KEY_ID = 4;
    private TermView mTermView;
    private ShellTermSession mSession;
    /**
     * Should we use keyboard shortcuts?
     */
    private boolean mUseKeyboardShortcuts;
    /**
     * Intercepts keys before the view/terminal gets it.
     */
    private View.OnKeyListener mKeyListener = new View.OnKeyListener() {
        public boolean onKey(View v, int keyCode, KeyEvent event) {
            return keyboardShortcuts(keyCode, event);
        }

        /**
         * Keyboard shortcuts (tab management, paste)
         */
        private boolean keyboardShortcuts(int keyCode, KeyEvent event) {
            if (event.getAction() != KeyEvent.ACTION_DOWN) {
                return false;
            }
            if (!mUseKeyboardShortcuts) {
                return false;
            }
            boolean isCtrlPressed = (event.getMetaState() & KeycodeConstants.META_CTRL_ON) != 0;
            boolean isShiftPressed = (event.getMetaState() & KeycodeConstants.META_SHIFT_ON) != 0;

            if (keyCode == KeycodeConstants.KEYCODE_V && isCtrlPressed && isShiftPressed) {
                doPaste();

                return true;
            } else {
                return false;
            }
        }
    };
    private TermSettings mSettings;
    private boolean mHaveFullHwKeyboard = false;

    private ShellTermSession createShellTermSession(String cmd, String workdir) {
        cmd = cmd.replaceAll("\\s+", " ");
        Log.i(TAG, "Shell sesion for " + cmd + "\n");
        String[] envp = Environment.buildDefaultEnv(this);
        String[] argv = cmd.split("\\s+");
        Log.i(TAG, "argv " + Arrays.toString(argv));
        TermSettings settings = new TermSettings(getResources(), PreferenceManager.getDefaultSharedPreferences(this));

        ShellTermSession shellTermSession = new ShellTermSession(settings, argv, envp, workdir);
        shellTermSession.setOnProcessExitListener(this);
//        shellTermSession.setProcessExitMessage(getString(R.string.console_finished));

        return shellTermSession;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_term);

        setSupportActionBar((Toolbar) findViewById(R.id.toolbar));
        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.setDisplayHomeAsUpEnabled(true);
            actionBar.setTitle(R.string.title_menu_terminal);
            actionBar.setSubtitle(R.string.console_executing);
        }
        String fileName = getIntent().getStringExtra(BuildConstants.EXTRA_FILE_NAME);
        String workDir = getIntent().getStringExtra(BuildConstants.EXTRA_WORK_DIR);


        mSession = createShellTermSession(fileName, workDir);
        mSession.setFinishCallback(this);

        mTermView = createEmulatorView(mSession);
        FrameLayout frameLayout = findViewById(R.id.content);
        frameLayout.removeAllViews();
        frameLayout.addView(mTermView);

        final SharedPreferences mPrefs = PreferenceManager.getDefaultSharedPreferences(this);
        mSettings = new TermSettings(getResources(), mPrefs);
        mPrefs.registerOnSharedPreferenceChangeListener(this);

        mHaveFullHwKeyboard = checkHaveFullHwKeyboard(getResources().getConfiguration());
        updatePrefs();
    }

    private boolean checkHaveFullHwKeyboard(Configuration c) {
        return (c.keyboard == Configuration.KEYBOARD_QWERTY) &&
                (c.hardKeyboardHidden == Configuration.HARDKEYBOARDHIDDEN_NO);
    }

    private void updatePrefs() {
        mUseKeyboardShortcuts = mSettings.getUseKeyboardShortcutsFlag();

        DisplayMetrics metrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metrics);


        mTermView.setDensity(metrics);
        mTermView.updatePrefs(mSettings);
        mSession.updatePrefs(mSettings);

        int orientation = mSettings.getScreenOrientation();
        int o = 0;
        if (orientation == 0) {
            o = ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED;
        } else if (orientation == 1) {
            o = ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;
        } else if (orientation == 2) {
            o = ActivityInfo.SCREEN_ORIENTATION_PORTRAIT;
        } else {
            /* Shouldn't be happened. */
        }
        setRequestedOrientation(o);
    }

    private TermView createEmulatorView(ShellTermSession session) {
        DisplayMetrics metrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metrics);
        TermView emulatorView = new TermView(this, session, metrics);

        emulatorView.setExtGestureListener(new EmulatorViewGestureListener(emulatorView));
        emulatorView.setOnKeyListener(mKeyListener);
        registerForContextMenu(emulatorView);

        return emulatorView;
    }

    @Override
    protected void onResume() {
        super.onResume();
        mTermView.onResume();
    }

    @Override
    protected void onPause() {
        mTermView.onPause();
        super.onPause();

         /* Explicitly close the input method
           Otherwise, the soft keyboard could cover up whatever activity takes
           our place */
        final IBinder token = mTermView.getWindowToken();
        new Thread() {
            @Override
            public void run() {
                InputMethodManager imm = (InputMethodManager) getSystemService(INPUT_METHOD_SERVICE);
                if (imm != null) {
                    imm.hideSoftInputFromWindow(token, 0);
                }
            }
        }.start();
    }

    @Override
    protected void onStop() {
        super.onStop();
    }

    @Override
    protected void onDestroy() {
        PreferenceManager.getDefaultSharedPreferences(this)
                .unregisterOnSharedPreferenceChangeListener(this);
        if (mSession != null) {
            mSession.setFinishCallback(null);
            mSession.finish();
        }
        super.onDestroy();
    }

    @Override
    public void onConfigurationChanged(Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        mHaveFullHwKeyboard = checkHaveFullHwKeyboard(newConfig);

        EmulatorView v = mTermView;
        if (v != null) {
            v.updateSize(false);
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_terminal, menu);
        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        if (id == R.id.menu_preferences) {
            doPreferences();
        } else if (id == R.id.menu_reset) {
            doResetTerminal();
            Toast toast = Toast.makeText(this, R.string.reset_toast_notification, Toast.LENGTH_LONG);
            toast.setGravity(Gravity.CENTER, 0, 0);
            toast.show();
        } else if (id == R.id.menu_special_keys) {
            doDocumentKeys();
        } else if (id == R.id.menu_toggle_soft_keyboard) {
            doToggleSoftKeyboard();
        } else if (id == R.id.action_help) {
            Intent openHelp = new Intent(Intent.ACTION_VIEW, Uri.parse(getString(R.string.help_url)));
            startActivity(openHelp);
        } else if (id == R.id.action_key_tab) {
            sendTabKeyEvent();
        }
        return super.onOptionsItemSelected(item);
    }

    private void sendTabKeyEvent() {
        KeyEvent tabDown = new KeyEvent(KeyEvent.ACTION_DOWN, KeyEvent.KEYCODE_TAB);
        KeyEvent tabUp = new KeyEvent(KeyEvent.ACTION_UP, KeyEvent.KEYCODE_TAB);
        getCurrentEmulatorView().onKeyDown(tabDown.getKeyCode(), tabDown);
        getCurrentEmulatorView().onKeyUp(tabUp.getKeyCode(), tabUp);
    }

    @Override
    public void onCreateContextMenu(ContextMenu menu, View v,
                                    ContextMenu.ContextMenuInfo menuInfo) {
        super.onCreateContextMenu(menu, v, menuInfo);
        menu.setHeaderTitle(R.string.edit_text);
        menu.add(0, SELECT_TEXT_ID, 0, R.string.select_text);
        menu.add(0, COPY_ALL_ID, 0, R.string.copy_all);
        menu.add(0, PASTE_ID, 0, R.string.paste);
        menu.add(0, SEND_CONTROL_KEY_ID, 0, R.string.send_control_key);
        menu.add(0, SEND_FN_KEY_ID, 0, R.string.send_fn_key);
        if (!canPaste()) {
            menu.getItem(PASTE_ID).setEnabled(false);
        }
    }

    @Override
    public boolean onContextItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case SELECT_TEXT_ID:
                mTermView.toggleSelectingText();
                return true;
            case COPY_ALL_ID:
                doCopyAll();
                return true;
            case PASTE_ID:
                doPaste();
                return true;
            case SEND_CONTROL_KEY_ID:
                doSendControlKey();
                return true;
            case SEND_FN_KEY_ID:
                doSendFnKey();
                return true;
            default:
                return super.onContextItemSelected(item);
        }
    }

    private void doCopyAll() {
        ClipboardManagerCompat clip = ClipboardManagerCompatFactory
                .getManager(getApplicationContext());
        clip.setText(getCurrentTermSession().getTranscriptText().trim());
    }

    private TermSession getCurrentTermSession() {
        return mSession;
    }

    private void doSendControlKey() {
        getCurrentEmulatorView().sendControlKey();
    }

    private EmulatorView getCurrentEmulatorView() {
        return mTermView;
    }

    private void doSendFnKey() {
        getCurrentEmulatorView().sendFnKey();
    }

    /**
     * Send a URL up to Android to be handled by a browser.
     *
     * @param link The URL to be opened.
     */
    private void execURL(String link) {
        Uri webLink = Uri.parse(link);
        Intent openLink = new Intent(Intent.ACTION_VIEW, webLink);
        PackageManager pm = getPackageManager();
        List<ResolveInfo> handlers = pm.queryIntentActivities(openLink, 0);
        if (handlers.size() > 0)
            startActivity(openLink);
    }

    private void doUIToggle(int x, int y, int width, int height) {
        if (!mHaveFullHwKeyboard) {
            doToggleSoftKeyboard();
        }

        getCurrentEmulatorView().requestFocus();
    }

    private void doToggleSoftKeyboard() {
        InputMethodManager imm = (InputMethodManager)
                getSystemService(Context.INPUT_METHOD_SERVICE);
        if (imm != null) {
            imm.toggleSoftInput(InputMethodManager.SHOW_FORCED, 0);
        }

    }

    private void doDocumentKeys() {
        AlertDialog.Builder dialog = new AlertDialog.Builder(this);
        Resources r = getResources();
        dialog.setTitle(r.getString(R.string.control_key_dialog_title));
        dialog.setMessage(
                formatMessage(mSettings.getControlKeyId(), TermSettings.CONTROL_KEY_ID_NONE,
                        r, R.array.control_keys_short_names,
                        R.string.control_key_dialog_control_text,
                        R.string.control_key_dialog_control_disabled_text, "CTRLKEY")
                        + "\n\n" +
                        formatMessage(mSettings.getFnKeyId(), TermSettings.FN_KEY_ID_NONE,
                                r, R.array.fn_keys_short_names,
                                R.string.control_key_dialog_fn_text,
                                R.string.control_key_dialog_fn_disabled_text, "FNKEY"));
        dialog.show();
    }

    private String formatMessage(int keyId, int disabledKeyId,
                                 Resources r, int arrayId,
                                 int enabledId,
                                 int disabledId, String regex) {
        if (keyId == disabledKeyId) {
            return r.getString(disabledId);
        }
        String[] keyNames = r.getStringArray(arrayId);
        String keyName = keyNames[keyId];
        String template = r.getString(enabledId);
        return template.replaceAll(regex, keyName);
    }

    private void doPaste() {
        if (!canPaste()) {
            return;
        }
        ClipboardManagerCompat clip = ClipboardManagerCompatFactory
                .getManager(getApplicationContext());
        CharSequence paste = clip.getText();
        mSession.write(paste.toString());
    }

    private boolean canPaste() {
        ClipboardManagerCompat clip = ClipboardManagerCompatFactory
                .getManager(getApplicationContext());
        return clip.hasText();
    }

    private void doPreferences() {
        startActivity(new Intent(this, TermPreferencesActivity.class));
    }

    private void doResetTerminal() {
        TermSession session = getCurrentTermSession();
        if (session != null) {
            session.reset();
        }
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String s) {
        super.onSharedPreferenceChanged(sharedPreferences, s);
        mSettings.readPrefs(sharedPreferences);
    }

    @Override
    public void onSessionFinish(TermSession session) {
        finish();
    }

    @Override
    public void onProcessExit(int exitCode) {
        getSupportActionBar().setSubtitle(R.string.console_finished);
    }


    private class EmulatorViewGestureListener extends GestureDetector.SimpleOnGestureListener {
        private EmulatorView view;

        public EmulatorViewGestureListener(EmulatorView view) {
            this.view = view;
        }

        @Override
        public boolean onSingleTapUp(MotionEvent e) {
            // Let the EmulatorView handle taps if mouse tracking is active
            if (view.isMouseTrackingActive()) return false;

            //Check for link at tap location
            String link = view.getURLat(e.getX(), e.getY());
            if (link != null)
                execURL(link);
            else
                doUIToggle((int) e.getX(), (int) e.getY(), view.getVisibleWidth(), view.getVisibleHeight());
            return true;
        }
    }

}
