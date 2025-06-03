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




void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

int setupShader();

const GLuint WIDTH = 800, HEIGHT = 600;


const GLchar* vertexShaderSource = "#version 400\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 colors;\n"
"layout (location = 2) in vec2 texture_mapping;\n"
"out vec2 texture_coordinates;\n"
"out vec3 color_values;\n"
"uniform mat4 projection;\n"
"uniform mat4 model;\n"
"void main()\n"
"{\n"
"texture_coordinates = texture_mapping;\n"
"color_values = colors;\n"
"gl_Position = projection * model * vec4(position.x, position.y, position.z, 1.0);\n"
"}\0";


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

//Main
int main()
{
	std::cout << "Working directory: " << std::filesystem::current_path() << std::endl;
	// Inicializa GLFW
	glfwInit();

	// Cria janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Modulo 4 Tarefa Mapeamento de Texturas", nullptr, nullptr);
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

	std::vector<Sprite*> sprites;
	
	
	sprites.push_back(new Sprite("assets/Building1_light_shadow.png", shaderID));
	sprites[0]->setScale(glm::vec3(300, 300, 0));
	sprites[0]->setTranslate(glm::vec3(300 / 2.0f, 300 / 2.0f, 0)); 
	
	sprites.push_back(new Sprite("assets/Building2_dark_shadow.png", shaderID));
	sprites[1]->setScale(glm::vec3(300, 300, 0));
	sprites[1]->setTranslate(glm::vec3(800 - 300 / 2.0f, 300 / 2.0f, 0)); 

	sprites.push_back(new Sprite("assets/caveman_statue_light_shadow1.png", shaderID));
	sprites[2]->setScale(glm::vec3(100, 100, 0));
	sprites[2]->setTranslate(glm::vec3(050, 100 / 2.0f, 0)); 

	sprites.push_back(new Sprite("assets/Gates_light_shadow2.png", shaderID));
	sprites[3]->setScale(glm::vec3(100, 100, 0));
	sprites[3]->setTranslate(glm::vec3(400, 100 / 2.0f, 0));

	sprites.push_back(new Sprite("assets/white_crystal_light_shadow5.png", shaderID));
	sprites[4]->setScale(glm::vec3(100, 100, 0));
	sprites[4]->setTranslate(glm::vec3(750, 100 / 2.0f, 0));

	sprites.push_back(new Sprite("assets/centipede_dark_shadow1.png", shaderID));
	sprites[5]->setScale(glm::vec3(300, 300, 0));
	sprites[5]->setTranslate(glm::vec3(250, 100 / 2.0f, 0));


	sprites.push_back(new Sprite("assets/Dark_totem_dark_shadow1.png", shaderID));
	sprites[6]->setScale(glm::vec3(300, 300, 0));
	sprites[6]->setTranslate(glm::vec3(550, 150 / 2.0f, 0));





	//game loop
	while (!glfwWindowShouldClose(window))
	{
		
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		for (int i = 0; i < sprites.size(); i++)
		{
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
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