

cmake_minimum_required(VERSION 3.4.1)

message("Hello")
#创建一个source_file变量
file(GLOB source_file src/main/cpp/*.cpp)
message("file -> ${source_file}")

add_library(
        native-lib
        SHARED
        ${source_file})

set(FFMPEG_LIB_PATH ${CMAKE_SOURCE_DIR}/src/main/cpp/libs/${CMAKE_ANDROID_ARCH_ABI})

add_library(avcodec STATIC IMPORTED)
set_target_properties(avcodec PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_PATH}/libavcodec.a)

add_library(avfilter STATIC IMPORTED)
set_target_properties(avfilter PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_PATH}/libavfilter.a)

add_library(avformat STATIC IMPORTED)
set_target_properties(avformat PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_PATH}/libavformat.a)

add_library(avutil STATIC IMPORTED)
set_target_properties(avutil PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_PATH}/libavutil.a)

add_library(swresample STATIC IMPORTED)
set_target_properties(swresample PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_PATH}/libswresample.a)

add_library(swscale STATIC IMPORTED)
set_target_properties(swscale PROPERTIES IMPORTED_LOCATION ${FFMPEG_LIB_PATH}/libswscale.a)

include_directories(src/main/cpp/include)

find_library(
        log-lib
        log)


target_link_libraries(
        native-lib
        avfilter
        avformat
        avcodec
        avutil
        swresample
        swscale
        z
        OpenSLES
        android
        ${log-lib})