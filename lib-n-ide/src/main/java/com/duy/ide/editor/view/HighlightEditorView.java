/*
 *  Copyright (C)  2018  Duy Tran Le
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

package com.duy.ide.editor.view;

import android.content.Context;
import android.content.SharedPreferences;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Typeface;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.text.Editable;
import android.text.InputFilter;
import android.text.InputType;
import android.text.Layout;
import android.text.Spannable;
import android.text.Spanned;
import android.text.TextPaint;
import android.text.TextWatcher;
import android.util.AttributeSet;
import android.util.TypedValue;
import android.view.inputmethod.EditorInfo;

import com.duy.ide.editor.text.LayoutContext;
import com.duy.ide.editor.text.LineManager;
import com.duy.ide.editor.text.TextLineNumber;
import com.duy.ide.editor.text.style.TabSpan;
import com.duy.ide.editor.theme.model.EditorTheme;
import com.jecelyin.common.utils.SysUtils;
import com.jecelyin.editor.v2.Preferences;

import java.util.List;

public abstract class HighlightEditorView extends android.support.v7.widget.AppCompatEditText
        implements IEditAreaView, SharedPreferences.OnSharedPreferenceChangeListener,
        TextWatcher {
    private static final String TAG = "EditAreaView2";
    private final LayoutContext mLayoutContext = new LayoutContext();
    protected Preferences mPreferences;
    /**
     * Line manager will be calculate real lines and virtual lines
     */
    protected LineManager mLineManager;
    /**
     * Editor color schemes, include text color, text background and more color attrs
     */
    @Nullable
    private EditorTheme mEditorTheme;
    /**
     * Store last line count has been calculated
     */
    private int mPreLineCount;
    /**
     * We can not update line count when layout is null, lazy init
     */
    private boolean mNeedUpdateLineNumber = false;
    private boolean mIsAutoIndent = true;
    private int mTabWidth = 3;

    public HighlightEditorView(Context context) {
        super(context);
        init(context);
    }

    public HighlightEditorView(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(context);
    }

    public HighlightEditorView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context);
    }

    private void init(Context context) {

        if (isInEditMode()) {
            return;
        }
        //avoid crash with large data
        setSaveEnabled(false);

        setImeOptions(EditorInfo.IME_ACTION_DONE | EditorInfo.IME_FLAG_NO_EXTRACT_UI);
        setInputType(InputType.TYPE_CLASS_TEXT
                | InputType.TYPE_TEXT_FLAG_NO_SUGGESTIONS
                | InputType.TYPE_TEXT_FLAG_MULTI_LINE
                | InputType.TYPE_TEXT_FLAG_IME_MULTI_LINE);

        mLineManager = new LineManager(this);
        mPreferences = mLayoutContext.preferences = Preferences.getInstance(getContext());
        mPreferences.registerOnSharedPreferenceChangeListener(this);

        TextPaint gutterForegroundPaint = new TextPaint(getPaint());
        gutterForegroundPaint.setTextSize(getTextSize() * LayoutContext.LINE_NUMBER_FACTOR);
        mLayoutContext.setGutterForegroundPaint(gutterForegroundPaint);
        mLayoutContext.setGutterDividerPaint(new Paint(getPaint()));
        mLayoutContext.setGutterBackgroundPaint(new Paint(getPaint()));

        setInitLineNumber(1);
        setTypeface(Typeface.MONOSPACE);
        setTextSize(getTextSize());
        setTheme(mPreferences.getEditorTheme());

        onSharedPreferenceChanged(null, Preferences.KEY_FONT_SIZE);
        onSharedPreferenceChanged(null, Preferences.KEY_SHOW_LINE_NUMBER);
        onSharedPreferenceChanged(null, Preferences.KEY_WORD_WRAP);
        onSharedPreferenceChanged(null, Preferences.KEY_SHOW_WHITESPACE);
        onSharedPreferenceChanged(null, Preferences.KEY_TAB_SIZE);
        onSharedPreferenceChanged(null, Preferences.KEY_AUTO_INDENT);
        onSharedPreferenceChanged(null, Preferences.KEY_AUTO_CAPITALIZE);

        initAutoIndent();
        addTextChangedListener(this);
    }

    private void initAutoIndent() {
        final InputFilter indentFilter = new InputFilter() {
            @Override
            public CharSequence filter(CharSequence source, int start, int end, Spanned dest, int dstart, int dend) {
                if (mIsAutoIndent) {
                    if (!(source.length() == 1 && source.charAt(0) == '\n')) {
                        return null;
                    }
                    dstart = dstart - 1;
                    if (dstart < 0 || dstart >= dest.length())
                        return null;

                    char ch;
                    for (; dstart >= 0; dstart--) {
                        ch = dest.charAt(dstart);
                        if (ch != '\r')
                            break;
                    }

                    StringBuilder sb = new StringBuilder();
                    for (int i = dstart; i >= 0; i--) {
                        ch = dest.charAt(i);
                        if (ch == '\n' || ch == '\r') {
                            break;
                        } else if (ch == ' ' || ch == '\t') {
                            sb.append(ch);
                        } else {
                            sb.setLength(0);
                        }
                    }
                    sb.reverse();

                    return "\n" + sb.toString();
                }
                return null;
            }
        };
        //only support \n
        InputFilter newLineFilter = new InputFilter() {
            @Override
            public CharSequence filter(CharSequence source, int start, int end, Spanned dest,
                                       int dstart, int dend) {
                final String s = source.toString();
                if (s.contains("\r")) {
                    return s.replace("\r", "");
                }
                return null;
            }
        };
        setFilters(new InputFilter[]{indentFilter, newLineFilter});
    }

    @Override
    public void setText(CharSequence text, BufferType type) {
        super.setText(text, type);
        mNeedUpdateLineNumber = true;
        mPreLineCount = -1;
    }

    @Override
    public void setTheme(@NonNull EditorTheme editorTheme) {
        mEditorTheme = editorTheme;

        setBackgroundColor(editorTheme.getBgColor());
        setTextColor(editorTheme.getFgColor());
        setHighlightColor(editorTheme.getSelectionColor());
        setCursorColor(editorTheme.getCaretColor());

        mLayoutContext.getGutterForegroundPaint().setColor(editorTheme.getGutterStyle().getFgColor());
        mLayoutContext.getGutterBackgroundPaint().setColor(editorTheme.getGutterStyle().getBgColor());
        mLayoutContext.getGutterDividerPaint().setColor(editorTheme.getGutterStyle().getFoldColor());

        mLayoutContext.whiteSpaceColor = editorTheme.getWhiteSpaceStyle().getWhitespace();
        postInvalidate();
    }

    @Override
    public EditorTheme getEditorTheme() {
        return mEditorTheme;
    }

    @Override
    protected void onDraw(Canvas canvas) {
        super.onDraw(canvas);

        drawLineNumber(canvas);
    }

    private void setCursorColor(int caretColor) {
        try {
            //        mCurrCursorColor = caretColor;
//        invalidateCursor();
            // https://github.com/android/platform_frameworks_base/blob/kitkat-release/core/java/android/widget/TextView.java#L562-564
//            Field f = TextView.class.getDeclaredField("mCursorDrawableRes");
//            f.setAccessible(true);
//            f.set(this, /*R.drawable.cursor*/-1);
        } catch (Exception ignored) {
        }
    }

    @Override
    public void scrollToLine(int line) {

    }

    @Override
    public int getLineForOffset(int offset) {
        if (getLayout() == null) {
            return -1;
        }
        return getLayout().getLineForOffset(offset);
    }

    /**
     * Calculate padding line number padding
     */
    @Override
    public void setInitLineNumber(int lineNumber) {
        if (!mLayoutContext.preferences.isShowLineNumber()) {
            //invalidate
            setPadding(getPaddingLeft(), getPaddingTop(), getPaddingRight(), getPaddingBottom());
            return;
        }
        mLayoutContext.setLineNumber(lineNumber);
        updateLineNumberCount(0);
    }

    private void drawLineNumber(Canvas canvas) {
        if (!mLayoutContext.getPreferences().isShowLineNumber()) {
            return;
        }

        if (mNeedUpdateLineNumber) {
            updateLineNumberCount(0);
            mNeedUpdateLineNumber = false;
        }

        //calculate position
        mLineManager.calculateLinePositionForDraw();

        int width = getScrollX() + mLayoutContext.getGutterWidth();
        int height = getScrollY() + getHeight();
        canvas.drawRect(getScrollX(), getScrollY(), width, height, mLayoutContext.getGutterBackgroundPaint());
        //draw gutter divider
        canvas.drawLine(width, getScrollY(), width, height, mLayoutContext.getGutterDividerPaint());

        List<TextLineNumber.LineInfo> lines = mLineManager.getTextLineNumber().getLines();
        int x = mLayoutContext.getLineNumberX() + getScrollX();
        int paddingTop = getPaddingTop();
        Paint paint = mLayoutContext.getGutterForegroundPaint();
        for (TextLineNumber.LineInfo line : lines) {
            canvas.drawText(line.getText(), x, line.getY() + paddingTop, paint);
        }
    }

    private void updateLineNumberCount(int start) {
        Layout layout = getLayout();
        if (layout == null) {
            return;
        }
        int lineCount = layout.getLineCount();
        if (mPreLineCount == lineCount) {
            //updated before
            return;
        }
        int startLineToUpdate = layout.getLineForOffset(start);
        mLineManager.updateRealLines(startLineToUpdate);
        mPreLineCount = lineCount;
        updateGutterSize();
    }

    public LayoutContext getLayoutContext() {
        return mLayoutContext;
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences sharedPreferences, String key) {
        switch (key) {
            case Preferences.KEY_FONT_SIZE:
                setTextSize(TypedValue.COMPLEX_UNIT_SP, mPreferences.getFontSize());
                break;
            case Preferences.KEY_SHOW_LINE_NUMBER:
                setInitLineNumber(mLayoutContext.lineNumber);
                break;
            case Preferences.KEY_WORD_WRAP:
                setHorizontallyScrolling(!mPreferences.isWordWrap());
                break;
            case Preferences.KEY_SHOW_WHITESPACE:
                mLayoutContext.isShowWhiteSpace = mPreferences.isShowWhiteSpace();
                break;
            case Preferences.KEY_TAB_SIZE:
                updateTabChar();
                break;
            case Preferences.KEY_AUTO_INDENT:
                mIsAutoIndent = mPreferences.isAutoIndent();
                break;
            case Preferences.KEY_AUTO_CAPITALIZE:
                if (!mPreferences.isAutoCapitalize()) {
                    setInputType(getInputType() & ~EditorInfo.TYPE_TEXT_FLAG_CAP_SENTENCES);
                } else {
                    setInputType(getInputType() | EditorInfo.TYPE_TEXT_FLAG_CAP_SENTENCES);
                }
                break;
        }
    }

    @Override
    public void setTextSize(int unit, float size) {
        boolean needCalculate = false;
        if (size != getTextSize()) {
            needCalculate = true;
        }
        super.setTextSize(unit, size);
        if (needCalculate) {

            updateLayoutContext();
            updateTabChar();

            mNeedUpdateLineNumber = true;
            mPreLineCount = -1;
        }
    }

    @Override
    public void beforeTextChanged(CharSequence s, int start, int count, int after) {

    }

    @Override
    public void afterTextChanged(Editable s) {

    }

    @Override
    public void onTextChanged(CharSequence s, int start, int before, int count) {
        updateLineNumberCount(start);
        if (s instanceof Spannable) {
            applyTabWidth((Spannable) s, start, start + count - 1);
        }
    }

    public void applyTabWidth(Spannable text, int start, int end) {
        for (int index = start; index <= end; index++) {
            if (text.charAt(index) == '\t') {
                text.setSpan(new TabSpan(getLayoutContext(), mTabWidth),
                        index, index + 1, Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
            }
        }
    }

    /**
     * Update tab width
     */
    private void updateTabChar() {
        float spaceWidth = getPaint().measureText(" ");
        float tabWidth = spaceWidth * (mPreferences == null ? 4 : mPreferences.getTabSize());
        mTabWidth = (int) tabWidth;
    }

    /**
     * Update text size of gutter paint, padding and some attrs
     */
    private void updateLayoutContext() {
        if (mLayoutContext.getGutterForegroundPaint() != null) {
            mLayoutContext.getGutterForegroundPaint().setTextSize(getTextSize() * LayoutContext.LINE_NUMBER_FACTOR);
            updateGutterSize();
        }
    }

    private void updateGutterSize() {
        int numberPadding = SysUtils.dpToPixels(getContext(), 2);

        float textWidth = mLayoutContext.getGutterForegroundPaint().measureText("8");
        //plus 1 for some case: log(100) = 2, but we need 3
        double columnCount = Math.ceil(Math.log10(mLineManager.getRealLineCount() + 1)) + 1;
        mLayoutContext.setGutterWidth(((int) (textWidth * columnCount)) + numberPadding * 2/*Left and right*/);
        mLayoutContext.setLineNumberX(numberPadding);

        int gutterPaddingRight = SysUtils.dpToPixels(getContext(), 2);
        int newPaddingLeft = mLayoutContext.getGutterWidth() + gutterPaddingRight;
        setPadding(newPaddingLeft, getPaddingTop(), getPaddingRight(), getPaddingBottom());
    }

    public int getMaxScrollY() {
        if (getLayout() == null)
            return 0;
        int vspace = getBottom() - getTop() - getExtendedPaddingTop() - getExtendedPaddingBottom();
        return getLayout().getHeight() - vspace;
    }
}
