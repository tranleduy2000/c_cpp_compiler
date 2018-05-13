#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <android/log.h>

#include <unistd.h>
#include <utime.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_net.h>
#include <SDL_ttf.h>

#include "utils.h"

#define  LOG_TAG    "ccsdlplugin"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

/*
 * Class:     com_pdaxrom_cctools_sdlplugin_Utils
 * Method:    setEnv
 * Signature: (Ljava/lang/String;Ljava/lang/String;Z)I
 */
JNIEXPORT jint JNICALL Java_com_pdaxrom_cctools_sdlplugin_Utils_setEnv
  (JNIEnv *env, jobject this, jstring name, jstring value, jboolean overwrite)
{
    const char *_name  = (*env)->GetStringUTFChars(env, name, 0);
    const char *_value = (*env)->GetStringUTFChars(env, value, 0);

    return setenv(_name, _value, overwrite);
}

/*
 * Class:     com_pdaxrom_cctools_sdlplugin_Utils
 * Method:    unSetEnv
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_pdaxrom_cctools_sdlplugin_Utils_unSetEnv
  (JNIEnv *env, jobject this, jstring name)
{
    const char *_name = (*env)->GetStringUTFChars(env, name, 0);

    return unsetenv(_name);
}

/*
 * Class:     com_pdaxrom_cctools_sdlplugin_Utils
 * Method:    getEnv
 * Signature: (Ljava/lang/String;)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_pdaxrom_cctools_sdlplugin_Utils_getEnv
  (JNIEnv *env, jobject this, jstring name)
{
    const char *_name = (*env)->GetStringUTFChars(env, name, 0);

    char *value = getenv(_name);
    if (!value) {
	return NULL;
    } else {
	return (*env)->NewStringUTF(env, value);
    }
}

/*
 * Class:     com_pdaxrom_cctools_sdlplugin_Utils
 * Method:    chDir
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_com_pdaxrom_cctools_sdlplugin_Utils_chDir
  (JNIEnv *env, jobject this, jstring path)
{
    const char *_path = (*env)->GetStringUTFChars(env, path, 0);
    return chdir(_path);
}

/*
 * Class:     com_pdaxrom_cctools_sdlplugin_Utils
 * Method:    getSDLVersion
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_com_pdaxrom_cctools_sdlplugin_Utils_getSDLVersion
  (JNIEnv *env, jclass this, jint lib)
{
    char buf[256];
    SDL_version version;

    switch(lib) {
    case 1:
	SDL_IMAGE_VERSION(&version);
	break;
    case 2:
	SDL_MIXER_VERSION(&version);
	break;
    case 3:
	SDL_NET_VERSION(&version);
	break;
    case 4:
	SDL_TTF_VERSION(&version);
	break;
    default:
	SDL_GetVersion(&version);
	break;
    }

    snprintf(buf, sizeof(buf), "%d.%d.%d", version.major, version.minor, version.patch);

    return (*env)->NewStringUTF(env, buf);
}
