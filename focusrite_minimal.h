#ifndef __FOCUSRITE_MINIMAL_H__
#define __FOCUSRITE_MINIMAL_H__

#include <jni.h>
#include <windows.h>

// Use ONLY standard ASIO SDK headers
#include "iasiodrv.h"
#include "asiodrivers.h"

// theAsioDriver is now defined in the .cpp file, not extern here

// JNI function declarations
extern "C" {
    JNIEXPORT jint JNICALL Java_FocusriteCrashDemoMain_loadAsioDriverByName(JNIEnv*, jobject, jstring);
    JNIEXPORT jboolean JNICALL Java_FocusriteCrashDemoMain_initAsio(JNIEnv*, jobject);
}

#endif // __FOCUSRITE_MINIMAL_H__