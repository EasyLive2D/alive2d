//
// Created by kasumi on 2025/2/28.
//

#ifndef ALIVE2D_JE_H
#define ALIVE2D_JE_H

#include <jni.h>

extern JavaVM *g_VM;
extern jclass g_stringClass;
extern jmethodID g_stringConstructor;

extern jmethodID g_biConsumerAcceptMethod;

extern jclass g_integerClass;
extern jmethodID g_integerValueOfMethod;

extern jclass g_hashMapClass;
extern jmethodID g_hashMapConstructor;
extern jmethodID g_hashMapPutMethod;
extern jmethodID g_hashMapGetMethod;

extern jclass g_arrayListClass;
extern jmethodID g_arrayListConstructor;
extern jmethodID g_arrayListAddMethod;

#endif //ALIVE2D_JE_H
