package com.pdaxrom.cctools;

import android.annotation.SuppressLint;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.ActivityInfo;
import android.content.pm.PackageManager;
import android.content.pm.ResolveInfo;
import android.content.res.Configuration;
import android.net.Uri;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.preference.PreferenceManager;
import android.support.v7.widget.Toolbar;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.ContextMenu;
import android.view.GestureDetector;
import android.view.KeyEvent;
import android.view.MenuItem;
import android.view.MotionEvent;
import android.view.View;
import android.view.inputmethod.InputMethodManager;
import android.widget.FrameLayout;

import com.jecelyin.editor.v2.FullScreenActivity;
import com.pdaxrom.packagemanager.Environment;
import com.pdaxrom.term.ShellTermSession;

import java.util.Arrays;
import java.util.List;

import jackpal.androidterm.TermView;
import jackpal.androidterm.emulatorview.EmulatorView;
import jackpal.androidterm.emulatorview.TermSession;
import jackpal.androidterm.emulatorview.compat.ClipboardManagerCompat;
import jackpal.androidterm.emulatorview.compat.ClipboardManagerCompatFactory;
import jackpal.androidterm.emulatorview.compat.KeycodeConstants;
import jackpal.androidterm.util.TermSettings;

public class TermActivity extends FullScreenActivity implements SharedPreferences.OnSharedPreferenceChangeListener {
    private static final String TAG = "TermActivity";
    private final static int SELECT_TEXT_ID = 0;
    private final static int COPY_ALL_ID = 1;
    private final static int PASTE_ID = 2;
    private final static int SEND_CONTROL_KEY_ID = 3;
    private final static int SEND_FN_KEY_ID = 4;

    private TermView mTermView;
    private ShellTermSession mSession;
    private boolean isRunning = false;
    private Handler handler = new Handler();
    @SuppressLint("HandlerLeak")
    private final Handler mMsgHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            if (!isRunning) {
                return;
            }
            if (msg.what == 123) {
                Log.i(TAG, "Message - Process exited!!!");
                showTitle(getString(R.string.console_name) + " - " + getString(R.string.console_finished));
                isRunning = false;
            }
        }
    };
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

    private ShellTermSession createShellTermSession(String cmdline, String workdir) {
        cmdline = cmdline.replaceAll("\\s+", " ");
        Log.i(TAG, "Shell sesion for " + cmdline + "\n");
        String[] envp = Environment.buildDefaultEnv(this);
        String[] argv = cmdline.split("\\s+");
        Log.i(TAG, "argv " + Arrays.toString(argv));
        isRunning = true;
        return new ShellTermSession(argv, envp, workdir, mMsgHandler);
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_term);
        setSupportActionBar((Toolbar) findViewById(R.id.toolbar));
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        String fileName = getIntent().getStringExtra(BuildConstants.EXTRA_FILE_NAME);
        String workDir = getIntent().getStringExtra(BuildConstants.EXTRA_WORK_DIR);

        showTitle(getString(R.string.console_name) + " - " + getString(R.string.console_executing));

        mSession = createShellTermSession(fileName, workDir);
        mTermView = createEmulatorView(mSession);
        FrameLayout frameLayout = findViewById(R.id.content);
        frameLayout.removeAllViews();
        frameLayout.addView(mTermView);

        final SharedPreferences mPrefs = PreferenceManager.getDefaultSharedPreferences(this);
        mSettings = new TermSettings(getResources(), mPrefs);
        mPrefs.registerOnSharedPreferenceChangeListener(this);

        updatePrefs();
    }

    private void updatePrefs() {
        mUseKeyboardShortcuts = mSettings.getUseKeyboardShortcutsFlag();

        DisplayMetrics metrics = new DisplayMetrics();
        getWindowManager().getDefaultDisplay().getMetrics(metrics);


        mTermView.setDensity(metrics);
        mTermView.updatePrefs(mSettings);
//        mSession.updatePrefs(mSettings);

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
        EmulatorView v = mTermView;
        if (v != null) {
            v.updateSize(false);
        }
    }

    @Override
    public void onBackPressed() {
        if (isRunning) {
            Log.i(TAG, "kill process group");
            mSession.hangup();
        }
        super.onBackPressed();
    }

    private void showTitle(final String str) {
        Runnable proc = new Runnable() {
            public void run() {
                setTitle(str);
            }
        };
        handler.post(proc);
    }

    @Override
    public void onCreateContextMenu(ContextMenu menu, View v,
                                    ContextMenu.ContextMenuInfo menuInfo) {
        super.onCreateContextMenu(menu, v, menuInfo);
        menu.setHeaderTitle(jackpal.androidterm.R.string.edit_text);
        menu.add(0, SELECT_TEXT_ID, 0, jackpal.androidterm.R.string.select_text);
        menu.add(0, COPY_ALL_ID, 0, jackpal.androidterm.R.string.copy_all);
        menu.add(0, PASTE_ID, 0, jackpal.androidterm.R.string.paste);
        menu.add(0, SEND_CONTROL_KEY_ID, 0, jackpal.androidterm.R.string.send_control_key);
        menu.add(0, SEND_FN_KEY_ID, 0, jackpal.androidterm.R.string.send_fn_key);
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
        mTermView.requestFocus();
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

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String s) {
        mSettings.readPrefs(sharedPreferences);
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
