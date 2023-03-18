#include "VideoPlayer.h"


VideoPlayer::VideoPlayer(std::string FileName)
{
	w_wid = 800;
	w_hei = 600;
	w_name = "VideoPlayer";
}

VideoPlayer::VideoPlayer(std::string FileName, int WindowWidth = 800, int WindowHeight = 600, std::string WindowName = "VideoPlayer")
	:filename(FileName), w_wid(WindowWidth), w_hei(WindowHeight), w_name(WindowName)
{}

VideoPlayer::~VideoPlayer()
{
	Cleanup();
}

//Initializes glad and glfw, opens a window, and defines a viewport
int VideoPlayer::Init()
{
	if (InitRendering() < 0)
	{
		std::cerr << "Couldn't initialize rendering." << std::endl;
		return -1;
	}
	if (InitEncoder() < 0)
	{
		std::cerr << "Couldn't initialize Encoder." << std::endl;
		return -1;
	}

	return 0;
}

void VideoPlayer::Cleanup()
{
	VAO1->Delete();
	VBO1->Delete();
	EBO1->Delete();
	shaderProgram->Delete();

	delete(uni_IDs);

	glfwDestroyWindow(window);
	glfwTerminate();
}

void VideoPlayer::Run()
{
	while (!glfwWindowShouldClose(window))
	{
		DrawToScreen();
	}
}

void VideoPlayer::DrawToScreen()
{
	//pollKeys(window);
		/*
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1))
		{
			//glfwGetCursorPos(window, &x, &y);
		}
		*/

		//Clear buffer and set to solid navy blue color
	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	//Apply function to color buffer
	glClear(GL_COLOR_BUFFER_BIT);

	//std::cout << xScale << std::endl;

	//Actually use shaderProgram
	shaderProgram->Activate();
	//Set the values of the uniforms
	glUniform1i(uni_IDs[0], w_wid);
	glUniform1i(uni_IDs[1], w_hei);
	/*glUniform1f(uniID_xScale, xScale);
	glUniform1f(uniID_yScale, yScale);
	glUniform1f(uniID_xOff, xOffset);
	glUniform1f(uniID_yOff, yOffset);*/

	//Bind the VAO. Unnecesary if only one, but a good habit
	VAO1->Bind();

	//Draw the primatives: Type, starting ind, number of verticies to draw
	//glDrawArrays(GL_TRIANGLES, 0, 3);

	//Draw the primatives: Type, Num of indices, datatype of indices, index of indices
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//Swap buffers to actually draw the frame
	glfwSwapBuffers(window);


	//Allows window to respond
	glfwPollEvents();
}

int VideoPlayer::InitRendering()
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


	GLfloat vertices[] =
	{
		-1.0f, -1.0f, 0.0f,
		-1.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f
	};

	GLuint indices[] =
	{
				0, 1, 2,
				0, 2, 3
	};

	//Initialize Shaders
	shaderProgram = new Shader("default.vert", "default.frag");

	VAO1 = new VAO;

	//Bind the Vertex Array to make it active
	VAO1->Bind();

	//Create the vertex and index buffers
	VBO1 = new VBO(vertices, sizeof(vertices));
	EBO1 = new EBO(indices, sizeof(indices));

	//Link Vertex buffer to the vertex array (configures attributes in the VAO)
	VAO1->LinkAttrib(*VBO1, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0); //Vertex positions
	//VAO1->LinkAttrib(*VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));  //Vertex colors

	//Unbind all when finished (VAO first so unbinding VBO doesn't mess with it)
	VAO1->Unbind();
	VBO1->Unbind();
	EBO1->Unbind();

	//Get the IDs of the uniforms from the shader
	uni_IDs = new GLuint[2];
	uni_IDs[0] = glGetUniformLocation(shaderProgram->ID, "width");
	uni_IDs[1] = glGetUniformLocation(shaderProgram->ID, "height");

	return 0;
}

int VideoPlayer::InitDecoder()
{
	//Some initiliazation??
	return 0;
}

void VideoPlayer::ReceiveEncodedData()
{
	//Receive bytes from server

	//Decode once there's enough

	//Reconstruct into frame data

	//Send to Renderer
}

int VideoPlayer::InitEncoder()
{
	//Create bytestream from file
	IMFByteStream* videoStream;
	std::wstring filename_w(filename.begin(), filename.end());
	HRESULT vsOK = MFCreateFile(MF_ACCESSMODE_READ, MF_OPENMODE_FAIL_IF_NOT_EXIST, MF_FILEFLAGS_NONE, filename_w.c_str(), &videoStream);

	//Create the source resolver
	IMFSourceResolver* resolver;
	HRESULT srOK = MFCreateSourceResolver(&resolver);
	if (srOK != S_OK)
	{
		std::cerr << "Couldn't create source resolver" << std::endl;
	}

	//Resolve the Source from the bytestream
	MF_OBJECT_TYPE objectType;
	IUnknown* source;
	HRESULT objOK= resolver->CreateObjectFromByteStream(videoStream, NULL, NULL, NULL, &objectType, &source);
	if (objOK != S_OK)
	{
		std::cerr << "Couldn't create Source object from filestream." << std::endl;
		return -1;
	}
	if (objectType == MF_OBJECT_INVALID)
	{
		std::cerr << "Source Object was invalid when created." << std::endl;
		return -1;
	}


	//Get an array of activators for possible transformers
	MFT_REGISTER_TYPE_INFO h265_t = { MFMediaType_Video, MFVideoFormat_H265 };
	//MFT_REGISTER_TYPE_INFO mp4_t = {MFMediaType_Video, MFVideoFormat_MP4}
	IMFActivate** encoderActivatorArr;
	UINT32 numActivators;
	HRESULT mftOK = MFTEnumEx(MFT_CATEGORY_VIDEO_ENCODER, 0, NULL, &h265_t, &encoderActivatorArr, &numActivators);//(MFT Category, flags, input media type, output media type, (out) activator[], (out) numActivatorsFound)
	if (numActivators <= 0)
	{
		std::cerr << "No available matching Media Transforms. Exiting" << std::endl;
		return -1;
	}
	if (mftOK != S_OK)
	{
		std::cerr << "Couldn't create Media Transform Activator." << std::endl;
		return -1;
	}
	if (numActivators == 1)
	{
		//std::cout << "MFT guid was " << encoderActivatorArr[0]->GetGUID << std::endl;
	}
	std::cout << "Num of activators: " << numActivators << std::endl;


	//Choose an activator

	//Create the sink
	
	//Open file and get bytestream

	//Create ASF Media Sink

	return 0;
}


void VideoPlayer::SendEncodedData()
{
	//Take available bytes from bytestream

	//Encode once there's enough data

	//Send to client
}

