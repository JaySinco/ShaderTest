cmake_minimum_required(VERSION 3.16)

project(ShaderTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL")

add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

include_directories(AFTER
    ${CMAKE_SOURCE_DIR}/src/
    ${CMAKE_SOURCE_DIR}/deps/gflags/include/
    ${CMAKE_SOURCE_DIR}/deps/glog/include/
    ${CMAKE_SOURCE_DIR}/deps/glfw/include/
    ${CMAKE_SOURCE_DIR}/deps/glad/include/
    ${CMAKE_SOURCE_DIR}/deps/fmt/include/
    ${CMAKE_SOURCE_DIR}/deps/glm/include/
)

set(dep_LIB
    ${CMAKE_SOURCE_DIR}/deps/gflags/lib/gflags_static.lib
    ${CMAKE_SOURCE_DIR}/deps/glog/lib/glog.lib
    ${CMAKE_SOURCE_DIR}/deps/glfw/lib/glfw3.lib
    ${CMAKE_SOURCE_DIR}/deps/fmt/lib/fmt.lib
)

add_executable(sample
    ${CMAKE_SOURCE_DIR}/deps/glad/src/glad.c
    ${CMAKE_SOURCE_DIR}/src/utils.cpp
    ${CMAKE_SOURCE_DIR}/src/shader.cpp
    ${CMAKE_SOURCE_DIR}/src/mesh.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)
set_target_properties(sample PROPERTIES LINK_FLAGS "/MANIFEST:NO")
target_link_libraries(sample shlwapi.lib opengl32.lib ${dep_LIB})
