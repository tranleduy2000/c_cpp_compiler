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

package com.duy.ide.editor;

import android.graphics.Color;
import android.text.Editable;
import android.text.Spannable;
import android.text.SpannableStringBuilder;
import android.text.Spanned;
import android.text.style.BackgroundColorSpan;
import android.text.style.CharacterStyle;
import android.text.style.ForegroundColorSpan;
import android.text.style.StyleSpan;

import com.duy.ide.editor.text.style.ErrorSpan;
import com.duy.ide.editor.theme.model.EditorTheme;
import com.duy.ide.editor.theme.model.SyntaxStyle;
import com.jecelyin.common.utils.DLog;
import com.jecelyin.editor.v2.highlight.Buffer;
import com.jecelyin.editor.v2.highlight.HighlightInfo;

import org.gjt.sp.jedit.Segment;
import org.gjt.sp.jedit.awt.Font;
import org.gjt.sp.jedit.syntax.DefaultTokenHandler;
import org.gjt.sp.jedit.syntax.Token;

import java.util.ArrayList;
import java.util.HashMap;

public class Highlighter {
    public Highlighter() {

    }

    /**
     * Highlight error, line in buffer is real line, but line start at 0
     */
    public void highlightError(Buffer buffer, EditorTheme editorTheme,
                               HashMap<Integer, ArrayList<? extends CharacterStyle>> colorsMap,
                               Editable spannable,
                               int startLine, int endLine, boolean includeWhitespace) {

        Segment segment = new Segment();
        for (int line = startLine; line <= endLine; line++) {
            ArrayList<? extends CharacterStyle> oldSpans = colorsMap.remove(line);
            if (oldSpans != null) {
                for (CharacterStyle span : oldSpans) {
                    spannable.removeSpan(span);
                }
            }
            buffer.getLineText(line, segment);
            int startIndex;
            startIndex = segment.offset;
            int endIndex = segment.offset + segment.count;
            if (!includeWhitespace) {
                while (startIndex < endIndex) {
                    if (Character.isWhitespace(segment.charAt(startIndex))) {
                        startIndex++;
                        continue;
                    }
                    if (Character.isWhitespace(segment.charAt(endIndex - 1))) {
                        endIndex--;
                        continue;
                    }
                    break;
                }
            }
            if (startIndex < endIndex) {
                int lineStartOffset = buffer.getLineManager().getLineStartOffset(line);
                ArrayList<CharacterStyle> spans = new ArrayList<>(2);
                SyntaxStyle color = editorTheme.getSyntaxStyles()[Token.INVALID];
                Font font = color.getFont();
                if (font != null) {
                    StyleSpan span = new StyleSpan(font.getStyle());
                    spans.add(span);
                    spannable.setSpan(span, lineStartOffset + startIndex, lineStartOffset + endIndex, Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
                }
                ErrorSpan span = new ErrorSpan(color.getForegroundColor());
                spans.add(span);
                spannable.setSpan(span, lineStartOffset + startIndex, lineStartOffset + endIndex, Spanned.SPAN_EXCLUSIVE_EXCLUSIVE);
                colorsMap.put(line, spans);
            }
        }
    }

    public void highlightWarn(Buffer buffer, EditorTheme editorTheme,
                              HashMap<Integer, ArrayList<? extends CharacterStyle>> colorsMap,
                              Spannable spannable,
                              int startLine, int endLine, boolean includeWhitespace) {

    }

    public void highlight(Buffer buffer, EditorTheme editorTheme,
                          HashMap<Integer, ArrayList<? extends CharacterStyle>> colorsMap,
                          Spannable spannable,
                          int startLine, int endLine) {
        if (!buffer.isCanHighlight())
            return;
        SyntaxStyle[] syntaxStyles = editorTheme.getSyntaxStyles();

        DefaultTokenHandler tokenHandler;
        ArrayList<HighlightInfo> mergerArray;

        for (int line = startLine; line <= endLine; line++) {
            tokenHandler = new DefaultTokenHandler();
            buffer.markTokens(line, tokenHandler);
            Token token = tokenHandler.getTokens();

            mergerArray = new ArrayList<>();
            collectToken(syntaxStyles, buffer, line, token, mergerArray);
            addTokenSpans(spannable, line, mergerArray, colorsMap);
        }
    }

    private void addTokenSpans(Spannable spannable, int line, ArrayList<HighlightInfo> mergerArray,
                               HashMap<Integer, ArrayList<? extends CharacterStyle>> colorsMap) {
        CharacterStyle fcs;

        ArrayList<? extends CharacterStyle> oldSpans = colorsMap.remove(line);
        if (oldSpans != null) {
            for (CharacterStyle span : oldSpans) {
                spannable.removeSpan(span);
            }
        }

        int length = spannable.length();

        ArrayList<CharacterStyle> spans = new ArrayList<>(mergerArray.size());
        for (HighlightInfo info : mergerArray) {
            if (info.endOffset > length) {
                DLog.e("assert hi.endOffset %d > maxLength %d", info.endOffset, length);
                info.endOffset = length;
            }
            if (info.startOffset >= info.endOffset) {
                DLog.e("hi.startOffset %d >= hi.endOffset %d", info.startOffset, info.endOffset);
                continue;
            }
            fcs = new ForegroundColorSpan(info.style.getForegroundColor());
            spannable.setSpan(fcs, info.startOffset, info.endOffset, SpannableStringBuilder.SPAN_EXCLUSIVE_EXCLUSIVE);
            spans.add(fcs);

            if (info.style.getFont() != null) {
                if (info.style.getFont().getStyle() != Font.NORMAL) {
                    fcs = new StyleSpan(info.style.getFont().getStyle());
                    spannable.setSpan(fcs, info.startOffset, info.endOffset, SpannableStringBuilder.SPAN_EXCLUSIVE_EXCLUSIVE);
                    spans.add(fcs);
                }
            }
            if (info.style.getBackgroundColor() != Color.TRANSPARENT) {
                if (info.style.getFont().getStyle() != Font.NORMAL) {
                    fcs = new BackgroundColorSpan(info.style.getFont().getStyle());
                    spannable.setSpan(fcs, info.startOffset, info.endOffset, SpannableStringBuilder.SPAN_EXCLUSIVE_EXCLUSIVE);
                    spans.add(fcs);
                }
            }
        }
        colorsMap.put(line, spans);
    }

    private void collectToken(SyntaxStyle[] syntaxStyles, Buffer buffer, int lineNumber, Token token,
                              ArrayList<HighlightInfo> mergerArray) {

        int lineStartOffset = buffer.getLineManager().getLineStartOffset(lineNumber);

        HighlightInfo hi;
        while (token.id != Token.END) {
            int startIndex = lineStartOffset + token.offset;
            int endIndex = lineStartOffset + token.offset + token.length;
            SyntaxStyle style = syntaxStyles[token.id];
            token = token.next;

            if (style == null)
                continue;

            if (mergerArray.isEmpty()) {
                mergerArray.add(new HighlightInfo(startIndex, endIndex, style));
            } else {
                hi = mergerArray.get(mergerArray.size() - 1);
                if (hi.style.equals(style) && hi.endOffset == startIndex) {
                    hi.endOffset = endIndex;
                } else {
                    mergerArray.add(new HighlightInfo(startIndex, endIndex, style));
                }
            }
        }


    }

}
