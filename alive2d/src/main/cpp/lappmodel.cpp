#include <jni.h>
#include "LAppModel.hpp"
#include "JE.h"
#include "Log.hpp"
//
// Created by kasumi on 2025/2/28.
//

#define getModel(handle) reinterpret_cast<LAppModel *>(handle)

static void DefaultStartCallback(Csm::ACubismMotion *motion) {
    if (motion->onStartedCallee == nullptr) return;
    JNIEnv *env;
    g_VM->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);

    auto on_start_callback = static_cast<jobject>(motion->onStartedCallee);

    jstring group_str = env->NewStringUTF(motion->group.c_str());
    env->CallVoidMethod(on_start_callback, g_onStartMethod, group_str, motion->no);
}

static void DefaultFinishCallback(Csm::ACubismMotion *motion) {
    if (motion->onFinishedCallee == nullptr) return;

    JNIEnv *env;
    g_VM->GetEnv(reinterpret_cast<void **>(&env), JNI_VERSION_1_6);

    auto on_finish_callback = static_cast<jobject>(motion->onFinishedCallee);
    env->CallVoidMethod(on_finish_callback, g_onFinishMethod);
}

extern "C"
JNIEXPORT jlong JNICALL
Java_com_arkueid_alive2d_LAppModel_create(JNIEnv *env, jclass clazz) {
    return reinterpret_cast<jlong>(new LAppModel());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_destroy(JNIEnv *env, jclass clazz, jlong handle) {
    delete reinterpret_cast<LAppModel *>(handle);
    Info("deallocate cpp LAppModel(at=%x)", handle);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_loadModelJson(JNIEnv *env, jclass clazz, jlong handle,
                                                 jstring file_name) {
    const char *file_name_str = env->GetStringUTFChars(file_name, nullptr);
    getModel(handle)->LoadAssets(file_name_str);
    env->ReleaseStringUTFChars(file_name, file_name_str);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_resize(JNIEnv *env, jclass clazz, jlong handle, jint ww,
                                          jint wh) {
    getModel(handle)->Resize(ww, wh);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_draw(JNIEnv *env, jclass clazz, jlong handle) {
    getModel(handle)->Draw();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_startMotion(JNIEnv *env, jclass clazz, jlong handle,
                                               jstring group, jint no, jint priority,
                                               jobject on_start_motion_callback,
                                               jobject on_finish_motion_callback) {
    const char *group_str = env->GetStringUTFChars(group, nullptr);

    jobject sce = nullptr, fce = nullptr;
    Csm::ACubismMotion::BeganMotionCallback sc = nullptr;
    Csm::ACubismMotion::FinishedMotionCallback fc = nullptr;
    if (on_start_motion_callback != nullptr) {
        sce = env->NewGlobalRef(on_start_motion_callback);
        sc = DefaultStartCallback;
        env->DeleteLocalRef(on_start_motion_callback);
    }
    if (on_finish_motion_callback != nullptr) {
        fce = env->NewGlobalRef(on_finish_motion_callback);
        fc = DefaultFinishCallback;
        env->DeleteLocalRef(on_finish_motion_callback);
    }
    getModel(handle)->StartMotion(group_str, no, priority, sce, sc,
                                  fce, fc);
    env->ReleaseStringUTFChars(group, group_str);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_startRandomMotion(JNIEnv *env, jclass clazz, jlong handle,
                                                     jstring group, jint priority,
                                                     jobject on_start_motion_callback,
                                                     jobject on_finish_motion_callback) {

    const char *group_str = nullptr;
    if (group != nullptr) {
        group_str = env->GetStringUTFChars(group, nullptr);
    }
    jobject sce = nullptr, fce = nullptr;
    Csm::ACubismMotion::BeganMotionCallback sc = nullptr;
    Csm::ACubismMotion::FinishedMotionCallback fc = nullptr;
    if (on_start_motion_callback != nullptr) {
        sce = env->NewGlobalRef(on_start_motion_callback);
        sc = DefaultStartCallback;
        env->DeleteLocalRef(on_start_motion_callback);
    }
    if (on_finish_motion_callback != nullptr) {
        fce = env->NewGlobalRef(on_finish_motion_callback);
        fc = DefaultFinishCallback;
        env->DeleteLocalRef(on_finish_motion_callback);
    }
    getModel(handle)->StartRandomMotion(group_str, priority, sce, sc,
                                        fce, fc);
    env->ReleaseStringUTFChars(group, group_str);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_setExpression(JNIEnv *env, jclass clazz, jlong handle,
                                                 jstring expression_id) {
    const char *expression_id_str = env->GetStringUTFChars(expression_id, nullptr);
    getModel(handle)->SetExpression(expression_id_str);
    env->ReleaseStringUTFChars(expression_id, expression_id_str);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_setRandomExpression(JNIEnv *env, jclass clazz, jlong handle) {
    getModel(handle)->SetRandomExpression();
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_arkueid_alive2d_LAppModel_hitTest(JNIEnv *env, jclass clazz, jlong handle,
                                           jstring hit_area_name, jfloat x, jfloat y) {
    const char *expression_id_str = env->GetStringUTFChars(hit_area_name, nullptr);
    jboolean ret = getModel(handle)->HitTest(expression_id_str, x, y);
    env->ReleaseStringUTFChars(hit_area_name, expression_id_str);
    return ret;
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_arkueid_alive2d_LAppModel_hasMocConsistencyFromFile(JNIEnv *env, jclass clazz,
                                                             jlong handle, jstring moc_file) {
    const char *expression_id_str = env->GetStringUTFChars(moc_file, nullptr);
    jboolean ret = getModel(handle)->HasMocConsistencyFromFile(expression_id_str);
    env->ReleaseStringUTFChars(moc_file, expression_id_str);
    return ret;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_drag(JNIEnv *env, jclass clazz, jlong handle, jfloat x,
                                        jfloat y) {
    getModel(handle)->Drag(x, y);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_arkueid_alive2d_LAppModel_isMotionFinished(JNIEnv *env, jclass clazz, jlong handle) {
    return getModel(handle)->IsMotionFinished();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_setOffset(JNIEnv *env, jclass clazz, jlong handle, jfloat dx,
                                             jfloat dy) {
    getModel(handle)->SetOffset(dx, dy);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_setScale(JNIEnv *env, jclass clazz, jlong handle, jfloat scale) {
    getModel(handle)->SetScale(scale);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_rotate(JNIEnv *env, jclass clazz, jlong handle, jfloat degrees) {
    getModel(handle)->Rotate(degrees);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_setParameterValue(JNIEnv *env, jclass clazz, jlong handle,
                                                     jstring param_id, jfloat value,
                                                     jfloat weight) {
    const char *param_id_str = env->GetStringUTFChars(param_id, nullptr);
    getModel(handle)->SetParameterValue(param_id_str, value, weight);
    env->ReleaseStringUTFChars(param_id, param_id_str);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_addParameterValue(JNIEnv *env, jclass clazz, jlong handle,
                                                     jstring param_id, jfloat value) {
    const char *param_id_str = env->GetStringUTFChars(param_id, nullptr);
    getModel(handle)->AddParameterValue(param_id_str, value);
    env->ReleaseStringUTFChars(param_id, param_id_str);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_update(JNIEnv *env, jclass clazz, jlong handle) {
    getModel(handle)->Update();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_setAutoBreathEnable(JNIEnv *env, jclass clazz, jlong handle,
                                                       jboolean enable) {
    getModel(handle)->SetAutoBreathEnable(enable);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_setAutoBlinkEnable(JNIEnv *env, jclass clazz, jlong handle,
                                                      jboolean enable) {
    getModel(handle)->SetAutoBlinkEnable(enable);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_arkueid_alive2d_LAppModel_getParameterCount(JNIEnv *env, jclass clazz, jlong handle) {
    return getModel(handle)->GetParameterCount();
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_arkueid_alive2d_LAppModel_getParameter(JNIEnv *env, jclass clazz, jlong handle,
                                                jint index) {
    const char *param_id_str;
    int type;
    float value, default_value, minimum_value, maximum_value;
    getModel(handle)->GetParameter(index, param_id_str, type, value, default_value, minimum_value,
                                   maximum_value);
    jstring param_id = env->NewStringUTF(param_id_str);
    jobject param = env->AllocObject(g_parameterClass);
    env->SetObjectField(param, g_parameterId, param_id);
    env->SetIntField(param, g_parameterType, type);
    env->SetFloatField(param, g_parameterValue, value);
    env->SetFloatField(param, g_parameterDefaultValue, default_value);
    env->SetFloatField(param, g_parameterMinimumValue, minimum_value);
    env->SetFloatField(param, g_parameterMaximumValue, maximum_value);
    return param;
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_arkueid_alive2d_LAppModel_getParameterValue(JNIEnv *env, jclass clazz, jlong handle,
                                                     jint index) {
    return getModel(handle)->GetParameterValue(index);
}
extern "C"
JNIEXPORT jint JNICALL
Java_com_arkueid_alive2d_LAppModel_getPartCount(JNIEnv *env, jclass clazz, jlong handle) {
    return getModel(handle)->GetPartCount();
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_arkueid_alive2d_LAppModel_getPartId(JNIEnv *env, jclass clazz, jlong handle, jint index) {
    return env->NewStringUTF(getModel(handle)->GetPartId(index).GetRawString());
}
extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_arkueid_alive2d_LAppModel_getPartIds(JNIEnv *env, jclass clazz, jlong handle) {
    auto *model = getModel(handle);
    jobjectArray arr = env->NewObjectArray(model->GetPartCount(),
                                           env->FindClass("java/lang/String"),
                                           nullptr);
    for (int i = 0; i < model->GetPartCount(); i++) {
        jstring part_id = env->NewStringUTF(model->GetPartId(i).GetRawString());
        env->SetObjectArrayElement(arr, i, part_id);
    }
    return arr;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_setPartOpacity(JNIEnv *env, jclass clazz, jlong handle,
                                                  jint index, jfloat opacity) {
    getModel(handle)->SetPartOpacity(index, opacity);
}
struct Collector {
    int index;
    jobjectArray arr;
    JNIEnv *env;
};
extern "C"
JNIEXPORT jobject JNICALL
Java_com_arkueid_alive2d_LAppModel_hitPart(JNIEnv *env, jclass clazz, jlong handle, jfloat x,
                                           jfloat y, jboolean top_only) {
    jobjectArray arr = env->NewObjectArray(2, env->FindClass("java/lang/String"), nullptr);
    Collector collection{0, arr};
    getModel(handle)->HitPart(x, y, top_only, &collection,
                              [](void *collection, const char *part_id) {
                                  auto collector = (Collector *) collection;
                                  collector->env->SetObjectArrayElement(
                                          collector->arr,
                                          collector->index,
                                          collector->env->NewStringUTF(part_id));
                                  collector->index++;
                              });
    return arr;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_setPartScreenColor(JNIEnv *env, jclass clazz, jlong handle,
                                                      jint index, jfloat r, jfloat g, jfloat b,
                                                      jfloat a) {
    getModel(handle)->SetPartScreenColor(index, r, g, b, a);
}
extern "C"
JNIEXPORT jfloatArray JNICALL
Java_com_arkueid_alive2d_LAppModel_getPartScreenColor(JNIEnv *env, jclass clazz, jlong handle,
                                                      jint index) {
    jfloatArray arr = env->NewFloatArray(4);
    float color[4];
    getModel(handle)->GetPartScreenColor(index, color[0], color[1], color[2], color[3]);
    env->SetFloatArrayRegion(arr, 0, 4, color);
    return arr;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_setPartMultiplyColor(JNIEnv *env, jclass clazz, jlong handle,
                                                        jint index, jfloat r, jfloat g, jfloat b,
                                                        jfloat a) {
    getModel(handle)->SetPartMultiplyColor(index, r, g, b, a);
}
extern "C"
JNIEXPORT jfloatArray JNICALL
Java_com_arkueid_alive2d_LAppModel_getPartMultiplyColor(JNIEnv *env, jclass clazz, jlong handle,
                                                        jint index) {
    jfloatArray arr = env->NewFloatArray(4);
    float color[4];
    getModel(handle)->GetPartMultiplyColor(index, color[0], color[1], color[2], color[3]);
    env->SetFloatArrayRegion(arr, 0, 4, color);
    return arr;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_clearMotions(JNIEnv *env, jclass clazz, jlong handle) {
    getModel(handle)->ClearMotions();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_LAppModel_resetExpression(JNIEnv *env, jclass clazz, jlong handle) {
    getModel(handle)->ResetExpression();
}