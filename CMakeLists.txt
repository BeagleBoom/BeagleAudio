cmake_minimum_required(VERSION 3.7)
project(BeagleAudio)

set(CMAKE_CXX_STANDARD 11)


file(GLOB MAXIMILIAN
    "maximilian/*.h"
    "maximilian/*.cpp"
    "maximilian/*.c"
    "maximilian/libs/*.h"
    "maximilian/libs/*.cpp"
    "maximilian/libs/*.c"
)


add_definitions(-D__MACOSX_CORE__)

set(SOURCE_FILES main.cpp ${MAXIMILIAN})
add_executable(BeagleAudio ${SOURCE_FILES})
#target_include_directories(BeagleAudio PRIVATE maximilian)

macro(ADD_OSX_FRAMEWORK fwname target)
    find_library(FRAMEWORK_${fwname}
            NAMES ${fwname}
            PATHS ${CMAKE_OSX_SYSROOT}/System/Library
            PATH_SUFFIXES Frameworks
            NO_DEFAULT_PATH)
    if( ${FRAMEWORK_${fwname}} STREQUAL FRAMEWORK_${fwname}-NOTFOUND)
        MESSAGE(ERROR ": Framework ${fwname} not found")
    else()
        TARGET_LINK_LIBRARIES(${target} PUBLIC "${FRAMEWORK_${fwname}}/${fwname}")
        MESSAGE(STATUS "Framework ${fwname} found at ${FRAMEWORK_${fwname}}")
    endif()
endmacro(ADD_OSX_FRAMEWORK)

# POCO
set(ENABLE_MONGODB false)
set(ENABLE_DATA false)
set(ENABLE_ZIP false)
set(ENABLE_PAGECOMPILER false)
set(ENABLE_PAGECOMPILER_FILE2PAGE false)
set(ENABLE_XML false)
set(ENABLE_NET false)
set(ENABLE_UTIL true)

set(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(Poco REQUIRED JSON Util Foundation)

find_package(EventQueue REQUIRED)

target_link_libraries(BeagleAudio PUBLIC ${EventQueue_LIBRARY})
target_link_libraries(BeagleAudio PUBLIC ${Poco_Foundation_LIBRARY})
target_link_libraries(BeagleAudio PUBLIC ${Poco_Util_LIBRARY})
target_link_libraries(BeagleAudio PUBLIC ${Poco_JSON_LIBRARY})
target_link_libraries(BeagleAudio PUBLIC ${EventQueue_LIBRARY})


ADD_OSX_FRAMEWORK(CoreFoundation BeagleAudio)
ADD_OSX_FRAMEWORK(CoreAudio BeagleAudio)
ADD_OSX_FRAMEWORK(Accelerate BeagleAudio)