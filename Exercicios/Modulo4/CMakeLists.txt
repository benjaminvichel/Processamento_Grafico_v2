set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/common/glad/include")
set(GLAD_SRC "${CMAKE_SOURCE_DIR}/common/glad/src/glad.c")

set(GLFW_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/common/glfw/include")
set(GLFW_LIB_DIR "${CMAKE_SOURCE_DIR}/common/glfw/build/src/Debug")

set(GLM_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/common/glm")

add_executable(Modulo4 main.cpp ${GLAD_SRC} Sprite.cpp)


target_include_directories(Modulo4 PRIVATE
    ${GLAD_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    "${CMAKE_SOURCE_DIR}/Exercicios/Modulo4"
)

target_link_libraries(Modulo4 PRIVATE
    "${GLFW_LIB_DIR}/glfw3.lib"
    opengl32
)

add_custom_command(TARGET Modulo4 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/exercicios/Modulo4/assets"
    $<TARGET_FILE_DIR:Modulo4>/assets
)

