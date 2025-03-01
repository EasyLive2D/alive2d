#include <jni.h>
#include <string>
#include <GLES/gl.h>
#include "LAppAllocator.hpp"
#include "LAppPal.hpp"
#include "Log.hpp"

static Csm::CubismFramework::Option _option;
static LAppAllocator _allocator;

JavaVM *g_VM;
jclass g_parameterClass;
jclass g_onStartCallbackClass;
jclass g_onFinishCallbackClass;
jclass g_live2dClass;
jmethodID g_onStartMethod;
jmethodID g_onFinishMethod;
jmethodID g_loadFileMethod;

jfieldID g_parameterId;
jfieldID g_parameterValue;
jfieldID g_parameterDefaultValue;
jfieldID g_parameterMaximumValue;
jfieldID g_parameterMinimumValue;
jfieldID g_parameterType;


extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2D_init(JNIEnv *env, jclass clazz) {
    _option.LogFunction = LAppPal::PrintLn;
    _option.LoggingLevel = Csm::CubismFramework::Option::LogLevel_Verbose;
    Csm::CubismFramework::CleanUp();
    Csm::CubismFramework::StartUp(&_allocator, &_option);
    Csm::CubismFramework::Initialize();

    env->GetJavaVM(&g_VM);
    jclass localClass = env->FindClass("com/arkueid/alive2d/Parameter");
    g_parameterClass = (jclass)env->NewGlobalRef(localClass);
    env->DeleteLocalRef(localClass);

    localClass = env->FindClass("com/arkueid/alive2d/OnStartMotionCallback");
    g_onStartCallbackClass = (jclass)env->NewGlobalRef(localClass);
    env->DeleteLocalRef(localClass);

    localClass = env->FindClass("com/arkueid/alive2d/OnFinishMotionCallback");
    g_onFinishCallbackClass = (jclass)env->NewGlobalRef(localClass);
    env->DeleteLocalRef(localClass);

    localClass = env->FindClass("com/arkueid/alive2d/Live2D");
    g_live2dClass = (jclass)env->NewGlobalRef(localClass);
    env->DeleteLocalRef(localClass);

    g_parameterId = env->GetFieldID(g_parameterClass, "id", "Ljava/lang/String;");
    g_parameterValue = env->GetFieldID(g_parameterClass, "value", "F");
    g_parameterDefaultValue = env->GetFieldID(g_parameterClass, "defaultValue", "F");
    g_parameterMaximumValue = env->GetFieldID(g_parameterClass, "maxValue", "F");
    g_parameterMinimumValue = env->GetFieldID(g_parameterClass, "minValue", "F");
    g_parameterType = env->GetFieldID(g_parameterClass, "type", "I");

    g_onStartMethod = env->GetMethodID(g_onStartCallbackClass, "onStart", "(Ljava/lang/String;I)V");
    g_onFinishMethod = env->GetMethodID(g_onFinishCallbackClass, "onFinish", "()V");
    g_loadFileMethod = env->GetStaticMethodID(g_live2dClass, "loadFile", "(Ljava/lang/String;)[B");
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2D_dispose(JNIEnv *env, jclass clazz) {
    Csm::CubismFramework::Dispose();
    env->DeleteGlobalRef(g_parameterClass);
    env->DeleteGlobalRef(g_onFinishCallbackClass);
    env->DeleteGlobalRef(g_onStartCallbackClass);
    env->DeleteGlobalRef(g_live2dClass);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2D_clearBuffer(JNIEnv *env, jclass clazz, jfloat r, jfloat g, jfloat b,
                                            jfloat a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2D_setLogEnable(JNIEnv *env, jclass clazz, jboolean enable) {
    live2dLogEnable = enable;
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_arkueid_alive2d_Live2D_logEnable(JNIEnv *env, jclass clazz) {
    return live2dLogEnable;
}