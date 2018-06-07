adb uninstall com.duy.c.cpp.compiler
adb install -r arch_arm7/release/app-arch_arm7-release.apk
adb shell am start -n "com.duy.c.cpp.compiler/com.duy.ccppcompiler.ide.InstallActivity" -a android.intent.action.MAIN -c android.intent.category.LAUNCHER
