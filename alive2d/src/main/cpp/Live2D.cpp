#include <jni.h>
#include <string>
#include <GLES/gl.h>
#include "LAppAllocator.hpp"
#include "LAppPal.hpp"
#include "Log.hpp"

#include <Rendering/OpenGL/CubismShader_OpenGLES2.hpp>

static Csm::CubismFramework::Option option;
static LAppAllocator allocator;

JavaVM *g_VM;
jclass g_stringClass;
jmethodID g_stringConstructor;

jmethodID g_biConsumerAcceptMethod;

jclass g_integerClass;
jmethodID g_integerValueOfMethod;

jclass g_hashMapClass;
jmethodID g_hashMapConstructor;
jmethodID g_hashMapPutMethod;
jmethodID g_hashMapGetMethod;

jclass g_arrayListClass;
jmethodID g_arrayListConstructor;
jmethodID g_arrayListAddMethod;

extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2D_init(JNIEnv *env, jclass clazz) {
    option.LogFunction = LAppPal::PrintLn;
    option.LoggingLevel = Csm::CubismFramework::Option::LogLevel_Verbose;
    Csm::CubismFramework::CleanUp();
    Csm::CubismFramework::StartUp(&allocator, &option);
    Csm::CubismFramework::Initialize();

    env->GetJavaVM(&g_VM);
    jclass local = env->FindClass("java/lang/String");
    g_stringClass = (jclass) env->NewGlobalRef(local);
    env->DeleteLocalRef(local);

    g_stringConstructor = env->GetMethodID(g_stringClass, "<init>", "(Ljava/lang/String;)V");

    local = env->FindClass("java/util/function/BiConsumer");
    g_biConsumerAcceptMethod = env->GetMethodID(local, "accept",
                                                "(Ljava/lang/Object;Ljava/lang/Object;)V");
    env->DeleteLocalRef(local);

    local = env->FindClass("java/lang/Integer");
    g_integerClass = (jclass) env->NewGlobalRef(local);
    g_integerValueOfMethod = env->GetStaticMethodID(local, "valueOf", "(I)Ljava/lang/Integer;");
    env->DeleteLocalRef(local);

    local = env->FindClass("java/util/HashMap");
    g_hashMapClass = (jclass) env->NewGlobalRef(local);
    g_hashMapConstructor = env->GetMethodID(local, "<init>", "()V");
    g_hashMapPutMethod = env->GetMethodID(local, "put",
                                          "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    g_hashMapGetMethod = env->GetMethodID(local, "get", "(Ljava/lang/Object;)Ljava/lang/Object;");
    env->DeleteLocalRef(local);

    local = env->FindClass("java/util/ArrayList");
    g_arrayListClass = (jclass) env->NewGlobalRef(local);
    g_arrayListConstructor = env->GetMethodID(local, "<init>", "()V");
    g_arrayListAddMethod = env->GetMethodID(local, "add", "(Ljava/lang/Object;)Z");
    env->DeleteLocalRef(local);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2D_dispose(JNIEnv *env, jclass clazz) {
    Csm::CubismFramework::Dispose();

    env->DeleteGlobalRef(g_stringClass);
    env->DeleteGlobalRef(g_integerClass);
    env->DeleteGlobalRef(g_hashMapClass);
    env->DeleteGlobalRef(g_arrayListClass);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2D_clearBuffer(JNIEnv *env, jclass clazz, jfloat r, jfloat g, jfloat b,
                                            jfloat a) {
    glClearColor(r, g, b, a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

#include "Rendering/OpenGL/CubismRenderer_OpenGLES2.hpp"
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2D_glRelease(JNIEnv *env, jclass clazz) {
    Csm::Rendering::CubismRenderer::StaticRelease();
}