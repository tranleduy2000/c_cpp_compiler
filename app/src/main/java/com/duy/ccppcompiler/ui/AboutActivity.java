package com.duy.ccppcompiler.ui;

import android.os.Bundle;
import android.support.annotation.Nullable;

import com.duy.ccppcompiler.R;
import com.jecelyin.editor.v2.FullScreenActivity;

public class AboutActivity extends FullScreenActivity {
    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_about);
    }
}
