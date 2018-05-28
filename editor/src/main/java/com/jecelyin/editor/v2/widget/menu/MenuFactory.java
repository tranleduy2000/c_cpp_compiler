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

package com.jecelyin.editor.v2.widget.menu;

import android.content.Context;

import com.duy.ide.editor.editor.R;
import com.jecelyin.editor.v2.Preferences;
import com.jecelyin.editor.v2.common.Command;

import java.util.ArrayList;
import java.util.EnumMap;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import static com.jecelyin.editor.v2.common.Command.CommandEnum.CONVERT_WRAP_CHAR;
import static com.jecelyin.editor.v2.common.Command.CommandEnum.CURSOR_BACK;
import static com.jecelyin.editor.v2.common.Command.CommandEnum.CURSOR_FORWARD;
import static com.jecelyin.editor.v2.common.Command.CommandEnum.DOC_INFO;
import static com.jecelyin.editor.v2.common.Command.CommandEnum.FIND;
import static com.jecelyin.editor.v2.common.Command.CommandEnum.FORMAT_SOURCE;
import static com.jecelyin.editor.v2.common.Command.CommandEnum.GOTO_END;
import static com.jecelyin.editor.v2.common.Command.CommandEnum.GOTO_INDEX;
import static com.jecelyin.editor.v2.common.Command.CommandEnum.GOTO_TOP;
import static com.jecelyin.editor.v2.common.Command.CommandEnum.NONE;
import static com.jecelyin.editor.v2.common.Command.CommandEnum.OPEN;
import static com.jecelyin.editor.v2.common.Command.CommandEnum.REDO;
import static com.jecelyin.editor.v2.common.Command.CommandEnum.SAVE;
import static com.jecelyin.editor.v2.common.Command.CommandEnum.SAVE_AS;
import static com.jecelyin.editor.v2.common.Command.CommandEnum.SHARE_CODE;
import static com.jecelyin.editor.v2.common.Command.CommandEnum.UNDO;

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
        Integer[] toolbarIcons = Preferences.getInstance(context).getToolbarIcons();
        if (toolbarIcons == null) {
            toolbarIcons = new Integer[]{R.id.action_run, R.id.action_undo};
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
        if (id == R.id.m_readonly) {
            return Preferences.getInstance(context).isReadOnly();
        } else if (id == R.id.m_fullscreen) {
            return Preferences.getInstance(context).isFullScreenMode();
        }
        return false;
    }

    public List<MenuItemInfo> getToolbarIcon() {
        return groups.get(MenuGroup.TOP);
    }

    private void initAllMenuItem() {
        menuItemInfos.add(new MenuItemInfo(MenuGroup.FILE, R.id.action_new_file, NONE, R.drawable.ic_add_white_24dp, R.string.new_file));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.FILE, R.id.action_open, OPEN, R.drawable.ic_folder_open_white_24dp, R.string.open));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.FILE, R.id.action_save, SAVE, R.drawable.ic_save_white_24dp, R.string.save));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.FILE, R.id.action_save_all, NONE, R.drawable.ic_save_white_24dp, R.string.save_all));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.FILE, R.id.action_save_as, SAVE_AS, R.drawable.ic_save_white_24dp, R.string.save_as));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.FILE, R.id.action_file_history, NONE, R.drawable.ic_history_white_24dp, R.string.recent_files));

        menuItemInfos.add(new MenuItemInfo(MenuGroup.EDIT, R.id.action_undo, UNDO, R.drawable.ic_undo_white_24dp, R.string.undo));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.EDIT, R.id.action_redo, REDO, R.drawable.ic_redo_white_24dp, R.string.redo));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.EDIT, R.id.action_wrap, CONVERT_WRAP_CHAR, R.drawable.ic_wrap_text_white_24dp, R.string.line_separator));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.EDIT, R.id.action_find_replace, FIND, R.drawable.ic_find_in_page_white_24dp, R.string.find_or_replace));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.EDIT, R.id.action_goto_top, GOTO_TOP, R.drawable.ic_vertical_align_top_white_24dp, R.string.jump_to_start));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.EDIT, R.id.action_goto_end, GOTO_END, R.drawable.ic_vertical_align_bottom_white_24dp, R.string.jump_to_end));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.EDIT, R.id.action_goto_line, GOTO_INDEX, R.drawable.m_goto_line, R.string.goto_line));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.EDIT, R.id.action_cursor_back, CURSOR_BACK, R.drawable.ic_arrow_back_white_24dp, R.string.cursor_back));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.EDIT, R.id.action_cursor_forward, CURSOR_FORWARD, R.drawable.ic_arrow_forward_white_24dp, R.string.cursor_forward));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.EDIT, R.id.action_share_code, SHARE_CODE, R.drawable.baseline_share_24, R.string.share_code));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.EDIT, R.id.action_format_source, FORMAT_SOURCE, R.drawable.baseline_format_indent_increase_24, R.string.format_source));

        menuItemInfos.add(new MenuItemInfo(MenuGroup.VIEW, R.id.m_info, DOC_INFO, R.drawable.ic_info_white_24dp, R.string.document_info));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.VIEW, R.id.action_encoding, NONE, R.drawable.m_encoding, R.string.encoding));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.VIEW, R.id.action_highlight, NONE, R.drawable.ic_highlight_white_24dp, R.string.highlight_language));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.OTHER, R.id.action_editor_setting, NONE, R.drawable.ic_settings_white_24dp, R.string.editor_setting));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.OTHER, R.id.action_share, NONE, R.drawable.baseline_share_24, R.string.share_this_app));
        menuItemInfos.add(new MenuItemInfo(MenuGroup.OTHER, R.id.action_rate, NONE, R.drawable.baseline_rate_review_24, R.string.rate_this_app));
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
        return NONE;
    }
}
