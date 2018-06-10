package com.duy.ide.editor.view;

import android.content.Context;
import android.graphics.Paint;
import android.graphics.Rect;
import android.support.annotation.IdRes;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.text.Layout;
import android.util.AttributeSet;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.view.inputmethod.InputMethodManager;
import android.widget.AdapterView;
import android.widget.ListPopupWindow;

import com.duy.common.DLog;
import com.duy.ide.code.api.SuggestItem;
import com.duy.ide.editor.internal.suggestion.OnSuggestItemClickListener;
import com.duy.ide.editor.internal.suggestion.SuggestionAdapter;

import java.util.List;

public class SuggestionEditor extends EditActionSupportEditor
        implements OnSuggestItemClickListener {

    private static final String TAG = "SuggestionEditor";

    private int mCharHeight = 0;

    @Nullable
    private SuggestionAdapter mAdapter;
    @Nullable
    private OnSuggestItemClickListener mOnSuggestItemClickListener;
    private AdapterView.OnItemClickListener mSuggestClickListener
            = new AdapterView.OnItemClickListener() {
        @Override
        public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
            if (mAdapter != null) {
                SuggestItem description = mAdapter.getAllItems().get(position);
                onClickSuggest(SuggestionEditor.this, description);
            }
        }
    };
    private ListPopupWindow mPopup;
    @IdRes
    private int mDropDownAnchorId = View.NO_ID;

    public SuggestionEditor(Context context) {
     this(context, null, 0);
    }

    public SuggestionEditor(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public SuggestionEditor(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);

        mPopup = new ListPopupWindow(context, attrs, defStyleAttr, 0);
        mPopup.setSoftInputMode(WindowManager.LayoutParams.SOFT_INPUT_ADJUST_RESIZE);
        mPopup.setPromptPosition(ListPopupWindow.POSITION_PROMPT_BELOW);


        init(context, attrs, defStyleAttr);
    }

    private void init(Context context, AttributeSet attrs, int defStyleAttr) {
        updateCharHeight();
    }

    @Override
    public void onClickSuggest(@NonNull IEditAreaView editAreaView,
                               @NonNull SuggestItem item) {
        if (mOnSuggestItemClickListener != null) {
            mOnSuggestItemClickListener.onClickSuggest(this, item);
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
            int pos = getSelectionStart();
            int line = layout.getLineForOffset(pos);
            int baseline = layout.getLineBaseline(line);
            int ascent = layout.getLineAscent(line);

            float x = layout.getPrimaryHorizontal(pos);
            float y = baseline + ascent;

            int offsetHorizontal = (int) x + getPaddingLeft();
            setDropDownHorizontalOffset(offsetHorizontal);

            int heightVisible = getHeightVisible();
            int offsetVertical = (int) ((y + mCharHeight) - getScrollY());

            int tmp = offsetVertical + getDropDownHeight() + mCharHeight;
            if (tmp < heightVisible) {
                tmp = offsetVertical + mCharHeight / 2;
                setDropDownVerticalOffset(tmp);
            } else {
                tmp = offsetVertical - getDropDownHeight() - mCharHeight;
                setDropDownVerticalOffset(tmp);
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
        getWindowVisibleDisplayFrame(rect);
        if (DLog.DEBUG) DLog.d(TAG, "rect = " + rect);
        // 1/2 width of screen
        setDropDownWidth((int) (rect.width() * 0.6f));
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
            mAdapter.setListener(this);
        }
        mAdapter.setData(data);
        setAdapter(mAdapter);
        if (data.size() > 0) {
            onDropdownChangeSize();
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
        Paint.FontMetrics fontMetrics = getPaint().getFontMetrics();
        mCharHeight = (int) (fontMetrics.bottom - fontMetrics.top);
    }

    /**
     * @return the height of view display on screen
     */
    public int getHeightVisible() {
        Rect r = new Rect();
        // r will be populated with the coordinates of your view
        // that area still visible.
        getWindowVisibleDisplayFrame(r);
        return r.bottom - r.top;
    }

    @Override
    public void setOnSuggestItemClickListener(
            @Nullable OnSuggestItemClickListener listener) {
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
        mPopup.setVerticalOffset(offset);
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
        mPopup.setHorizontalOffset(offset);
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


}
