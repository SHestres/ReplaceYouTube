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


#include <mfapi.h>  //Microsoft Media Foundation


#pragma comment(lib, "Mfplat.lib")  //Media foundation


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

	VAO* VAO1;
	VBO* VBO1;
	EBO* EBO1;

	Shader* shaderProgram;
	GLuint* uni_IDs;

	int InitRendering();
	void DrawToScreen();

	void InitDecoder();
	void ReceiveEncodedData();

	//For server
	void InitEncoder();
	void SendEncodedData();
};
