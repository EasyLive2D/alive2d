#include <jni.h>
#include "fine-grained/Model.hpp"
#include "JE.h"
#include "Log.hpp"
//
// Created by kasumi on 2025/2/28.
//

#define GetModel(ptr) reinterpret_cast<Model *>(ptr)

static void DefaultStartCallback(Csm::ACubismMotion *motion) {
    if (motion->GetBeganMotionCustomData() == nullptr) return;
    JNIEnv *env;

    g_VM->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);

    auto on_start_callback = static_cast<jobject>(motion->GetBeganMotionCustomData());

    jstring group_str = env->NewStringUTF(motion->group.c_str());
    jobject motion_no = env->CallStaticObjectMethod(g_integerClass, g_integerValueOfMethod,
                                              motion->no);
    env->CallVoidMethod(on_start_callback, g_biConsumerAcceptMethod, group_str, motion_no);
    env->DeleteLocalRef(group_str);
    env->DeleteLocalRef(motion_no);

    env->DeleteGlobalRef(on_start_callback);
}

static void DefaultFinishCallback(Csm::ACubismMotion *motion) {
    if (motion->GetFinishedMotionCustomData() == nullptr) return;

    JNIEnv *env;
    g_VM->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);

    auto on_finish_callback = static_cast<jobject>(motion->GetFinishedMotionCustomData());
    jstring group_str = env->NewStringUTF(motion->group.c_str());
    jobject motion_no = env->CallStaticObjectMethod(g_integerClass, g_integerValueOfMethod,
                                              motion->no);

    env->CallVoidMethod(on_finish_callback, g_biConsumerAcceptMethod, group_str, motion_no);
    env->DeleteLocalRef(group_str);
    env->DeleteLocalRef(motion_no);

    env->DeleteGlobalRef(on_finish_callback);
}


extern "C"
JNIEXPORT jlong JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeCreate(JNIEnv *env, jclass clazz) {
    return reinterpret_cast<jlong>(new Model());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeDestroy(JNIEnv *env, jclass clazz, jlong handle) {
    delete reinterpret_cast<Model *>(handle);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeLoadModelJson(JNIEnv *env, jclass clazz, jlong handle,
                                                         jstring model_json) {
    const char *model_json_str = env->GetStringUTFChars(model_json, nullptr);
    GetModel(handle)->LoadModelJson(model_json_str);
    env->ReleaseStringUTFChars(model_json, model_json_str);
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeGetModelHomeDir(JNIEnv *env, jclass clazz,
                                                           jlong handle) {
    return env->NewStringUTF(GetModel(handle)->GetModelHomeDir());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeUpdate(JNIEnv *env, jclass clazz, jlong handle,
                                                  jfloat delta_time_seconds) {
    GetModel(handle)->Update(delta_time_seconds);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeUpdateMotion(JNIEnv *env, jclass clazz, jlong handle,
                                                        jfloat delta_time_seconds) {
    return GetModel(handle)->UpdateMotion(delta_time_seconds);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeUpdateDrag(JNIEnv *env, jclass clazz, jlong handle,
                                                      jfloat delta_time_seconds) {
    GetModel(handle)->UpdateDrag(delta_time_seconds);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeUpdateBreath(JNIEnv *env, jclass clazz, jlong handle,
                                                        jfloat delta_time_seconds) {
    GetModel(handle)->UpdateBreath(delta_time_seconds);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeUpdateBlink(JNIEnv *env, jclass clazz, jlong handle,
                                                       jfloat delta_time_seconds) {
    GetModel(handle)->UpdateBlink(delta_time_seconds);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeUpdateExpression(JNIEnv *env, jclass clazz, jlong handle,
                                                            jfloat delta_time_seconds) {
    GetModel(handle)->UpdateExpression(delta_time_seconds);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeUpdatePhysics(JNIEnv *env, jclass clazz, jlong handle,
                                                         jfloat delta_time_seconds) {
    GetModel(handle)->UpdatePhysics(delta_time_seconds);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeUpdatePose(JNIEnv *env, jclass clazz, jlong handle,
                                                      jfloat delta_time_seconds) {
    GetModel(handle)->UpdatePose(delta_time_seconds);
}
extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeGetParameterIds(JNIEnv *env, jclass clazz,
                                                           jlong handle) {
    auto *model = GetModel(handle);
    jobjectArray ids = env->NewObjectArray(model->GetParameterCount(), g_stringClass, nullptr);
    int index = 0;
    void *collector[5] = {env, ids, &index};
    model->GetParameterIds(collector, [](void *collector, const char *id) {
        auto *env = static_cast<JNIEnv *>(((void **) collector)[0]);
        auto array = static_cast<jobjectArray>(((void **) collector)[1]);
        int *index = static_cast<int *>(((void **) collector)[2]);
        env->SetObjectArrayElement(array,
                                   index[0]++,
                                   env->NewObject(g_stringClass, g_stringConstructor,
                                                  env->NewStringUTF(id)));
    });
    return ids;
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeGetParameterValue(JNIEnv *env, jclass clazz,
                                                             jlong handle, jint index) {
    return GetModel(handle)->GetParameterValue(index);
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeGetParameterMaximumValue(JNIEnv *env, jclass clazz,
                                                                    jlong handle, jint index) {
    return GetModel(handle)->GetParameterMaximumValue(index);
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeGetParameterMinimumValue(JNIEnv *env, jclass clazz,
                                                                    jlong handle, jint index) {
    return GetModel(handle)->GetParameterMinimumValue(index);
}
extern "C"
JNIEXPORT jfloat JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeGetParameterDefaultValue(JNIEnv *env, jclass clazz,
                                                                    jlong handle, jint index) {
    return GetModel(handle)->GetParameterDefaultValue(index);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeSetParameterValue(JNIEnv *env, jclass clazz,
                                                             jlong handle, jint index, jfloat value,
                                                             jfloat weight) {
    GetModel(handle)->SetParameterValue(index, value, weight);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeSetParameterValueById(JNIEnv *env, jclass clazz,
                                                                 jlong handle, jstring param_id,
                                                                 jfloat value, jfloat weight) {
    const char *param_id_str = env->GetStringUTFChars(param_id, nullptr);
    GetModel(handle)->SetParameterValue(param_id_str, value, weight);
    env->ReleaseStringUTFChars(param_id, param_id_str);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeAddParameterValue(JNIEnv *env, jclass clazz,
                                                             jlong handle, jint index,
                                                             jfloat value) {
    GetModel(handle)->AddParameterValue(index, value);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeAddParameterValueById(JNIEnv *env, jclass clazz,
                                                                 jlong handle, jstring param_id,
                                                                 jfloat value) {
    const char *param_id_str = env->GetStringUTFChars(param_id, nullptr);
    GetModel(handle)->AddParameterValue(param_id_str, value);
    env->ReleaseStringUTFChars(param_id, param_id_str);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeLoadParameters(JNIEnv *env, jclass clazz, jlong handle) {
    GetModel(handle)->LoadParameters();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeSaveParameters(JNIEnv *env, jclass clazz, jlong handle) {
    GetModel(handle)->SaveParameters();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeResize(JNIEnv *env, jclass clazz, jlong handle,
                                                  jint width, jint height) {
    GetModel(handle)->Resize(width, height);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeSetOffset(JNIEnv *env, jclass clazz, jlong handle,
                                                     jfloat x, jfloat y) {
    GetModel(handle)->SetOffset(x, y);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeRotate(JNIEnv *env, jclass clazz, jlong handle,
                                                  jfloat degrees) {
    GetModel(handle)->Rotate(degrees);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeSetScale(JNIEnv *env, jclass clazz, jlong handle,
                                                    jfloat scale) {
    GetModel(handle)->SetScale(scale);
}
extern "C"
JNIEXPORT jfloatArray JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeGetMvp(JNIEnv *env, jclass clazz, jlong handle) {
    jfloatArray mvp = env->NewFloatArray(16);
    env->SetFloatArrayRegion(mvp, 0, 16, GetModel(handle)->GetMvp());
    return mvp;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeStartMotion(JNIEnv *env, jclass clazz, jlong handle,
                                                       jstring group, jint no, jint priority,
                                                       jobject on_start, jobject on_finish) {
    const char *group_str = env->GetStringUTFChars(group, nullptr);
    GetModel(handle)->StartMotion(
            group_str,
            no,
            priority,
            env->NewGlobalRef(on_start),
            DefaultStartCallback,
            env->NewGlobalRef(on_finish),
            DefaultFinishCallback
    );
    env->ReleaseStringUTFChars(group, group_str);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeStartRandomMotion(JNIEnv *env, jclass clazz,
                                                             jlong handle, jstring group,
                                                             jint priority, jobject on_start,
                                                             jobject on_finish) {
    const char *group_str = group == nullptr ? nullptr : env->GetStringUTFChars(group, nullptr);
    GetModel(handle)->StartRandomMotion(
            group_str,
            priority,
            env->NewGlobalRef(on_start),
            DefaultStartCallback,
            env->NewGlobalRef(on_finish),
            DefaultFinishCallback
    );
    if (group_str != nullptr) {
        env->ReleaseStringUTFChars(group, group_str);
    }
}


extern "C"
JNIEXPORT jboolean JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeIsMotionFinished(JNIEnv *env, jclass clazz,
                                                            jlong handle) {
    return GetModel(handle)->IsMotionFinished();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeLoadExtraMotion(JNIEnv *env, jclass clazz, jlong handle,
                                                           jstring group, jint no,
                                                           jstring motion_json_path) {
    const char *motion_json_path_str = env->GetStringUTFChars(motion_json_path, nullptr);
    const char *group_str = env->GetStringUTFChars(group, nullptr);

    GetModel(handle)->LoadExtraMotion(group_str, no, motion_json_path_str);

    env->ReleaseStringUTFChars(motion_json_path, motion_json_path_str);
    env->ReleaseStringUTFChars(group, group_str);
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeGetMotions(JNIEnv *env, jclass clazz, jlong handle) {
    jobject map = env->NewObject(g_hashMapClass, g_hashMapConstructor);
    void *collector[2] = {env, map};
    GetModel(handle)->GetMotions(collector,
                                 [](void *collector, const char *group, int no, const char *file,
                                    const char *sound) {
                                     auto *env = static_cast<JNIEnv *>(((void **) collector)[0]);
                                     auto map = static_cast<jobject>(((void **) collector)[1]);
                                     // map.get
                                     jobject group_jstr = env->NewStringUTF(group);
                                     jobject motions = env->CallObjectMethod(map,
                                                                             g_hashMapGetMethod,
                                                                             group_jstr);
                                     if (motions == nullptr) {
                                         motions = env->NewObject(g_arrayListClass,
                                                                  g_arrayListConstructor);
                                         // map.put
                                         env->CallVoidMethod(map, g_hashMapPutMethod, group_jstr,
                                                             motions);
                                     }
                                     jobject motion = env->NewObject(g_hashMapClass,
                                                                     g_hashMapConstructor);
                                     // map.put file
                                     env->CallVoidMethod(motion, g_hashMapPutMethod,
                                                         env->NewStringUTF("File"),
                                                         env->NewStringUTF(file));
                                     // map.put sound
                                     env->CallVoidMethod(motion, g_hashMapPutMethod,
                                                         env->NewStringUTF("Sound"),
                                                         env->NewStringUTF(sound));
                                     env->CallVoidMethod(motions, g_arrayListAddMethod, motion);

                                     env->DeleteLocalRef(group_jstr);
                                     env->DeleteLocalRef(motions);
                                     env->DeleteLocalRef(motion);
                                 });

    return map;
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeHitPart(JNIEnv *env, jclass clazz, jlong handle,
                                                   jfloat x, jfloat y, jboolean top_only) {
    jobject arrayList = env->NewObject(g_arrayListClass, g_arrayListConstructor);
    void *collector[2] = {env, arrayList};
    GetModel(handle)->HitPart(x, y, collector,
                              [](void *collector, const char *id) {
                                  auto *env = static_cast<JNIEnv *>(((void **) collector)[0]);
                                  auto arrayList = static_cast<jobject>(((void **) collector)[1]);
                                  jobject id_jstr = env->NewStringUTF(id);
                                  env->CallVoidMethod(arrayList, g_arrayListAddMethod,
                                                      id_jstr);
                                  env->DeleteLocalRef(id_jstr);
                              }, top_only);
    return arrayList;
}
extern "C"
JNIEXPORT jobject JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeHitDrawable(JNIEnv *env, jclass clazz, jlong handle,
                                                       jfloat x, jfloat y, jboolean top_only) {

    jobject arrayList = env->NewObject(g_arrayListClass, g_arrayListConstructor);
    void *collector[2] = {env, arrayList};
    GetModel(handle)->HitDrawable(x, y, collector,
                                  [](void *collector, const char *id) {
                                      auto *env = static_cast<JNIEnv *>(((void **) collector)[0]);
                                      auto arrayList = static_cast<jobject>(((void **) collector)[1]);
                                      jobject drawable_id = env->NewStringUTF(id);
                                      env->CallVoidMethod(arrayList, g_arrayListAddMethod,
                                                          drawable_id);
                                      env->DeleteLocalRef(drawable_id);
                                  }, top_only);
    return arrayList;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeDrag(JNIEnv *env, jclass clazz, jlong handle, jfloat x,
                                                jfloat y) {
    GetModel(handle)->Drag(x, y);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeIsAreaHit(JNIEnv *env, jclass clazz, jlong handle,
                                                     jstring area_name, jfloat x, jfloat y) {
    const char *area_name_str = env->GetStringUTFChars(area_name, nullptr);
    bool ret = GetModel(handle)->IsAreaHit(env->GetStringUTFChars(area_name, nullptr), x, y);
    env->ReleaseStringUTFChars(area_name, area_name_str);
    return ret;
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeIsPartHit(JNIEnv *env, jclass clazz, jlong handle,
                                                     jint index, jfloat x, jfloat y) {
    return GetModel(handle)->IsPartHit(index, x, y);
}
extern "C"
JNIEXPORT jboolean JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeIsDrawableHit(JNIEnv *env, jclass clazz, jlong handle,
                                                         jint index, jfloat x, jfloat y) {
    return GetModel(handle)->IsDrawableHit(index, x, y);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeCreateRenderer(JNIEnv *env, jclass clazz, jlong handle,
                                                          jint mask_buffer_count) {
    GetModel(handle)->CreateRenderer(mask_buffer_count);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeDraw(JNIEnv *env, jclass clazz, jlong handle) {
    GetModel(handle)->Draw();
}
extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeGetPartIds(JNIEnv *env, jclass clazz, jlong handle) {
    jobjectArray array = env->NewObjectArray(GetModel(handle)->GetPartCount(), g_stringClass,
                                             nullptr);
    int index = 0;
    void *collector[3] = {env, array, &index};
    GetModel(handle)->GetPartIds(collector, [](void *collector, const char *id) {
        auto *env = static_cast<JNIEnv *>(((void **) collector)[0]);
        auto array = static_cast<jobjectArray>(((void **) collector)[1]);
        auto *index = static_cast<int *>(((void **) collector)[2]);
        jobject id_jstr = env->NewStringUTF(id);
        env->SetObjectArrayElement(array, index[0]++, id_jstr);
        env->DeleteLocalRef(id_jstr);
    });
    return array;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeSetPartOpacity(JNIEnv *env, jclass clazz, jlong handle,
                                                          jint index, jfloat opacity) {
    GetModel(handle)->SetPartOpacity(index, opacity);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeSetPartScreenColor(JNIEnv *env, jclass clazz,
                                                              jlong handle, jint index, jfloat r,
                                                              jfloat g, jfloat b, jfloat a) {
    GetModel(handle)->SetPartScreenColor(index, r, g, b, a);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeSetPartMultiplyColor(JNIEnv *env, jclass clazz,
                                                                jlong handle, jint index, jfloat r,
                                                                jfloat g, jfloat b, jfloat a) {
    GetModel(handle)->SetPartMultiplyColor(index, r, g, b, a);
}
extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeGetDrawableIds(JNIEnv *env, jclass clazz, jlong handle) {
    jobjectArray array = env->NewObjectArray(GetModel(handle)->GetDrawableCount(), g_stringClass,
                                             nullptr);
    int index = 0;
    void *collector[3] = {env, array, &index};
    GetModel(handle)->GetDrawableIds(collector, [](void *collector, const char *id) {
        auto *env = static_cast<JNIEnv *>(((void **) collector)[0]);
        auto array = static_cast<jobjectArray>(((void **) collector)[1]);
        auto *index = static_cast<int *>(((void **) collector)[2]);
        jobject id_jstr = env->NewStringUTF(id);
        env->SetObjectArrayElement(array, index[0]++, id_jstr);
        env->DeleteLocalRef(id_jstr);
    });

    return array;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeSetExpression(JNIEnv *env, jclass clazz, jlong handle,
                                                         jstring expression_id) {
    const char *expression_id_str = env->GetStringUTFChars(expression_id, nullptr);
    GetModel(handle)->SetExpression(expression_id_str);
    env->ReleaseStringUTFChars(expression_id, expression_id_str);
}
extern "C"
JNIEXPORT jobjectArray JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeGetExpressions(JNIEnv *env, jclass clazz, jlong handle) {
    jobjectArray array = env->NewObjectArray(GetModel(handle)->GetExpressionCount(), g_hashMapClass,
                                             nullptr);
    int index = 0;
    void *collector[3] = {env, array, &index};
    GetModel(handle)->GetExpressions(collector,
                                     [](void *collector, const char *id, const char *file) {
                                         auto *env = static_cast<JNIEnv *>(((void **) collector)[0]);
                                         auto array = static_cast<jobjectArray>(((void **) collector)[1]);
                                         auto *index = static_cast<int *>(((void **) collector)[2]);
                                         jstring id_jstr = env->NewStringUTF(id);
                                         jstring file_jstr = env->NewStringUTF(file);
                                         jobject map = env->NewObject(g_hashMapClass,
                                                                      g_hashMapConstructor);

                                         env->CallVoidMethod(map, g_hashMapPutMethod,
                                                             env->NewStringUTF("Name"), id_jstr);
                                         env->CallVoidMethod(map, g_hashMapPutMethod,
                                                             env->NewStringUTF("File"), file_jstr);
                                         env->SetObjectArrayElement(array, index[0]++, map);

                                         env->DeleteLocalRef(id_jstr);
                                         env->DeleteLocalRef(file_jstr);
                                         env->DeleteLocalRef(map);
                                     });
    return array;
}
extern "C"
JNIEXPORT jstring JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeSetRandomExpression(JNIEnv *env, jclass clazz,
                                                               jlong handle) {
    const char *expression_id_str = GetModel(handle)->SetRandomExpression();
    return expression_id_str == nullptr ? nullptr : env->NewStringUTF(expression_id_str);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeResetExpression(JNIEnv *env, jclass clazz,
                                                           jlong handle) {
    GetModel(handle)->ResetExpression();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeStopAllMotions(JNIEnv *env, jclass clazz, jlong handle) {
    GetModel(handle)->StopAllMotions();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeResetAllParameters(JNIEnv *env, jclass clazz,
                                                              jlong handle) {
    GetModel(handle)->ResetAllParameters();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeResetPose(JNIEnv *env, jclass clazz, jlong handle) {
    GetModel(handle)->ResetPose();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeSetDrawableMultiColor(JNIEnv *env, jclass clazz,
                                                                 jlong handle, jint index, jfloat r,
                                                                 jfloat g, jfloat b, jfloat a) {
    GetModel(handle)->SetDrawableMultiColor(index, r, g, b, a);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeSetDrawableScreenColor(JNIEnv *env, jclass clazz,
                                                                  jlong handle, jint index,
                                                                  jfloat r, jfloat g, jfloat b,
                                                                  jfloat a) {
    GetModel(handle)->SetDrawableScreenColor(index, r, g, b, a);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeResetExpressions(JNIEnv *env, jclass clazz,
                                                            jlong handle) {
    GetModel(handle)->ResetExpressions();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeRemoveExpression(JNIEnv *env, jclass clazz, jlong handle,
                                                            jstring expression_id) {
    const char* expression_id_c_str = env->GetStringUTFChars(expression_id, nullptr);
    GetModel(handle)->RemoveExpression(expression_id_c_str);
    env->ReleaseStringUTFChars(expression_id, expression_id_c_str);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeAddExpression(JNIEnv *env, jclass clazz, jlong handle,
                                                         jstring expression_id) {
    const char* expression_id_c_str = env->GetStringUTFChars(expression_id, nullptr);
    GetModel(handle)->AddExpression(expression_id_c_str);
    env->ReleaseStringUTFChars(expression_id, expression_id_c_str);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeDestroyRenderer(JNIEnv *env, jclass clazz,
                                                           jlong handle) {
    GetModel(handle)->DestroyRenderer();
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeSetAndSaveParameterValue(JNIEnv *env, jclass clazz,
                                                                    jlong handle, jint index,
                                                                    jfloat value, jfloat weight) {
    GetModel(handle)->SetAndSaveParameterValue(index, value, weight);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeSetAndSaveParameterValueById(JNIEnv *env, jclass clazz,
                                                                        jlong handle, jstring id,
                                                                        jfloat value,
                                                                        jfloat weight) {
    const char* id_c_str = env->GetStringUTFChars(id, nullptr);
    GetModel(handle)->SetAndSaveParameterValue(id_c_str, value, weight);
    env->ReleaseStringUTFChars(id, id_c_str);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeAddAndSaveParameterValue(JNIEnv *env, jclass clazz,
                                                                    jlong handle, jint index,
                                                                    jfloat value) {
    GetModel(handle)->AddAndSaveParameterValue(index, value);
}
extern "C"
JNIEXPORT void JNICALL
Java_com_arkueid_alive2d_Live2DModel_nativeAddAndSaveParameterValueById(JNIEnv *env, jclass clazz,
                                                                        jlong handle, jstring id,
                                                                        jfloat value) {
    const char* id_c_str = env->GetStringUTFChars(id, nullptr);
    GetModel(handle)->AddAndSaveParameterValue(id_c_str, value);
    env->ReleaseStringUTFChars(id, id_c_str);
}