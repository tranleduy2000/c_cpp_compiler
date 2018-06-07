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

package com.duy.ccppcompiler.ide.editor;

import android.content.DialogInterface;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.text.Editable;
import android.view.Menu;
import android.view.MenuItem;
import android.view.SubMenu;
import android.widget.Toast;

import com.duy.ccppcompiler.R;
import com.duy.ccppcompiler.compiler.CompilerSettingActivity;
import com.duy.ccppcompiler.compiler.analyze.CppCheckAnalyzer;
import com.duy.ccppcompiler.compiler.analyze.CppCheckOutputParser;
import com.duy.ccppcompiler.compiler.compilers.CompilerFactory;
import com.duy.ccppcompiler.compiler.compilers.ICompiler;
import com.duy.ccppcompiler.compiler.compilers.NativeCompileImpl;
import com.duy.ccppcompiler.compiler.manager.CompileManager;
import com.duy.ccppcompiler.console.TermActivity;
import com.duy.ccppcompiler.ide.diagnostic.GccOutputParser;
import com.duy.ccppcompiler.ide.dialogs.CompilerOptionsDialog;
import com.duy.ccppcompiler.ide.dialogs.PremiumDialog;
import com.duy.ccppcompiler.ide.editor.theme.ThemeActivity;
import com.duy.ccppcompiler.ide.examples.ExampleActivity;
import com.duy.ccppcompiler.packagemanager.Environment;
import com.duy.ccppcompiler.packagemanager.PackageManagerActivity;
import com.duy.common.StoreUtil;
import com.duy.common.function.Action;
import com.duy.common.purchase.InAppPurchaseHelper;
import com.duy.common.purchase.Premium;
import com.duy.file.explorer.FileExplorerActivity;
import com.duy.ide.core.IdeActivity;
import com.duy.ide.diagnostic.DiagnosticPresenter;
import com.duy.ide.diagnostic.parser.PatternAwareOutputParser;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.editor.EditorDelegate;
import com.jecelyin.editor.v2.editor.IEditorDelegate;
import com.jecelyin.editor.v2.widget.menu.MenuDef;
import com.kobakei.ratethisapp.RateThisApp;
import com.pdaxrom.cctools.BuildConstants;
import com.sothree.slidinguppanel.SlidingUpPanelLayout;

import java.io.File;
import java.util.regex.Pattern;

import jackpal.androidterm.TermPreferencesActivity;

import static com.pdaxrom.cctools.BuildConstants.EXTRA_FILE_NAME;

/**
 * Created by Duy on 19-May-18.
 */

public class CppIdeActivity extends IdeActivity {
    private static final String TAG = "CodeEditorActivity";
    private static final int RC_BUILD = 1234;
    private static final int RC_SELECT_NATIVE_ACTIVITY = 1237;

    private InAppPurchaseHelper mPremiumHelper;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mPremiumHelper = new InAppPurchaseHelper(this);
        // Monitor launch times and interval from installation
        RateThisApp.onCreate(this);
    }

    @Override
    protected void populateDiagnostic(@NonNull DiagnosticPresenter diagnosticPresenter) {
        PatternAwareOutputParser[] parsers = {
                new CppCheckOutputParser(),
                new GccOutputParser()
        };
        diagnosticPresenter.setOutputParser(parsers);
    }

    @Override
    public void onEditorViewCreated(IEditorDelegate editorDelegate) {
        super.onEditorViewCreated(editorDelegate);

        Preferences setting = Preferences.getInstance(this);
        boolean enableStaticCodeAnalysis = setting.getBoolean(getString(R.string.pref_key_cpp_check), true);
        if (enableStaticCodeAnalysis) {
            File file = editorDelegate.getDocument().getFile();
            String fileName = file.getName().toLowerCase();
            if (fileName.endsWith(".cpp") || fileName.endsWith(".c")) {
                CppCheckAnalyzer analyzer = new CppCheckAnalyzer(this, editorDelegate,
                        mDiagnosticPresenter);
                analyzer.start();
            }
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu container) {
        //add run button
        container.add(MenuDef.GROUP_TOOLBAR, R.id.action_run, 0, R.string.run)
                .setIcon(R.drawable.ic_play_arrow_white_24dp)
                .setShowAsAction(MenuItem.SHOW_AS_ACTION_ALWAYS);
        return super.onCreateOptionsMenu(container);
    }

    @Override
    protected void onCreateNavigationMenu(Menu menu) {
        //add run button
        if (!Premium.isPremiumUser(this)) {
            menu.add(MenuDef.GROUP_NAVIGATION, R.id.action_premium, 0, R.string.title_premium_version)
                    .setIcon(R.drawable.baseline_lock_open_24);
        }
        menu.add(MenuDef.GROUP_NAVIGATION, R.id.action_editor_color_scheme, 0, R.string.editor_theme)
                .setIcon(R.drawable.ic_color_lens_white_24dp);
        {
            SubMenu buildMenu = menu.addSubMenu(R.string.build);
            buildMenu.add(MenuDef.GROUP_NAVIGATION, R.id.action_run_sdl_activity, 0, R.string.run_sdl_activity)
                    .setIcon(R.drawable.ic_play_arrow_white_24dp);
        }

        {
            SubMenu codeMenu = menu.addSubMenu(R.string.code);
            codeMenu.add(MenuDef.GROUP_NAVIGATION, R.id.action_c_example, 0, R.string.title_menu_c_example)
                    .setIcon(R.drawable.ic_code_black_24dp);
            codeMenu.add(MenuDef.GROUP_NAVIGATION, R.id.action_cpp_example, 0, R.string.title_menu_cpp_example)
                    .setIcon(R.drawable.ic_code_black_24dp);
            codeMenu.add(MenuDef.GROUP_NAVIGATION, R.id.action_open_terminal, 0, R.string.title_menu_terminal)
                    .setIcon(R.drawable.ic_terminal_black);
            codeMenu.add(MenuDef.GROUP_NAVIGATION, R.id.action_install_libraries, 0, R.string.title_menu_install_libraries)
                    .setIcon(R.drawable.baseline_extension_24);
            codeMenu.add(MenuDef.GROUP_NAVIGATION, R.id.action_report_bug, 0, R.string.report_bug)
                    .setIcon(R.drawable.baseline_bug_report_24);
        }

        {
            SubMenu settingMenu = menu.addSubMenu(R.string.settings);
            settingMenu.add(MenuDef.GROUP_NAVIGATION, R.id.action_term_preferences, 0, R.string.title_term_preferences)
                    .setIcon(R.drawable.ic_settings_white_24dp);
            settingMenu.add(MenuDef.GROUP_NAVIGATION, R.id.action_compiler_setting, 0, R.string.compiler_setting)
                    .setIcon(R.drawable.ic_settings_white_24dp);
        }
        super.onCreateNavigationMenu(menu);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case R.id.action_install_libraries:
                startActivity(new Intent(this, PackageManagerActivity.class));
                return true;

            case R.id.action_open_terminal:
                openTerminal();
                return true;

            case R.id.action_term_preferences:
                startActivity(new Intent(this, TermPreferencesActivity.class));
                break;

            case R.id.action_compiler_setting:
                startActivity(new Intent(this, CompilerSettingActivity.class));
                break;

            case R.id.action_cpp_example:
                ExampleActivity.openExample(this, "cpp");
                break;
            case R.id.action_c_example:
                ExampleActivity.openExample(this, "c");
                break;

            case R.id.action_premium:
                clickUpgrade();
                break;

            case R.id.action_editor_color_scheme:
                startActivity(new Intent(this, ThemeActivity.class));
                break;

            case R.id.action_run:
                saveAll(RC_BUILD);
                return true;

            case R.id.action_build_native_activity:
                saveAll(RC_BUILD);
                break;

            case R.id.action_run_sdl_activity:
                selectSDLFile();
                break;

            case R.id.action_report_bug:
                StoreUtil.openBrowser(this, "https://github.com/tranleduy2000/c_cpp_compiler/issues", 0);
                break;
        }
        return super.onOptionsItemSelected(item);
    }

    private void selectSDLFile() {
        EditorDelegate currentEditorDelegate = getCurrentEditorDelegate();
        String path = null;
        if (currentEditorDelegate != null) {
            path = currentEditorDelegate.getPath();
        }
        FileExplorerActivity.startPickFileActivity(this, path, Environment.getSdCardHomeDir(),
                RC_SELECT_NATIVE_ACTIVITY);

    }

    @Override
    protected void onSaveComplete(int requestCode) {
        super.onSaveComplete(requestCode);

        switch (requestCode) {
            case RC_BUILD:
                build(detectBuildType());
                break;
        }
    }

    private int detectBuildType() {
        EditorDelegate currentEditor = getCurrentEditorDelegate();
        if (currentEditor == null) {
            return NativeCompileImpl.BUILD_EXECUTABLE;
        }

        Editable srcCode = currentEditor.getEditText().getText();
        Pattern nativeActivityPattern =
                //#include "android_native_app_glue.h" in C++
                //#inlcude <android_native_app_glue.h> in C
                Pattern.compile("#include\\s+([<\"])android_native_app_glue.h[>\"]");
        if (nativeActivityPattern.matcher(srcCode).find()) {
            return NativeCompileImpl.BUILD_NATIVE_ACTIVITY;
        }
        Pattern sdl =
                //#include "android_native_app_glue.h" in C++
                //#inlcude <android_native_app_glue.h> in C
                Pattern.compile("#include\\s+([<\"])SDL.h[>\"]");
        if (sdl.matcher(srcCode).find()) {
            return NativeCompileImpl.BUILD_SDL_ACTIVITY;
        }

        return NativeCompileImpl.BUILD_EXECUTABLE;
    }

    private void build(final int buildType) {
        final Action<CppIdeActivity> compileAction = new Action<CppIdeActivity>() {
            @Override
            public void execute(@Nullable CppIdeActivity activity) {
                EditorDelegate currentEditor = getCurrentEditorDelegate();
                if (currentEditor == null) {
                    return;
                }
                File[] srcFiles = new File[]{new File(currentEditor.getPath())};

                ICompiler compiler = CompilerFactory.getCompilerForFile(activity, srcFiles, buildType);
                if (compiler != null) {
                    CompileManager compileManager = new CompileManager(activity);
                    compileManager.setDiagnosticPresenter(mDiagnosticPresenter);
                    compileManager.setCompiler(compiler);
                    compileManager.compile(srcFiles);
                } else {
                    Toast.makeText(activity, R.string.unknown_filetype, Toast.LENGTH_SHORT).show();
                }
            }
        };

        switch (buildType) {
            case NativeCompileImpl.BUILD_EXECUTABLE:
                compileAction.execute(this);
                break;
            case NativeCompileImpl.BUILD_SDL_ACTIVITY: {
                CompilerOptionsDialog dialog = new CompilerOptionsDialog(this,
                        R.string.build_sdl,
                        new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                compileAction.execute(CppIdeActivity.this);
                            }
                        });
                dialog.show();
                break;
            }
            case NativeCompileImpl.BUILD_NATIVE_ACTIVITY: {
                CompilerOptionsDialog dialog = new CompilerOptionsDialog(this,
                        R.string.build_native_activity,
                        new DialogInterface.OnClickListener() {
                            @Override
                            public void onClick(DialogInterface dialog, int which) {
                                compileAction.execute(CppIdeActivity.this);
                            }
                        });
                dialog.show();
                break;
            }
        }
    }

    @Override
    protected String[] getSupportedFileExtensions() {
        String[] defaultExts = super.getSupportedFileExtensions();
        String[] supportedExts = getResources().getStringArray(R.array.file_extensions);
        String[] finalResult = new String[defaultExts.length + supportedExts.length];
        System.arraycopy(supportedExts, 0, finalResult, 0, supportedExts.length);
        System.arraycopy(defaultExts, 0, finalResult, supportedExts.length, defaultExts.length);
        return finalResult;
    }

    private void openTerminal() {
        EditorDelegate currentEditorDelegate = getCurrentEditorDelegate();
        String workDir = null;
        if (currentEditorDelegate != null) {
            workDir = new File(currentEditorDelegate.getPath()).getParent();
        }
        if (workDir == null) {
            workDir = Environment.getHomeDir(this);
        }
        Intent intent = new Intent(this, TermActivity.class);
        intent.putExtra(EXTRA_FILE_NAME, "-" + Environment.getShell(this));
        intent.putExtra(BuildConstants.EXTRA_WORK_DIR, workDir);
        startActivity(intent);
    }

    private void clickUpgrade() {
        PremiumDialog dialog = new PremiumDialog(this, mPremiumHelper);
        dialog.show();
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == ExampleActivity.RC_OPEN_EXAMPLE) {
            if (resultCode == RESULT_OK) {
                final String path = data.getStringExtra(Intent.EXTRA_RETURN_RESULT);
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        openFile(path, "UTF-8", 0);
                    }
                });
            }
        } else if (requestCode == RC_SELECT_NATIVE_ACTIVITY) {
            if (resultCode == RESULT_OK) {
                String path = FileExplorerActivity.getFile(data);
                if (path == null) {
                    return;
                }
                Intent intent = getPackageManager().getLaunchIntentForPackage(BuildConstants.SDL_PACKAGE_NAME);
                if (intent != null) {
                    intent.putExtra("sdlmain", path);
                    startActivity(intent);//null pointer check in case package name was not found
                } else {
                    UIUtils.alert(this, "Can not run SDL application");
                }
            }
        }
    }

    @Override
    public void onBackPressed() {
        if (closeDrawers()) {
            return;
        }
        if (mSlidingUpPanelLayout != null) {
            SlidingUpPanelLayout.PanelState state = mSlidingUpPanelLayout.getPanelState();
            if (state == SlidingUpPanelLayout.PanelState.EXPANDED
                    || state == SlidingUpPanelLayout.PanelState.DRAGGING) {
                mSlidingUpPanelLayout.setPanelState(SlidingUpPanelLayout.PanelState.COLLAPSED);
                return;
            }
        }


        // If the condition is satisfied, "Rate this app" dialog will be shown
        if (!RateThisApp.showRateDialogIfNeeded(this)) {
            super.onBackPressed();
        }
    }

    @Override
    protected void onDestroy() {
        mPremiumHelper.destroy();
        super.onDestroy();
    }
}
