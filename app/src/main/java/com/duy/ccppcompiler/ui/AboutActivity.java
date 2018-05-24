package com.duy.ccppcompiler.ui;

import android.os.Bundle;
import android.support.annotation.Nullable;

import com.duy.ccppcompiler.R;
import com.jecelyin.editor.v2.BaseActivity;

public class AboutActivity extends BaseActivity {
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_about);
    }
}
