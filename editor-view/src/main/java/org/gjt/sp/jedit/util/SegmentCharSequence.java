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

package org.gjt.sp.jedit.util;

import org.gjt.sp.jedit.Segment;

/**
 * Class that lets java.util.regex search within a javax.swing.text.Segment.
 *
 * @author Marcelo Vanzin
 */
public class SegmentCharSequence implements CharSequence {
    private int offset;
    private int length;
    private Segment seg;

    public SegmentCharSequence(Segment seg) {
        this(seg, 0, seg.count);
    }

    public SegmentCharSequence(Segment seg, int off, int len) {
        this.offset = off;
        this.length = len;
        this.seg = seg;
    }

    public char charAt(int index) {
        return seg.array[seg.offset + offset + index];
    }

    public int length() {
        return length;
    }

    public CharSequence subSequence(int start, int end) {
        return new SegmentCharSequence(seg, offset + start, end - start);
    }

    public String toString() {
        return new String(seg.array, offset + seg.offset, length);
    }
}

