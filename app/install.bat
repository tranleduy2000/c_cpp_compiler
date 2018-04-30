cd app
cp build\outputs\apk\release\app-release.apk release/app-release.apk
adb uninstall com.duy.ccppcompiler
adb install -r release/app-release.apk
adb shell am start -n "com.duy.ccppcompiler/com.duy.ccppcompiler.InstallActivity" -a android.intent.action.MAIN -c android.intent.category.LAUNCHER
exit