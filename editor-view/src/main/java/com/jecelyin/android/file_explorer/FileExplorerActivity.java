/*
 * Copyright (C) 2016 Jecelyin Peng <jecelyin@gmail.com>
 *
 * This file is part of 920 Text Editor.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.jecelyin.android.file_explorer;

import android.app.Activity;
import android.content.DialogInterface;
import android.content.Intent;
import android.databinding.DataBindingUtil;
import android.os.Bundle;
import android.os.Environment;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v7.app.AlertDialog;
import android.support.v7.widget.Toolbar;
import android.text.TextUtils;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;

import com.duy.ide.editor.editor.R;
import com.duy.ide.editor.editor.databinding.ActivityFileExplorerBinding;
import com.jecelyin.android.file_explorer.io.JecFile;
import com.jecelyin.android.file_explorer.io.LocalFile;
import com.jecelyin.android.file_explorer.listener.OnClipboardDataChangedListener;
import com.jecelyin.android.file_explorer.util.FileListSorter;
import com.jecelyin.common.utils.IOUtils;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.FullScreenActivity;
import com.jecelyin.editor.v2.Preferences;

import java.io.File;
import java.nio.charset.Charset;
import java.util.Iterator;
import java.util.Set;
import java.util.SortedMap;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class FileExplorerActivity extends FullScreenActivity implements View.OnClickListener, OnClipboardDataChangedListener {
    public static final String EXTRA_HOME_PATH = "home_path";
    public static final String EXTRA_INIT_PATH = "dest_file";
    public static final String EXTRA_MODE = "mode";

    private static final int MODE_PICK_FILE = 1;
    private static final int MODE_PICK_PATH = 2;
    private FileListPagerFragment mFileListPagerFragment;
    private ActivityFileExplorerBinding binding;
    private int mMode;
    private String fileEncoding = null;
    private String mLastPath;
    private String mHomePath;
    private FileClipboard fileClipboard;
    private MenuItem pasteMenu;

    public static void startPickFileActivity(Activity activity,
                                             @Nullable String destFile,
                                             @Nullable String homePath,
                                             int requestCode) {
        Intent it = new Intent(activity, FileExplorerActivity.class);
        it.putExtra(EXTRA_MODE, MODE_PICK_FILE);
        it.putExtra(EXTRA_INIT_PATH, destFile);
        it.putExtra(EXTRA_HOME_PATH, homePath);
        activity.startActivityForResult(it, requestCode);
    }

    public static void startPickPathActivity(Activity activity, String destFile, String encoding, int requestCode) {
        Intent it = new Intent(activity, FileExplorerActivity.class);
        it.putExtra(EXTRA_MODE, MODE_PICK_PATH);
        it.putExtra(EXTRA_INIT_PATH, destFile);
        it.putExtra("encoding", encoding);
        activity.startActivityForResult(it, requestCode);
    }

    @NonNull
    public static String getFile(Intent it) {
        return it.getStringExtra("file");
    }

    @Nullable
    public static String getFileEncoding(Intent it) {
        return it.getStringExtra("encoding");
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        binding = DataBindingUtil.setContentView(this, R.layout.activity_file_explorer);

        Intent intent = getIntent();
        mMode = intent.getIntExtra(EXTRA_MODE, MODE_PICK_FILE);
        mHomePath = intent.getStringExtra(EXTRA_HOME_PATH);
        if (TextUtils.isEmpty(mHomePath)) {
            mHomePath = Environment.getExternalStorageDirectory().getPath();
        }

        setSupportActionBar((Toolbar) findViewById(R.id.toolbar));
        getSupportActionBar().setDisplayHomeAsUpEnabled(true);
        getSupportActionBar().setTitle(mMode == MODE_PICK_FILE ? R.string.open_file : R.string.save_file);

        mLastPath = Preferences.getInstance(this).getLastOpenPath();
        if (TextUtils.isEmpty(mLastPath)) {
            mLastPath = Environment.getExternalStorageDirectory().getPath();
        } else {
            File file = new File(mLastPath);
            if (!file.exists() || !file.canRead()) {
                mLastPath = Environment.getExternalStorageDirectory().getPath();
            }
        }

        String initPath = intent.getStringExtra(EXTRA_INIT_PATH);

        if (!TextUtils.isEmpty(initPath)) {
            File dest = new File(initPath);
            mLastPath = dest.isFile() ? dest.getParent() : dest.getPath();
            binding.filenameEditText.setText(dest.getName());
        } else {
            binding.filenameEditText.setText(getString(R.string.untitled_file_name));
        }

        initFileView();
        binding.saveBtn.setOnClickListener(this);
        binding.fileEncodingTextView.setOnClickListener(this);

        String encoding = intent.getStringExtra("encoding");
        fileEncoding = encoding;
        if (TextUtils.isEmpty(encoding)) {
            encoding = getString(R.string.auto_detection_encoding);
        }
        binding.fileEncodingTextView.setText(encoding);

        binding.filenameLayout.setVisibility(mMode == MODE_PICK_FILE ? View.GONE : View.VISIBLE);

        getFileClipboard().setOnClipboardDataChangedListener(this);
    }

    private void initFileView() {
        JecFile path = new LocalFile(mLastPath);
        mFileListPagerFragment = (FileListPagerFragment) getSupportFragmentManager()
                .findFragmentByTag(FileListPagerFragment.class.getName());
        if (mFileListPagerFragment == null) {
            mFileListPagerFragment = FileListPagerFragment.newFragment(path);
        }
        getSupportFragmentManager()
                .beginTransaction()
                .replace(R.id.content, mFileListPagerFragment, FileListPagerFragment.class.getName())
                .commit();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.menu_explorer, menu);

        Preferences preferences = Preferences.getInstance(this);
        menu.findItem(R.id.show_hidden_files_menu).setChecked(preferences.isShowHiddenFiles());
        pasteMenu = menu.findItem(R.id.paste_menu);

        int sortId;
        switch (preferences.getFileSortType()) {
            case FileListSorter.SORT_DATE:
                sortId = R.id.sort_by_datetime_menu;
                break;
            case FileListSorter.SORT_SIZE:
                sortId = R.id.sort_by_size_menu;
                break;
            case FileListSorter.SORT_TYPE:
                sortId = R.id.sort_by_type_menu;
                break;
            default:
                sortId = R.id.sort_by_name_menu;
                break;
        }

        menu.findItem(sortId).setChecked(true);

        return super.onCreateOptionsMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        Preferences preferences = Preferences.getInstance(this);
        int id = item.getItemId();
        if (id == R.id.show_hidden_files_menu) {
            item.setChecked(!item.isChecked());
            preferences.setShowHiddenFiles(item.isChecked());
        } else if (id == R.id.sort_by_name_menu) {
            item.setChecked(true);
            preferences.setFileSortType(FileListSorter.SORT_NAME);
        } else if (id == R.id.sort_by_datetime_menu) {
            item.setChecked(true);
            preferences.setFileSortType(FileListSorter.SORT_DATE);
        } else if (id == R.id.sort_by_size_menu) {
            item.setChecked(true);
            preferences.setFileSortType(FileListSorter.SORT_SIZE);
        } else if (id == R.id.sort_by_type_menu) {
            item.setChecked(true);
            preferences.setFileSortType(FileListSorter.SORT_TYPE);
        } else if (id == R.id.action_goto_home) {
            LocalFile home = new LocalFile(mHomePath);
            mFileListPagerFragment.switchToPath(home);
        }
        return super.onOptionsItemSelected(item);
    }

    boolean onSelectFile(JecFile file) {
        if (file.isFile()) {
            if (mMode == MODE_PICK_FILE) {
                Intent it = new Intent();
                it.putExtra("file", file.getPath());
                it.putExtra("encoding", fileEncoding);
                setResult(RESULT_OK, it);
                finish();
            } else {
                binding.filenameEditText.setText(file.getName());
            }

            return true;
        } else if (file.isDirectory()) {
            mLastPath = file.getPath();
        }

        return false;
    }

    @Override
    public void onClick(View v) {
        int id = v.getId();
        if (id == R.id.save_btn) {
            onSave();
        } else if (id == R.id.file_encoding_textView) {
            onShowEncodingList();
        }
    }

    private void onShowEncodingList() {
        SortedMap m = Charset.availableCharsets();
        Set k = m.keySet();

        int selected = 0;
        final String[] names = new String[m.size() + 1];
        names[0] = getString(R.string.auto_detection_encoding);
        Iterator iterator = k.iterator();
        int i = 1;
        while (iterator.hasNext()) {
            String n = (String) iterator.next();
            if (n.equals(fileEncoding))
                selected = i;
            names[i++] = n;
        }

        new AlertDialog.Builder(this)
                .setSingleChoiceItems(names, -1, new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {
                        binding.fileEncodingTextView.setText(names[which]);
                        if (which > 0) {
                            fileEncoding = names[which];
                        }
                    }
                })
                .show();
    }

    private void onSave() {
        String fileName = binding.filenameEditText.getText().toString().trim();
        if (TextUtils.isEmpty(fileName)) {
            binding.filenameEditText.setError(getString(R.string.can_not_be_empty));
            return;
        }
        if (IOUtils.isInvalidFilename(fileName)) {
            binding.filenameEditText.setError(getString(R.string.illegal_filename));
            return;
        }
        if (TextUtils.isEmpty(mLastPath)) {
            binding.filenameEditText.setError(getString(R.string.unknown_path));
            return;
        }

        File f = new File(mLastPath);
        if (f.isFile()) {
            f = f.getParentFile();
        }

        final File newFile = new File(f, fileName);
        if (newFile.exists()) {
            UIUtils.showConfirmDialog(this, getString(R.string.override_file_prompt, fileName), new UIUtils.OnClickCallback() {
                @Override
                public void onOkClick() {
                    saveAndFinish(newFile);
                }
            });
        } else {
            saveAndFinish(newFile);
        }
    }

    private void saveAndFinish(File file) {
        Intent it = new Intent();
        it.putExtra("file", file.getPath());
        it.putExtra("encoding", fileEncoding);
        setResult(RESULT_OK, it);
        finish();
    }

    public FileClipboard getFileClipboard() {
        if (fileClipboard == null)
            fileClipboard = new FileClipboard();

        return fileClipboard;
    }

    @Override
    public void onClipboardDataChanged() {
        if (pasteMenu == null)
            return;

        pasteMenu.setVisible(getFileClipboard().canPaste());
    }
}
