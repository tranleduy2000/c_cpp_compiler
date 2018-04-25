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

package com.duy.ccppcompiler.console.activities;

import android.app.ActionBar;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.net.Uri;
import android.os.Bundle;
import android.os.IBinder;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.text.TextUtils;
import android.util.TypedValue;
import android.view.MenuItem;

import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.console.services.TermuxService;
import com.duy.ccppcompiler.console.services.TermuxViewClient;
import com.termux.terminal.TerminalSession;
import com.termux.view.TerminalView;

public class ConsoleActivity extends AppCompatActivity implements ServiceConnection {

    public static final String EXTRA_BINARY_FILE_PATH = "file_path";
    private static final String TAG = "ConsoleActivity";

    private static final int MAX_FONTSIZE = 256;
    private static int MIN_FONTSIZE;
    public TerminalView mEmulatorView;
    public TermuxService mTermService;
    private String cmd;
    private int mFontSize;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_console);
        setSupportActionBar((Toolbar) findViewById(R.id.toolbar));
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);

        computeFontSize();
        initView();
        startService();
    }

    private void computeFontSize() {
        float dipInPixels = TypedValue.applyDimension(TypedValue.COMPLEX_UNIT_DIP, 1, this.getResources().getDisplayMetrics());
        MIN_FONTSIZE = (int) (4f * dipInPixels);
        int defaultFontSize = Math.round(12 * dipInPixels);
        // Make it divisible by 2 since that is the minimal adjustment step:
        if (defaultFontSize % 2 == 1) defaultFontSize--;

        mFontSize = defaultFontSize;
        mFontSize = Math.max(MIN_FONTSIZE, Math.min(mFontSize, MAX_FONTSIZE));
    }

    private void initView() {
        ActionBar actionBar = getActionBar();
        if (actionBar != null)
            actionBar.setDisplayHomeAsUpEnabled(true);
        mEmulatorView = findViewById(R.id.emulatorView);
        cmd = getIntent().getStringExtra(EXTRA_BINARY_FILE_PATH);
        mEmulatorView.setTextSize(mFontSize);
        mEmulatorView.requestFocus();
        mEmulatorView.setOnKeyListener(new TermuxViewClient(this));
    }

    private void startService() {
        Intent serviceIntent = new Intent(this, TermuxService.class);
        // Start the service and make it run regardless of who is bound to it:
        serviceIntent.setAction(TermuxService.ACTION_EXECUTE);
        String uriStr = "file:///" + cmd;
        serviceIntent.setData(Uri.parse(uriStr));
        startService(serviceIntent);
        if (!bindService(serviceIntent, this, 0)) {
            throw new RuntimeException("bindService() failed");
        }
    }

    @Override
    protected void onStart() {
        super.onStart();
        mEmulatorView.onScreenUpdated();
    }

    public void doPaste() {
        ClipboardManager clipboard = (ClipboardManager) getSystemService(Context.CLIPBOARD_SERVICE);
        ClipData clipData = clipboard.getPrimaryClip();
        if (clipData == null) return;
        CharSequence paste = clipData.getItemAt(0).coerceToText(this);
        if (!TextUtils.isEmpty(paste))
            getCurrentTermSession().getEmulator().paste(paste.toString());
    }

    private TerminalSession getCurrentTermSession() {
        return mTermService.getTermSession();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        unbindService(this);
        mTermService.stopSelf();
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == android.R.id.home) {
            finish();
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onServiceConnected(ComponentName componentName, IBinder service) {
        mTermService = ((TermuxService.LocalBinder) service).service;
        mEmulatorView.attachSession(mTermService.getTermSession());
        mTermService.mSessionChangeCallback = new TerminalSession.SessionChangedCallback() {
            @Override
            public void onTextChanged(TerminalSession changedSession) {
                mEmulatorView.onScreenUpdated();
            }

            @Override
            public void onTitleChanged(TerminalSession updatedSession) {

            }

            @Override
            public void onSessionFinished(final TerminalSession finishedSession) {
            }

            @Override
            public void onClipboardText(TerminalSession session, String text) {
                ClipboardManager clipboard = (ClipboardManager) getSystemService(Context.CLIPBOARD_SERVICE);
                clipboard.setPrimaryClip(new ClipData(null, new String[]{"text/plain"}, new ClipData.Item(text)));
            }

            @Override
            public void onBell(TerminalSession session) {


            }

            @Override
            public void onColorsChanged(TerminalSession changedSession) {

            }
        };
    }

    @Override
    public void onServiceDisconnected(ComponentName componentName) {

    }

    public void changeFontSize(boolean increase) {
        mFontSize += (increase ? 1 : -1) * 2;
        mFontSize = Math.max(MIN_FONTSIZE, Math.min(mFontSize, MAX_FONTSIZE));
        mEmulatorView.setTextSize(mFontSize);
    }

}
