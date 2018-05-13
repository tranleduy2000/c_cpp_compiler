package com.pdaxrom.editor;

import java.util.regex.Matcher;

import android.text.Editable;

public class ShellSyntax extends SyntaxHighLighting {
	private final static int QUOTE = 1;
	private final static int COMMENT = 2;
	private final static int COMMAND = 3;
	private final static int CONDITION = 4;
	private final static int FILECONDITION = 5;
	private final static int VARIABLE = 6;
	private final static int CTRLCHARS = 7;
	private final static int EXTCOMMAND = 8;
	private final static int FUNCTION = 9;
	private final static int HEADER = 10;
	
	private final static int COLOR_QUOTE = 0xffff00ff;
	private final static int COLOR_COMMENT = 0xff2500ff;
	private final static int COLOR_COMMAND = 0xffa52a2a;
	private final static int COLOR_CONDITION = 0xff3a8d9b;
	private final static int COLOR_FILECONDITION = 0xffec0709;
	private final static int COLOR_VARIABLE = 0xff2e8b57;
	private final static int COLOR_CTRLCHARS = 0xfff5ff0b;
	private final static int COLOR_EXTCOMMAND = 0xffa52a2a;
	private final static int COLOR_FUNCTION = 0xff04cf23;
	private final static int COLOR_HEADER = 0xff6500ff;
	
	private HighLightRule[] mRules = {
			new HighLightRule(
					"(?-md)^#!.*/(ba|k|pdk)?sh[-0-9_]*" 
					, HEADER),
			new HighLightRule(
					"(?md)#.*$" 
					, COMMENT),
			new HighLightRule(
						"\"(\\\\.|[^\\\\\"])*\""
						, QUOTE),
			new HighLightRule(
						"(?md)^[0-9A-Za-z_]+\\(\\)"
						, FUNCTION),
			new HighLightRule(
						"\\b(case|do|done|elif|else|esac|exit|fi|for|function|if|in|local|read|return|select|shift|then|time|until|while)\\b"
						, COMMAND),
			new HighLightRule(
						"\\$\\{?[0-9A-Za-z_!@#$*?-]+\\}?"
						, VARIABLE),
			new HighLightRule(
						"(\\{|\\}|\\(|\\)|\\;|\\]|\\[|`|\\\\|\\$|<|>|!|=|&|\\|)"
						, CTRLCHARS),
			new HighLightRule(
						"-(eq|ne|gt|lt|ge|le|s|n|z)"
						, CONDITION),
			new HighLightRule(
						"-[Ldefgruwx]"
						, FILECONDITION),
			new HighLightRule(
						"\\b(cat|cd|chmod|chown|cp|echo|env|export|false|grep|install|let|ln|ls|make|mkdir|mv|rm|sed|set|tar|touch|true|umask|unset)\\b"
						, EXTCOMMAND),
		};

		public ShellSyntax() {
			setRules(mRules);
		}

		protected int getColorFor(Editable text, Matcher m, int color_id) {
			switch (color_id) {
			case QUOTE: return COLOR_QUOTE;
			case COMMENT: return COLOR_COMMENT;
			case COMMAND: return COLOR_COMMAND;
			case CONDITION: return COLOR_CONDITION;
			case FILECONDITION: return COLOR_FILECONDITION;
			case VARIABLE: return COLOR_VARIABLE;
			case CTRLCHARS: return COLOR_CTRLCHARS;
			case EXTCOMMAND: return COLOR_EXTCOMMAND;
			case FUNCTION: return COLOR_FUNCTION;
			case HEADER: return COLOR_HEADER;
			}
			return COLOR_BLACK;
		}
}
