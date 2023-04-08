#pragma once

#include <iostream>
#include <stdio.h>
#include <string>
#include <Windows.h>
#include <shobjidl.h>

#include "imgui.h"
#include "imgui_impl_GLFW.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"

#include "VideoImporter.h"


#define DEFAULT_FONT_SIZE default_font_size_var
#define TITLE_FONT_SIZE default_font_size_var * 1.25f

bool openFileWithExplorer(std::string* SelectedFile, std::string* FilePath);
void Title(const char* title, float multiplier = 0);

class Window
{
public:
	Window();
	~Window();

	int OpenWindow(const char* WindowTitle);
	void Run(VideoLibrary* library);

private:
	GLFWwindow* m_window;

	const char* glsl_version = "#version 130";
	ImGuiIO* m_pio;

	int InitImGui(const char* WindowTitle);
	void Cleanup();
	void ChooseGLFWVersionForPlatform();

	void DemoWindows();

};