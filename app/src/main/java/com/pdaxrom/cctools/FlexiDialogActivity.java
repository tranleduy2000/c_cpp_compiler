package com.pdaxrom.cctools;

import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.text.InputType;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.TableLayout;
import android.widget.TableRow;
import android.widget.TextView;

import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.pkgmanager.Environment;
import com.pdaxrom.utils.FileDialog;
import com.pdaxrom.utils.SelectionMode;
import com.pdaxrom.utils.XMLParser;

import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

public abstract class FlexiDialogActivity extends AppCompatActivity {
    protected static final String PKGS_LISTS_DIR = "/installed/";
    private final static String TAG = "FlexiDialog";
    private final static int REQUEST_DIALOG_FILE_SELECTOR = 1000;
    private final static int MAX_REQUESTS_FROM_FILE_DIALOG = 20;
    List<NamedView> namedViews = null;
    private Context context = this;
    private int fileSelectorId;

    private String toolchainDir;
    private String sdHomeDir;
    private String serviceDir;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        sdHomeDir = Environment.getSdCardHomeDir();
        toolchainDir = Environment.getToolchainsDir(this);
        serviceDir = Environment.getServiceDir(this);
        Environment.getSdCardTmpDir();
        Environment.getSdCardBackupDir();
        Environment.getDalvikCacheDir(context);
        updateClassPathEnv();
    }

    protected String getToolchainDir() {
        return toolchainDir;
    }

    protected String getSDHomeDir() {
        return sdHomeDir;
    }

    protected String getServiceDir() {
        return serviceDir;
    }

    private View getNamedView(List<NamedView> views, String name) {
        for (NamedView view : views) {
            if (view.getName().equals(name)) {
                return view.getView();
            }
        }
        return null;
    }

    private View getNamedView(List<NamedView> views, int id) {
        for (NamedView view : views) {
            if (view.getId() == id) {
                return view.getView();
            }
        }
        return null;
    }

    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (resultCode == RESULT_OK) {
            if (requestCode >= REQUEST_DIALOG_FILE_SELECTOR &&
                    requestCode < REQUEST_DIALOG_FILE_SELECTOR +
                            MAX_REQUESTS_FROM_FILE_DIALOG) {
                EditText edit = (EditText) getNamedView(namedViews,
                        requestCode - REQUEST_DIALOG_FILE_SELECTOR);
                if (edit != null) {
                    edit.setText(data.getStringExtra(FileDialog.RESULT_PATH));
                }
            }
        }

    }

    /**
     * Create Dialog popup window from xml template
     *
     * @param ruleFile      xml template file
     * @param lastOpenedDir last opened directory
     */
    protected void dialogFromRule(String ruleFile, final String lastOpenedDir) {
        XMLParser xmlParser = new XMLParser();
        String xml = xmlParser.getXmlFromFile(ruleFile);
        if (xml != null) {
            Document doc = xmlParser.getDomElement(xml);
            if (doc != null) {
                NodeList nl = doc.getElementsByTagName("new");
                Element e = (Element) nl.item(0);
                String title = e.getAttribute("title");
                nl = e.getElementsByTagName("view");

                namedViews = new ArrayList<NamedView>();

                TableLayout table = new TableLayout(context);
                table.setColumnStretchable(1, true);

                fileSelectorId = 0;
                for (int i = 0; i < nl.getLength(); i++) {
                    Element ne = (Element) nl.item(i);
                    Log.i(TAG, "-- " + ne.getTagName());
                    Log.i(TAG, "--- " + ne.getAttribute("type"));
                    Log.i(TAG, "--- " + ne.getAttribute("title"));
                    Log.i(TAG, "--- " + ne.getAttribute("name"));
                    if (ne.getAttribute("type").equals("edit")) {
                        TableRow row = new TableRow(context);

                        TextView view = new TextView(context);
                        view.setText(ne.getAttribute("title"));

                        EditText edit = new EditText(context);
                        edit.setInputType(edit.getInputType() & ~InputType.TYPE_TEXT_FLAG_MULTI_LINE);
                        edit.setHint(ne.getAttribute("hint"));

                        namedViews.add(new NamedView(edit, ne.getAttribute("name")));

                        row.addView(view);
                        row.addView(edit);
                        table.addView(row);
                    } else if (ne.getAttribute("type").equals("dirpath") || ne.getAttribute("type").equals("filepath")) {
                        TableRow row = new TableRow(context);

                        TextView view = new TextView(context);
                        view.setText(ne.getAttribute("title"));

                        EditText edit = new EditText(context);
                        edit.setInputType(edit.getInputType() & ~InputType.TYPE_TEXT_FLAG_MULTI_LINE);
                        edit.setHint(ne.getAttribute("hint"));

                        namedViews.add(new NamedView(edit, ne.getAttribute("name"), fileSelectorId));

                        ImageButton button = new ImageButton(context);

                        if (ne.getAttribute("type").equals("dirpath")) {
                            button.setOnClickListener(new OnClickListener() {
                                private int id = fileSelectorId++;

                                public void onClick(View v) {
                                    Intent intent = new Intent(getBaseContext(), FileDialog.class);
                                    String dir = lastOpenedDir;
                                    if (dir == null || !new File(dir).exists()) {
                                        dir = android.os.Environment.getExternalStorageDirectory().getPath();
                                    }
                                    intent.putExtra(FileDialog.START_PATH, dir);
                                    intent.putExtra(FileDialog.SELECTION_MODE, SelectionMode.MODE_SELECT_DIR);
                                    startActivityForResult(intent, REQUEST_DIALOG_FILE_SELECTOR + id);

                                }
                            });
                        } else {
                            button.setOnClickListener(new OnClickListener() {
                                private int id = fileSelectorId++;

                                public void onClick(View v) {
                                    Intent intent = new Intent(getBaseContext(), FileDialog.class);
                                    String dir = lastOpenedDir;
                                    if (dir == null || !new File(dir).exists()) {
                                        dir = android.os.Environment.getExternalStorageDirectory().getPath();
                                    }
                                    intent.putExtra(FileDialog.START_PATH, dir);
                                    intent.putExtra(FileDialog.SELECTION_MODE, SelectionMode.MODE_OPEN);
                                    startActivityForResult(intent, REQUEST_DIALOG_FILE_SELECTOR + id);

                                }
                            });
                        }

                        row.addView(view);
                        row.addView(edit);
                        row.addView(button);
                        table.addView(row);
                    }
                }

                nl = e.getElementsByTagName("command");
                e = (Element) nl.item(0);
                final String execAttr = e.getAttribute("exec");

                new AlertDialog.Builder(context)
                        .setTitle(title)
                        .setView(table)
                        .setPositiveButton(getText(R.string.button_continue), new DialogInterface.OnClickListener() {
                            private String exec = execAttr;

                            public void onClick(DialogInterface dialog, int which) {
                                exec.replaceAll("\\s+", " ");
                                String[] argv = exec.split("\\s+");
                                for (int i = 0; i < argv.length; i++) {
                                    if (argv[i].startsWith("@") && argv[i].endsWith("@")) {
                                        String var = argv[i].substring(1, argv[i].length() - 1);
                                        EditText edit = (EditText) getNamedView(namedViews, var);
                                        if (edit != null) {
                                            argv[i] = edit.getText().toString();
                                        }
                                    }

                                    Log.i(TAG, ":: " + argv[i]);
                                }
                                system(argv, false);
                                namedViews = null;
                            }
                        })
                        .setCancelable(true)
                        .show();

            }
        }
    }

    /**
     * Execute a shell command
     */
    protected void system(String[] argv) {
        system(argv, true);
    }

    /**
     * Execute a shell command
     */
    protected void system(String[] cmdarray, boolean waitForFinish) {
        try {
            String[] envp = Environment.buildDefaultEnv(this);
            Process p = Runtime.getRuntime().exec(cmdarray, envp);
            if (waitForFinish) {
                BufferedReader in = new BufferedReader(
                        new InputStreamReader(p.getErrorStream()));
                String line = null;
                while ((line = in.readLine()) != null) {
                    Log.i(TAG, "stderr: " + line);
                }
                p.waitFor();
            }
        } catch (Exception e) {
            Log.i(TAG, "Exec exception " + e);
        }
    }

    protected void updateClassPathEnv() {
        String cpEnvDir = toolchainDir + "/cctools/etc";
        if (!(new File(cpEnvDir)).exists()) {
            (new File(cpEnvDir)).mkdirs();
        }
        try {
            String env = "CCTOOLSCP=" + getPackageResourcePath() + "\n";
            FileOutputStream outf = new FileOutputStream(cpEnvDir + "/cp.env");
            outf.write(env.getBytes());
            outf.close();
        } catch (IOException e) {
            Log.e(TAG, "create cp.env " + e);
        }
    }

    public class NamedView {
        private View view;
        private String name;
        private int id;

        NamedView(View view, String name) {
            this.view = view;
            this.name = name;
            this.id = -1;
        }

        NamedView(View view, String name, int id) {
            this.view = view;
            this.name = name;
            this.id = id;
        }

        public View getView() {
            return view;
        }

        public String getName() {
            return name;
        }

        public int getId() {
            return id;
        }
    }
}