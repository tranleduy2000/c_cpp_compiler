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

package com.jecelyin.editor.v2.adapter;

import android.support.v4.app.Fragment;
import android.support.v4.app.FragmentManager;
import android.support.v4.app.FragmentStatePagerAdapter;

import com.duy.ide.editor.EditorFragment;

import java.io.File;
import java.util.ArrayList;

/**
 * Created by Duy on 25-Apr-18.
 */

public class EditorFragmentPagerAdapter extends FragmentStatePagerAdapter {
    private ArrayList<File> files = new ArrayList<>();

    public EditorFragmentPagerAdapter(FragmentManager fm) {
        super(fm);
    }

    @Override
    public Fragment getItem(int position) {
        return EditorFragment.newInstance(files.get(position));
    }

    @Override
    public int getCount() {
        return files.size();
    }
}
