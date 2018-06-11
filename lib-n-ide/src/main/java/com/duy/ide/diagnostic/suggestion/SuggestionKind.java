/*
 * Copyright (C) 2018 Tran Le Duy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

package com.duy.ide.diagnostic.suggestion;

/**
 * An empty replacement string indicates that the given range is to be removed. An empty range
 * (e.g. “45:3-45:3”) indicates that the string is to be inserted at the given position.
 * Otherwise replace
 * Created by Duy on 29-Apr-18.
 */

public enum SuggestionKind {
    INSERT, REPLACE, DELETE
}
