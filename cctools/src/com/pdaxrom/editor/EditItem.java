package com.pdaxrom.editor;

public class EditItem {
	public int mmIndex;
	public CharSequence mmBefore;
	public CharSequence mmAfter;

	public EditItem(int start, CharSequence before, CharSequence after) {
		mmIndex = start;
		mmBefore = before;
	    mmAfter = after;
	}
}
