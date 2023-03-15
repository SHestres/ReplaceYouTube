#pragma once

#include <iostream>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

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


};
