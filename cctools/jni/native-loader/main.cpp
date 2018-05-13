/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

//BEGIN_INCLUDE(all)
#include <jni.h>
#include <errno.h>

#include <android/log.h>
#include <android_native_app_glue.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <dlfcn.h>

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-loader", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-loader", __VA_ARGS__))
#define LOGE(...) ((void)__android_log_print(ANDROID_LOG_ERROR, "native-loader", __VA_ARGS__))

static char *get_app_name(char *confName, char *str, int size)
{
    LOGI("Loading config file %s\n", confName);
    FILE *f = fopen(confName, "rb");
    if (f) {
    	char *r = fgets(str, size, f);
    	fclose(f);
    	if (r) {
    	    return r;
    	}
    }

    LOGE("Can't open file %s\n", confName);
    return NULL;
}

/**
 * This is the main entry point of a native application that is using
 * android_native_app_glue.  It runs in its own thread, with its own
 * event loop for receiving input events and doing other things.
 */
void android_main(struct android_app* state) {
    char confDir[PATH_MAX];
    char buf[PATH_MAX];
    char *nativeApp = NULL;

    // Make sure glue isn't stripped.
    app_dummy();

    JNIEnv *env;
    state->activity->vm->AttachCurrentThread(&env, 0);

    jobject me = state->activity->clazz;

    jclass acl = env->GetObjectClass(me); //class pointer of NativeActivity
    jmethodID giid = env->GetMethodID(acl, "getIntent", "()Landroid/content/Intent;");
    jobject intent = env->CallObjectMethod(me, giid); //Got our intent

    jclass icl = env->GetObjectClass(intent); //class pointer of Intent
    jmethodID gseid = env->GetMethodID(icl, "getStringExtra", "(Ljava/lang/String;)Ljava/lang/String;");

    jstring jsParam1 = (jstring) env->CallObjectMethod(intent, gseid, env->NewStringUTF("nativeApp"));

    if (jsParam1) {
	const char *param1 = env->GetStringUTFChars(jsParam1, 0);

	LOGI("nativeApp=%s\n", param1);

	if (param1) {
	    nativeApp = strdup(param1);
	}

	env->ReleaseStringUTFChars(jsParam1, param1);
    }

    state->activity->vm->DetachCurrentThread();

    if (!nativeApp) {
        getcwd(buf, sizeof(buf));
        LOGI("current path %s\n", buf);

        snprintf(confDir, sizeof(confDir), "%s/tmp/native-loader.conf", buf);
        nativeApp = get_app_name(confDir, buf, sizeof(buf));
        if (!nativeApp) {
        	LOGW("Fail-safe mode...\n");
        	nativeApp = get_app_name("/data/data/com.pdaxrom.cctools/root/tmp/native-loader.conf", buf, sizeof(buf));
        	if (!nativeApp) {
        	    nativeApp = get_app_name("/data/data/com.pdaxrom.cctools.free/cache/tmp/native-loader.conf", buf, sizeof(buf));
        	    if (!nativeApp) {
            		LOGE("Can't load native-loader.conf!\n");
            		return;
        	    }
        	}
        }
    }

    void *handle;
    void (*newMain)(struct android_app* state);
    const char *error;

    LOGI("Load native activity %s\n", nativeApp);

    handle = dlopen(nativeApp, RTLD_LAZY);
    if (!handle) {
    	LOGE("%s\n", dlerror());
    	return;
    }

    dlerror();

    *(void **) (&newMain) = dlsym(handle, "android_main");
    if ((error = dlerror()) != NULL)  {
    	LOGE("%s\n", error);
    	return;
    }

    LOGI("start!!!\n");

    (*newMain)(state);

    dlclose(handle);
}
//END_INCLUDE(all)
