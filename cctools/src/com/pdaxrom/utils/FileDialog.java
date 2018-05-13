package com.pdaxrom.utils;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.TreeMap;

import com.actionbarsherlock.app.SherlockListActivity;
import com.actionbarsherlock.view.ActionMode;
import com.actionbarsherlock.view.Menu;
import com.actionbarsherlock.view.MenuItem;
import com.pdaxrom.cctools.R;

import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.text.InputType;
import android.util.Log;
import android.util.SparseBooleanArray;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.inputmethod.InputMethodManager;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.ListView;
import android.widget.SimpleAdapter;
import android.widget.TextView;

public class FileDialog extends SherlockListActivity {
	private static final String TAG				= "FileDialog";
	
	private static final String ITEM_KEY		= "key";
	private static final String ITEM_IMAGE		= "image";

	private static final String ROOT			= "/";

	public static final String START_PATH		= "START_PATH";
	public static final String FORMAT_FILTER	= "FORMAT_FILTER";
	public static final String RESULT_PATH		= "RESULT_PATH";
	public static final String SELECTION_MODE	= "SELECTION_MODE";

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

		myPath = (TextView) findViewById(R.id.path);
		myPath.setSelected(true);
		//myPath.setEllipsize(TruncateAt.MARQUEE);
		//myPath.setSingleLine(true);
		
		mFileName = (EditText) findViewById(R.id.fdEditTextFile);

		if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB) {
			getListView().setChoiceMode(ListView.CHOICE_MODE_SINGLE);
		}
		
		inputManager = (InputMethodManager) getSystemService(INPUT_METHOD_SERVICE);

		selectButton = (Button) findViewById(R.id.fdButtonSelect);
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

		layoutSelect = (LinearLayout) findViewById(R.id.fdLinearLayoutSelect);
		layoutCreate = (LinearLayout) findViewById(R.id.fdLinearLayoutCreate);

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

		final Button cancelButton = (Button) findViewById(R.id.fdButtonCancel);
		cancelButton.setOnClickListener(new OnClickListener() {

			public void onClick(View v) {
				finish();
			}

		});

		final Button cancelButton1 = (Button) findViewById(R.id.fdButtonCancel1);
		cancelButton1.setOnClickListener(new OnClickListener() {

			public void onClick(View v) {
				finish();
			}

		});

		
		final Button createButton = (Button) findViewById(R.id.fdButtonCreate);
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

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
    	getSupportMenuInflater().inflate(R.menu.file_dialog_menu, menu);
    	return true;
    }
    
    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        case R.id.home_folder:
        	if (currentPath.startsWith(Environment.getExternalStorageDirectory().getPath())) {
        		sdDirectory = currentPath;
        	}
        	if (!currentPath.startsWith(getCacheDir().getParentFile().getAbsolutePath())) {
        		getDir(homeDirectory);
        		if (selectionMode == SelectionMode.MODE_SELECT_DIR) {
        			selectedFile = new File(currentPath); 
        			mFileName.setText(selectedFile.getName());
        			selectButton.setEnabled(true);
        		}
        	}
        	break;
        case R.id.sd_folder:
        	if (currentPath.startsWith(getCacheDir().getParentFile().getAbsolutePath())) {
        		homeDirectory = currentPath;
        	}
        	if (!currentPath.startsWith(Environment.getExternalStorageDirectory().getPath())) {
            	getDir(sdDirectory);
        		if (selectionMode == SelectionMode.MODE_SELECT_DIR) {
        			selectedFile = new File(currentPath); 
        			mFileName.setText(selectedFile.getName());
        			selectButton.setEnabled(true);
        		}
        	}
        	break;
        case R.id.new_folder:
        	newDir();
        	break;
        }
        return true;
    }

    private void newDir() {
		final EditText input = new EditText(context);
		input.setInputType(InputType.TYPE_CLASS_TEXT);
		input.setSingleLine(true);
		new AlertDialog.Builder(context)
			.setMessage(getString(R.string.create_new))
			.setView(input)
		    .setPositiveButton(getString(R.string.button_continue), new DialogInterface.OnClickListener() {
		        public void onClick(DialogInterface dialog, int whichButton) {
		        	if ((new File(currentPath + "/" + input.getText().toString())).mkdirs() == false) {
		    	    	new AlertDialog.Builder(context)
		    	    	.setIcon(android.R.drawable.ic_dialog_alert)
		    	    	.setTitle(R.string.newDirectory)
		    	    	.setMessage(getString(R.string.cannot_create))
		    	    	.show();
		        	} else {
		        		getDir(currentPath + "/" + input.getText().toString());
		        		if (selectionMode == SelectionMode.MODE_SELECT_DIR) {
		        			selectedFile = new File(currentPath); 
		        			mFileName.setText(selectedFile.getName());
		        			selectButton.setEnabled(true);
		        		}
		        	}
		        }
		    })
		    .setNegativeButton(getString(R.string.button_cancel), new DialogInterface.OnClickListener() {
				
				public void onClick(DialogInterface dialog, int which) {
					// TODO Auto-generated method stub
					
				}
			}).show();
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
	 * @param dirPath
	 *            Diretorio pai.
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
			addItem("..", R.drawable.folder);
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

		SimpleAdapter fileList = new SimpleAdapter(this, mList, R.layout.file_dialog_row, new String[] {
				ITEM_KEY, ITEM_IMAGE }, new int[] { R.id.fdrowtext, R.id.fdrowimage });

		for (String dir : dirsMap.tailMap("").values()) {
			addItem(dir, R.drawable.folder);
		}

		for (String file : filesMap.tailMap("").values()) {
			addItem(file, getFileIcon(file));
		}

		fileList.notifyDataSetChanged();

		setListAdapter(fileList);

	}

	private void addItem(String fileName, int imageId) {
		HashMap<String, Object> item = new HashMap<String, Object>();
		item.put(ITEM_KEY, fileName);
		item.put(ITEM_IMAGE, imageId);
		mList.add(item);
	}

	protected void onLongListItemClick(View v, int pos, long id) {
	    Log.i(TAG, "onLongListItemClick id=" + id);
	    mMode = startActionMode(new FileActionMode());
	}
	
	@Override
	protected void onListItemClick(ListView l, View v, int position, long id) {

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
				new AlertDialog.Builder(this).setIcon(R.drawable.ic_launcher)
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
		String ext = "";
		int dot = file.lastIndexOf(".");
		if (dot != -1)
			ext = file.substring(dot);
		if (ext.contentEquals(".c"))
			return R.drawable.text_x_c;
		if (ext.contentEquals(".cpp") || ext.contentEquals(".c++"))
			return R.drawable.text_x_cpp;
		if (ext.contentEquals(".h") || ext.contentEquals(".hpp"))
			return R.drawable.text_x_h;
		if (ext.contentEquals(".sh"))
			return R.drawable.text_x_script;
		if (ext.contentEquals(".mk") || ext.contentEquals(".mak") || file.contentEquals("Makefile") || file.contentEquals("makefile"))
			return R.drawable.text_x_make;
		return R.drawable.application_octet_stream;
	}
	
	private final class FileActionMode implements ActionMode.Callback {
		private List<String> getSelectedFiles() {
			List<String> files = new ArrayList<String>();
			SparseBooleanArray checked = getListView().getCheckedItemPositions();
			
			for (int i = 0; i < checked.size(); i++) {
				if (checked.valueAt(i)) {
					HashMap <String, Object> item = (HashMap<String, Object>) getListView().getAdapter().getItem(checked.keyAt(i));
					if (((String) item.get(ITEM_KEY)).equals("..")) {
						continue;
					}
					files.add(currentPath + "/" + (String) item.get(ITEM_KEY));
				}
			}
			
			return files;
		}
		
		public boolean onActionItemClicked(ActionMode mode, MenuItem item) {
			switch(item.getItemId()) {
			case R.id.file_delete:
				List<String>files = getSelectedFiles();
				if (files.size() > 0) {
					actionOp = item.getItemId();
					new ActionFiles().execute(files);
				}
				break;
			case R.id.file_copy:
			case R.id.file_cut:
				actionFiles = getSelectedFiles();
				if (actionFiles.size() > 0) {
					actionOp = item.getItemId();
				}
				break;
			case R.id.file_paste:
				if (actionOp == R.id.file_copy || actionOp == R.id.file_cut) {
					new ActionFiles().execute(actionFiles);
				}
				break;
			}
			mode.finish();
			return true;
		}

		public boolean onCreateActionMode(ActionMode mode, Menu menu) {
			getSupportMenuInflater().inflate(R.menu.file_action_menu, menu);
			if (actionOp == 0) {
				menu.findItem(R.id.file_paste).setVisible(false);
			}
			return true;
		}

		public void onDestroyActionMode(ActionMode mode) {
		    getListView().setChoiceMode(ListView.CHOICE_MODE_SINGLE);
		    getDir(currentPath);
		    mMode = null;
		}

		public boolean onPrepareActionMode(ActionMode mode, Menu menu) {
		    getListView().setChoiceMode(ListView.CHOICE_MODE_MULTIPLE);
			return false;
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
    		switch (actionOp) {
    		case R.id.file_copy:
    			message = getString(R.string.message_file_copy);
    			break;
    		case R.id.file_cut:
    			message = getString(R.string.message_file_move);
    			break;
    		case R.id.file_delete:
    			message = getString(R.string.message_file_delete);
    			break;
    		}
    		progressDialog.setMessage(message + " " + (new File(value[0])).getName());
    		progressDialog.setProgress(Integer.parseInt(value[1]));
    		progressDialog.setMax(Integer.parseInt(value[2]));
    	}
    	
		protected Boolean doInBackground(List<String>... params) {
			int max = params[0].size();
			int count = 0;
			for (String file: params[0]) {
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
