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



#define DEFAULT_FONT_SIZE default_font_size_var
#define TITLE_FONT_SIZE default_font_size_var * 1.25f

#define MAX_VIDEO_TITLE_LENGTH 40
#define MAX_VIDEO_GENRE_LENGTH 40
#define MAX_VIDEO_DESCRIPTION_LENGTH 5000
#define MAX_ACTOR_LIST_LENGTH 100

#define MAX_FILEPATH_LENGTH 256

bool openFileWithExplorer(std::string* SelectedFile, std::string* FilePath);
void Title(const char* title, float multiplier = 0);

class Window
{
public:
	Window(std::string libraryFilename);
	~Window();

	int OpenWindow(const char* WindowTitle);
	void Run();

private:
	GLFWwindow* m_window;

	const char* glsl_version = "#version 130";
	ImGuiIO* m_pio;

	//Library vars
	char m_libraryFilepath[MAX_FILEPATH_LENGTH];

	int InitImGui(const char* WindowTitle);
	void Cleanup();
	void ChooseGLFWVersionForPlatform();

	void DemoWindows();

};