cmake_minimum_required(VERSION 3.16)

project(ShaderTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

include_directories(AFTER
    ${CMAKE_SOURCE_DIR}/deps/gflags/include/
    ${CMAKE_SOURCE_DIR}/deps/glog/include/
)

add_executable(sample
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)
set_target_properties(sample PROPERTIES LINK_FLAGS "/MANIFEST:NO")
target_link_libraries(sample
    ${CMAKE_SOURCE_DIR}/deps/gflags/lib/gflags_static.lib
    ${CMAKE_SOURCE_DIR}/deps/glog/lib/glog.lib
    shlwapi.lib
)
