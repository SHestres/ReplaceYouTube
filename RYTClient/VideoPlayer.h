#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
//#include <input.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"



#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")


#include <mfapi.h>  //Microsoft Media Foundation api
#include <mfidl.h>	//Source resolver
#include <mfreadwrite.h>
#include <codecapi.h> //h264 MPEG2 profile

#pragma comment(lib, "Mfplat.lib")  
#pragma comment(lib, "mfuuid.lib") //GUID symbols
#pragma comment(lib, "mfreadwrite.lib")

//#pragma comment(lib, "mfh265enc.dll")

//#pragma comment(lib, "dxva2.lib")
//#pragma comment(lib, "evr.lib")
//#pragma comment(lib, "mf.lib")
//#pragma comment(lib, "mfplat.lib")
//#pragma comment(lib, "mfplay.lib")
//#pragma comment(lib, "mfreadwrite.lib")



class VideoPlayer
{

public:
	VideoPlayer(std::string FileName);

	VideoPlayer(std::string FileName, int WindowWidth, int WindowHeight, std::string WindowName);

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

	int InitDecoder();
	void ReceiveEncodedData();

	//For server
	int InitEncoder();
	int SendEncodedData();
	IMFSourceReader* m_sourceReader;
	IMFSinkWriter* m_sinkWriter;
	IMFByteStream* m_outNetStream;

	std::string filename;
	
};
