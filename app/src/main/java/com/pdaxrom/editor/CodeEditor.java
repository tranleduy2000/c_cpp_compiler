package com.pdaxrom.editor;

import android.content.Context;
import android.content.res.TypedArray;
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

import com.duy.ccppcompiler.R;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class CodeEditor extends EditText {
    private final static String TAG = "cctools-codeEditor";
    private String m_FileName = null;
    private boolean m_textHasChanged;
    private boolean m_textHasChangedPrev;
    private boolean m_drawLineNumbers;
    private boolean m_drawGutterLine;
    private boolean m_highlightText;
    private int m_gutterSize;
    private Rect m_gutterLineRect;
    private Paint m_gutterLinePaint;
    private Paint m_gutterTextPaint;
    private FontMetrics fm;
    private SyntaxHighLighting mSyntax;
    private int m_lastSearchPos;
    private String m_lastSearchText;
    private EditHistory m_History;
    private boolean m_useAutoPair;
    private boolean m_useAutoIndent;
    private CodeEditorInterface m_codeEditorInterface = null;
    private int m_gutterTextColor;
    private int m_gutterLineColor;
    private int m_gutterPadding;

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

    public void setDefaults(AttributeSet attrs) {
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
            m_gutterSize = (int) a.getDimension(R.styleable.CodeEditor_gutterSize, getContext().getResources().getDimension(R.dimen.gutter_size));
            m_gutterPadding = (int) a.getDimension(R.styleable.CodeEditor_gutterPadding, getContext().getResources().getDimension(R.dimen.gutter_padding));
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
    }

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

    public void drawLineNumbers(boolean d) {
        m_drawLineNumbers = d;
    }

    public void drawGutterLine(boolean g) {
        m_drawGutterLine = g;
        if (g) {
            super.setPadding(m_gutterSize, 5, 5, 5);
        } else {
            super.setPadding(5, 5, 5, 5);
        }
    }

    public void setGutterLineColor(int a, int r, int g, int b) {
        m_gutterLinePaint.setARGB(a, r, g, b);
    }

    public void setGutterTextColor(int a, int r, int g, int b) {
        m_gutterTextPaint.setARGB(a, r, g, b);
    }

    public boolean isDrawingLineNumbers() {
        return m_drawLineNumbers;
    }

    public boolean isDrawingGutterLine() {
        return m_drawGutterLine;
    }

    public int getGutterSize() {
        return m_gutterSize;
    }

    public void setGutterSize(int g) {
        m_gutterSize = g;
    }

    public void setAutoPair(boolean val) {
        m_useAutoPair = val;
    }


    public void setAutoIndent(boolean val) {
        m_useAutoIndent = val;
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

        return false;
    }

    public String getFilePath() {
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
        text.replace(start, end, edit.mmBefore);
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
        text.replace(start, end, edit.mmAfter);
        setSelection(edit.mmAfter == null ? start : (start + edit.mmAfter.length()));
    }
}
