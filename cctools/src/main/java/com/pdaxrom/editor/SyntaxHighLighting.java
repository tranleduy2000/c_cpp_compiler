package com.pdaxrom.editor;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.text.Editable;
import android.text.Spannable;
import android.text.style.ForegroundColorSpan;
//import android.util.Log;

public class SyntaxHighLighting {
//	private final static String TAG = "cctools-syntaxhighlighting";

	public final int COLOR_BLACK	= 0xff000000;
	public final int COLOR_RED		= 0xffff0000;
	public final int COLOR_GREEN	= 0xff00ff00;
	public final int COLOR_BLUE		= 0xff0000ff;
	public final int COLOR_WHITE	= 0xffffffff;	
	
	private HighLightRule[] mRules; // = { new HighLightRule("int", 0x0000ff00) };

	public void setRules(HighLightRule[] rules) {
		mRules = rules;
	}
	
	public void reset(Editable text) {
		ForegroundColorSpan[] toRemoveSpans = text.getSpans(0, text.length(), ForegroundColorSpan.class);
		for (int i = 0; i < toRemoveSpans.length; i++) 
		    text.removeSpan(toRemoveSpans[i]);
	}
	
	public void exec(Editable text) {
		if (mRules == null)
			return;

		reset(text);
		
		for (HighLightRule rule: mRules) {
			Matcher m = Pattern.compile(rule.getExpr()).matcher(text);
			while (m.find()) {
				//Log.i(TAG, "Found " + m.start() + " " + m.end());
				ForegroundColorSpan[] ss = text.getSpans(m.start(), m.end(), ForegroundColorSpan.class);
				if (ss.length > 0) {
					//Log.i(TAG, "already spanned");
					continue;
				}
				text.setSpan(new ForegroundColorSpan(getColorFor(text, m, rule.getColor())), m.start(), m.end(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
			}	
		}
	}
	
	protected int getColorFor(Editable text, Matcher m, int color_id) {
		return COLOR_BLACK;
	}
	
	public boolean isAutoPair() {
		return false;
	}

	public boolean isAutoIndent() {
		return false;
	}
}
