package com.duy.ide.editor.view;

import android.content.Context;
import android.graphics.Paint;
import android.graphics.Rect;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.text.Layout;
import android.util.AttributeSet;
import android.view.View;
import android.widget.AdapterView;

import com.duy.common.DLog;
import com.duy.ide.code.api.SuggestItem;
import com.duy.ide.editor.editor.R;
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

    public SuggestionEditor(Context context) {
        super(context);
        init(context);
    }

    public SuggestionEditor(Context context, AttributeSet attrs) {
        super(context, attrs);
        init(context);
    }

    public SuggestionEditor(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context);
    }

    private void init(Context context) {
        updateCharHeight();
        //setDropDownBackgroundDrawable(ContextCompat.getDrawable(context, R.drawable.bg_popup_suggest));
        setOnItemClickListener(mSuggestClickListener);
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

    @Override
    public void showDropDown() {
        if (!isPopupShowing()) {
            if (hasFocus()) {
                try {
                    super.showDropDown();
                } catch (Exception e) {
                    if (DLog.DEBUG) DLog.e(TAG, "showDropDown: ", e);
                }
            }
        }
    }

    @Override
    public boolean enoughToFilter() {
        return mAdapter != null;
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
            mAdapter.clearAllData(); //gc
            mAdapter.setListener(null);
            if (isPopupShowing()) {
                dismissDropDown();
            }
        }

        mAdapter = new SuggestionAdapter(getContext(), R.layout.list_item_suggest_default, data);
        mAdapter.setListener(this);

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


}
