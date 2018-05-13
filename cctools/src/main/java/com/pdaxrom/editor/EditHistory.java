package com.pdaxrom.editor;

import java.util.ArrayList;
import java.util.List;

public class EditHistory {
	private int mmPosition = 0;
	private List<EditItem> mmHistory = new ArrayList<EditItem>();
	
	public void clean() {
		mmPosition = 0;
		mmHistory = new ArrayList<EditItem>();
	}
	
	public void add(EditItem item) {
		while (mmHistory.size() >= mmPosition + 1) {
			mmHistory.remove(mmHistory.size() - 1);
		}
		mmHistory.add(item);
		mmPosition++;
	}

	public EditItem getPrevious() {
		if (mmPosition == 0) {
			return null;
		}
		mmPosition--;
		return mmHistory.get(mmPosition);
	}

	public EditItem getNext() {
		if (mmPosition == mmHistory.size()) {
			return null;
		}
		EditItem item = mmHistory.get(mmPosition);
		mmPosition++;
		return item;
	}
	
	public int getPosition() {
		return mmPosition;
	}	
}
