package com.duy.ide.editor.view;

import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Rect;
import android.graphics.drawable.ColorDrawable;
import android.os.Build;
import android.support.annotation.IdRes;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v7.widget.ListPopupWindow;
import android.text.Layout;
import android.util.AttributeSet;
import android.view.KeyEvent;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.widget.AdapterView;

import com.duy.common.DLog;
import com.duy.ide.code.api.SuggestItem;
import com.duy.ide.editor.internal.suggestion.OnSuggestItemClickListener;
import com.duy.ide.editor.internal.suggestion.SuggestionAdapter;
import com.duy.ide.editor.theme.model.EditorTheme;

import java.util.List;

public class SuggestionEditor extends EditActionSupportEditor {

    private static final String TAG = "SuggestionEditor";
    private final Rect mTmpRect = new Rect();
    @Nullable
    private SuggestionAdapter mAdapter;
    @Nullable
    private OnSuggestItemClickListener mOnSuggestItemClickListener;
    private ListPopupWindow mPopup;

    private AdapterView.OnItemClickListener mSuggestClickListener
            = new AdapterView.OnItemClickListener() {
        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            if (mAdapter != null) {
                performCompletion(position);
            }
        }
    };

    @IdRes
    private int mDropDownAnchorId = View.NO_ID;

    public SuggestionEditor(Context context) {
        super(context);
        init(context, null, 0);
    }

    public SuggestionEditor(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(context, attrs, 0);

    }

    public SuggestionEditor(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context, attrs, defStyleAttr);
    }

    private void init(Context context, AttributeSet attrs, int defStyleAttr) {
        updateCharHeight();
        mPopup = new ListPopupWindow(context, attrs, defStyleAttr, 0);
        mPopup.setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_RESIZE);
        if (getEditorTheme() != null) {
            mPopup.setBackgroundDrawable(new ColorDrawable(getEditorTheme().getBgColor()));
        }
    }

    @Override
    public void setTheme(@NonNull EditorTheme editorTheme) {
        super.setTheme(editorTheme);
        if (mPopup != null) {
            mPopup.setBackgroundDrawable(new ColorDrawable(editorTheme.getBgColor()));
        }
    }


    @Override
    public void onTextChanged(CharSequence s, int start, int before, int count) {
        super.onTextChanged(s, start, before, count);
        onPopupChangePosition();
    }

    protected void onPopupChangePosition() {
        try {
            Layout layout = getLayout();
            //not ready
            if (layout == null) {
                return;
            }
            int cursor = getSelectionStart();
            if (cursor < 0) {
                return;
            }
            int line = layout.getLineForOffset(cursor);
            int bottom = layout.getLineBottom(line);
            int top = layout.getLineTop(line);

            float cursorX = layout.getPrimaryHorizontal(cursor);

            //left of cursor
            int offsetHorizontal = (int) cursorX + getPaddingLeft() - getScrollX();
            setDropDownHorizontalOffset(offsetHorizontal);

            getGlobalVisibleRect(mTmpRect);
            int heightVisible = mTmpRect.height();

            int popupBottom = bottom - getScrollY() + getDropDownHeight();
            if (popupBottom < heightVisible) {
                popupBottom = bottom - getScrollY() + getDropDownHeight();
                setDropDownVerticalOffset(popupBottom);
            } else {
                int popupTop = top - getScrollY() - getDropDownHeight();
                setDropDownVerticalOffset(popupTop + getDropDownHeight());
            }
        } catch (Exception ignored) {
            if (DLog.DEBUG) DLog.e(TAG, "onPopupChangePosition: ", ignored);
        }
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        onDropdownChangeSize();
    }

    /**
     * this method will be change size of popup window
     */
    protected void onDropdownChangeSize() {
        Rect rect = new Rect();
        getGlobalVisibleRect(rect);
        if (DLog.DEBUG) DLog.d(TAG, "rect = " + rect);
        // 1/2 width of screen
        setDropDownWidth((int) (rect.width() * 0.65f));
        // 0.5 height of screen
        setDropDownHeight((int) (rect.height() * 0.5f));
        onPopupChangePosition();
    }

    public void showDropDown() {
        if (!isPopupShowing()) {
            if (hasFocus()) {
                try {
                    showDropDownImpl();
                } catch (Exception e) {
                    if (DLog.DEBUG) DLog.e(TAG, "showDropDown: ", e);
                }
            }
        }
    }

    /**
     * <p>Displays the drop down on screen.</p>
     */
    private void showDropDownImpl() {
        if (mPopup.getAnchorView() == null) {
            if (mDropDownAnchorId != View.NO_ID) {
                mPopup.setAnchorView(getRootView().findViewById(mDropDownAnchorId));
            } else {
                mPopup.setAnchorView(this);
            }
        }
        if (!isPopupShowing()) {
            // Make sure the list does not obscure the IME when shown for the first time.
            mPopup.setInputMethodMode(ListPopupWindow.INPUT_METHOD_NEEDED);
        }
        mPopup.show();
        mPopup.getListView().setOverScrollMode(View.OVER_SCROLL_ALWAYS);
    }


    @Override
    public void setSuggestEnable(boolean enable) {
        if (!enable) {
            mAdapter = null;
            dismissDropDown();
        }
    }

    /**
     * invalidate data for auto suggest
     */
    @Override
    public void setSuggestData(@NonNull List<SuggestItem> data) {
        if (mAdapter != null) {
            mAdapter.clearAllData();
        } else {
            mAdapter = new SuggestionAdapter(getContext(), data);
            mAdapter.setListener(mSuggestClickListener);
            mPopup.setAdapter(mAdapter);
        }
        mAdapter.setData(data);
        mAdapter.notifyDataSetChanged();

        if (data.size() > 0) {
            showDropDown();
        }
    }

    @Override
    public void setTextSize(int unit, float size) {
        float oldTextSize = getTextSize();
        super.setTextSize(unit, size);
        if (oldTextSize != size) {
            updateCharHeight();
        }
    }

    private void updateCharHeight() {
//        Paint.FontMetrics fontMetrics = getPaint().getFontMetrics();
//        mCharHeight = (int) (fontMetrics.bottom - fontMetrics.top);
    }

    @Override
    public void setOnSuggestItemClickListener(@Nullable OnSuggestItemClickListener listener) {
        this.mOnSuggestItemClickListener = listener;
    }

    /**
     * <p>Gets the vertical offset used for the auto-complete drop-down list.</p>
     *
     * @return the vertical offset
     * @attr ref android.R.styleable#ListPopupWindow_dropDownVerticalOffset
     */
    public int getDropDownVerticalOffset() {
        return mPopup.getVerticalOffset();
    }

    /**
     * <p>Sets the vertical offset used for the auto-complete drop-down list.</p>
     *
     * @param offset the vertical offset
     * @attr ref android.R.styleable#ListPopupWindow_dropDownVerticalOffset
     */
    public void setDropDownVerticalOffset(int offset) {
        if (offset != mPopup.getVerticalOffset()) {
            mPopup.setVerticalOffset(offset);
        }
    }

    /**
     * <p>Gets the horizontal offset used for the auto-complete drop-down list.</p>
     *
     * @return the horizontal offset
     * @attr ref android.R.styleable#ListPopupWindow_dropDownHorizontalOffset
     */
    public int getDropDownHorizontalOffset() {
        return mPopup.getHorizontalOffset();
    }

    /**
     * <p>Sets the horizontal offset used for the auto-complete drop-down list.</p>
     *
     * @param offset the horizontal offset
     * @attr ref android.R.styleable#ListPopupWindow_dropDownHorizontalOffset
     */
    public void setDropDownHorizontalOffset(int offset) {
        if (offset != mPopup.getHorizontalOffset()) {
            mPopup.setHorizontalOffset(offset);
        }
    }

    /**
     * <p>Returns the current height for the auto-complete drop down list. This can
     * be a fixed height, or {@link ViewGroup.LayoutParams#MATCH_PARENT} to fill
     * the screen, or {@link ViewGroup.LayoutParams#WRAP_CONTENT} to fit the height
     * of the drop down's content.</p>
     *
     * @return the height for the drop down list
     * @attr ref android.R.styleable#AutoCompleteTextView_dropDownHeight
     */
    public int getDropDownHeight() {
        return mPopup.getHeight();
    }

    /**
     * <p>Sets the current height for the auto-complete drop down list. This can
     * be a fixed height, or {@link ViewGroup.LayoutParams#MATCH_PARENT} to fill
     * the screen, or {@link ViewGroup.LayoutParams#WRAP_CONTENT} to fit the height
     * of the drop down's content.</p>
     *
     * @param height the height to use
     * @attr ref android.R.styleable#AutoCompleteTextView_dropDownHeight
     */
    public void setDropDownHeight(int height) {
        mPopup.setHeight(height);
    }

    /**
     * <p>Sets the current width for the auto-complete drop down list. This can
     * be a fixed width, or {@link ViewGroup.LayoutParams#MATCH_PARENT} to fill the screen, or
     * {@link ViewGroup.LayoutParams#WRAP_CONTENT} to fit the width of its anchor view.</p>
     *
     * @param width the width to use
     * @attr ref android.R.styleable#AutoCompleteTextView_dropDownWidth
     */
    public void setDropDownWidth(int width) {
        mPopup.setWidth(width);
    }

    /**
     * <p>Indicates whether the popup menu is showing.</p>
     *
     * @return true if the popup menu is showing, false otherwise
     */
    public boolean isPopupShowing() {
        return mPopup.isShowing();
    }

    /**
     * <p>Closes the drop down if present on screen.</p>
     */
    public void dismissDropDown() {
        InputMethodManager imm = (InputMethodManager) getContext().getSystemService(Context.INPUT_METHOD_SERVICE);
        if (imm != null) {
            imm.displayCompletions(this, null);
        }
        mPopup.dismiss();

    }

    public void setAdapter(SuggestionAdapter adapter) {
        mAdapter = adapter;
        mPopup.setAdapter(mAdapter);
    }


    @SuppressLint("RestrictedApi")
    @Override
    public void onWindowFocusChanged(boolean hasWindowFocus) {
        super.onWindowFocusChanged(hasWindowFocus);
        if (!hasWindowFocus && !mPopup.isDropDownAlwaysVisible()) {
            dismissDropDown();
        }
    }

    @SuppressLint("RestrictedApi")
    @Override
    protected void onDisplayHint(int hint) {
        super.onDisplayHint(hint);
        switch (hint) {
            case INVISIBLE:
                if (!mPopup.isDropDownAlwaysVisible()) {
                    dismissDropDown();
                }
                break;
            case View.GONE:
                break;
            case View.VISIBLE:
                break;
        }
    }

    @SuppressLint("RestrictedApi")
    @Override
    protected void onFocusChanged(boolean focused, int direction, Rect previouslyFocusedRect) {
        super.onFocusChanged(focused, direction, previouslyFocusedRect);

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.N) {
            if (isTemporarilyDetached()) {
                // If we are temporarily in the detach state, then do nothing.
                return;
            }
        }
        if (!focused && !mPopup.isDropDownAlwaysVisible()) {
            dismissDropDown();
        }
    }

    @Override
    protected void onDetachedFromWindow() {
        dismissDropDown();
        super.onDetachedFromWindow();
    }

    @Override
    protected boolean setFrame(final int l, int t, final int r, int b) {
        boolean result = super.setFrame(l, t, r, b);

        if (isPopupShowing()) {
            showDropDown();
        }

        return result;
    }

    @Override
    public boolean onKeyUp(int keyCode, KeyEvent event) {
        boolean consumed = mPopup.onKeyUp(keyCode, event);
        if (consumed) {
            switch (keyCode) {
                // if the list accepts the key events and the key event
                // was a click, the text view gets the selected item
                // from the drop down as its content
                case KeyEvent.KEYCODE_ENTER:
                case KeyEvent.KEYCODE_DPAD_CENTER:
                case KeyEvent.KEYCODE_TAB:
                    if (event.hasNoModifiers()) {
                        performCompletion();
                    }
                    return true;
            }
        }

        if (isPopupShowing() && keyCode == KeyEvent.KEYCODE_TAB && event.hasNoModifiers()) {
            performCompletion();
            return true;
        }

        return super.onKeyUp(keyCode, event);
    }

    @SuppressLint("RestrictedApi")
    private void performCompletion() {
        performCompletion(-1);
    }

    @SuppressLint("RestrictedApi")
    private void performCompletion(int position) {
        if (isPopupShowing() && mAdapter != null) {
            if (position < 0) {
                position = mPopup.getSelectedItemPosition();
            }
            if (position < 0) {
                return;
            }
            SuggestItem item = mAdapter.getItem(position);
            if (mOnSuggestItemClickListener != null) {
                //noinspection ConstantConditions
                mOnSuggestItemClickListener.onClickSuggest(this, position, item);
            }
        }

        if (!mPopup.isDropDownAlwaysVisible()) {
            dismissDropDown();
        }
    }

    @Override
    public boolean onKeyDown(int keyCode, KeyEvent event) {
        if (mPopup.onKeyDown(keyCode, event)) {
            return true;
        }
        if (isPopupShowing() && keyCode == KeyEvent.KEYCODE_TAB && event.hasNoModifiers()) {
            return true;
        }
        boolean handled = super.onKeyDown(keyCode, event);

        if (handled && isPopupShowing()) {
            clearListSelection();
        }

        return handled;
    }

    /**
     * <p>Clear the list selection.  This may only be temporary, as user input will often bring
     * it back.
     */
    public void clearListSelection() {
        mPopup.clearListSelection();
    }
}
