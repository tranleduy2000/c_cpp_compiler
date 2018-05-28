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

package com.duy.ide.suggestion;

/**
 * An empty replacement string indicates that the given range is to be removed. An empty range
 * (e.g. “45:3-45:3”) indicates that the string is to be inserted at the given position.
 * Otherwise replace
 * Created by Duy on 29-Apr-18.
 */

public enum SuggestionKind {
    INSERT, REPLACE, DELETE
}
