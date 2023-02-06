set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/cmake-output/${CMAKE_SYSTEM_NAME})

add_subdirectory(external/cmakelibs/glfw)
add_subdirectory(external/shared/glm)

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE GLAD_SOURCES external/shared/glad/*.c)

add_executable(${PROJECT_NAME} ${SOURCES} ${GLAD_SOURCES}
        include/types.h include/shader.h include/camera.h external/shared/stb/stb_image.h
        src/shader.cpp src/mesh.cpp src/camera.cpp include/texture.h src/texture.cpp)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        include/
        external/shared/glad/include
        glfw
        glm
        external/shared/stb
        textures/
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        glfw
        glm
        OpenGL::GL
)

# copy shaders file to the runtime output directory
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
file(REMOVE ${CMAKE_SOURCE_DIR}/cmake-output/${CMAKE_SYSTEM_NAME}/shaders)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shaders DESTINATION ${CMAKE_SOURCE_DIR}/cmake-output/${CMAKE_SYSTEM_NAME}/)

# copy texture file to the runtime output directory
file(REMOVE ${CMAKE_SOURCE_DIR}/cmake-output/${CMAKE_SYSTEM_NAME}/textures)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures DESTINATION ${CMAKE_SOURCE_DIR}/cmake-output/${CMAKE_SYSTEM_NAME}/)