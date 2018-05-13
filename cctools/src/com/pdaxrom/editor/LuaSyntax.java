package com.pdaxrom.editor;

import java.util.regex.Matcher;

import android.text.Editable;

public class LuaSyntax extends SyntaxHighLighting {
	private final static int UNKNOWN		= 0;
	private final static int OPERATOR		= 1;
	private final static int STATEMENT		= 2;
	private final static int KEYWORD		= 3;
	private final static int LIBRARY		= 4;
	private final static int FILEHANDLE		= 5;
	private final static int CONSTANT		= 6;
	private final static int EXTFILE		= 7;
	private final static int NUMBER			= 8;
	private final static int STRING			= 9;
	private final static int COMMENT		= 10;
	private final static int SYMBOL			= 11;
	
	private final static int COLOR_UNKNOWN		= 0xff000000;
	private final static int COLOR_OPERATOR		= 0xffffff00;
	private final static int COLOR_STATEMENT	= 0xff6396fc;
	private final static int COLOR_KEYWORD		= 0xffffff00;
	private final static int COLOR_LIBRARY		= 0xffffff00;
	private final static int COLOR_FILEHANDLE	= 0xffffff00;
	private final static int COLOR_CONSTANT		= 0xffff00ff;
	private final static int COLOR_EXTFILE		= 0xff00ff00;
	private final static int COLOR_NUMBER		= 0xffff0000;
	private final static int COLOR_STRING		= 0xffff0000;
	private final static int COLOR_COMMENT		= 0xff009000;
	private final static int COLOR_SYMBOL		= 0xffcc009c;

	private HighLightRule[] mRules = {
		new HighLightRule(
				"(?md)--[^\\[\\[].*$|(?s)--\\[\\[.*\\]\\]"
				, COMMENT
				),
		new HighLightRule(
				"(?s)\"(\\\\.|[^\\\\\"])*\"|" +
				"'([^'\\\\]|(\\\\[\\\\\"'abfnrtv\\\\]))'|'\\\\[0-3]?[0-7]{1,2}'|'\\\\x[0-9A-Fa-f]{1,2}'"
				, STRING),
		new HighLightRule(
				":|\\*\\*|\\*|/|%|\\+|-|\\^|>|>=|<|<=|~=|=|\\.\\.|\\b(not|and|or)\\b"
				, OPERATOR),
		new HighLightRule(
				"\\(|\\)|\\[|\\]|\\{|\\}"
				, SYMBOL),
		new HighLightRule(
				"\\b(do|end|while|repeat|until|if|elseif|then|else|for|in|function|local|return)\\b"
				, STATEMENT),
		new HighLightRule(
				"io\\.\\b(close|flush|input|lines|open|output|popen|read|tmpfile|type|write)\\b|" +
				"math\\.\\b(abs|acos|asin|atan2|atan|ceil|cosh|cos|deg|exp|floor|fmod|frexp|huge|ldexp|log10|log|max|min|modf|pi|pow|rad|random|randomseed|sinh|tan)\\b|" +
				"os\\.\\b(clock|date|difftime|execute|exit|getenv|remove|rename|setlocale|time|tmpname)\\b|" +
				"package\\.\\b(cpath|loaded|loadlib|path|preload|seeall)\\b|" +
				"string\\.\\b(byte|char|dump|find|format|gmatch|gsub|len|lower|match|rep|reverse|sub|upper)\\b|" +
				"table\\.\\b(concat|insert|maxn|remove|sort)\\b|" +
				"coroutine\\.\\b(create|resume|running|status|wrap|yield)\\b|" +
				"debug\\.\\b(debug|getfenv|gethook|getinfo|getlocal|getmetatable|getregistry|getupvalue|setfenv|sethook|setlocal|setmetatable|setupvalue|traceback)\\b"
				, LIBRARY),
		new HighLightRule(
				"\\b(debug|string|math|table|io|coroutine|os)\\b\\.|" +
				"\\b(_G|_VERSION|assert|collectgarbage|dofile|error|getfenv|getmetatable|ipairs|load|loadfile|module|next|pairs|pcall|print|rawequal|rawget|rawset|require|select|setfenv|setmetatable|tonumber|tostring|type|unpack|xpcall)\\b"
				, KEYWORD),
		new HighLightRule(
				"\\:\\b(close|flush|lines|read|seek|setvbuf|write)\\b"
				, FILEHANDLE),
		new HighLightRule(
				"\\b(false|nil|true)\\b"
				, CONSTANT),
		new HighLightRule(
				"\\b(dofile|require|include)\\b"
				, EXTFILE),
		new HighLightRule(
				"[0-9]+"
				, NUMBER),
	};

	public LuaSyntax() {
		setRules(mRules);
	}
		
	protected int getColorFor(Editable text, Matcher m, int color_id) {
		switch (color_id) {
		case UNKNOWN: return COLOR_UNKNOWN;
		case OPERATOR: return COLOR_OPERATOR;
		case STATEMENT: return COLOR_STATEMENT;
		case KEYWORD: return COLOR_KEYWORD;
		case LIBRARY: return COLOR_LIBRARY;
		case FILEHANDLE: return COLOR_FILEHANDLE;
		case CONSTANT: return COLOR_CONSTANT;
		case EXTFILE: return COLOR_EXTFILE;
		case NUMBER: return COLOR_NUMBER;
		case STRING: return COLOR_STRING;
		case COMMENT: return COLOR_COMMENT;
		case SYMBOL: return COLOR_SYMBOL;
		}
		return COLOR_WHITE;
	}
}
