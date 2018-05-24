package com.jecelyin.editor.v2.editor;

import android.core.text.SpannableStringBuilder;
import android.text.Spannable;
import android.text.style.ForegroundColorSpan;

import com.duy.ide.editor.theme.model.EditorTheme;
import com.duy.ide.editor.theme.model.SyntaxStyle;
import com.jecelyin.common.utils.DLog;
import com.jecelyin.editor.v2.highlight.Buffer;
import com.jecelyin.editor.v2.highlight.HighlightInfo;

import org.gjt.sp.jedit.syntax.DefaultTokenHandler;
import org.gjt.sp.jedit.syntax.Token;

import java.util.ArrayList;
import java.util.HashMap;

public class Highlighter {
    public Highlighter() {

    }

    public void highlight(Buffer buffer, EditorTheme editorTheme,
                          HashMap<Integer, ArrayList<ForegroundColorSpan>> colorsMap,
                          Spannable spannable,
                          int startLine, int endLine) {
        if (!buffer.isCanHighlight())
            return;
        SyntaxStyle[] syntaxStyles = editorTheme.getSyntaxStyles();

        DefaultTokenHandler tokenHandler;
        ArrayList<HighlightInfo> mergerArray;

        for (int i = startLine; i <= endLine; i++) {
            tokenHandler = new DefaultTokenHandler();
            buffer.markTokens(i, tokenHandler);
            Token token = tokenHandler.getTokens();

            mergerArray = new ArrayList<>();
            collectToken(syntaxStyles, buffer, i, token, mergerArray);
            addTokenSpans(spannable, i, mergerArray, colorsMap);
        }
    }

    private void addTokenSpans(Spannable spannable, int line, ArrayList<HighlightInfo> mergerArray,
                               HashMap<Integer, ArrayList<ForegroundColorSpan>> colorsMap) {
        ForegroundColorSpan fcs;

        ArrayList<ForegroundColorSpan> oldSpans = colorsMap.remove(line);
        if (oldSpans != null) {
            for (ForegroundColorSpan span : oldSpans) {
                spannable.removeSpan(span);
            }
        }

        int length = spannable.length();

        ArrayList<ForegroundColorSpan> spans = new ArrayList<>(mergerArray.size());
        for (HighlightInfo hi : mergerArray) {
            if (hi.endOffset > length) {
                DLog.e("assert hi.endOffset %d > maxLength %d", hi.endOffset, length);
                hi.endOffset = length;
            }
            if (hi.startOffset >= hi.endOffset) {
                DLog.e("hi.startOffset %d >= hi.endOffset %d", hi.startOffset, hi.endOffset);
                continue;
            }
            fcs = new ForegroundColorSpan(hi.color);
            spannable.setSpan(fcs, hi.startOffset, hi.endOffset, SpannableStringBuilder.SPAN_EXCLUSIVE_EXCLUSIVE);
            spans.add(fcs);
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

            int color = style.getForegroundColor();

            if (mergerArray.isEmpty()) {
                mergerArray.add(new HighlightInfo(startIndex, endIndex, color));
            } else {
                hi = mergerArray.get(mergerArray.size() - 1);
                if (hi.color == color && hi.endOffset == startIndex) {
                    hi.endOffset = endIndex;
                } else {
                    mergerArray.add(new HighlightInfo(startIndex, endIndex, color));
                }
            }
        }


    }

}
