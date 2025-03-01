//
// Created by kasumi on 2025/2/28.
//

#ifndef ALIVE2D_JE_H
#define ALIVE2D_JE_H

#include <jni.h>

extern JavaVM *g_VM;

extern jclass g_parameterClass;
extern jclass g_onStartCallbackClass;
extern jclass g_onFinishCallbackClass;
extern jclass g_live2dClass;
extern jmethodID g_onStartMethod;
extern jmethodID g_onFinishMethod;
extern jmethodID g_loadFileMethod;

extern jfieldID g_parameterId;
extern jfieldID g_parameterValue;
extern jfieldID g_parameterDefaultValue;
extern jfieldID g_parameterMaximumValue;
extern jfieldID g_parameterMinimumValue;
extern jfieldID g_parameterType;

#endif //ALIVE2D_JE_H
