cmake_minimum_required(VERSION 3.4.1)


#添加库
add_library(audioEncode SHARED src/main/cpp/audioEncoder.cpp)


set(LAME_LIB_PATH ${CMAKE_SOURCE_DIR}/src/main/cpp/libs/${CMAKE_ANDROID_ARCH_ABI})

add_library(mp3lame STATIC IMPORTED)
set_target_properties(mp3lame PROPERTIES IMPORTED_LOCATION ${LAME_LIB_PATH}/libmp3lame.a)

include_directories(src/main/cpp/include)

message("CMAKE_SOURCE_DIR-> ${CMAKE_SOURCE_DIR}")
message("Android abi-> ${CMAKE_ANDROID_ARCH_ABI}")

target_link_libraries(audioEncode
        mp3lame
        log)