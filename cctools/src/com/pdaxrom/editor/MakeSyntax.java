package com.pdaxrom.editor;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.text.Editable;

public class MakeSyntax extends SyntaxHighLighting {
	private final static int UNKNOWN		= 0;
	private final static int KEYWORD		= 1;
	private final static int VARIABLE		= 2;
	private final static int PREPROCESSOR	= 3;
	private final static int DATA			= 4;
	private final static int COMMENT		= 5;
	private final static int COLONEQU		= 6;
	private final static int TARGET		= 7;
	
	private final static int COLOR_UNKNOWN		= 0xff000000;
	private final static int COLOR_COMMENT		= 0xff2500ff;
	private final static int COLOR_PREPROCESSOR	= 0xffa52a2a;
	private final static int COLOR_VARIABLE		= 0xff2e8b57;
	private	 final static int COLOR_TARGET			= 0xff00a7da;
	
	private final static int COLOR_KEYWORD		= 0xffa020f0;
	private final static int COLOR_DATA			= 0xffff00ff;

	private HighLightRule[] mRules = {
		new HighLightRule(
					"(?md)#.*$" 
					, COMMENT),
		new HighLightRule(
					"\\b(if|ifeq|else|endif)\\b"
					, PREPROCESSOR),
		new HighLightRule(
					":="
					, COLONEQU),
		new HighLightRule(
					"(?m)^.*:[^=]"
					, TARGET),
		new HighLightRule(
					"\\$\\(.*?\\)"
					, VARIABLE),
		new HighLightRule(
					"\\b(cat|cd|chmod|chown|cp|echo|env|export|grep|install|let|ln|make|mkdir|mv|rm|sed|set|tar|touch|umask|unset)\\b"
					, PREPROCESSOR),
	};

	public MakeSyntax() {
		setRules(mRules);
	}
	
	public void exec(Editable text) {
		super.exec(text);
		// Replace two spaces from begin line to two tabs
		Matcher m = Pattern.compile("(?m)^  ").matcher(text);
		while(m.find())
			text.replace(m.start(), m.end(), "\t\t");
	}
	
	protected int getColorFor(Editable text, Matcher m, int color_id) {
		switch (color_id) {
		case VARIABLE: return COLOR_VARIABLE;
		case KEYWORD: return COLOR_KEYWORD; 
		case PREPROCESSOR: return COLOR_PREPROCESSOR;
		case DATA: return COLOR_DATA;
		case COMMENT: return COLOR_COMMENT;
		case COLONEQU: return COLOR_RED;
		case TARGET: return COLOR_TARGET;
		case UNKNOWN: return COLOR_UNKNOWN;
		}
		return COLOR_BLACK;
	}
}
