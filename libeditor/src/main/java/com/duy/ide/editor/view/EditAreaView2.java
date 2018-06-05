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
import android.core.text.LayoutContext;
import android.core.text.SpannableStringBuilder;
import android.core.widget.model.EditorIndex;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v7.widget.AppCompatEditText;
import android.text.Layout;
import android.text.TextPaint;
import android.util.AttributeSet;
import android.util.TypedValue;
import android.view.inputmethod.EditorInfo;

import com.duy.common.DLog;
import com.duy.ide.editor.text.LineManager;
import com.duy.ide.editor.text.TextLineNumber;
import com.duy.ide.editor.theme.model.EditorTheme;
import com.jecelyin.common.utils.ReflectionUtil;
import com.jecelyin.common.utils.SysUtils;
import com.jecelyin.editor.v2.Preferences;

import java.lang.reflect.Field;
import java.util.List;

public class EditAreaView2 extends AppCompatEditText implements IEditAreaView, SharedPreferences.OnSharedPreferenceChangeListener {
    private static final String TAG = "EditAreaView2";
    private final LayoutContext mLayoutContext = new LayoutContext();
    protected Preferences mPreferences;
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
     * Line manager will be calculate real lines and virtual lines
     */
    private LineManager mLineManager;

    public EditAreaView2(Context context) {
        super(context);
        init(context);
    }

    public EditAreaView2(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(context);

    }

    public EditAreaView2(Context context, AttributeSet attrs, int defStyleAttr) {
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

        mLineManager = new LineManager(this);
        mPreferences = mLayoutContext.preferences = Preferences.getInstance(getContext());
        mPreferences.registerOnSharedPreferenceChangeListener(this);

        TextPaint gutterForegroundPaint = new TextPaint(getPaint());
        gutterForegroundPaint.setTextSize(getTextSize() * LayoutContext.LINE_NUMBER_FACTOR);
        mLayoutContext.setGutterForegroundPaint(gutterForegroundPaint);
        mLayoutContext.setGutterDividerPaint(new Paint(getPaint()));
        mLayoutContext.setGutterBackgroundPaint(new Paint(getPaint()));

        setInitLineNumber(1);
        setTextSize(getTextSize());
        setTheme(mPreferences.getEditorTheme());

        onSharedPreferenceChanged(null, Preferences.KEY_FONT_SIZE);
        onSharedPreferenceChanged(null, Preferences.KEY_CURSOR_WIDTH);
        onSharedPreferenceChanged(null, Preferences.KEY_SHOW_LINE_NUMBER);
        onSharedPreferenceChanged(null, Preferences.KEY_WORD_WRAP);
        onSharedPreferenceChanged(null, Preferences.KEY_SHOW_WHITESPACE);
        onSharedPreferenceChanged(null, Preferences.KEY_TAB_SIZE);
        onSharedPreferenceChanged(null, Preferences.KEY_AUTO_INDENT);
        onSharedPreferenceChanged(null, Preferences.KEY_AUTO_CAPITALIZE);
    }

    @Override
    protected void onTextChanged(CharSequence text, int start, int lengthBefore, int lengthAfter) {
        super.onTextChanged(text, start, lengthBefore, lengthAfter);
        updateLineNumberCount(start);
    }


    @Override
    public void setText(CharSequence text, BufferType type) {
        if (text instanceof SpannableStringBuilder) {
            super.setText(text, type);
        } else {
            super.setText(new SpannableStringBuilder(text), type);
        }
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
    public void setReadOnly(boolean readOnly) {

    }

    @Override
    public void gotoTop() {
        EditorCompat.gotoTop(this);
    }

    @Override
    public void gotoEnd() {
        EditorCompat.gotoEnd(this);
    }

    @Override
    public void undo() {
    }

    @Override
    public void redo() {

    }

    @Override
    public boolean canUndo() {
        return false;
    }

    @Override
    public boolean canRedo() {
        return false;
    }

    @Override
    public boolean cut() {
        return false;
    }

    @Override
    public boolean copy() {
        return false;
    }

    @Override
    public boolean paste() {
        return false;
    }

    @Override
    public void duplicateSelection() {

    }

    @Override
    public EditorIndex getCursorIndex(int realLine, int column) {
        if (getLayout() == null) {
            return null;
        }
        if (realLine <= 0 || realLine > getLineCount())
            return null;

//        int virtualLine = getLayout().realLineToVirtualLine(realLine);
        int virtualLine = realLine;
        if (virtualLine == -1)
            return null;

        int offset = getLayout().getLineStart(virtualLine);
        if (column > 0) {
            offset += column - 1; /*the column start at 1*/
        }
        if (offset > length()) {
            return null;
        }
        return new EditorIndex(virtualLine, column, offset);
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
    public void setInitLineNumber(int lineNumber) {
        if (!mLayoutContext.preferences.isShowLineNumber()) {
            //invalidate
            setPadding(getPaddingLeft(), getPaddingTop(), getPaddingRight(), getPaddingBottom());
            return;
        }
        mLayoutContext.setLineNumber(lineNumber);
        updateGutterSize();
    }


    private void drawLineNumber(Canvas canvas) {
        if (!mLayoutContext.getPreferences().isShowLineNumber()) {
            return;
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
        Paint paint = mLayoutContext.getGutterForegroundPaint();
        for (TextLineNumber.LineInfo line : lines) {
            canvas.drawText(line.getText(), x, line.getY(), paint);
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
            case Preferences.KEY_CURSOR_WIDTH:
                mLayoutContext.cursorThickness = mPreferences.getCursorThickness();
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
                if (getText() != null && getText() instanceof SpannableStringBuilder) {
                    ((SpannableStringBuilder) getText()).setAutoIndent(mPreferences.isAutoIndent());
                }
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
        super.setTextSize(unit, size);
        updateLayoutContext();
        updateTabChar();
    }

    /**
     * Update tab width
     */
    private void updateTabChar() {
        if (DLog.DEBUG) DLog.d(TAG, "updateTabChar() called");
        float spaceWidth = getPaint().measureText(" ");
        float tabWidth = spaceWidth * (mPreferences == null ? 4 : mPreferences.getTabSize());
        try {
            Field tabIncrement = ReflectionUtil.getField(Layout.class, "TAB_INCREMENT", true);
            ReflectionUtil.setFinalStatic(tabIncrement, (int) tabWidth);
            postInvalidate();
        } catch (Exception e) {
            e.printStackTrace();
        }
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

        float textWidth = mLayoutContext.getGutterForegroundPaint().measureText(" ");
        //plus 1 for some case: log(100) = 2, but we need 3
        double columnCount = Math.ceil(Math.log10(mLineManager.getRealLineCount() + 1)) + 1;
        mLayoutContext.setGutterWidth(((int) (textWidth * columnCount)) + numberPadding * 2/*Left and right*/);
        mLayoutContext.setLineNumberX(numberPadding);

        int gutterPaddingRight = SysUtils.dpToPixels(getContext(), 2);
        int newPaddingLeft = mLayoutContext.getGutterWidth() + gutterPaddingRight;
        setPadding(newPaddingLeft, getPaddingTop(), getPaddingRight(), getPaddingBottom());
    }

}
