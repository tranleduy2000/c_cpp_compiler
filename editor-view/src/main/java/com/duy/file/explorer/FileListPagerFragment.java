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

package com.duy.file.explorer;

import android.content.Context;
import android.content.SharedPreferences;
import android.databinding.DataBindingUtil;
import android.os.Bundle;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v4.app.Fragment;
import android.support.v4.widget.SwipeRefreshLayout;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.view.ActionMode;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.text.Editable;
import android.text.TextWatcher;
import android.view.LayoutInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

import com.duy.ide.editor.editor.R;
import com.duy.ide.editor.editor.databinding.FileExplorerFragmentBinding;
import com.duy.file.explorer.adapter.FileListItemAdapter;
import com.duy.file.explorer.adapter.PathButtonAdapter;
import com.duy.file.explorer.io.JecFile;
import com.duy.file.explorer.listener.FileListResultListener;
import com.duy.file.explorer.listener.OnClipboardPasteFinishListener;
import com.duy.file.explorer.util.FileListSorter;
import com.jecelyin.common.listeners.OnItemClickListener;
import com.jecelyin.common.task.JecAsyncTask;
import com.jecelyin.common.task.TaskListener;
import com.jecelyin.common.task.TaskResult;
import com.jecelyin.common.utils.UIUtils;
import com.jecelyin.editor.v2.Preferences;
import com.yqritc.recyclerviewflexibledivider.HorizontalDividerItemDecoration;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;


/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class FileListPagerFragment extends Fragment implements SwipeRefreshLayout.OnRefreshListener, OnItemClickListener, FileExplorerView, ExplorerContext, SharedPreferences.OnSharedPreferenceChangeListener {
    private static final String EXTRA_PATH = "path";

    private FileListItemAdapter adapter;
    private JecFile path;
    private FileExplorerFragmentBinding binding;
    private PathButtonAdapter pathAdapter;
    private ScanFilesTask task;
    private FileExplorerAction action;

    public static FileListPagerFragment newFragment(JecFile path) {
        FileListPagerFragment f = new FileListPagerFragment();
        Bundle b = new Bundle();
        b.putParcelable(EXTRA_PATH, path);
        f.setArguments(b);
        return f;
    }

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, ViewGroup container, Bundle savedInstanceState) {
        path = getArguments().getParcelable(EXTRA_PATH);
        binding = DataBindingUtil.inflate(inflater, R.layout.file_explorer_fragment, container, false);
        return binding.getRoot();
    }

    @Override
    public void onViewCreated(@NonNull View view, @Nullable Bundle savedInstanceState) {
        action = new FileExplorerAction(getContext(), this, ((FileExplorerActivity) getActivity()).getFileClipboard(), this);
        adapter = new FileListItemAdapter();
        adapter.setOnCheckedChangeListener(action);
        adapter.setOnItemClickListener(this);
        adapter.registerAdapterDataObserver(new RecyclerView.AdapterDataObserver() {
            @Override
            public void onChanged() {
                binding.emptyLayout.post(new Runnable() {
                    @Override
                    public void run() {
                        binding.emptyLayout.setVisibility(adapter.getItemCount() > 0 ? View.GONE : View.VISIBLE);
                    }
                });

            }
        });

        binding.pathScrollView.setLayoutManager(new LinearLayoutManager(getContext(), LinearLayoutManager.HORIZONTAL, false));
        pathAdapter = new PathButtonAdapter();
        pathAdapter.registerAdapterDataObserver(new RecyclerView.AdapterDataObserver() {
            @Override
            public void onChanged() {
                binding.pathScrollView.scrollToPosition(pathAdapter.getItemCount() - 1);
            }
        });
        pathAdapter.setPath(path);
        pathAdapter.setOnItemClickListener(new OnItemClickListener() {
            @Override
            public void onItemClick(int position, View view) {
                JecFile file = pathAdapter.getItem(position);
                switchToPath(file);
            }
        });
        binding.pathScrollView.setAdapter(pathAdapter);

        binding.explorerSwipeRefreshLayout.setOnRefreshListener(this);
        binding.recyclerView.setLayoutManager(new LinearLayoutManager(getContext()));
        binding.recyclerView.setAdapter(adapter);
        binding.recyclerView.addItemDecoration(new HorizontalDividerItemDecoration.Builder(getContext()).margin(getResources().getDimensionPixelSize(R.dimen.file_list_item_divider_left_margin), 0).build());
        binding.explorerSwipeRefreshLayout.post(new Runnable() {
            @Override
            public void run() {
                binding.explorerSwipeRefreshLayout.setRefreshing(true);
            }
        });
        binding.nameFilterEditText.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {
                adapter.filter(s);
            }

            @Override
            public void afterTextChanged(Editable s) {

            }
        });

        Preferences.getInstance(getContext()).registerOnSharedPreferenceChangeListener(this);

        view.post(new Runnable() {
            @Override
            public void run() {
                onRefresh();
            }
        });
    }

    @Override
    public void onDestroyView() {
        super.onDestroyView();
        Preferences.getInstance(getContext()).unregisterOnSharedPreferenceChangeListener(this);
        if (action != null) {
            action.destroy();
        }

    }

    @Override
    public void onStop() {
        super.onStop();
        // 不能加在onPause，因为请求Root UI会导致pause
        if (task != null) {
            task.cancel(true);
            task = null;
        }
    }

    @Override
    public void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setHasOptionsMenu(true);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        if (item.getItemId() == R.id.paste_menu) {
            final FileClipboard fileClipboard = ((FileExplorerActivity) getActivity()).getFileClipboard();
            fileClipboard.paste(getContext(), getCurrentDirectory(), new OnClipboardPasteFinishListener() {
                @Override
                public void onFinish(int count, String error) {
                    onRefresh();
                    fileClipboard.showPasteResult(getContext(), count, error);
                }
            });
            item.setVisible(false);
        } else if (item.getItemId() == R.id.add_folder_menu) {
            action.doCreateFolder();
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    public void onRefresh() {
        UpdateRootInfo updateRootInfo = new UpdateRootInfo() {

            @Override
            public void onUpdate(JecFile f) {
                path = f;
            }
        };
        task = new ScanFilesTask(getActivity(), path, updateRootInfo);
        task.setTaskListener(new TaskListener<JecFile[]>() {
            @Override
            public void onCompleted() {
                if (binding.explorerSwipeRefreshLayout != null)
                    binding.explorerSwipeRefreshLayout.setRefreshing(false);
            }

            @Override
            public void onSuccess(JecFile[] result) {
                if (adapter != null)
                    adapter.setData(result);
            }

            @Override
            public void onError(Exception e) {
                if (binding.explorerSwipeRefreshLayout == null)
                    return;
                binding.explorerSwipeRefreshLayout.setRefreshing(false);
                UIUtils.toast(getContext(), e);
            }
        });
        task.execute();
    }

    @Override
    public void onItemClick(int position, View view) {
        JecFile file = adapter.getItem(position);
        if (!((FileExplorerActivity) getActivity()).onSelectFile(file)) {
            if (file.isDirectory()) {
                switchToPath(file);
            }
        }
    }

    public boolean onBackPressed() {
        JecFile parent = path.getParentFile();
        if (parent == null || parent.getPath().startsWith(path.getPath())) {
            switchToPath(parent);
            return true;
        }
        return false;
    }

    public void switchToPath(JecFile file) {
        if (file.canRead()) {
            path = file;
            pathAdapter.setPath(file);
            Preferences.getInstance(getContext()).setLastOpenPath(file.getPath());
            onRefresh();
        } else {
            Toast.makeText(getContext(), R.string.cannot_read_folder, Toast.LENGTH_SHORT).show();
        }
    }

    @Override
    public ActionMode startActionMode(ActionMode.Callback callback) {
        return ((AppCompatActivity) getActivity()).startSupportActionMode(callback);
    }

    @Override
    public void setSelectAll(boolean checked) {
        adapter.checkAll(checked);
    }

    @Override
    public void refresh() {
        onRefresh();
    }

    @Override
    public void finish() {
        getActivity().finish();
    }

    @Override
    public JecFile getCurrentDirectory() {
        return path;
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {
        onRefresh();
    }

    private interface UpdateRootInfo {
        void onUpdate(JecFile path);
    }

    private static class ScanFilesTask extends JecAsyncTask<Void, Void, JecFile[]> {
        private final UpdateRootInfo updateRootInfo;
        private final Context context;
        private final boolean isRoot = false;
        private JecFile path;

        private ScanFilesTask(Context context, JecFile path, UpdateRootInfo updateRootInfo) {
            this.context = context.getApplicationContext();
            this.path = path;
            this.updateRootInfo = updateRootInfo;
        }

        @Override
        protected void onRun(final TaskResult<JecFile[]> taskResult, Void... params) throws Exception {
            Preferences preferences = Preferences.getInstance(context);
            final boolean showHiddenFiles = preferences.isShowHiddenFiles();
            final int sortType = preferences.getFileSortType();
            updateRootInfo.onUpdate(path);
            path.listFiles(new FileListResultListener() {
                @Override
                public void onResult(JecFile[] result) {
                    if (result.length == 0) {
                        taskResult.setResult(result);
                        return;
                    }
                    if (!showHiddenFiles) {
                        List<JecFile> list = new ArrayList<>(result.length);
                        for (JecFile file : result) {
                            if (file.getName().charAt(0) == '.') {
                                continue;
                            }
                            list.add(file);
                        }
                        result = new JecFile[list.size()];
                        list.toArray(result);
                    }
                    Arrays.sort(result, new FileListSorter(true, sortType, true));
                    taskResult.setResult(result);
                }
            });
        }
    }
}
