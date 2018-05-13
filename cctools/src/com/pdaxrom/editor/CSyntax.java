package com.pdaxrom.editor;

import java.util.regex.Matcher;

import android.text.Editable;

public class CSyntax extends SyntaxHighLighting {
	private final static int UNKNOWN		= 0;
	private final static int KEYWORD		= 1;
	private final static int VARIABLE		= 2;
	private final static int PREPROCESSOR	= 3;
	private final static int DATA			= 4;
	private final static int COMMENT		= 5;
	
	private final static int COLOR_UNKNOWN		= 0xff000000;
	private final static int COLOR_KEYWORD		= 0xffa52a2a;
	private final static int COLOR_VARIABLE		= 0xff2e8b57;
	private final static int COLOR_PREPROCESSOR	= 0xffa020f0;
	private final static int COLOR_DATA			= 0xffff00ff;
	private final static int COLOR_COMMENT		= 0xff2500ff;
	private HighLightRule[] mRules = {
		new HighLightRule(
					"(?md)//.*$|(?s)/\\*.*?\\*/|" 
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
					"(?md)^\\s*#\\s*(define|include(_next)?|(un|ifn?)def|endif|el(if|se)|if|warning|error|pragma)\\b"
					, PREPROCESSOR),
		new HighLightRule(
					"\\b(float|double|bool|char|int|short|long|sizeof|enum|void|auto|static|const|struct|union|typedef|extern|(un)?signed|inline)\\b|" +
					"\\b((s?size)|((u_?)?int(8|16|32|64|ptr)))_t\\b"
					, VARIABLE),
		new HighLightRule(
					"\\b(class|namespace|template|public|protected|private|typename|this|friend|virtual|using|mutable|volatile|register|explicit)\\b|" +
					"\\b(for|if|while|do|else|case|default|switch)\\b|" +
					"\\b(try|throw|catch|operator|new|delete)\\b|" +
					"\\b(goto|continue|break|return)\\b"
					, KEYWORD),
//		new HighLightRule(
//					"\\b[0-9].*"
//					, DATA)
	};

	public CSyntax() {
		setRules(mRules);
	}
	
	public void exec(Editable text) {
		super.exec(text);
	}
	
	protected int getColorFor(Editable text, Matcher m, int color_id) {
		switch (color_id) {
		case VARIABLE: return COLOR_VARIABLE;
		case KEYWORD: return COLOR_KEYWORD; 
		case PREPROCESSOR: return COLOR_PREPROCESSOR;
		case DATA: return COLOR_DATA;
		case COMMENT: return COLOR_COMMENT;
		case UNKNOWN: return decodeUnknown(text.charAt(m.start()));
		}
		return COLOR_BLACK;
	}
	
	private int decodeUnknown(char ch) {
		switch (ch) {
		case '/': return COLOR_COMMENT;
		case '"': 
		case '\'':
		case '<': return COLOR_DATA;
//		case ';': return COLOR_KEYWORD;
		}
		return COLOR_UNKNOWN;
	}
	
	public boolean isAutoPair() {
		return true;
	}

	public boolean isAutoIndent() {
		return true;
	}
}
