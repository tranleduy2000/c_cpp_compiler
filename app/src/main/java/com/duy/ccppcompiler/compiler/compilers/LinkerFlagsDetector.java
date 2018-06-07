package com.duy.ccppcompiler.compiler.compilers;

import android.support.v4.util.ArraySet;
import android.support.v4.util.Pair;

import org.apache.commons.io.IOUtils;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Set;
import java.util.regex.Pattern;

public class LinkerFlagsDetector {
    public static final LinkerFlagsDetector INSTANCE = new LinkerFlagsDetector();
    private static final Pattern FILE_NAME = Pattern.compile("[A-Za-z_.][A-Za-z0-9_.]+");
    //-lSDL2
    private static final Pattern INCLUDE_SDL = Pattern.compile("#include\\s+[<\"]SDL.h[>\"]");
    //-lGLESv1_CM
    private static final Pattern INCLUDE_GLESV1_CM
            = Pattern.compile("#include\\s+[<\"]GLES/(" + FILE_NAME.pattern() + ")[>\"]");

    //-landroid
    private static final Pattern INCLUDE_ANDROID
            = Pattern.compile("#include\\s+[<\"]android/(" + FILE_NAME.pattern() + ")[>\"]");
    //-llog
    private static final Pattern INCLUDE_ANDROID_LOG
            = Pattern.compile("#include\\s+[<\"]<android/log.h>[>\"]");
    //-lEGL
    private static final Pattern INCLUDE_EGL
            = Pattern.compile("#include\\s+[<\"]EGL/(" + FILE_NAME.pattern() + ")[>\"]");
    //-latomic
    private static final Pattern INCLUDE_ATOMIC = Pattern.compile("#include\\s+[<\"]atomic.h[>\"]");
    //-latomic
    private static final Pattern INCLUDE_MATH = Pattern.compile("#include\\s+[<\"]math.h[>\"]");


    private static final ArrayList<Pair<Pattern, String>> PATTERNS;

    static {
        PATTERNS = new ArrayList<>();
        PATTERNS.add(new Pair<>(INCLUDE_SDL, "-lSDL2"));
        PATTERNS.add(new Pair<>(INCLUDE_ANDROID, "-landroid"));
        PATTERNS.add(new Pair<>(INCLUDE_ANDROID_LOG, "-llog"));
        PATTERNS.add(new Pair<>(INCLUDE_EGL, "-lEGL"));
        PATTERNS.add(new Pair<>(INCLUDE_MATH, "-lm"));
        PATTERNS.add(new Pair<>(INCLUDE_GLESV1_CM, "-lGLESv1_CM"));
        PATTERNS.add(new Pair<>(INCLUDE_ATOMIC, "-latomic"));

    }

    private LinkerFlagsDetector() {

    }

    @SuppressWarnings("ConstantConditions")
    public Set<String> detect(File[] srcFiles) throws IOException {
        Set<String> ldFlags = new ArraySet<>();
        for (File srcFile : srcFiles) {
            final FileInputStream input = new FileInputStream(srcFile);
            final String content = IOUtils.toString(input);
            input.close();

            for (Pair<Pattern, String> pattern : PATTERNS) {
                if (pattern.first.matcher(content).find()) {
                    ldFlags.add(pattern.second);
                }
            }
        }
        return ldFlags;
    }
}
