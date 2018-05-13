package com.pdaxrom.editor;

import java.util.regex.Matcher;

import android.text.Editable;

public class AsmSyntax extends SyntaxHighLighting {
	private final static int GREEN			= 0xff228b22;
	private final static int BRIGHTGREEN	= 0xff32cd32;
	private final static int BRIGHTYELLOW	= 0xffffff00;
	private final static int BRIGHTBLUE		= 0xff4169e1;
	private final static int BRIGHTCYAN		= 0xff00ffff;
//	private final static int RED			= 0xffff0000;
	private final static int BRIGHTRED		= 0xffff7f50;
	private final static int GRAY			= 0xffbebebe;

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
					"\\.\\b(data|subsection|text)\\b",
					BRIGHTGREEN),
		new HighLightRule(
					"\\.\\b(align|file|globl|global|hidden|section|size|type|weak|ident|arch|fpu)\\b",
					GREEN),
		new HighLightRule(
					"\\.\\b(ascii|asciz|byte|double|float|hword|int|long|short|single|struct|word|uleb)\\b"
					, BRIGHTYELLOW),
		new HighLightRule(
					"(?md)^\\s*([.0-9A-Za-z_])*:",
					BRIGHTRED
					),
		new HighLightRule(
					"(?md)^\\s*#\\s*(define|include(_next)?|(un|ifn?)def|endif|el(if|se)|if|warning|error|pragma)\\b"
					, BRIGHTCYAN),
	};

	public AsmSyntax() {
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
		case '@':
		case '/': return BRIGHTBLUE;
		case '"': 
		case '\'':
		}
		return GRAY;
	}	
}
