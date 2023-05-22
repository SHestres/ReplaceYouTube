#pragma once


//#define CPPHTTPLIB_OPENSSL_SUPPORT
//httplib must be included before Windows.h
//This means before DashPackager.h which also includes Windows.h
#include "httplib.h"
#include <Windows.h>

#include "json.hpp"
#include "DashPackager.h"

#include <iostream>
#include <stdio.h>
#include <string>

#include <shobjidl.h>

#include "imgui.h"
#include "imgui_impl_GLFW.h"
#include "imgui_impl_opengl3.h"
#include "GLFW/glfw3.h"



using json = nlohmann::json;

typedef unsigned char stbi_uc;

#define DEFAULT_FONT_SIZE default_font_size_var
#define TITLE_FONT_SIZE default_font_size_var * 1.25f

#define MAX_VIDEO_TITLE_LENGTH 40
#define MAX_VIDEO_GENRE_LENGTH 40
#define MAX_VIDEO_DESCRIPTION_LENGTH 5000
#define MAX_ACTOR_LIST_LENGTH 100

#define MAX_FILEPATH_LENGTH 256

#define GENRES_FILEPATH_EXT "/data/genres.json"
#define FAVORITES_FILEPATH_EXT "/data/genres.json"
#define CATEGORIES_FILEPATH_EXT "/data/categories.json"
#define IMP_INFO_FILEPATH_EXT "/data/importerInfo.json"
#define DATA_FILEPATH_EXT "/data/"

//Forward Declarations
void GetMaxMonitorResolution(int* w, int* h);
bool openFileWithExplorer(std::string* SelectedFile, std::string* FilePath);
bool openFolderWithExplorer(std::string* SelectedFolder, std::string* FolderPath);
void Title(const char* title, bool wrap = false, float multiplier = 0);
bool LoadTextureFromFile(const char* filename, GLuint* out_texture, int* out_width, int* out_height);
bool LoadTextureFromMemory(stbi_uc* buffer, int bufLen, GLuint* out_texture, int* out_width, int* out_height);
void getMoviePosterAsImage(GLuint* texRef, std::string id, float* ratio, std::string apiKey, std::vector<GLuint>* posterIDs);

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
	char m_libraryFolderpath[MAX_FILEPATH_LENGTH];
	json* m_pImpInfo;
	std::vector<GLuint>* m_pPosterIDs;

	std::string m_apiKey = "f16097b7";

	int InitImGui(const char* WindowTitle);
	void Cleanup();
	void ChooseGLFWVersionForPlatform();

	bool findLibraryFolderPage(char* folderPath, int folderPathMaxLength, json* categoriesObj, json* libObj, bool* persistentBool);
	bool loadLibraryFiles(json* categories, json* library);

	//GLuint getMoviePosterAsImage(std::string id, float* ratio);
	//void searchDb(std::string vidTitle, json* resp);
	void chooseFromDb(std::string vidTitle, json* resp, json* choice, bool* choosing, bool* madeSelection, bool* postersLoaded);

	void DemoWindows();

};