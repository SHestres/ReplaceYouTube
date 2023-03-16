#pragma once

#include <iostream>
#include <string>
//#include <input.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")


class VideoPlayer
{

public:
	VideoPlayer();

	VideoPlayer(int WindowWidth, int WindowHeight, std::string WindowName);

	~VideoPlayer();

	int Init();

	void Run();

	void Cleanup();

private:
	int w_wid;
	int w_hei;
	std::string w_name;
	GLFWwindow* window;

	//Vertex Array
	VAO* VAO1;
	//Vertex Buffer
	VBO* VBO1;
	//Index Buffer
	EBO* EBO1;

	Shader* shaderProgram;

	GLuint* uni_IDs;
	/*
	GLfloat vertices[12];
	GLuint indices[6];
	*/
};
