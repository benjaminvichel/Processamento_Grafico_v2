#pragma once
#include <iostream>
#include <string>
#include <cassert>
#include <vector>
#include <random>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Sprite.h"
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "CharacterController.h"




void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

int setupShader();

const GLuint WIDTH = 800, HEIGHT = 600;


const GLchar* vertexShaderSource = R"(#version 400
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 colors;
layout (location = 2) in vec2 texture_mapping;

out vec2 texture_coordinates;
out vec3 color_values;

uniform mat4 projection;
uniform mat4 model;
uniform vec2 scrollOffset;

uniform ivec2 sheetSize;   
uniform int frameIndex;

void main()
{
    int column = frameIndex % sheetSize.x;
    int row    = frameIndex / sheetSize.x;
    vec2 cellSize = vec2(1.0) / vec2(sheetSize);
    vec2 frameOffset = vec2(column, row) * cellSize;
    texture_coordinates = texture_mapping * cellSize + frameOffset + scrollOffset;
    color_values = colors;
    gl_Position = projection * model * vec4(position, 1.0);
})";


const GLchar* fragmentShaderSource = "#version 400\n"
"in vec2 texture_coordinates;\n"
"in vec3 color_values;\n"
"out vec4 color;\n"
"uniform sampler2D spriteTexture;\n"
"void main()\n"
"{\n"
"    vec4 texColor = texture(spriteTexture, texture_coordinates);\n"
"    color = texColor;\n"
"}\n\0";

std::vector<Sprite*> sprites;

//Main
int main()
{
	std::cout << "Working directory: " << std::filesystem::current_path() << std::endl;
	// Inicializa GLFW
	glfwInit();

	// Cria janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Modulo 5", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	// GLAD: carrega todos os ponteiros da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	// Obtendo versoes
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	std::cout << "Renderer: " << renderer << std::endl;
	std::cout << "OpenGL version supported " << version << std::endl;


	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	GLuint shaderID = setupShader();

	glUseProgram(shaderID);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	GLint colorLoc = glGetUniformLocation(shaderID, "inputColor");

	glm::mat4 projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
	glUniformMatrix4fv(glGetUniformLocation(shaderID, "projection"), 1, GL_FALSE, value_ptr(projection));
	
	
	// Building 1
	sprites.push_back(new Sprite("assets/Building1_light_shadow.png", shaderID));
	sprites[0]->setScale(glm::vec3(300, 300, 0));
	sprites[0]->setTranslate(glm::vec3(150, 150, 0));

	// Building 2
	sprites.push_back(new Sprite("assets/Building2_dark_shadow.png", shaderID));
	sprites[1]->setScale(glm::vec3(300, 300, 0));
	sprites[1]->setTranslate(glm::vec3(800 - 150, 150, 0)); 

	// Caveman Statue
	sprites.push_back(new Sprite("assets/caveman_statue_light_shadow1.png", shaderID));
	sprites[2]->setScale(glm::vec3(100, 100, 0));
	sprites[2]->setTranslate(glm::vec3(50, 50, 0));

	// Gates
	sprites.push_back(new Sprite("assets/Gates_light_shadow2.png", shaderID));
	sprites[3]->setScale(glm::vec3(100, 100, 0));
	sprites[3]->setTranslate(glm::vec3(400, 50, 0));

	// White Crystal
	sprites.push_back(new Sprite("assets/white_crystal_light_shadow5.png", shaderID));
	sprites[4]->setScale(glm::vec3(100, 100, 0));
	sprites[4]->setTranslate(glm::vec3(750, 50, 0));

	// Centipede
	sprites.push_back(new Sprite("assets/centipede_dark_shadow1.png", shaderID));
	sprites[5]->setScale(glm::vec3(300, 300, 0));
	sprites[5]->setTranslate(glm::vec3(250, 150, 0));

	// Dark Totem
	sprites.push_back(new Sprite("assets/Dark_totem_dark_shadow1.png", shaderID));
	sprites[6]->setScale(glm::vec3(300, 300, 0));
	sprites[6]->setTranslate(glm::vec3(550, 150, 0));

	sprites.push_back(new CharacterController("assets/man_walking.png", shaderID));
	sprites[7]->setScale(glm::vec3(100, 100, 0));
	sprites[7]->setTranslate(glm::vec3(400, 400, 0));
	sprites[7]->setSpriteSheet(12, 4);
	sprites[7]->setVelocity(glm::vec3(0, 0, 0));



	float lastTime = glfwGetTime();

	//game loop
	while (!glfwWindowShouldClose(window))
	{
		float currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		for (int i = 0; i < sprites.size(); i++)
		{
			sprites[i]->update(deltaTime);
			sprites[i]->Draw();
		}

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	for (int i = 0; i < sprites.size(); i++)
	{
		sprites[i]->deleteVertexArray();
	}
	glfwTerminate();
	return 0;
}

// Função de callback de teclado - só pode ter uma instância (deve ser estática se
// estiver dentro de uma classe) - É chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key == GLFW_KEY_W && action == GLFW_PRESS)
		sprites[7]->setVelocity(sprites[7]->getVelocity() + glm::vec3(0, 1, 0));
	if (key == GLFW_KEY_S && action == GLFW_PRESS)
		sprites[7]->setVelocity(sprites[7]->getVelocity() + glm::vec3(0, -1, 0));
	if (key == GLFW_KEY_D && action == GLFW_PRESS)
		sprites[7]->setVelocity(sprites[7]->getVelocity() + glm::vec3(1, 0, 0));
	if (key == GLFW_KEY_A && action == GLFW_PRESS)
		sprites[7]->setVelocity(sprites[7]->getVelocity() + glm::vec3(-1, 0, 0));

	if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		sprites[7]->setVelocity(sprites[7]->getVelocity() + glm::vec3(0, -1, 0));
	if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		sprites[7]->setVelocity(sprites[7]->getVelocity() + glm::vec3(0, 1, 0));
	if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		sprites[7]->setVelocity(sprites[7]->getVelocity() + glm::vec3(-1, 0, 0));
	if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		sprites[7]->setVelocity(sprites[7]->getVelocity() + glm::vec3(1, 0, 0));
}


int setupShader()
{

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);
	
	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	// Fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{

}