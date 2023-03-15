#include "VideoPlayer.h"

VideoPlayer::VideoPlayer()
{
	w_wid = 800;
	w_hei = 600;
	w_name = "VideoPlayer";
}

VideoPlayer::VideoPlayer(int WindowWidth = 800, int WindowHeight = 600, std::string WindowName = "VideoPlayer")
	:w_wid(WindowWidth), w_hei(WindowHeight), w_name(WindowName)
{

}

VideoPlayer::~VideoPlayer()
{

}

//Initializes glad and glfw, opens a window, and defines a viewport
int VideoPlayer::Init()
{
	/////////Configuration
	glfwInit();

	//Tell glfw I'm using openGL v3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	//Tell glfw I'm using the core profile (doesn't have compatibility functions)
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	//Window setup and creation
	//
	//Create window: Width, height, name, fullscreen, idk
	window = glfwCreateWindow(w_wid, w_hei, w_name.c_str(), NULL, NULL);
	//Make sure it actually exists
	if (window == NULL)
	{
		std::cout << "Failed to create window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//Actually open and show the window by making it the context
	glfwMakeContextCurrent(window);
	//Load Glad to configure openGL  (idk why, but this can't be with the configuration section)
	gladLoadGL();
	//Tell openGL the area of the window we want to render in
	glViewport(0, 0, w_wid, w_hei);

	return 0;
}

void VideoPlayer::Cleanup()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void VideoPlayer::Run()
{
	std::string in;
	getline(std::cin, in);
}