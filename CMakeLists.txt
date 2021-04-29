cmake_minimum_required(VERSION 3.16)

project(ShaderTest)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

include_directories(AFTER
    ${CMAKE_SOURCE_DIR}/deps/gflags/include/
    ${CMAKE_SOURCE_DIR}/deps/glog/include/
    ${CMAKE_SOURCE_DIR}/deps/panda3d/include/
)

add_executable(sample ${CMAKE_SOURCE_DIR}/src/main.cpp)
target_precompile_headers(sample PUBLIC ${CMAKE_SOURCE_DIR}/src/prec.h)
set_target_properties(sample PROPERTIES LINK_FLAGS "/MANIFEST:NO")
target_link_libraries(sample shlwapi.lib
    ${CMAKE_SOURCE_DIR}/deps/gflags/lib/gflags_static.lib
    ${CMAKE_SOURCE_DIR}/deps/glog/lib/glog.lib
    ${CMAKE_SOURCE_DIR}/deps/panda3d/lib/libp3framework.lib
    ${CMAKE_SOURCE_DIR}/deps/panda3d/lib/libpanda.lib
    ${CMAKE_SOURCE_DIR}/deps/panda3d/lib/libpandaexpress.lib
    ${CMAKE_SOURCE_DIR}/deps/panda3d/lib/libp3dtool.lib
    ${CMAKE_SOURCE_DIR}/deps/panda3d/lib/libp3dtoolconfig.lib
    ${CMAKE_SOURCE_DIR}/deps/panda3d/lib/libp3direct.lib
    ${CMAKE_SOURCE_DIR}/deps/panda3d/lib/libp3pystub.lib
    ${CMAKE_SOURCE_DIR}/deps/panda3d/lib/libpandafx.lib
    ${CMAKE_SOURCE_DIR}/deps/panda3d/lib/libpandaphysics.lib
)
