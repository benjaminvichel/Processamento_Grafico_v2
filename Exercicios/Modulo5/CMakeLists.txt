set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/common/glad/include")
set(GLAD_SRC "${CMAKE_SOURCE_DIR}/common/glad/src/glad.c")

set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/common/glfw/include")
set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/common/glfw/build/src/Debug")

set(GLM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/common/glm")

add_executable(Modulo5 main.cpp ${GLAD_SRC} Sprite.cpp CharacterController.cpp)


target_include_directories(Modulo5 PRIVATE
    ${GLAD_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    "${CMAKE_SOURCE_DIR}/Exercicios/Modulo5"
)

target_link_libraries(Modulo5 PRIVATE
    "${GLFW_LIB_DIR}/glfw3.lib"
    opengl32
)

add_custom_command(TARGET Modulo5 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/exercicios/Modulo5/assets"
    $<TARGET_FILE_DIR:Modulo5>/assets

)

