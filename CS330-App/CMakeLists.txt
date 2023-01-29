add_subdirectory(external/cmakelibs/glfw)
add_subdirectory(external/shared/glm)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE GLAD_SOURCES external/shared/glad/*.c)

add_executable(${PROJECT_NAME} ${SOURCES} ${GLAD_SOURCES}
        include/types.h include/shader.h include/camera.h
        src/shader.cpp src/mesh.cpp src/camera.cpp)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        include/
        external/shared/glad/include
        glfw
        glm
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glfw
        glm
        OpenGL::GL
)

# copy shaders file to the runtime output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
file(REMOVE ${OUTPUT_DIR}/shaders)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${OUTPUT_DIR}/)