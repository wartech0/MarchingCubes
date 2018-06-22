project(MarchCubeGame)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${MarchCubeGame_SOURCE_DIR}/Modules")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(SDL2 REQUIRED)
find_package(LibNoise REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)

include_directories(${SDL2_INCLUDE_DIR} ${MarchCubeGame_SOURCE_DIR}/Include)

file(GLOB HEADER_FILES ${MarchCubeGame_SOURCE_DIR}/Include/*.h)
file(GLOB SHADER_FILES ${MarchCubeGame_SOURCE_DIR}/Shaders/*)
aux_source_directory(${MarchCubeGame_SOURCE_DIR}/Source SRC_LIST)

add_executable(${PROJECT_NAME} ${SRC_LIST} ${HEADER_FILES} ${SHADER_FILES})
target_link_libraries(MarchCubeGame ${SDL2_LIBRARY} ${OPENGL_gl_LIBRARY} ${LIBNOISE_LIBRARY})

file(COPY ${CMAKE_SOURCE_DIR}/Shaders DESTINATION ${CMAKE_BINARY_DIR})

add_custom_command(TARGET MarchCubeGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/Shaders
        ${CMAKE_BINARY_DIR}/Shaders)
