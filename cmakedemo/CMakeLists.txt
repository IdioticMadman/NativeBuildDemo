cmake_minimum_required(VERSION 3.4.1)

#添加库
add_library(hello SHARED src/main/cpp/hello.cpp)


#添加动态库
add_library(Test SHARED IMPORTED)
#设置动态库so包的位置
set_target_properties(Test PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libTest.so)


#add_library(Test STATIC IMPORTED)
#set_target_properties(Test PROPERTIES IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${CMAKE_ANDROID_ARCH_ABI}/libTest.a)

message("CMAKE_SOURCE_DIR-> ${CMAKE_SOURCE_DIR}")
message("Android abi-> ${CMAKE_ANDROID_ARCH_ABI}")

target_link_libraries(hello Test log)