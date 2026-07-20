cmake_minimum_required(VERSION 3.15)

project(MajSimaiCpp VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(OS_PREFIX "win")
    set(LIB_PREFIX "")
    set(LINK_EXT ".lib")
    set(RUN_EXT ".dll")
    set(USER_HOME_DIR "$ENV{USERPROFILE}")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(OS_PREFIX "mac")
    set(LIB_PREFIX "lib")
    set(LINK_EXT ".dylib")
    set(RUN_EXT ".dylib")
    set(USER_HOME_DIR "$ENV{HOME}")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(OS_PREFIX "linux")
    set(LIB_PREFIX "lib")
    set(LINK_EXT ".so")
    set(RUN_EXT ".so")
    set(USER_HOME_DIR "$ENV{HOME}")

elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
    set(OS_PREFIX "android")
    set(LIB_PREFIX "lib")
    set(LINK_EXT ".so")
    set(RUN_EXT ".so")
    set(USER_HOME_DIR "$ENV{HOME}")

elseif(CMAKE_SYSTEM_NAME STREQUAL "iOS")
    set(OS_PREFIX "ios")
    set(LIB_PREFIX "lib")
    set(LINK_EXT ".dylib")
    set(RUN_EXT ".dylib")
    set(USER_HOME_DIR "$ENV{HOME}")

else()
    message(FATAL_ERROR
        "OS not supported: ${CMAKE_SYSTEM_NAME}"
    )
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")

    if(CMAKE_VS_PLATFORM_NAME MATCHES "ARM64")
        set(ARCH_PREFIX "arm64")

    elseif(CMAKE_VS_PLATFORM_NAME MATCHES "x64")
        set(ARCH_PREFIX "x64")

    elseif(CMAKE_VS_PLATFORM_NAME MATCHES "Win32")
        set(ARCH_PREFIX "x86")

    elseif(CMAKE_VS_PLATFORM_NAME MATCHES "ARM")
        set(ARCH_PREFIX "arm")

    else()
        message(FATAL_ERROR
            "Unknown Windows platform: ${CMAKE_VS_PLATFORM_NAME}"
        )
    endif()

else()

    if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(arm64|aarch64|ARM64)")
        set(ARCH_PREFIX "arm64")

    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_64|AMD64|amd64)")
        set(ARCH_PREFIX "x64")

    elseif(CMAKE_SYSTEM_PROCESSOR MATCHES "^(i[3-6]86|x86)")
        set(ARCH_PREFIX "x86")

    else()
        message(FATAL_ERROR
            "Unknown processor: ${CMAKE_SYSTEM_PROCESSOR}"
        )
    endif()

endif()

set(MAJSIMAI_LINK_FILE "${CMAKE_CURRENT_SOURCE_DIR}/external/MajSimai/lib/${OS_PREFIX}-${ARCH_PREFIX}/${LIB_PREFIX}MajSimai${LINK_EXT}")

file(TO_CMAKE_PATH "${USER_HOME_DIR}" USER_HOME_DIR)
set(DOTNET_ILC_SDK "${USER_HOME_DIR}/.nuget/packages/runtime.${OS_PREFIX}-${ARCH_PREFIX}.microsoft.dotnet.ilcompiler/9.0.16/sdk")

message(DEBUG "CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME}")
message(DEBUG "CMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}")
message(DEBUG "ARCH_PREFIX=${ARCH_PREFIX}")
message(DEBUG "MAJSIMAI_LINK_FILE=${MAJSIMAI_LINK_FILE}")
message(DEBUG "DOTNET_ILC_SDK=${DOTNET_ILC_SDK}")

set(SOURCES
    src/SimaiChartView.cpp
    src/SimaiFileView.cpp
    src/SimaiNoteView.cpp
    src/SimaiParser.cpp
    src/SimaiTimingPointView.cpp
)

add_library(MajSimaiCpp SHARED ${SOURCES})

target_include_directories(MajSimaiCpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(ARCH_PREFIX MATCHES "arm64")
    target_link_libraries(MajSimaiCpp PRIVATE 
        "${DOTNET_ILC_SDK}/Runtime.WorkstationGC.lib"
        "${DOTNET_ILC_SDK}/eventpipe-disabled.lib"
        "${DOTNET_ILC_SDK}/standalonegc-disabled.lib"
        "${DOTNET_ILC_SDK}/zlibstatic.lib"
        "${DOTNET_ILC_SDK}/System.Globalization.Native.Aot.lib"
        "${DOTNET_ILC_SDK}/System.IO.Compression.Native.Aot.lib"

        "${MAJSIMAI_LINK_FILE}"
        Advapi32.lib Bcrypt.lib Crypt32.lib Iphlpapi.lib Normaliz.lib Ncrypt.lib NtDll.lib OLE32.lib OleAut32.lib User32.lib Ws2_32.lib
    )

else()
    target_link_libraries(MajSimaiCpp PRIVATE 
        "${DOTNET_ILC_SDK}/Runtime.WorkstationGC.lib"
        "${DOTNET_ILC_SDK}/eventpipe-disabled.lib"
        "${DOTNET_ILC_SDK}/Runtime.VxsortEnabled.lib"
        "${DOTNET_ILC_SDK}/standalonegc-disabled.lib"
        "${DOTNET_ILC_SDK}/zlibstatic.lib"
        "${DOTNET_ILC_SDK}/System.Globalization.Native.Aot.lib"
        "${DOTNET_ILC_SDK}/System.IO.Compression.Native.Aot.lib"

        "${MAJSIMAI_LINK_FILE}"
        Advapi32.lib Bcrypt.lib Crypt32.lib Iphlpapi.lib Normaliz.lib Ncrypt.lib NtDll.lib OLE32.lib OleAut32.lib User32.lib Ws2_32.lib
    )

endif()

if(APPLE)
    set_target_properties(MySimaiCpp PROPERTIES BUILD_RPATH "@executable_path")
elseif(UNIX AND NOT APPLE)
    set_target_properties(MySimaiCpp PROPERTIES BUILD_RPATH "$ORIGIN")
endif()

if(MSVC)
    target_compile_options(MajSimaiCpp PRIVATE /utf-8)
endif()