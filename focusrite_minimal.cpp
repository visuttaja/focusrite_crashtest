#include "focusrite_minimal.h"
#include <iostream>

// Define the global variable that ASIO SDK expects (was only declared as extern in headers)
IASIO* theAsioDriver = nullptr;

// Use standard ASIO SDK global variables - these are defined in asiodrivers.cpp
extern AsioDrivers* asioDrivers;

// Forward declaration of the standard ASIO SDK function
bool loadAsioDriver(char* name);

JNIEXPORT jint JNICALL Java_FocusriteCrashDemoMain_loadAsioDriverByName(JNIEnv* env, jobject obj, jstring driverName) {
    const char* driverStr = env->GetStringUTFChars(driverName, 0);

    std::cout << "Loading ASIO driver: " << driverStr << std::endl;

    // Use the standard ASIO SDK function from asiodrivers.cpp
    if (loadAsioDriver((char*)driverStr)) {
        std::cout << "Driver loaded successfully" << std::endl;
        env->ReleaseStringUTFChars(driverName, driverStr);
        return 0; // Success
    }
    else {
        std::cout << "Failed to load driver" << std::endl;
        env->ReleaseStringUTFChars(driverName, driverStr);
        return -1;
    }
}

JNIEXPORT jboolean JNICALL Java_FocusriteCrashDemoMain_initAsio(JNIEnv* env, jobject obj) {
    if ( !theAsioDriver) {
        std::cout << "No ASIO driver loaded" << std::endl;
        return JNI_FALSE;
    }

    std::cout << "Attempting ASIO init..." << std::endl;

    // Use standard ASIO SDK structures and init call
    ASIODriverInfo driverInfo = { 0 };
    driverInfo.asioVersion = 2;
    driverInfo.sysRef = nullptr;  // Standard ASIO requires a system reference

    // This is the standard ASIO SDK init call where Focusrite driver crashes
    if (theAsioDriver->init(&driverInfo)) {
        std::cout << "ASIO init completed successfully" << std::endl;
        return JNI_TRUE;
    }
    else {
        std::cout << "ASIO init failed" << std::endl;
        return JNI_FALSE;
    }
}