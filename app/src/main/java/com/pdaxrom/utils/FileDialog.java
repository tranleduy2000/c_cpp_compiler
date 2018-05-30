package com.pdaxrom.utils;

import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.view.ActionMode;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.inputmethod.InputMethodManager;
import android.widget.AbsListView;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ListAdapter;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.TextView;

import com.duy.ccppcompiler.R;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.TreeMap;

public class FileDialog extends AppCompatActivity implements AdapterView.OnItemClickListener {
    public static final String START_PATH = "START_PATH";
    public static final String FORMAT_FILTER = "FORMAT_FILTER";
    public static final String RESULT_PATH = "RESULT_PATH";
    public static final String SELECTION_MODE = "SELECTION_MODE";
    private static final String TAG = "FileDialog";
    private static final String ITEM_KEY = "key";
    private static final String ITEM_IMAGE = "image";
    private static final String ROOT = "/";
    private List<String> path = null;
    private TextView myPath;
    private EditText mFileName;
    private ArrayList<HashMap<String, Object>> mList;

    private Button selectButton;

    private LinearLayout layoutSelect;
    private LinearLayout layoutCreate;
    private InputMethodManager inputManager;
    private String parentPath;
    private String currentPath = ROOT;

    private int selectionMode = SelectionMode.MODE_CREATE;

    private String[] formatFilter = null;

    private File selectedFile;
    private HashMap<String, Integer> lastPositions = new HashMap<String, Integer>();

    private Context context = this;

    private String homeDirectory = "";
    private String sdDirectory = "";

    private ActionMode mMode = null;
    private List<String> actionFiles = null;
    private int actionOp = 0;

    private ProgressDialog progressDialog;

    /**
     * Called when the activity is first created. Configura todos os parametros
     * de entrada e das VIEWS..
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setResult(RESULT_CANCELED, getIntent());

        setContentView(R.layout.file_dialog_main);

        myPath = findViewById(R.id.path);
        myPath.setSelected(true);

        mFileName = findViewById(R.id.fdEditTextFile);

        getListView().setChoiceMode(ListView.CHOICE_MODE_SINGLE);
        getListView().setOnItemClickListener(this);

        inputManager = (InputMethodManager) getSystemService(INPUT_METHOD_SERVICE);

        selectButton = findViewById(R.id.fdButtonSelect);
        selectButton.setEnabled(false);
        selectButton.setOnClickListener(new OnClickListener() {

            public void onClick(View v) {
                if (selectedFile != null) {
                    getIntent().putExtra(RESULT_PATH, selectedFile.getPath());
                    setResult(RESULT_OK, getIntent());
                    finish();
                }
            }
        });

        selectionMode = getIntent().getIntExtra(SELECTION_MODE, SelectionMode.MODE_CREATE);

        formatFilter = getIntent().getStringArrayExtra(FORMAT_FILTER);

        layoutSelect = findViewById(R.id.fdLinearLayoutSelect);
        layoutCreate = findViewById(R.id.fdLinearLayoutCreate);

        if (selectionMode == SelectionMode.MODE_OPEN) {
            setTitle(getString(R.string.open_file));
            layoutCreate.setVisibility(View.GONE);
        } else if (selectionMode == SelectionMode.MODE_CREATE) {
            setTitle(getString(R.string.save_file));
            layoutSelect.setVisibility(View.GONE);
        } else {
            setTitle(getString(R.string.select_folder));
            layoutCreate.setVisibility(View.GONE);
        }

        final Button cancelButton = findViewById(R.id.fdButtonCancel);
        cancelButton.setOnClickListener(new OnClickListener() {

            public void onClick(View v) {
                finish();
            }

        });

        final Button cancelButton1 = findViewById(R.id.fdButtonCancel1);
        cancelButton1.setOnClickListener(new OnClickListener() {

            public void onClick(View v) {
                finish();
            }

        });


        final Button createButton = findViewById(R.id.fdButtonCreate);
        createButton.setOnClickListener(new OnClickListener() {

            public void onClick(View v) {
                if (mFileName.getText().length() > 0) {
                    getIntent().putExtra(RESULT_PATH, currentPath + "/" + mFileName.getText());
                    setResult(RESULT_OK, getIntent());
                    finish();
                }
            }
        });

        getListView().setLongClickable(true);
        getListView().setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
            public boolean onItemLongClick(AdapterView<?> av, View v, int pos, long id) {
                onLongListItemClick(v, pos, id);
                return true;
            }
        });

        String startPath = getIntent().getStringExtra(START_PATH);
        startPath = startPath != null ? startPath : ROOT;
        if (selectionMode == SelectionMode.MODE_SELECT_DIR) {
            File file = new File(startPath);
            selectedFile = file;
            selectButton.setEnabled(true);
        }

        homeDirectory = getCacheDir().getParentFile().getAbsolutePath() + "/root/cctools/home";
        sdDirectory = Environment.getExternalStorageDirectory().getPath();

        getDir(startPath);
    }

    private AbsListView getListView() {
        return findViewById(R.id.list_view);
    }

    private void getDir(String dirPath) {

        boolean useAutoSelection = dirPath.length() < currentPath.length();

        Integer position = lastPositions.get(parentPath);

        getDirImpl(dirPath);

        if (position != null && useAutoSelection) {
            getListView().setSelection(position);
        }

    }

    /**
     * Monta a estrutura de arquivos e diretorios filhos do diretorio fornecido.
     *
     * @param dirPath Diretorio pai.
     */
    private void getDirImpl(final String dirPath) {

        currentPath = dirPath;

        final List<String> item = new ArrayList<String>();
        path = new ArrayList<String>();
        mList = new ArrayList<HashMap<String, Object>>();

        File f = new File(currentPath);
        File[] files = f.listFiles();
        if (files == null) {
            currentPath = ROOT;
            f = new File(currentPath);
            files = f.listFiles();
        }
        myPath.setText(/*getText(R.string.location) + ": " + */currentPath);

        if (!currentPath.equals(ROOT)) {

            //item.add(ROOT);
            //addItem(ROOT, R.drawable.folder);
            //path.add(ROOT);

            item.add("../");
            path.add(f.getParent());
            parentPath = f.getParent();

        }

        TreeMap<String, String> dirsMap = new TreeMap<String, String>();
        TreeMap<String, String> dirsPathMap = new TreeMap<String, String>();
        TreeMap<String, String> filesMap = new TreeMap<String, String>();
        TreeMap<String, String> filesPathMap = new TreeMap<String, String>();
        for (File file : files) {
            if (file.isDirectory()) {
                String dirName = file.getName();
                dirsMap.put(dirName, dirName);
                dirsPathMap.put(dirName, file.getPath());
            } else {
                final String fileName = file.getName();
                final String fileNameLwr = fileName.toLowerCase();
                // se ha um filtro de formatos, utiliza-o
                if (formatFilter != null) {
                    boolean contains = false;
                    for (int i = 0; i < formatFilter.length; i++) {
                        final String formatLwr = formatFilter[i].toLowerCase();
                        if (fileNameLwr.endsWith(formatLwr)) {
                            contains = true;
                            break;
                        }
                    }
                    if (contains) {
                        filesMap.put(fileName, fileName);
                        filesPathMap.put(fileName, file.getPath());
                    }
                    // senao, adiciona todos os arquivos
                } else {
                    filesMap.put(fileName, fileName);
                    filesPathMap.put(fileName, file.getPath());
                }
            }
        }
        item.addAll(dirsMap.tailMap("").values());
        item.addAll(filesMap.tailMap("").values());
        path.addAll(dirsPathMap.tailMap("").values());
        path.addAll(filesPathMap.tailMap("").values());

        SimpleAdapter fileList = new SimpleAdapter(this, mList, R.layout.file_dialog_row, new String[]{
                ITEM_KEY, ITEM_IMAGE}, new int[]{R.id.fdrowtext, R.id.fdrowimage});

        for (String dir : dirsMap.tailMap("").values()) {
            addItem(dir, -1);
        }

        for (String file : filesMap.tailMap("").values()) {
            addItem(file, getFileIcon(file));
        }

        fileList.notifyDataSetChanged();

        setListAdapter(fileList);

    }

    private void setListAdapter(ListAdapter adapter) {
        getListView().setAdapter(adapter);
    }

    private void addItem(String fileName, int imageId) {
        HashMap<String, Object> item = new HashMap<String, Object>();
        item.put(ITEM_KEY, fileName);
        item.put(ITEM_IMAGE, imageId);
        mList.add(item);
    }

    protected void onLongListItemClick(View v, int pos, long id) {
    }


    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if ((keyCode == KeyEvent.KEYCODE_BACK)) {
            selectButton.setEnabled(false);

            if (!currentPath.equals(ROOT)) {
                getDir(parentPath);
            } else {
                return super.onKeyDown(keyCode, event);
            }

            return true;
        } else {
            return super.onKeyDown(keyCode, event);
        }
    }

    /**
     * Define se o botao de SELECT e visivel.
     *
     * @param v
     */
    private void hideKeyboard(View v) {
        inputManager.hideSoftInputFromWindow(v.getWindowToken(), 0);
    }

    private int getFileIcon(String file) {
      return -1;
    }

    @Override
    public void onItemClick(AdapterView<?> parent, View v, int position, long id) {

        File file = new File(path.get(position));

        hideKeyboard(v);
        selectButton.setEnabled(false);

        if (file.isDirectory()) {
            if (file.canRead()) {
                if (mMode != null) {
                    selectedFile = file;
                    mFileName.setText(file.getName());
                } else {
                    lastPositions.put(currentPath, position);
                    getDir(path.get(position));
                    if (selectionMode == SelectionMode.MODE_SELECT_DIR) {
                        selectedFile = file;
                        mFileName.setText(file.getName());
                        selectButton.setEnabled(true);
                    }
                }
            } else {
                new AlertDialog.Builder(this)
                        .setTitle("[" + file.getName() + "] " + getText(R.string.cant_read_folder))
                        .setPositiveButton("OK", new DialogInterface.OnClickListener() {

                            public void onClick(DialogInterface dialog, int which) {

                            }
                        }).show();
            }
        } else if (selectionMode != SelectionMode.MODE_SELECT_DIR) {
            selectedFile = file;
            mFileName.setText(file.getName());
            if (mMode == null) {
                selectButton.setEnabled(true);
            }
        }
    }

    private class ActionFiles extends AsyncTask<List<String>, String, Boolean> {
        protected void onPreExecute() {
            super.onPreExecute();
            progressDialog = new ProgressDialog(context);
            progressDialog.setMessage("...");
            progressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
            progressDialog.setCancelable(false);
            progressDialog.show();
        }

        protected void onProgressUpdate(String... value) {
            super.onProgressUpdate(value);
            String message = "";
            if (actionOp == R.id.file_copy) {
                message = getString(R.string.message_file_copy);

            } else if (actionOp == R.id.file_cut) {
                message = getString(R.string.message_file_move);

            } else if (actionOp == R.id.file_delete) {
                message = getString(R.string.message_file_delete);

            }
            progressDialog.setMessage(message + " " + (new File(value[0])).getName());
            progressDialog.setProgress(Integer.parseInt(value[1]));
            progressDialog.setMax(Integer.parseInt(value[2]));
        }

        protected Boolean doInBackground(List<String>... params) {
            int max = params[0].size();
            int count = 0;
            for (String file : params[0]) {
                try {
                    publishProgress(file, String.valueOf(++count), String.valueOf(max));
                    if (actionOp == R.id.file_copy || actionOp == R.id.file_cut) {
                        Log.i(TAG, "Copy " + file + " to " + currentPath);
                        if (new File(currentPath + "/" + (new File(file).getName())).getCanonicalPath()
                                .startsWith(new File(file).getCanonicalPath())) {
                            Log.e(TAG, "Cannot copy a directory, '" + file + "', into itself.");
                            continue;
                        }
                        Utils.copyDirectory(new File(file), new File(currentPath + "/" + (new File(file).getName())));
                    }
                    if (actionOp == R.id.file_cut || actionOp == R.id.file_delete) {
                        Log.i(TAG, "Delete " + file);
                        Utils.deleteDirectory(new File(file));
                    }
                } catch (IOException e) {
                    Log.e(TAG, "IOException " + e);
                    return false;
                }
            }
            return true;
        }

        protected void onPostExecute(final Boolean result) {
            actionOp = 0;
            actionFiles = null;
            if (progressDialog.isShowing()) {
                progressDialog.dismiss();
            }
            try {
                if (mMode != null) {
                    mMode.wait();
                }
            } catch (InterruptedException e) {
                Log.e(TAG, "ActionFiles onPostExecute Interrupted Exception " + e);
            }
            getDir(currentPath);
        }
    }

}
