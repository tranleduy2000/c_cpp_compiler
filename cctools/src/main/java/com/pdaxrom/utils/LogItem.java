package com.pdaxrom.utils;

public class LogItem {
	private String	file_;
	private int		line_;
	private int		pos_;
	private String	message_;
	private String	type_;
		
	public LogItem(String type, String file, int line, int pos, String message) {
		type_ = new String(type);
		file_ = new String(file);
		line_ = line;
		pos_ = pos;
		message_ = new String(message);
	}
	
	public void setFile(String file) {
		file_ = new String(file);
	}
	
	public String getFile() {
		return file_;
	}
	
	public void setLine(int line) {
		line_ = line;
	}
	
	public int getLine() {
		return line_;
	}
	
	public void setPos(int pos) {
		pos_ = pos;
	}
	
	public int getPos() {
		return pos_;
	}
	
	public void setMessage(String message) {
		message_ = new String(message);
	}
	
	public String getMessage() {
		return message_;
	}
	
	public void setType(String type) {
		type_ = new String(type);
	}
	
	public String getType() {
		return type_;
	}
}
