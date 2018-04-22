package com.duy.ccppcompiler.console;

import android.annotation.SuppressLint;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.net.wifi.WifiManager;
import android.os.Binder;
import android.os.Handler;
import android.os.IBinder;
import android.os.PowerManager;
import android.util.Log;
import android.widget.Toast;

import com.termux.terminal.EmulatorDebug;
import com.termux.terminal.TerminalSession;
import com.termux.terminal.TerminalSession.SessionChangedCallback;

import java.io.File;

public final class TermuxService extends Service implements SessionChangedCallback {

    /**
     * Intent action to launch a new terminal session. Executed from TermuxWidgetProvider.
     */
    public static final String ACTION_EXECUTE = "com.termux.service_execute";
    /**
     * Note that this is a symlink on the Android M preview.
     */
    @SuppressLint("SdCardPath")
    private static final String FILES_PATH = "/data/data/com.luoye.simpleC/files";
    public static final String PREFIX_PATH = FILES_PATH + "/usr";
    public static final String HOME_PATH = FILES_PATH + "/home";
    private static final String EXTRA_ARGUMENTS = "com.termux.execute.arguments";
    private static final String EXTRA_CURRENT_WORKING_DIRECTORY = "com.termux.execute.cwd";
    private static final int NOTIFICATION_ID = 1337;
    private static final String ACTION_STOP_SERVICE = "com.termux.service_stop";
    private static final String ACTION_LOCK_WAKE = "com.termux.service_wake_lock";
    private static final String ACTION_UNLOCK_WAKE = "com.termux.service_wake_unlock";
    private static final String EXTRA_EXECUTE_IN_BACKGROUND = "com.termux.execute.background";
    private final IBinder mBinder = new LocalBinder();
    private final Handler mHandler = new Handler();
    SessionChangedCallback mSessionChangeCallback;
    /**
     * If the user has executed the {@link #ACTION_STOP_SERVICE} intent.
     */
    private boolean mWantsToStop = false;
    private TerminalSession mTerminalSessions = null;
    /**
     * The wake lock and wifi lock are always acquired and released together.
     */
    private PowerManager.WakeLock mWakeLock;
    private WifiManager.WifiLock mWifiLock;
    private Toast toast;

    @SuppressLint("Wakelock")
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {

        String action = intent.getAction();
        if (ACTION_STOP_SERVICE.equals(action)) {
            mWantsToStop = true;
            mTerminalSessions.finishIfRunning();
            stopSelf();
        } else if (ACTION_LOCK_WAKE.equals(action)) {
            if (mWakeLock == null) {
                PowerManager pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
                mWakeLock = pm.newWakeLock(PowerManager.PARTIAL_WAKE_LOCK, EmulatorDebug.LOG_TAG);
                mWakeLock.acquire();

                // http://tools.android.com/tech-docs/lint-in-studio-2-3#TOC-WifiManager-Leak
                WifiManager wm = (WifiManager) getApplicationContext().getSystemService(Context.WIFI_SERVICE);
                mWifiLock = wm.createWifiLock(WifiManager.WIFI_MODE_FULL_HIGH_PERF, EmulatorDebug.LOG_TAG);
                mWifiLock.acquire();

            }
        } else if (ACTION_UNLOCK_WAKE.equals(action)) {
            if (mWakeLock != null) {
                mWakeLock.release();
                mWakeLock = null;

                mWifiLock.release();
                mWifiLock = null;

            }
        } else if (ACTION_EXECUTE.equals(action)) {
            Uri executableUri = intent.getData();
            String executablePath = (executableUri == null ? null : executableUri.getPath());

            String[] arguments = (executableUri == null ? null : intent.getStringArrayExtra(EXTRA_ARGUMENTS));
            String cwd = intent.getStringExtra(EXTRA_CURRENT_WORKING_DIRECTORY);

            if (intent.getBooleanExtra(EXTRA_EXECUTE_IN_BACKGROUND, false)) {
                BackgroundJob task = new BackgroundJob(cwd, executablePath, arguments, this);
            } else {
                mTerminalSessions = createTermSession(executablePath, arguments, cwd);//true使用系统环境变量

                // Transform executable path to session name, e.g. "/bin/do-something.sh" => "do something.sh".
                if (executablePath != null) {
                    int lastSlash = executablePath.lastIndexOf('/');
                    String name = (lastSlash == -1) ? executablePath : executablePath.substring(lastSlash + 1);
                    name = name.replace('-', ' ');
                    mTerminalSessions.mSessionName = name;
                }

                // Make the newly created session the current one to be displayed:

                // Launch the main Termux app, which will now show the current session:
            }
        } else if (action != null) {
            Log.e(EmulatorDebug.LOG_TAG, "Unknown TermuxService action: '" + action + "'");
        }


        // If this service really do get killed, there is no point restarting it automatically - let the user do on next
        // start of {@link Term):
        return Service.START_NOT_STICKY;
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    @Override
    public void onCreate() {

    }


    public TerminalSession getTermSession() {
        return mTerminalSessions;
    }

    @Override
    public void onDestroy() {
        if (mWakeLock != null) mWakeLock.release();
        if (mWifiLock != null) mWifiLock.release();

        stopForeground(true);
        //System.out.println("TermuxService:onDestroy");
        mTerminalSessions.finishIfRunning();

    }

    private TerminalSession createTermSession(String executablePath, String[] arguments, String cwd) {
        //System.out.println("----------------->createTermSession");
        File f = new File(HOME_PATH);
        if (!f.exists())
            f.mkdirs();

        if (cwd == null)
            cwd = HOME_PATH;

        String[] env = BackgroundJob.buildEnvironment(true, cwd);
        boolean isLoginShell = false;

        if (executablePath == null) {
            for (String shellBinary : new String[]{"login", "bash", "zsh"}) {
                File shellFile = new File(PREFIX_PATH + "/bin/" + shellBinary);
                if (shellFile.canExecute()) {
                    executablePath = shellFile.getAbsolutePath();
                    break;
                }
            }

            if (executablePath == null) {
                // Fall back to system shell as last resort:
                executablePath = "/system/bin/sh";
            }
            isLoginShell = true;
        }

        String[] processArgs = BackgroundJob.setupProcessArgs(executablePath, arguments);
        executablePath = processArgs[0];
        int lastSlashIndex = executablePath.lastIndexOf('/');
        String processName = (isLoginShell ? "-" : "") +
                (lastSlashIndex == -1 ? executablePath : executablePath.substring(lastSlashIndex + 1));

        String[] args = new String[processArgs.length];
        args[0] = processName;
        if (processArgs.length > 1)
            System.arraycopy(processArgs, 1, args, 1, processArgs.length - 1);

        TerminalSession session = new TerminalSession(executablePath, cwd, args, env, this);
        return session;
    }

    public void removeTermSession() {
        stopSelf();
    }

    @Override
    public void onTitleChanged(TerminalSession changedSession) {
        if (mSessionChangeCallback != null) mSessionChangeCallback.onTitleChanged(changedSession);
    }

    @Override
    public void onSessionFinished(final TerminalSession finishedSession) {
        if (mSessionChangeCallback != null)
            mSessionChangeCallback.onSessionFinished(finishedSession);
    }

    @Override
    public void onTextChanged(TerminalSession changedSession) {
        if (mSessionChangeCallback != null) mSessionChangeCallback.onTextChanged(changedSession);
    }

    @Override
    public void onClipboardText(TerminalSession session, String text) {
        if (mSessionChangeCallback != null) mSessionChangeCallback.onClipboardText(session, text);
    }

    @Override
    public void onBell(TerminalSession session) {
        if (mSessionChangeCallback != null) mSessionChangeCallback.onBell(session);
    }

    @Override
    public void onColorsChanged(TerminalSession session) {
        if (mSessionChangeCallback != null) mSessionChangeCallback.onColorsChanged(session);
    }

    public void onBackgroundJobExited(final BackgroundJob task) {
        stopSelf();
    }

    private void showToast(CharSequence text) {
        if (toast == null) {
            toast = Toast.makeText(this, text, Toast.LENGTH_SHORT);
        } else {
            toast.setText(text);
        }
        toast.show();
    }

    /**
     * This service is only bound from inside the same process and never uses IPC.
     */
    class LocalBinder extends Binder {
        public final TermuxService service = TermuxService.this;
    }
}
