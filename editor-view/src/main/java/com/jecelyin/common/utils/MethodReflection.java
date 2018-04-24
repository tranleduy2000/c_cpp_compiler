package com.jecelyin.common.utils;

import android.os.Parcel;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

public class MethodReflection {
    private Method method;
    private String className;
    private String methodName;


    public MethodReflection(Object object, String methodName, Class<?>[] argTypes) {
        this(object.getClass(), methodName, argTypes);
    }

    public MethodReflection(Class<?> cls, String methodName, Class<?>[] argTypes) {
        className = cls.getName();
        this.methodName = methodName;

        try {
            method = cls.getDeclaredMethod(methodName, argTypes);
        } catch (Exception e) {
            try {
                method = cls.getMethod(methodName, argTypes);
            } catch (Exception e1) {
                DLog.e("MethodReflection", "Can't reflection method: " + methodName, e1);
            }
        }

        if (method != null)
            method.setAccessible(true);
    }


    public static void setFinalStatic(Field field, Object newValue) throws Exception {
        field.setAccessible(true);
        field.set(null, newValue);
    }

    public static void setFinalStatic(Class<Parcel> cls, String varName, Object value) throws Exception {
        Field f;
        try {
            f = cls.getDeclaredField(varName);
        } catch (NoSuchFieldException e) {
            f = cls.getField(varName);
        }
        setFinalStatic(f, value);
    }

    public static Object callAny(Object obj, String methodName, Class[] type, Object[] objects) throws
            NoSuchMethodException, InvocationTargetException, IllegalAccessException {
        Method method = obj.getClass().getMethod(methodName, type);
        return method.invoke(obj, objects);
    }

    public static Object getField(Object obj, String name) throws IllegalAccessException, NoSuchFieldException {
        Field field = obj.getClass().getField(name);
        Object o = field.get(obj);
        return o;
    }

    public static Object getStaticMethod(Class<?> clazz, String methodName, Class[] type, Object[] objects) throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {
        Method method = clazz.getMethod(methodName, type);
        return method.invoke(null, objects);
    }

    public static Object callGet(Object imm, String methodName) throws NoSuchMethodException, InvocationTargetException, IllegalAccessException {
        Method method = imm.getClass().getMethod(methodName, new Class[]{});
        return method.invoke(imm);
    }

    public boolean exists() {
        return method != null;
    }

    private Object call(Object instance, Object... args) throws Throwable {
        if (method == null)
            throw new NoSuchMethodException(className + "#" + methodName);

        try {
            return method.invoke(instance, args);
        } catch (Exception e) {
            if (e instanceof InvocationTargetException) {
                throw ((InvocationTargetException) e).getTargetException();
            } else {
                throw e;
            }
        }
    }

    @SuppressWarnings("unchecked")
    public <T> T get(Object instance, Object... args) throws Throwable {
        return (T) call(instance, args);
    }

    @SuppressWarnings("unchecked")
    public <T> T staticGet(Object... args) throws Throwable {
        return (T) call(null, args);
    }

    /**
     * @param instance null if static method
     * @param args
     * @throws Throwable
     */
    public void invoke(Object instance, Object... args) throws Throwable {
        get(instance, args);
    }

    public void staticInvoke(Object... args) throws Throwable {
        get(null, args);
    }
}
