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

import android.content.Intent;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.DividerItemDecoration;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;

import com.duy.ccppcompiler.R;
import com.duy.common.DLog;

import org.w3c.dom.Document;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
import org.xml.sax.SAXException;

import java.io.File;
import java.io.IOException;
import java.util.ArrayList;

import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.parsers.ParserConfigurationException;

/**
 * Created by Duy on 02-May-18.
 */

public class ExampleActivity extends AppCompatActivity {

    public static final String EXTRA_INDEX_PATH = "EXTRA_INDEX_PATH";
    private static final String TAG = "ExampleActivity";
    private RecyclerView mRecyclerView;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_example);
        Intent intent = getIntent();
        if (intent == null) {
            finish();
            return;
        }

        String path = intent.getStringExtra(EXTRA_INDEX_PATH);
        File file = new File(path);
        if (!file.exists()) {
            finish();
            if (DLog.DEBUG) DLog.w(TAG, "onCreate: file not exist");
            return;
        }

        initUi();
        try {
            parseData(path);
        } catch (IOException e) {
            e.printStackTrace();
        } catch (SAXException e) {
            e.printStackTrace();
        } catch (ParserConfigurationException e) {
            e.printStackTrace();
        }
    }

    private void initUi() {
        mRecyclerView = findViewById(R.id.recycler_view);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(this));
        mRecyclerView.addItemDecoration(new DividerItemDecoration(this, DividerItemDecoration.VERTICAL));
    }


    private ArrayList<ExampleItem> parseData(String indexPath) throws IOException, SAXException, ParserConfigurationException {
        ArrayList<ExampleItem> result = new ArrayList<>();
        File inputFile = new File(indexPath);
        DocumentBuilderFactory dbFactory = DocumentBuilderFactory.newInstance();
        DocumentBuilder dBuilder = dbFactory.newDocumentBuilder();
        Document doc = dBuilder.parse(inputFile);
        doc.getDocumentElement().normalize();
        NodeList examples = doc.getElementsByTagName("examples");
        for (int i = 0; i < examples.getLength(); i++) {
            Node example = examples.item(i);
            NodeList childNodes = example.getChildNodes();
            ExampleItem item = new ExampleItem();
            for (int j = 0; j < childNodes.getLength(); j++) {
                Node attr = childNodes.item(j);
                String nodeName = attr.getNodeName();
                if (nodeName.equalsIgnoreCase(ExampleItem.TITLE)) {
                    item.setTitle(attr.getTextContent());
                } else if (nodeName.equalsIgnoreCase(ExampleItem.DESC)) {
                    item.setDesc(attr.getTextContent());
                } else if (nodeName.equalsIgnoreCase(ExampleItem.RELATIVE_PATH)) {
                    item.setRelativePath(attr.getTextContent());
                }
            }
            System.out.println("item = " + item);
            result.add(item);
        }
        return result;
    }

}
