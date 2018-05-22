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

package com.duy.ccppcompiler.ui.examples;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.DividerItemDecoration;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;

import com.duy.ccppcompiler.R;

import org.xml.sax.SAXException;

import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;

import javax.xml.parsers.ParserConfigurationException;

/**
 * Created by Duy on 02-May-18.
 */

public class ExampleActivity extends AppCompatActivity {

    public static final String EXTRA_LANGUAGE = "EXTRA_LANGUAGE";
    private static final String TAG = "ExampleActivity";
    private static final int RC_OPEN_EXAMPLE = 13123;
    private RecyclerView mRecyclerView;

    public static void openExample(Activity activity, String language) {
        Intent intent = new Intent(activity, ExampleActivity.class);
        intent.putExtra(EXTRA_LANGUAGE, language);
        activity.startActivityForResult(intent, RC_OPEN_EXAMPLE);
    }

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_example);
        Intent intent = getIntent();
        if (intent == null || !intent.hasExtra(EXTRA_LANGUAGE)) {
            finish();
            return;
        }

        String language = intent.getStringExtra(EXTRA_LANGUAGE);
        String path = "example/" + language + "/index.xml";
        try {
            InputStream index = getAssets().open(path);
            parseData(index);

        } catch (Exception e) {
            e.printStackTrace();
            finish();
            return;
        }
        initUi();
    }

    private void initUi() {
        mRecyclerView = findViewById(R.id.recycler_view);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(this));
        mRecyclerView.addItemDecoration(new DividerItemDecoration(this, DividerItemDecoration.VERTICAL));
    }

    private ArrayList<ExampleItem> parseData(InputStream in) throws IOException, SAXException, ParserConfigurationException {
        return new ExampleParser().parse(in);
    }
}
