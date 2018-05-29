adb uninstall com.duy.c.cpp.compiler.sdl
adb install -r ./build/outputs/apk/arch_arm7/release/app-arch_arm7-release.apk
adb shell am start -n "com.duy.c.cpp.compiler.sdl/com.duy.c.cpp.compiler.ui.InstallActivity" -a android.intent.action.MAIN -c android.intent.category.LAUNCHER
