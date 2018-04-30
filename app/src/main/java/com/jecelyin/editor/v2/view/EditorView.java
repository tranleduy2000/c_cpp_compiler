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

package com.jecelyin.editor.v2.view;

import android.content.Context;
import android.core.widget.EditAreaView;
import android.os.Parcelable;
import android.util.AttributeSet;
import android.util.SparseArray;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;

import com.duy.ccppcompiler.R;
import com.duy.common.DLog;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class EditorView extends RelativeLayout {
    private static final String TAG = "EditorView";
    private EditAreaView mEditText;
    private ProgressBar mProgressView;

    public EditorView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public EditorView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
    }

    @Override
    public void restoreHierarchyState(SparseArray<Parcelable> container) {
        super.restoreHierarchyState(container);
    }

    @Override
    protected void dispatchRestoreInstanceState(SparseArray<Parcelable> container) {
        super.dispatchRestoreInstanceState(container);
        if (DLog.DEBUG)
            DLog.d(TAG, "dispatchRestoreInstanceState() called with: container = [" + container + "]");
    }

    @Override
    protected void onFinishInflate() {
        super.onFinishInflate();

        mEditText = findViewById(R.id.edit_text);
        mProgressView = findViewById(R.id.progress_view);

    }

    public EditAreaView getEditText() {
        return mEditText;
    }

    public void setLoading(boolean loading) {
        if (loading) {
            mEditText.setVisibility(INVISIBLE);
            mProgressView.setVisibility(VISIBLE);
        } else {
            mEditText.setVisibility(VISIBLE);
            mProgressView.setVisibility(GONE);
        }
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
    }

}
