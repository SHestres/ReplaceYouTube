#pragma once

#include <iostream>
#include <stdio.h>

#include "imgui.h"
#include "imgui_impl_GLFW.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"



class Window
{
public:
	Window();
	~Window();

	int OpenWindow();
	void Run();

private:
	GLFWwindow* m_window;

	const char* glsl_version = "#version 130";
	ImGuiIO* m_pio;

	int InitImGui();
	void Cleanup();
	void ChooseGLFWVersionForPlatform();

	void DemoWindows();

};