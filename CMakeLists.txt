cmake_minimum_required(VERSION 3.10)
project(ZPG_PROJ)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Paths to libraries
set(GLFW_DIR "${CMAKE_SOURCE_DIR}/libs/glfw-3.4.bin.WIN64")
set(GLEW_DIR "${CMAKE_SOURCE_DIR}/libs/glew-2.1.0")
set(GLM_DIR "${CMAKE_SOURCE_DIR}/libs/glm")

# Include directories
include_directories(
    ${GLFW_DIR}/include
    ${GLEW_DIR}/include
    ${GLM_DIR}
    ${CMAKE_SOURCE_DIR}/ZPG_PROJ
)

# Source files
set(SOURCES
    ZPG_PROJ/main.cpp
    ZPG_PROJ/Application.cpp
    ZPG_PROJ/Camera.cpp
    ZPG_PROJ/InputManager.cpp
    ZPG_PROJ/Light.cpp
    ZPG_PROJ/Material.cpp
    ZPG_PROJ/Model.cpp
    ZPG_PROJ/RenderableObject.cpp
    ZPG_PROJ/ResourceManager.cpp
    ZPG_PROJ/Scene.cpp
    ZPG_PROJ/SceneBuilder.cpp
    ZPG_PROJ/SceneManager.cpp
    ZPG_PROJ/Shader.cpp
    ZPG_PROJ/Texture.cpp
    ZPG_PROJ/VertexArray.cpp
    ZPG_PROJ/Window.cpp
    ZPG_PROJ/Skybox.cpp
    ZPG_PROJ/GameManager.cpp
)

# Header files
set(HEADERS
    ZPG_PROJ/Application.h
    ZPG_PROJ/BaseTransform.h
    ZPG_PROJ/Camera.h
    ZPG_PROJ/CustomTransform.h
    ZPG_PROJ/InputManager.h
    ZPG_PROJ/Light.h
    ZPG_PROJ/Material.h
    ZPG_PROJ/Model.h
    ZPG_PROJ/MoveTransform.h
    ZPG_PROJ/RenderableObject.h
    ZPG_PROJ/ResourceManager.h
    ZPG_PROJ/RotateTransform.h
    ZPG_PROJ/ScaleTransform.h
    ZPG_PROJ/Scene.h
    ZPG_PROJ/SceneBuilder.h
    ZPG_PROJ/SceneManager.h
    ZPG_PROJ/Shader.h
    ZPG_PROJ/ShaderObserver.h
    ZPG_PROJ/ShaderProgram.h
    ZPG_PROJ/Texture.h
    ZPG_PROJ/Transform.h
    ZPG_PROJ/TranslateTransform.h
    ZPG_PROJ/VertexArray.h
    ZPG_PROJ/Window.h
    ZPG_PROJ/assets/ForestScene/bushes.h
    ZPG_PROJ/assets/ForestScene/tree.h
    ZPG_PROJ/stb_image.h
    ZPG_PROJ/tiny_obj_loader.h
    ZPG_PROJ/Skybox.h
    ZPG_PROJ/GameManager.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link directories for libraries
if(WIN32)
    # For Windows
    target_link_directories(${PROJECT_NAME} PRIVATE
        ${GLFW_DIR}/lib-vc2022
        ${GLEW_DIR}/lib/Release/x64
    )
    target_link_libraries(${PROJECT_NAME}
        glfw3
        glew32s
        opengl32
    )
    target_compile_definitions(${PROJECT_NAME} PRIVATE GLEW_STATIC)
elseif(UNIX)
    # For Linux
    find_package(OpenGL REQUIRED)
    find_package(GLEW REQUIRED)
    find_package(glfw3 REQUIRED)
    
    target_link_libraries(${PROJECT_NAME}
        OpenGL::GL
        GLEW::GLEW
        glfw
    )
endif()

# Copy assets to build directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/ZPG_PROJ/assets
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

# Copy shader files to build directory
file(GLOB SHADER_FILES "ZPG_PROJ/Shaders/*.glsl")
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/ZPG_PROJ/Shaders
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/Shaders
)

# Add 'run' target
add_custom_target(run
    COMMAND ${PROJECT_NAME}
    DEPENDS ${PROJECT_NAME}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running ${PROJECT_NAME}..."
)
