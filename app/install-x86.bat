adb uninstall com.duy.c.cpp.compiler
adb install -r ./build/outputs/apk/arch_x86/release/app-arch_x86-release.apk
adb shell am start -n "com.duy.c.cpp.compiler/com.duy.ccppcompiler.filemanager.install.InstallActivity" -a android.intent.action.MAIN -c android.intent.category.LAUNCHER
