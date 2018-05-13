package com.pdaxrom.editor;

public class HighLightRule {
	private String e;
	private int c;
	
	public HighLightRule(String expr, int color) {
		e = new String(expr);
		c = color;
	}
	
	public String getExpr() {
		return e;
	}
	
	public int getColor() {
		return c;
	}
}
