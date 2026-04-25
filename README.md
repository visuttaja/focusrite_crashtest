# Focusrite ASIO Driver Crash Investigation

## 🚨 Technical Abstract - Critical Driver Stability Issue

### Overview
A systematic investigation has been conducted to identify a critical stability issue in the 
Focusrite USB ASIO driver when invoked through Java Native Interface (JNI) calls. The 
project demonstrates a reproducible crash occurring during ASIO driver initialization 
across multiple Java Development Kit (JDK) versions.

### Test Environment
- **Platform**: Windows 64-bit
- **JDK Versions Tested**: Java 17 through Java 26
- **Target Driver**: Focusrite USB ASIO
- **Control Driver**: RME Hammerfall DSP ASIO (for comparison)

### Technical Implementation
The test harness consists of:
1. **Java Application**: `FocusriteCrashDemoMain.java` - Minimal Java wrapper that loads 
   native DLL and invokes ASIO operations
2. **Native Bridge**: `focusrite_minimal.cpp` - C++ JNI implementation using standard 
   Steinberg ASIO SDK 2.3.4
3. **Core Operations**: 
   - Driver loading via standard ASIO SDK `loadAsioDriver()` function
   - Driver initialization through `theAsioDriver->init(&driverInfo)` call

### Reproducible Crash Pattern
**Consistent Failure Point**: The crash occurs specifically during the 
`theAsioDriver->init()` call, immediately following successful driver loading.

**Error Details**:
- **Exception Type**: `EXCEPTION_ACCESS_VIOLATION (0xc0000005)`
- **Root Cause**: Null pointer dereference (reading address 0x0000000000000000)
- **Stack Trace (JDK 24 test case)**:
  ```
  C  [msvcp140.dll+0x12f58]  (Microsoft Visual C++ runtime)
  C  [focusriteusbasio.dll+0x9386]  (Focusrite USB ASIO driver)
  C  [focusriteusbasio.dll+0x8d2d]  (Focusrite USB ASIO driver)
  C  [focusriteusbasio.dll+0x4c0d]  (Focusrite USB ASIO driver)
  C  [focusrite_minimal.dll+0x26ba]  Java_FocusriteCrashDemoMain_initAsio+0x8a
  ```
- **Scope**: Affects JDK versions 17-24; versions 25-26 initialize successfully
- **Context**: Crash occurs outside JVM in native driver code, originating within 
  `focusriteusbasio.dll`

### Key Findings
1. **Driver Loading Success**: The Focusrite USB ASIO driver loads successfully through 
   the standard ASIO SDK interface
2. **Init Call Failure**: The crash consistently occurs during the initialization phase 
   for JDK 17-24, with the stack trace clearly showing the failure originates within 
   the Focusrite driver code at multiple offsets
3. **JDK Version Behavior**: The issue affects JDK versions 17-24, while versions 25-26 
   complete initialization successfully
4. **Null Pointer Dereference**: The crash is caused by attempting to read from address 
   0x0000000000000000, suggesting improper null pointer validation within the 
   Focusrite driver
5. **Control Test**: RME ASIO driver operates correctly under identical conditions, 
   confirming the test harness validity

### Technical Significance
This crash represents a critical stability issue that prevents Java-based audio 
applications from utilizing Focusrite USB interfaces through the standard ASIO interface 
on JDK versions 17-24. The stack trace definitively shows the crash originates within 
`focusriteusbasio.dll` at multiple internal offsets before propagating to the Microsoft 
C++ runtime, indicating a bug within the Focusrite driver implementation when called 
from a JNI context.

### Possible Investigation Areas
- Memory management and null pointer validation in ASIO initialization sequence within 
  `focusriteusbasio.dll`
- Thread safety considerations when driver is invoked from JVM context
- Analysis of driver behavior at offsets 0x4c0d, 0x8d2d, and 0x9386 within 
  `focusriteusbasio.dll`
- Validation of ASIO driverInfo structure handling across different runtime 
  environments
- Investigation of environmental factors that may differ between JDK versions

---

## Prerequisites

### Development Environment Requirements

1. **Java IDE**: IntelliJ IDEA is required as the Java development environment for this 
   project.

2. **Native Compilation**: Visual Studio 2022 is used for building the native DLL 
   component (`focusrite_minimal.dll`).

3. **JDK Distribution**: The project utilizes the OpenJDK distribution:
   - **Download URL**: https://download.java.net/openjdk/jdk24/ri/openjdk-24+36_windows-x64_bin.zip
   - **Version**: OpenJDK 24
   - **Platform**: Windows x64 binary distribution

This OpenJDK distribution provides the Java components for testing the ASIO driver 
integration.

---

## Project Structure

- `javasrc/FocusriteCrashDemoMain.java` - Main Java test application
- `focusrite_minimal.cpp` - Native C++ JNI bridge implementation
- `focusrite_minimal.h` - Header file for native bridge
- `run_crashdemo.bat` - Batch script for testing across multiple JDK versions. See 
  [JDK Archive](https://jdk.java.net/archive/) for different JDK versions (available 
  in left column of page)
- `errors_per_all_jdks_focusrite.txt` - Detailed crash logs for Focusrite driver
- `errors_per_all_jdks_rme.txt` - Control test logs for RME driver
- `hs_err_pid*.log` - JVM crash dump files
