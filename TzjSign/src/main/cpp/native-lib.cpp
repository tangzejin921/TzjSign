#include <jni.h>
#include <string>
#include <iostream>
#include "md5.h"
#include <android/log.h>


#define TAG "test" // 这个是自定义的LOG的标识
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,TAG ,__VA_ARGS__) // 定义LOGD类型
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,TAG ,__VA_ARGS__) // 定义LOGI类型
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,TAG ,__VA_ARGS__) // 定义LOGW类型
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,TAG ,__VA_ARGS__) // 定义LOGE类型
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,TAG ,__VA_ARGS__) // 定义LOGF类型

static jclass contextClass;
static jclass signatureClass;
static jclass packageNameClass;
static jclass packageInfoClass;
/**
 * 签名信息
 */
static const char * sign;


/**
 * md5
 */
string md5(const char * str){
    return MD5((string)str).toStr();
}

/**
 * 获取context
 */
jobject getGlobalContext(JNIEnv *env){
    //获取Activity Thread的实例对象
    jclass activityThread = (jclass) env->NewGlobalRef((env)->FindClass("android/app/ActivityThread"));
//    jclass activityThread = (env)->FindClass("android/app/ActivityThread");
    jmethodID currentActivityThread = env->GetStaticMethodID(activityThread, "currentActivityThread", "()Landroid/app/ActivityThread;");
    jobject at = env->CallStaticObjectMethod(activityThread, currentActivityThread);
    //获取Application，也就是全局的Context
    jmethodID getApplication = env->GetMethodID(activityThread, "getApplication", "()Landroid/app/Application;");
    jobject context = env->CallObjectMethod(at, getApplication);
    return context;
}


/**
 * 签名
 */
extern "C"
JNIEXPORT jstring JNICALL
Java_com_tzj_so_tzjsign_UtilSign_getSign(JNIEnv *env, jclass type, jobject ctx) {

    //获取ID
    jmethodID getPackageManagerId =
            (env)->GetMethodID(contextClass, "getPackageManager",
                               "()Landroid/content/pm/PackageManager;");
    jmethodID getPackageNameId =
            (env)->GetMethodID(contextClass, "getPackageName",
                               "()Ljava/lang/String;");
    jmethodID toCharsStringId =
            (env)->GetMethodID(signatureClass, "toCharsString",
                               "()Ljava/lang/String;");
    jmethodID getPackageInfoId =
            (env)->GetMethodID(packageNameClass, "getPackageInfo",
                               "(Ljava/lang/String;I)Landroid/content/pm/PackageInfo;");
    jfieldID signaturefieldId =
            (env)->GetFieldID(packageInfoClass, "signatures",
                                                  "[Landroid/content/pm/Signature;");
    //调用方法
    jobject packageManagerObject = (env)->CallObjectMethod(ctx, getPackageManagerId);
    jstring packNameString = (jstring) (env)->CallObjectMethod(ctx, getPackageNameId);
    jobject packageInfoObject = (env)->CallObjectMethod(packageManagerObject, getPackageInfoId,packNameString, 64);
    jobjectArray signatureArray = (jobjectArray) (env)->GetObjectField(packageInfoObject,signaturefieldId);
    jobject signatureObject = (env)->GetObjectArrayElement(signatureArray, 0);

    const char* signStrng =
            (env)->GetStringUTFChars((jstring) (env)->CallObjectMethod(signatureObject, toCharsStringId), 0);

    string s = md5(signStrng);
    return env->NewStringUTF(s.data());
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_tzj_so_tzjsign_UtilSign_md5(JNIEnv *env, jclass type, jstring str_){
    const char * str = env->GetStringUTFChars(str_, 0);
    string s = md5(str);
    env->ReleaseStringUTFChars(str_, str);

    return env->NewStringUTF(s.data());
}



/**
 * 环境
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {

    JNIEnv *env = NULL;
    jint result = -1;
    if (vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK)
        return result;

    contextClass = (jclass) env->NewGlobalRef(
            (env)->FindClass("android/content/Context"));
    signatureClass = (jclass) env->NewGlobalRef(
            (env)->FindClass("android/content/pm/Signature"));
    packageNameClass = (jclass) env->NewGlobalRef(
            (env)->FindClass("android/content/pm/PackageManager"));
    packageInfoClass = (jclass) env->NewGlobalRef(
            (env)->FindClass("android/content/pm/PackageInfo"));
    jobject ctx = getGlobalContext(env);
    jstring jstr = Java_com_tzj_so_tzjsign_UtilSign_getSign(env,NULL,ctx);
    sign = env->GetStringUTFChars(jstr, 0);
    env->ReleaseStringUTFChars(jstr, sign);
    return JNI_VERSION_1_6;
}

