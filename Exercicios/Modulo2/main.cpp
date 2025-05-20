#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <cstdlib>  
#include <ctime>   

#include <glm/glm.hpp>       
#include <glm/gtc/matrix_transform.hpp>  
#include <glm/gtc/type_ptr.hpp> 


const char* vertexShaderSource = R"glsl(
#version 330 core
layout(location = 0) in vec3 aPos;

uniform vec2 offset;  // posição do triângulo na tela

void main() {
    gl_Position = vec4(aPos.xy + offset, aPos.z, 1.0);
}
)glsl";


const char* fragmentShaderSource = R"glsl(
#version 330 core
out vec4 FragColor;

uniform vec3 triangleColor;

void main() {
    FragColor = vec4(triangleColor, 1.0);
}
)glsl";

struct Triangle {
    glm::vec2 position;  
    glm::vec3 color;    
};


GLuint createStandardTriangle() {
    float vertices[] = {
        -0.1f, -0.1f, 0.0f,
         0.1f, -0.1f, 0.0f,
         0.0f,  0.1f, 0.0f
    };

    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Erro compilando shader: " << infoLog << "\n";
    }
    return shader;
}

GLuint createShaderProgram() {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "Erro linkando shader program: " << infoLog << "\n";
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}


glm::vec2 screenToGLCoords(GLFWwindow* window, double xpos, double ypos) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    // X normalizado para [-1, 1]
    float x = (float)(xpos) / width * 2.0f - 1.0f;
    // Y invertido e normalizado para [-1, 1]
    float y = -((float)(ypos) / height * 2.0f - 1.0f);

    return glm::vec2(x, y);
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr))); 

    if (!glfwInit()) {
        std::cerr << "Erro ao inicializar GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Triangulos com clique", NULL, NULL);
    if (!window) {
        std::cerr << "Falha ao criar janela GLFW\n";
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Falha ao inicializar GLAD\n";
        return -1;
    }

    GLuint shaderProgram = createShaderProgram();
    GLuint triangleVAO = createStandardTriangle();

    std::vector<Triangle> triangles;

    triangles.push_back({ glm::vec2(0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f) });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* win, int button, int action, int mods) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            double xpos, ypos;
            glfwGetCursorPos(win, &xpos, &ypos);

            glm::vec2 pos = screenToGLCoords(win, xpos, ypos);

          
            float r = static_cast<float>(rand()) / RAND_MAX;
            float g = static_cast<float>(rand()) / RAND_MAX;
            float b = static_cast<float>(rand()) / RAND_MAX;

           
            auto triangles_ptr = (std::vector<Triangle>*)glfwGetWindowUserPointer(win);
            triangles_ptr->push_back({ pos, glm::vec3(r, g, b) });
        }
        });

    
    glfwSetWindowUserPointer(window, &triangles);

  
    while (!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(triangleVAO);

       
        GLint offsetLoc = glGetUniformLocation(shaderProgram, "offset");
        GLint colorLoc = glGetUniformLocation(shaderProgram, "triangleColor");

       
        for (const auto& tri : triangles) {
            glUniform2fv(offsetLoc, 1, glm::value_ptr(tri.position));
            glUniform3fv(colorLoc, 1, glm::value_ptr(tri.color));
            glDrawArrays(GL_TRIANGLES, 0, 3);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

 
    glDeleteVertexArrays(1, &triangleVAO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
