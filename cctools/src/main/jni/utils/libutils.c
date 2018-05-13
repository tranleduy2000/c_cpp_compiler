#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <android/log.h>

# include <unistd.h>
# include <utime.h>

#define  LOG_TAG    "cctools-utils"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGW(...)  __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern int miniunz_main(int argc, const char *argv[]);
extern int unzip_file(const char *zipfilename, const char *dirname, const char *password, const char *logfilename, const char *filename_to_extract);
extern int get_unpacked_size(const char *filename);

JNIEXPORT jint Java_com_pdaxrom_utils_Utils_unzip(JNIEnv*  env,
                                      jobject  this,
                                      jstring file,
                                      jstring to,
                                      jstring log )
{
    const char *_file = (*env)->GetStringUTFChars(env, file, 0);
    const char *_to = (*env)->GetStringUTFChars(env, to, 0);
    const char *_log = (*env)->GetStringUTFChars(env, log, 0);
    int ret = unzip_file(_file, _to, NULL, _log, NULL);
    LOGI("miniunz returned %d", ret);
    return ret;
}

JNIEXPORT jint Java_com_pdaxrom_utils_Utils_chmod(JNIEnv*  env,
                                      jobject  this,
                                      jstring file,
                                      jint attr)
{
    const char *_file = (*env)->GetStringUTFChars(env, file, 0);
    int ret = chmod(_file, attr);
    LOGI("chmod(%s, %d) returned %d", _file, attr, ret);
    return ret;
}

JNIEXPORT jint Java_com_pdaxrom_utils_Utils_unzippedSize(JNIEnv*  env,
                                      jobject  this,
                                      jstring file)
{
    const char *_file = (*env)->GetStringUTFChars(env, file, 0);
    int ret = get_unpacked_size(_file);
    LOGI("get_unpacked_size(%s) returned %d", _file, ret);
    return ret;
}
