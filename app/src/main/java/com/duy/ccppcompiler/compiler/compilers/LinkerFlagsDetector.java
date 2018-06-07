package com.duy.ccppcompiler.compiler.compilers;

import android.support.v4.util.ArraySet;
import android.support.v4.util.Pair;

import org.apache.commons.io.IOUtils;
import org.gjt.sp.jedit.Catalog;
import org.gjt.sp.jedit.Mode;

import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Set;
import java.util.regex.Pattern;

public class LinkerFlagsDetector {
    public static final LinkerFlagsDetector INSTANCE = new LinkerFlagsDetector();
    private static final Pattern FILE_NAME = Pattern.compile("[A-Za-z_.][A-Za-z0-9_.\\-]+");


    //-lGLESv1_CM
    private static final Pattern INCLUDE_GLES_V1_CM
            = Pattern.compile("#include\\s+[<\"]" +
            "(GLES/gl\\.h|GLES/glext\\.h|GLES/glplatform\\.h|((\\S+)?gl[^2]+\\.h))" +
            "[>\"]");
    //case: gl2.h, gl2ext.h, gl2platform.h, SDL_opengles2.h, ..
    private static final Pattern INCLUDE_GLES_V2
            = Pattern.compile("#include\\s+[<\"]" +
            "(GLES2/gl2\\.h|GLES2/gl2ext\\.h|GLES2/gl2platform\\.h|((\\S+)?gl(\\S+)?2(\\S+)?\\.h))" +
            "[>\"]");
    //-lOpenSLES
    private static final Pattern INCLUDE_OPEN_SLES
            = Pattern.compile("#include\\s+[<\"]SLES/(" + FILE_NAME.pattern() + ")[>\"]");

    //-landroid
    private static final Pattern INCLUDE_ANDROID
            = Pattern.compile("#include\\s+[<\"]android/(" + FILE_NAME.pattern() + ")[>\"]");
    //-llog
    private static final Pattern INCLUDE_ANDROID_LOG
            = Pattern.compile("#include\\s+[<\"]<android/log\\.h>[>\"]");
    //-lEGL
    private static final Pattern INCLUDE_EGL
            = Pattern.compile("#include\\s+[<\"]EGL/(" + FILE_NAME.pattern() + ")[>\"]");
    //-latomic
    private static final Pattern INCLUDE_ATOMIC = Pattern.compile("#include\\s+[<\"]atomic\\.h[>\"]");
    //-latomic
    private static final Pattern INCLUDE_MATH = Pattern.compile("#include\\s+[<\"]math\\.h[>\"]");
    private static final Pattern INCLUDE_ZLIB = Pattern.compile("#include\\s+[<\"]zlib\\.h[>\"]");

    //-lSDL2
    private static final Pattern INCLUDE_SDL = Pattern.compile("#include\\s+[<\"]SDL\\.h[>\"]");
    //-lSDL2_ttf
    private static final Pattern INCLUDE_SDL_TTF = Pattern.compile("#include\\s+[<\"]SDL_ttf\\.h[>\"]");
    //-lSDL2_image
    private static final Pattern INCLUDE_SDL_IMAGE = Pattern.compile("#include\\s+[<\"]SDL_image\\.h[>\"]");
    private static final Pattern INCLUDE_SDL_MIXER = Pattern.compile("#include\\s+[<\"]SDL_mixer\\.h[>\"]");
    private static final Pattern INCLUDE_SDL_NET = Pattern.compile("#include\\s+[<\"]SDL_net\\.h[>\"]");

    private static final ArrayList<Pair<Pattern, String>> PATTERNS;
    private static final Mode C_EXT = Catalog.getModeByName("C");
    private static final Mode CPP_EXT = Catalog.getModeByName("C++");

    static {
        PATTERNS = new ArrayList<>();
        PATTERNS.add(new Pair<>(INCLUDE_ANDROID, "-landroid"));
        PATTERNS.add(new Pair<>(INCLUDE_ANDROID_LOG, "-llog"));
        PATTERNS.add(new Pair<>(INCLUDE_EGL, "-lEGL"));
        PATTERNS.add(new Pair<>(INCLUDE_MATH, "-lm"));
        PATTERNS.add(new Pair<>(INCLUDE_GLES_V1_CM, "-lGLESv1_CM"));
        PATTERNS.add(new Pair<>(INCLUDE_ATOMIC, "-latomic"));
        PATTERNS.add(new Pair<>(INCLUDE_GLES_V1_CM, "-lGLESv1_CM"));
        PATTERNS.add(new Pair<>(INCLUDE_GLES_V2, "-lGLESv2"));
        PATTERNS.add(new Pair<>(INCLUDE_OPEN_SLES, "-lOpenSLES"));
        PATTERNS.add(new Pair<>(INCLUDE_ZLIB, "-lz"));

        PATTERNS.add(new Pair<>(INCLUDE_SDL, "-lSDL2"));
        PATTERNS.add(new Pair<>(INCLUDE_SDL_TTF, "-lSDL2_ttf"));
        PATTERNS.add(new Pair<>(INCLUDE_SDL_IMAGE, "-lSDL2_image"));
        PATTERNS.add(new Pair<>(INCLUDE_SDL_MIXER, "-lSDL2_mixer"));
        PATTERNS.add(new Pair<>(INCLUDE_SDL_NET, "-lSDL2_net"));

    }

    private LinkerFlagsDetector() {

    }

    @SuppressWarnings("ConstantConditions")
    public Set<String> detect(File[] srcFiles) throws IOException {
        Set<String> ldFlags = new ArraySet<>();
        for (File srcFile : srcFiles) {
            if (!isCppOrCFile(srcFile)) {
                continue;
            }
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

    private boolean isCppOrCFile(File srcFile) {
        if (CPP_EXT.acceptFile(srcFile.getPath(), srcFile.getName())) {
            return true;
        }
        if (C_EXT.acceptFile(srcFile.getPath(), srcFile.getName())) {
            return true;
        }
        return false;
    }
}
