package com.pdaxrom.editor;

import java.util.regex.Matcher;

import android.text.Editable;

public class JavaSyntax extends SyntaxHighLighting {
	private final static int GREEN			= 0xff228b22;
//	private final static int BRIGHTGREEN	= 0xff32cd32;
	private final static int BRIGHTYELLOW	= 0xffffff00;
	private final static int BRIGHTBLUE		= 0xff4169e1;
	private final static int BLUE			= 0xff0000ee;
	private final static int BRIGHTCYAN		= 0xff00ffff;
	private final static int RED			= 0xffff0000;
//	private final static int BRIGHTRED		= 0xffff7f50;
	private final static int GRAY			= 0xffbebebe;
	private final static int FUCHSIA		= 0xffff00ff;

	private final static int UNKNOWN		= 0;
	
	private HighLightRule[] mRules = {
		new HighLightRule(
					"(?md)(//|@).*$|(?s)/\\*.*?\\*/|" 
					+ 
					"\"(\\\\.|[^\\\\\"])*\"|"
					+
					"'([^'\\\\]|(\\\\[\\\\\"'abfnrtv\\\\]))'|'\\\\[0-3]?[0-7]{1,2}'|'\\\\x[0-9A-Fa-f]{1,2}'|"
					+
					"<([^= \t])*?>"
//					+
//					";"
					, UNKNOWN),
		new HighLightRule(
					"\\b(boolean|byte|char|double|float|int|long|new|short|this|transient|void)\\b",
					GREEN),
		new HighLightRule(
					"\\b(break|case|catch|continue|default|do|else|finally|for|if|return|switch|throw|try|while)\\b"
					, RED),
		new HighLightRule(
					"\\b(abstract|class|extends|final|implements|import|instanceof|interface|native|package|private|protected|public|static|strictfp|super|synchronized|throws|volatile)\\b"
					, BRIGHTCYAN),
		new HighLightRule(
					"\\b(true|false|null)\\b"
					, BRIGHTYELLOW),
	};

	public JavaSyntax() {
		setRules(mRules);
	}
	
	public void exec(Editable text) {
		super.exec(text);
	}
	
	protected int getColorFor(Editable text, Matcher m, int color_id) {
		if (color_id == UNKNOWN) {
			return decodeUnknown(text.charAt(m.start()));
		}
		return color_id;
	}
	
	private int decodeUnknown(char ch) {
		switch (ch) {
		case '@': return BLUE;
		case '/': return BRIGHTBLUE;
		case '"': 
		case '\'': return FUCHSIA;
		}
		return GRAY;
	}
	
	public boolean isAutoPair() {
		return true;
	}

	public boolean isAutoIndent() {
		return true;
	}
}
