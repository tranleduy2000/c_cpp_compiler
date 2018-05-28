package org.gjt.sp.jedit.util;

/**
 * A simple collection that stores integers and grows automatically.
 */
public class IntegerArray {

    private int[] array;
    private int len;


    public IntegerArray() {
        this(2000);
    }


    public IntegerArray(int initialSize) {
        array = new int[initialSize];
    }


    public void add(int num) {
        if (len >= array.length) {
            int[] arrayN = new int[len * 2];
            System.arraycopy(array, 0, arrayN, 0, len);
            array = arrayN;
        }

        array[len++] = num;
    }


    public final int get(int index) {
        return array[index];
    }


    public final int getSize() {
        return len;
    }


    public final void setSize(int len) {
        this.len = len;
    }


    public final void clear() {
        len = 0;
    }


    public int[] getArray() {
        return array;
    }

}
