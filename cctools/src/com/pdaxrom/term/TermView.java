package com.pdaxrom.term;

import android.content.Context;
import android.util.AttributeSet;
import android.util.DisplayMetrics;
import android.view.MotionEvent;
import android.view.inputmethod.InputMethodManager;
import jackpal.androidterm.emulatorview.EmulatorView;
import jackpal.androidterm.emulatorview.TermSession;

public class TermView extends EmulatorView {

	public TermView(Context context, AttributeSet attrs) {
		super(context, attrs);
	}

	public TermView(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
	}

	public TermView(Context context, TermSession session, DisplayMetrics metrics) {
		super(context, session, metrics);
	}

	public boolean onSingleTapUp(MotionEvent e) {
		InputMethodManager imm = (InputMethodManager) getContext()
									.getSystemService(Context.INPUT_METHOD_SERVICE);
		imm.showSoftInput(this, InputMethodManager.SHOW_FORCED);

		return super.onSingleTapUp(e);
	}
}