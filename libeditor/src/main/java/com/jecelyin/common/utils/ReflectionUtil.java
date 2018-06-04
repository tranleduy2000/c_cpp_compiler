package com.jecelyin.common.utils;

import android.os.Parcel;

import org.apache.commons.lang3.ClassUtils;

import java.lang.reflect.Field;
import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;

public class ReflectionUtil {
    private Method method;
    private String className;
    private String methodName;


    public ReflectionUtil(Object object, String methodName, Class<?>[] argTypes) {
        this(object.getClass(), methodName, argTypes);
    }

    public ReflectionUtil(Class<?> cls, String methodName, Class<?>[] argTypes) {
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


    /**
     * Gets an accessible <code>Field</code> by name breaking scope
     * if requested. Superclasses/interfaces will be considered.
     *
     * @param cls         the class to reflect, must not be null
     * @param fieldName   the field name to obtain
     * @param forceAccess whether to break scope restrictions using the
     *                    <code>setAccessible</code> method. <code>False</code> will only
     *                    match public fields.
     * @return the Field object
     * @throws IllegalArgumentException if the class or field name is null
     */
    public static Field getField(final Class<?> cls, String fieldName, boolean forceAccess) {
        if (cls == null) {
            throw new IllegalArgumentException("The class must not be null");
        }
        if (fieldName == null) {
            throw new IllegalArgumentException("The field name must not be null");
        }
        // Sun Java 1.3 has a bugged implementation of getField hence we write the
        // code ourselves

        // getField() will return the Field object with the declaring class
        // set correctly to the class that declares the field. Thus requesting the
        // field on a subclass will return the field from the superclass.
        //
        // priority order for lookup:
        // searchclass private/protected/package/public
        // superclass protected/package/public
        //  private/different package blocks access to further superclasses
        // implementedinterface public

        // check up the superclass hierarchy
        for (Class<?> acls = cls; acls != null; acls = acls.getSuperclass()) {
            try {
                Field field = acls.getDeclaredField(fieldName);
                // getDeclaredField checks for non-public scopes as well
                // and it returns accurate results
                if (!Modifier.isPublic(field.getModifiers())) {
                    if (forceAccess) {
                        field.setAccessible(true);
                    } else {
                        continue;
                    }
                }
                return field;
            } catch (NoSuchFieldException ex) { // NOPMD
                // ignore
            }
        }
        // check the public interface case. This must be manually searched for
        // incase there is a public supersuperclass field hidden by a private/package
        // superclass field.
        Field match = null;
        for (Class<?> class1 : ClassUtils.getAllInterfaces(cls)) {
            try {
                Field test = ((Class<?>) class1).getField(fieldName);
                if (match != null) {
                    throw new IllegalArgumentException("Reference to field " + fieldName + " is ambiguous relative to " + cls
                            + "; a matching field exists on two or more implemented interfaces.");
                }
                match = test;
            } catch (NoSuchFieldException ex) { // NOPMD
                // ignore
            }
        }
        return match;
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
        field.setAccessible(true);
        return field.get(obj);
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
