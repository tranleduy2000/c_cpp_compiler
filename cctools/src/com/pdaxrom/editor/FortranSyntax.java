package com.pdaxrom.editor;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.text.Editable;

public class FortranSyntax extends SyntaxHighLighting {
	private final static int QUOTE = 1;
	private final static int COMMENT = 2;
	private final static int COMMAND = 3;
	private final static int CONDITION = 4;
	private final static int FUNCTION = 5;
	private final static int STATEMENT = 6;
	private final static int NUMBER = 7;
	private final static int UNKNOWN = 8;
	
	private final static int COLOR_QUOTE		= 0xFFFFFF33;
	private final static int COLOR_COMMENT		= 0xFF0033FF;
	private final static int COLOR_COMMAND		= 0xFF33FF00;
	private final static int COLOR_CONDITION	= 0xFFFFD700;
	private final static int COLOR_FUNCTION		= 0xFF00B7EB;
	private final static int COLOR_STATEMENT	= 0xFFEE82EE;
	private final static int COLOR_NUMBER		= 0xFFFF0033;
	private final static int COLOR_UNKNOWN		= 0xff808080;

	private HighLightRule[] mRules = {
			new HighLightRule(
					"(?md)!.*$|(?s)\"(\\\\.|[^\\\\\"])*\"" 
					, UNKNOWN),
			new HighLightRule(
					"(?i)\\b(action|advance|all|allocatable|allocated|any|apostrophe|" +
					"append|asis|assign|assignment|associated|character|common|" +
					"complex|data|default|delim|dimension|double precision|" +
					"elemental|end module|end program|end subroutine|epsilon|external|file|fmt|form|format|huge|" +
					"implicit|include|index|inquire|integer|intent|interface|" +
					"intrinsic|iostat|kind|logical|module|none|null|only|" +
					"operator|optional|pack|parameter|pointer|position|private|" +
					"program|public|real|recl|recursive|subroutine|status)\\b|>" +
					"\\b(selected_int_kind)\\b|\\b(selected_real_kind)\\b"
					, COMMAND),
			new HighLightRule(
					"(?i)\\b(abs|achar|adjustl|adjustr|allocate|bit_size|call|char|" +
					"close|contains|count|cpu_time|cshift|date_and_time|" +
					"deallocate|digits|dot_product|eor|eoshift|function|iachar|" +
					"iand|ibclr|ibits|ibset|ichar|ieor|iolength|ior|ishft|ishftc|" +
					"lbound|len|len_trim|matmul|maxexponent|maxloc|maxval|merge|" +
					"minexponent|minloc|minval|mvbits|namelist|nearest|nullify|" +
					"open|pad|present|print|product|pure|quote|radix|" +
					"random_number|random_seed|range|read|readwrite|replace|" +
					"reshape|rewind|save|scan|sequence|shape|sign|size|spacing|" +
					"spread|sum|system_clock|target|transfer|transpose|trim|" +
					"ubound|unpack|verify|write|tiny|type|use|yes)\\b"
					, FUNCTION),
			new HighLightRule(
					"(?i)\\b(case|do|else|else?if|else?where|end|end?do|end?if|" +
					"end?select|forall|if|lge|lgt|lle|llt|" +
					"repeat|select case|then|where|while)\\b|" +
					"\\.\\b(and|or|eqv|neqv|not)\\b\\."
					, CONDITION),
			new HighLightRule(
					"(?i)\\b(continue|cycle|exit|go?to|result|return)\\b"
					, STATEMENT),
			new HighLightRule(
					"\\b[0-9]+\\b"
					, NUMBER),
		};

		public FortranSyntax() {
			setRules(mRules);
		}
		
		public void exec(Editable text) {
			super.exec(text);
			// Replace two spaces from begin line to six spaces
			Matcher m = Pattern.compile("(?m)^  $").matcher(text);
			while(m.find())
				text.replace(m.start(), m.end(), "      ");
		}

		protected int getColorFor(Editable text, Matcher m, int color_id) {
			switch (color_id) {
			case QUOTE: return COLOR_QUOTE;
			case COMMENT: return COLOR_COMMENT;
			case COMMAND: return COLOR_COMMAND;
			case CONDITION: return COLOR_CONDITION;
			case FUNCTION: return COLOR_FUNCTION;
			case STATEMENT: return COLOR_STATEMENT;
			case NUMBER: return COLOR_NUMBER;
			case UNKNOWN: return decodeUnknown(text.charAt(m.start()));
			}
			return COLOR_BLACK;
		}
		
		private int decodeUnknown(char ch) {
			switch (ch) {
			case '!': return COLOR_COMMENT;
			case '"': return COLOR_QUOTE;
			}
			return COLOR_UNKNOWN;
		}
		
		
}
