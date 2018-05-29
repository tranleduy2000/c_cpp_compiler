adb uninstall com.duy.c.cpp.compiler.sdlplugin
adb install -r ./build/outputs/apk/release/sdlplugin-release.apk
adb shell am start -n "com.duy.c.cpp.compiler.sdlplugin/com.duy.c.cpp.compiler.sdlplugin.sdlpluginActivity" -a android.intent.action.MAIN -c android.intent.category.LAUNCHER
