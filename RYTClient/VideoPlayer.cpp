#include "VideoPlayer.h"

VideoPlayer::VideoPlayer()
{
	w_wid = 800;
	w_hei = 600;
	w_name = "VideoPlayer";

}

VideoPlayer::VideoPlayer(int WindowWidth = 800, int WindowHeight = 600, std::string WindowName = "VideoPlayer")
	:w_wid(WindowWidth), w_hei(WindowHeight), w_name(WindowName)
{}

VideoPlayer::~VideoPlayer()
{
	Cleanup();
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
}