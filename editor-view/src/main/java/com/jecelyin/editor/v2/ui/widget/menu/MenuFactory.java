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

package com.jecelyin.editor.v2.ui.widget.menu;

import android.content.Context;

import com.duy.text.editor.R;
import com.jecelyin.editor.v2.Pref;
import com.jecelyin.editor.v2.common.Command;

import java.util.ArrayList;
import java.util.EnumMap;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * @author Jecelyin Peng <jecelyin@gmail.com>
 */
public class MenuFactory {
    private static MenuFactory instance;
    private ArrayList<MenuItemInfo> menuItemInfos = new ArrayList<>();
    private EnumMap<MenuGroup, List<MenuItemInfo>> groups;
    private Map<Integer, Integer> toolbarMenus = new HashMap<>();

    private MenuFactory(Context context) {
        initAllMenuItem();
        groups = new EnumMap<>(MenuGroup.class);
        List<MenuItemInfo> topList = new ArrayList<>();
        groups.put(MenuGroup.TOP, topList);
        Integer[] toolbarIcons = Pref.getInstance(context).getToolbarIcons();
        if (toolbarIcons == null) {
            toolbarIcons = new Integer[]{R.id.m_new, R.id.m_open, R.id.m_save, R.id.m_undo, R.id.m_redo};
        }

        List<MenuItemInfo> items;
        for (MenuItemInfo item : menuItemInfos) {
            if (isToolbarMenu(toolbarIcons, item.getItemId())) {
                topList.add(item);
                toolbarMenus.put(item.getItemId(), item.getGroupId());
            } else {
                items = groups.get(item.getGroup());
                if (items == null) {
                    items = new ArrayList<>();
                    groups.put(item.getGroup(), items);
                }
                items.add(item);
            }
        }
    }

    public static MenuFactory getInstance(Context context) {
        if (instance == null)
            instance = new MenuFactory(context);
        return instance;
    }

    private static boolean isToolbarMenu(Integer[] def, int id) {
        for (Integer val : def) {
            if (id == val)
                return true;
        }
        return false;
    }

    public static boolean isCheckboxMenu(int id) {
        return id == R.id.m_readonly || id == R.id.m_fullscreen;
    }

    public static boolean isChecked(Context context, int id) {
        switch (id) {
            case R.id.m_readonly:
                return Pref.getInstance(context).isReadOnly();
            case R.id.m_fullscreen:
                return Pref.getInstance(context).isFullScreenMode();
        }
        return false;
    }

    public List<MenuItemInfo> getToolbarIcon() {
        return groups.get(MenuGroup.TOP);
    }

    private void initAllMenuItem() {
        menuItemInfos.add(new MenuItemInfo(MenuGroup.FILE, R.id.m_new, Command.CommandEnum.NONE, R.drawable.ic_add_white_24dp, R.string.new_file));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.FILE, R.id.m_open, Command.CommandEnum.OPEN, R.drawable.ic_folder_open_white_24dp, R.string.open));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.FILE, R.id.m_save, Command.CommandEnum.SAVE, R.drawable.ic_save_white_24dp, R.string.save));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.FILE, R.id.m_save_all, Command.CommandEnum.NONE, R.drawable.m_save_all, R.string.save_all));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.FILE, R.id.m_save_as, Command.CommandEnum.SAVE_AS, R.drawable.m_save_as, R.string.save_as));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.FILE, R.id.m_history, Command.CommandEnum.NONE, R.drawable.ic_history_white_24dp, R.string.recent_files));

//        menuItemInfos.add(new MenuItemInfo(MenuGroup.EDIT,  R.id.m_cut,            Command.CommandEnum.CUT,               R.drawable.m_cut, R.string.cut));
//        menuItemInfos.add(new MenuItemInfo(MenuGroup.EDIT,  R.id.m_copy,           Command.CommandEnum.COPY,              R.drawable.m_copy, R.string.copy));
//        menuItemInfos.add(new MenuItemInfo(MenuGroup.EDIT,  R.id.m_paste,          Command.CommandEnum.PASTE,             R.drawable.m_paste, R.string.paste));
//        menuItemInfos.add(new MenuItemInfo(MenuGroup.EDIT,  R.id.m_select_all,     Command.CommandEnum.SELECT_ALL,        R.drawable.m_select_all, R.string.select_all));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.EDIT, R.id.m_undo, Command.CommandEnum.UNDO, R.drawable.ic_undo_white_24dp, R.string.undo));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.EDIT, R.id.m_redo, Command.CommandEnum.REDO, R.drawable.ic_redo_white_24dp, R.string.redo));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.EDIT, R.id.m_wrap, Command.CommandEnum.CONVERT_WRAP_CHAR, R.drawable.ic_wrap_text_white_24dp, R.string.line_separator));
//        menuItemInfos.add(new MenuItemInfo(MenuGroup.EDIT,  R.id.m_duplication,    Command.CommandEnum.DUPLICATION,       R.drawable.m_duplication, R.string.duplication));

        menuItemInfos.add(new MenuItemInfo(MenuGroup.FIND, R.id.m_find_replace, Command.CommandEnum.FIND, R.drawable.ic_find_in_page_white_24dp, R.string.find_or_replace));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.FIND, R.id.m_goto_top, Command.CommandEnum.GOTO_TOP, R.drawable.ic_vertical_align_top_white_24dp, R.string.jump_to_start));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.FIND, R.id.m_goto_end, Command.CommandEnum.GOTO_END, R.drawable.ic_vertical_align_bottom_white_24dp, R.string.jump_to_end));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.FIND, R.id.m_goto_line, Command.CommandEnum.GOTO_LINE, R.drawable.m_goto_line, R.string.goto_line));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.FIND, R.id.m_back, Command.CommandEnum.BACK, R.drawable.ic_arrow_back_white_24dp, R.string.back));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.FIND, R.id.m_forward, Command.CommandEnum.FORWARD, R.drawable.ic_arrow_forward_white_24dp, R.string.forward));

        menuItemInfos.add(new MenuItemInfo(MenuGroup.VIEW, R.id.m_theme, Command.CommandEnum.THEME, R.drawable.ic_style_white_24dp, R.string.change_theme));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.VIEW, R.id.m_fullscreen, Command.CommandEnum.FULL_SCREEN, R.drawable.ic_fullscreen_white_24dp, R.string.fullscreen_mode));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.VIEW, R.id.m_info, Command.CommandEnum.DOC_INFO, R.drawable.ic_info_white_24dp, R.string.document_info));
//        menuItemInfos.add(new MenuItemInfo(MenuGroup.VIEW,  R.id.m_symbol,         Command.CommandEnum.NONE, R.drawable.m_symbol, R.string.symbol));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.VIEW, R.id.m_readonly, Command.CommandEnum.READONLY_MODE, R.drawable.ic_remove_red_eye_white_24dp, R.string.read_only));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.VIEW, R.id.m_highlight, Command.CommandEnum.NONE, R.drawable.ic_highlight_white_24dp, R.string.highlight_language));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.VIEW, R.id.m_encoding, Command.CommandEnum.NONE, R.drawable.m_encoding, R.string.encoding));

        menuItemInfos.add(new MenuItemInfo(MenuGroup.OTHER, R.id.m_color, Command.CommandEnum.NONE, R.drawable.ic_color_lens_white_24dp, R.string.insert_color));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.OTHER, R.id.m_datetime, Command.CommandEnum.NONE, R.drawable.ic_date_range_white_24dp, R.string.insert_datetime));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.OTHER, R.id.m_run, Command.CommandEnum.NONE, R.drawable.ic_play_arrow_white_24dp, R.string.run));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.OTHER, R.id.m_settings, Command.CommandEnum.NONE, R.drawable.ic_settings_white_24dp, R.string.settings));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.OTHER, R.id.m_exit, Command.CommandEnum.NONE, R.drawable.ic_exit_to_app_white_24dp, R.string.exit));
    }

    public List<MenuItemInfo> getMenuItemsWithoutToolbarMenu(MenuGroup group) {
        ArrayList<MenuItemInfo> list = new ArrayList<>();
        for (MenuItemInfo menuItemInfo : menuItemInfos) {
            if (menuItemInfo.getGroup() == group && !toolbarMenus.containsKey(menuItemInfo.getItemId())) {
                list.add(menuItemInfo);
            }
        }
        return list;
    }

    public Command.CommandEnum idToCommandEnum(int id) {
        for (MenuItemInfo itemInfo : menuItemInfos) {
            if (itemInfo.getItemId() == id)
                return itemInfo.getCommandEnum();
        }
        return Command.CommandEnum.NONE;
    }
}
