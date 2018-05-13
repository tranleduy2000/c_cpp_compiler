package com.pdaxrom.editor;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import android.content.res.TypedArray;
import com.pdaxrom.editor.CSyntax;
import com.pdaxrom.editor.EditHistory;
import com.pdaxrom.editor.EditItem;
import com.pdaxrom.editor.MakeSyntax;
import com.pdaxrom.editor.SyntaxHighLighting;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.graphics.Paint.FontMetrics;
import android.graphics.Rect;
import android.os.Handler;
import android.text.Editable;
import android.text.InputFilter;
import android.text.TextWatcher;
import android.util.AttributeSet;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.EditText;

import com.pdaxrom.cctools.R;

public class CodeEditor extends EditText {
	private final static String TAG = "cctools-codeEditor";
	private		int			TYPE_DELAY = 200;
	
	private		String		m_FileName = null;
	private 	boolean 	m_textHasChanged;
	private		boolean		m_textHasChangedPrev;
	private		boolean		m_drawLineNumbers;
	private		boolean		m_drawGutterLine;
	private		boolean		m_highlightText;
	private		int			m_gutterSize;
	private		Rect		m_gutterLineRect;
	private		Paint		m_gutterLinePaint;
	private		Paint		m_gutterTextPaint;
	private		FontMetrics	fm;
	private 	Handler 	m_Handler = new Handler();
	private 	SyntaxHighLighting mSyntax;
	
	private		int			m_lastSearchPos;
	private		String		m_lastSearchText;

	private		EditHistory	m_History;
	private		boolean		m_IsUndoOrRedo;
	
	private		boolean		m_useAutoPair;
	private		boolean		m_useAutoIndent;
	
	private		CodeEditorInterface m_codeEditorInterface = null;
	
	public CodeEditor(Context context) {
		super(context);
        setDefaults(null);
	}

	public CodeEditor(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		setDefaults(attrs);
	}

	public CodeEditor(Context context, AttributeSet attrs) {
		super(context, attrs);
		setDefaults(attrs);
	}
	
    private int m_gutterTextColor;
    private int m_gutterLineColor;
    private int m_gutterPadding;

	public void	setDefaults(AttributeSet attrs) {
		resetTriggers();
		resetHistory();
		
		int maxLength = 1000000;
		InputFilter[] FilterArray = new InputFilter[1];
		FilterArray[0] = new InputFilter.LengthFilter(maxLength);
		setFilters(FilterArray);

        if (attrs != null) {
            readAttrs(attrs);
        } else {
            setDefaultAttrs();
        }

		m_gutterLineRect = new Rect();
		m_gutterLinePaint = new Paint();	
		m_gutterTextPaint = new Paint();
		m_gutterTextPaint.setTextAlign(Paint.Align.RIGHT);
		m_gutterTextPaint.setColor(m_gutterTextColor);
		m_gutterLinePaint.setColor(m_gutterLineColor);
		m_gutterTextPaint.setTextSize(getTextSize());
		fm = m_gutterTextPaint.getFontMetrics();
		
		super.setPadding(m_gutterSize, m_gutterPadding, m_gutterPadding, m_gutterPadding);
		super.setHorizontallyScrolling(true);
		addTextChangedListener(new TextWatcher() {
		    private CharSequence mmBeforeChange;
		    private CharSequence mmAfterChange;
		    private boolean disableAutoPairing = false;
		    private boolean braceLeftPressed = false; 

			public void afterTextChanged(Editable arg0) {
				if (!m_highlightText) {
					return;
				}
				m_Handler.removeCallbacks(mUpdateSyntaxTask);
				m_Handler.postDelayed(mUpdateSyntaxTask, TYPE_DELAY);
			}

			public void beforeTextChanged(CharSequence s, int start, int count,
					int after) {
				if (!m_IsUndoOrRedo) {
					mmBeforeChange = s.subSequence(start, start + count);
				}
			}

			public void onTextChanged(CharSequence s, int start, int before,
					int count) {
				//Log.i(TAG, "onTextChanged " + start + " " + before + " " + count + " " + s.length() + "[" + s.subSequence(start, start + count) + "]");
				m_textHasChanged = true;
				if (m_codeEditorInterface != null && m_textHasChanged != m_textHasChangedPrev) {
					m_codeEditorInterface.textHasChanged(m_textHasChanged);
				}
				m_textHasChangedPrev = m_textHasChanged;

				if (!m_IsUndoOrRedo) {
					mmAfterChange = s.subSequence(start, start + count);
					m_History.add(new EditItem(start, mmBeforeChange, mmAfterChange));
					
					if (mSyntax != null) {
						if (m_useAutoPair && mSyntax.isAutoPair()) {
							if (!disableAutoPairing) {
								char begChar[] = {'(', '[', '"', '\''};
								CharSequence endChar[] = {")", "]", "\"", "\'"};
								int i;
								for (i = 0; i < begChar.length; i++) {
									if (count > 0 && mmAfterChange.charAt(0) == begChar[i]) {
										disableAutoPairing = true;
										getText().insert(start + 1, endChar[i]);
										setSelection(start + 1);
									}
								}
							} else {
								disableAutoPairing = false;
							}						
						}
						
						if (m_useAutoIndent && mSyntax.isAutoIndent()) {
							if (count > 0 && mmAfterChange.charAt(0) == '\n') {
								int i;
								String addString = "";
								for (i = start - 1; i > 0 && getText().charAt(i) != '\n'; i--) {
									char c = getText().charAt(i);
									if (c == ' ' || c == '\t') {
										addString = c + addString;
									} else {
										addString = "";
									}
								}
								if (m_useAutoPair && mSyntax.isAutoPair() && braceLeftPressed) {
									braceLeftPressed = false;
									String endLine = "";
									if (getText().length() - 1 == start) {
										endLine = "\n";
									}
									getText().insert(start + 1, addString + "\t\n" + addString + "}" + endLine);
									setSelection(start + 2 + addString.length());
								} else {
									getText().insert(start + 1, addString);							
								}
							}
							if (count > 0 && mmAfterChange.charAt(0) == '{') {
								braceLeftPressed = true;
							} else {
								braceLeftPressed = false;
							}						
						}	
					}					
				}
			}
		});
		
		setOnKeyListener(new OnKeyListener() {

			public boolean onKey(View arg0, int keyCode, KeyEvent event) {
				//Log.i(TAG, "keyCode " + event.getKeyCode() + " " + event.getAction() + " " + event.getMetaState());
				if (event.getAction() == KeyEvent.ACTION_DOWN) {
					if (((event.getMetaState() & 0x7000) != 0)) {
						switch (keyCode) {
						case KeyEvent.KEYCODE_Y:
							redo();
							break;
						case KeyEvent.KEYCODE_Z:
							undo();
							break;
						default:
							return false;
						}
						return true;
					} else {
						if (keyCode == KeyEvent.KEYCODE_TAB) {
							int start = getSelectionStart();
							getText().insert(start, "\t");
							return true;
						}
					}
				}
				return false;
			}
			
		});
	}

    private void setDefaultAttrs() {
        m_gutterTextColor = getContext().getResources().getColor(R.color.grey);
        m_gutterLineColor = getContext().getResources().getColor(R.color.grey);
        m_drawLineNumbers = true;
        m_drawGutterLine = true;
        m_highlightText = true;
        m_gutterSize = 60;
        m_gutterPadding = 5;
    }

    private void readAttrs(AttributeSet attrs) {
        TypedArray a = getContext().getTheme().obtainStyledAttributes(attrs, R.styleable.CodeEditor, 0, 0);

        try {
            m_gutterTextColor = a.getColor(R.styleable.CodeEditor_gutterTextColor, getContext().getResources().getColor(R.color.grey));
            m_gutterLineColor = a.getColor(R.styleable.CodeEditor_gutterLineColor, getContext().getResources().getColor(R.color.grey));
            m_drawLineNumbers = a.getBoolean(R.styleable.CodeEditor_drawLineNumbers, true);
            m_drawGutterLine = a.getBoolean(R.styleable.CodeEditor_drawGutterLine, true);
            m_highlightText = a.getBoolean(R.styleable.CodeEditor_highlightText, true);
            m_gutterSize = (int)a.getDimension(R.styleable.CodeEditor_gutterSize, getContext().getResources().getDimension(R.dimen.gutter_size));
            m_gutterPadding = (int)a.getDimension(R.styleable.CodeEditor_gutterPadding, getContext().getResources().getDimension(R.dimen.gutter_padding));
        } finally {
            a.recycle();
        }
    }

    private void resetTriggers() {
		m_textHasChanged = false;
		if (m_codeEditorInterface != null && m_textHasChanged != m_textHasChangedPrev) {
			m_codeEditorInterface.textHasChanged(m_textHasChanged);
		}
		m_textHasChangedPrev = m_textHasChanged;
		m_lastSearchPos = 0;		
	}

	private void resetHistory() {
		if (m_History == null) {
			m_History = new EditHistory();
		} else {
			m_History.clean();
		}
		m_IsUndoOrRedo = false;		
	}
	
	private Runnable mUpdateSyntaxTask = new Runnable() {
		public void run() {
			if (mSyntax != null) {
				mSyntax.exec(getText());
			}
		}
	};
	
	public void setTextSize(float size) {
		super.setTextSize(size);
		m_gutterTextPaint.setTextSize(getTextSize());
		fm = m_gutterTextPaint.getFontMetrics();
	}
	
	public void setCodeEditorInterface(CodeEditorInterface codeEditorInterface) {
		m_codeEditorInterface = codeEditorInterface;
	}
	
	public boolean hasChanged() {
		return m_textHasChanged;
	}
		
	public void showSyntax(boolean h) {
		m_highlightText = h;
		if (mSyntax != null) {
			if (!h) {
				mSyntax.reset(getText());
			} else {
				mSyntax.exec(getText());
			}
		}
	}
	
	public	void drawLineNumbers(boolean d) {
		m_drawLineNumbers = d;
	}
	
	public	void drawGutterLine(boolean g) {
		m_drawGutterLine = g;
		if (g) {
			super.setPadding(m_gutterSize, 5, 5, 5);
		} else {
			super.setPadding(5, 5, 5, 5);
		}
	}
	
	public	void setGutterSize(int g) {
		m_gutterSize = g;
	}
	
	public	void setGutterLineColor(int a, int r, int g, int b) {
		m_gutterLinePaint.setARGB(a, r, g, b);
	}
	
	public	void setGutterTextColor(int a, int r, int g, int b) {
		m_gutterTextPaint.setARGB(a, r, g, b);
	}
	
	public	boolean isDrawingLineNumbers() {
		return m_drawLineNumbers;
	}

	public boolean isDrawingGutterLine() {
		return m_drawGutterLine;
	}
	
	public int getGutterSize() {
		return m_gutterSize;
	}
	
	public void setAutoPair(boolean val) {
		m_useAutoPair = val;
	}
	
	public boolean getAutoPair() {
		return m_useAutoPair;
	}

	public void setAutoIndent(boolean val) {
		m_useAutoIndent = val;
	}
	
	public boolean getAutoIndent() {
		return m_useAutoIndent;
	}

	public void goToLinePos(int line, int pos) {
		if (line < 1) {
			line = 1;
		}
		Matcher m = Pattern.compile("(?md)^").matcher(getText());
		int lastLine = 0;
		while (m.find()) {
			if (--line == 0) {
				setSelection(m.start() + pos);
				return;
			}
			lastLine = m.start();
		}
		setSelection(lastLine);
	}
	
	public void goToLine(int line) {
		goToLinePos(line, 0);
	}
	
	public void searchText(String s) {
		Log.i(TAG, "getSelectionStart() " + getSelectionStart());
		
		try {
			if (s.length() == 0) {
				return;
			}
			m_lastSearchPos = getText().toString().indexOf(s, getSelectionStart());
			if (m_lastSearchPos != -1) {
				//setSelection(m_lastSearchPos + s.length());
				setSelection(m_lastSearchPos + s.length(), m_lastSearchPos);
			}
			m_lastSearchText = new String(s);
		} catch (Exception e) {
			Log.e(TAG, "searchText() " + e);
		}
	}
	
	public String getLastSearchText() {
		if (m_lastSearchText == null) {
			return new String("");
		}
		return m_lastSearchText;
	}
	
	Rect drawingRect = new Rect();
	Rect originalClipRect = new Rect();

	protected void onDraw(Canvas canvas) {
		if (isDrawingGutterLine() || isDrawingLineNumbers()) {
			int lineCount = 0;
			
			int horisontalScrollOffset = this.computeHorizontalScrollOffset();
			
			if (isDrawingGutterLine()) {
				canvas.drawLine(m_gutterSize - 4 + horisontalScrollOffset, 0, m_gutterSize - 4 + horisontalScrollOffset, this.computeVerticalScrollRange() + fm.bottom, m_gutterLinePaint);
			}

			if (isDrawingLineNumbers()) {
				lineCount = this.getLineCount();
				for (int i = 0; i < lineCount; i++) {
					this.getLineBounds(i, m_gutterLineRect);
					canvas.drawText(Integer.toString(i + 1), (float) m_gutterSize - 10 + horisontalScrollOffset, 
							(float) ((m_gutterLineRect.top + m_gutterLineRect.bottom) / 2 + (m_gutterTextPaint.getTextSize() / 2)), 
							m_gutterTextPaint);

				}
			}
		}
		
		super.onDraw(canvas);
	}
		
	private void setSyntax(String file) {
		mSyntax = null;
		String main_file = (new File(file)).getName();
		if (main_file.contentEquals("Makefile") || main_file.contentEquals("makefile")) {
			mSyntax = new MakeSyntax();
		} else  if (file.lastIndexOf(".") != -1) {
	    	String ext = file.substring(file.lastIndexOf("."));
	    	//Log.i(TAG, "Extension " + ext);
	    	if (ext.contentEquals(".mk") || ext.contentEquals(".mak")) {
	    		mSyntax = new MakeSyntax();
	    	} else if (ext.contentEquals(".sh")) {
	    		mSyntax = new ShellSyntax();
	    	} else if (ext.equals(".pl") || ext.equals(".pm")) {
	    		mSyntax = new PerlSyntax();
	    	} else if (ext.contentEquals(".lua")) {
	    		mSyntax = new LuaSyntax();
	    	} else if (ext.contentEquals(".java")) {
	    		mSyntax = new JavaSyntax();
	    	} else if (ext.contentEquals(".f") || ext.contentEquals(".f90") ||
	    			   ext.contentEquals(".f95") || ext.contentEquals(".f03")) {
	    		mSyntax = new FortranSyntax();
	    	} else if (ext.contentEquals(".s") || ext.contentEquals(".S") ||
	    			   ext.contentEquals(".asm")) {
	    		mSyntax = new AsmSyntax();
	    	} else if (ext.contentEquals(".c") || ext.contentEquals(".cpp") ||
	    			   ext.contentEquals(".c++") ||
				ext.contentEquals(".h") || ext.contentEquals(".hpp") || ext.contentEquals(".h++")) {
				mSyntax = new CSyntax();
	    	}
		}
		if (mSyntax != null) {
			mSyntax.exec(getText());
		}
	}

	public void newFile() {
		setText("");
		resetTriggers();
		resetHistory();
		m_FileName = null;
	}
	
	public boolean loadFile(String file) {
		m_FileName = file;
		mSyntax = null;
		FileInputStream fis;
		try {
			fis = new FileInputStream(file);
			byte[] reader = new byte[fis.available()];
			if (fis.read(reader) > 0) {
				setText(new String(reader));
			}
			if (fis != null)
				fis.close();
			resetTriggers();
			resetHistory();
			setSyntax(file);
			return true;
		} catch (FileNotFoundException e) {
			Log.i(TAG, "File not found " + e);
		} catch (IOException e) {
			Log.i(TAG, "Error " + e);
		}
		return false;
	}
	
	public boolean saveFile(String file) {
		m_FileName = file;
		FileOutputStream fout;
		try {
			fout = new FileOutputStream(file);
			fout.write(getText().toString().getBytes());
			fout.close();
			setSyntax(file);
			resetTriggers();
			return true;
		} catch (FileNotFoundException e) {
			Log.i(TAG, "File not found " + e);
		} catch (IOException e) {
			Log.i(TAG, "Error " + e);
		}
		return false;
	}
	
	public String getFileName() {
		return m_FileName;
	}
	
	public void undo() {
		EditItem edit = m_History.getPrevious();
		if (edit == null) {
			return;
		}
		Editable text = getText();
		int start = edit.mmIndex;
		int end = start + (edit.mmAfter != null ? edit.mmAfter.length() : 0);
		m_IsUndoOrRedo = true;
		text.replace(start, end, edit.mmBefore);
		m_IsUndoOrRedo = false;
		setSelection(edit.mmBefore == null ? start : (start + edit.mmBefore.length()));
		if (m_History.getPosition() == 0) {
			m_textHasChanged = false;
			if (m_codeEditorInterface != null && m_textHasChanged != m_textHasChangedPrev) {
				m_codeEditorInterface.textHasChanged(m_textHasChanged);
			}
			m_textHasChangedPrev = m_textHasChanged;
		}
	}

	public void redo() {
		EditItem edit = m_History.getNext();
		if (edit == null) {
			return;
		}
		Editable text = getText();
		int start = edit.mmIndex;
		int end = start + (edit.mmBefore != null ? edit.mmBefore.length() : 0);
		m_IsUndoOrRedo = true;
		text.replace(start, end, edit.mmAfter);
		m_IsUndoOrRedo = false;
		setSelection(edit.mmAfter == null ? start : (start + edit.mmAfter.length()));
	}
}
