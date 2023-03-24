#include "VideoPlayer.h"


VideoPlayer::VideoPlayer(std::string FileName)
	: filename(FileName)
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
	if (InitEncoder() < 0)
	{
		std::cerr << "Couldn't initialize Encoder." << std::endl;
		return -1;
	}
	if (InitRendering() < 0)
	{
		std::cerr << "Couldn't initialize rendering." << std::endl;
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
	if (SendEncodedData() < 0)
	{
		std::cerr << "Sending encoded data returned <0" << std::endl;
		return;
	}
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
	ULONG startOK = MFStartup(MF_VERSION);
	if (startOK != S_OK)
	{
		std::cerr << "Couldn't start Media Foundation." << std::endl;
		return -1;
	}
	
	std::cout << "Filename: " << filename << std::endl;

	//Create bytestream from file
	//IMFByteStream* videoStream;
	std::wstring filename_w(filename.begin(), filename.end());
	/*HRESULT vsOK = MFCreateFile(MF_ACCESSMODE_READ, MF_OPENMODE_FAIL_IF_NOT_EXIST, MF_FILEFLAGS_NONE, filename_w.c_str(), &videoStream);
	if (vsOK != S_OK)
	{
		std::cerr << "Couldn't create bytestream from file. Error: " << vsOK << std::endl;
		return -1;
	}
	*/

	//Create the source reader
	HRESULT srOK = MFCreateSourceReaderFromURL(filename_w.c_str(), NULL, &m_sourceReader);
	if (!SUCCEEDED(srOK))
	{
		std::cerr << "Couldn't create media reader. Error: " << srOK << std::endl;
		return -1;
	}

	//Get Source input type
	IMFMediaType* sourceInputVideo;
	HRESULT hr = m_sourceReader->GetNativeMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, 0, &sourceInputVideo);
	if (FAILED(hr)) { std::cerr << "Couldn't get native type" << std::endl; return -1; }

	//Setup Source reader
	IMFMediaType* sourceOutputVideo;
	IMFMediaType* sourceOutputAudio;
	hr = MFCreateMediaType(&sourceOutputVideo);
	hr = MFCreateMediaType(&sourceOutputAudio);
	UINT32 sz_num = 0;
	UINT32 sz_denom = 0;
	UINT32 fr_num = 0;
	UINT32 fr_denom = 0;

	hr = sourceOutputVideo->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
	if (FAILED(hr)) { std::cerr << "Couldn't set sourceOutputVideo Major Type" << std::endl; return -1; }
	hr = sourceOutputVideo->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_RGB32);
	if (FAILED(hr)) { std::cerr << "Couldn't set source video output subtype. Error: " << hr << std::endl; return -1; }
	hr = sourceOutputVideo->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Unknown);
	if (FAILED(hr)) { std::cerr << "Couldn't set sourceOutputVideo interlace mode" << std::endl; return -1; }
	hr = MFGetAttributeSize(sourceInputVideo, MF_MT_FRAME_SIZE, &sz_num, &sz_denom);
	if (FAILED(hr)) { std::cerr << "Couldn't get sourceInputVideo frame size" << std::endl; return -1; }
	std::cout << "Frame Size ratio: " << sz_num << "/" << sz_denom << std::endl;
	hr = MFSetAttributeSize(sourceOutputVideo, MF_MT_FRAME_SIZE, sz_num, sz_denom);
	if (FAILED(hr)) { std::cerr << "Couldn't set sourceOutputVideo frame size" << std::endl; return -1; }
	hr = MFGetAttributeRatio(sourceInputVideo, MF_MT_FRAME_RATE, &fr_num, &fr_denom);
	if (FAILED(hr)) { std::cerr << "Couldn't get sourceInputVideo framrate" << std::endl; return -1; }
	//std::cout << "Frame rate ratio: " << fr_num << "/" << fr_denom << std::endl;
	hr = MFSetAttributeRatio(sourceOutputVideo, MF_MT_FRAME_RATE, fr_num, fr_denom);
	if (FAILED(hr)) { std::cerr << "Couldn't set sourceOutputVideo framrate" << std::endl; return -1; }
	hr = MFSetAttributeRatio(sourceOutputVideo, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);
	if (FAILED(hr)) { std::cerr << "Couldn't set sourceOutputVideo pixel aspect ratio" << std::endl; return -1; }
	//hr = m_sourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, sourceOutputVideo);
	if (FAILED(hr)) { std::cerr << "Couldn't set source video output. Error: " << hr << std::endl; return -1; }
	
	//Get sourceOutput stride
	IMFMediaType* sOutReal;
	hr = m_sourceReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, &sOutReal);
	if (FAILED(hr)) { std::cerr << "IDEK BRO" << std::endl; }
	//UINT32 stride;
	//hr = sOutReal->GetUINT32(MF_MT_DEFAULT_STRIDE, &stride);
	//std::cout << "stride: " << stride << " Error: " << hr << std::endl;
	
	sourceOutputAudio->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	sourceOutputAudio->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	hr = m_sourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, sourceOutputAudio);
	if (FAILED(hr)) { std::cerr << "Couldn't set source audio output. Error: " << hr << std::endl; }

	/*
	//Resolve the Source from the bytestream
	MF_OBJECT_TYPE objectType;
	IUnknown* source;
	//HRESULT objOK= resolver->CreateObjectFromByteStream(videoStream, NULL, NULL, NULL, &objectType, &source);
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
	MFT_REGISTER_TYPE_INFO h264_t = { MFMediaType_Video, MFVideoFormat_H264 };
	//MFT_REGISTER_TYPE_INFO mp4_t = {MFMediaType_Video, MFVideoFormat_MP4}
	IMFActivate** encoderActivatorArr;
	UINT32 numActivators;
	HRESULT mftOK = MFTEnumEx(MFT_CATEGORY_VIDEO_ENCODER, 0, NULL, &h264_t, &encoderActivatorArr, &numActivators);//(MFT Category, flags, input media type, output media type, (out) activator[], (out) numActivatorsFound)
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
	if (numActivators != 1)
	{
		std::cout << "Too many MFTransforms." << std::endl;
		return -1;
	}

	

	//Choose an activator, build transform
	IMFTransform* encoder;
	HRESULT encodeOK = encoderActivatorArr[0]->ActivateObject(IID_IMFTransform, (void**)&encoder);
	if (encodeOK != S_OK)
	{
		std::cerr << "Couldn't create encoder" << std::endl;
		return -1;
	}
	*/
	
	//Create the output ByteStream
	IStream* strm;
	HGLOBAL hgl = GlobalAlloc(GHND, 4210688);
	hr = CreateStreamOnHGlobal(hgl, false, &strm); //Istream needed to ByteStream
	if (FAILED(hr)) { std::cerr << "Couldn't create output Istream. Error: " << hr << std::endl; return -1; }
	hr = MFCreateMFByteStreamOnStream(strm, &m_outNetStream); //Create the ByteStream
	if (FAILED(hr)) { std::cerr << "Couldn't create output bytestream. Error: " << hr << std::endl; return -1; }
	m_outNetStream->SetLength(4210688);
	
	
	//IMFMediaSink* encodedSink;
	//MFCreateADTSMediaSink(outNetStream, encodedType, &encodedSink);
	
	//Create the attributes for the encoder
	UINT32 bitrate = 4096 * 1024;

	
	//Set Attribute values for h264 encoding
	IMFAttributes* encodedAttr;
	MFCreateAttributes(&encodedAttr, 1);
	
	//Create Sink Writer output type
	IMFMediaType* encodedType;
	MFCreateMediaType(&encodedType);
	
	hr = encodedType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video); //Required attributes for h264
	if (FAILED(hr)) { std::cerr << "Couldn't set encodedType video Major Type" << std::endl; return -1; }
	hr = encodedType->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264);// H264);
	if (FAILED(hr)) { std::cerr << "Couldn't set encodedType video subtype" << std::endl; return -1; }
	hr = encodedType->SetUINT32(MF_MT_AVG_BITRATE, bitrate);
	if (FAILED(hr)) { std::cerr << "Couldn't set encodedType video bitrate" << std::endl; return -1; }
	hr = MFSetAttributeRatio(encodedType, MF_MT_FRAME_RATE, fr_num, fr_denom); //Only works if num and denom are unchanged from getting native framrate
	if (FAILED(hr)) { std::cerr << "Couldn't set encodedType framerate" << std::endl; return -1; }
	hr = MFSetAttributeSize(encodedType, MF_MT_FRAME_SIZE, sz_num, sz_denom);
	if (FAILED(hr)) { std::cerr << "Couldn't set encodedType video size" << std::endl; return -1; }
	//hr = MFSetAttributeRatio(encodedType, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);
	if (FAILED(hr)) { std::cerr << "Couldn't set encodedType video aspect ratio" << std::endl; return -1; }
	hr = encodedType->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
	if (FAILED(hr)) { std::cerr << "Couldn't set encodedType video interlace mode" << std::endl; return -1; }
	//hr = encodedType->SetUINT32(MF_MT_MPEG2_PROFILE, eAVEncH264VProfile_Base);	
	if (FAILED(hr)) { std::cerr << "Couldn't set encodedType video mpeg2 profile" << std::endl; return -1; }
	

	encodedAttr->SetGUID(MF_TRANSCODE_CONTAINERTYPE, MFTranscodeContainerType_ADTS); //Required for SinkWriter to create without URL

	//Threading init is needed before CreateSinkWriter is called
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (FAILED(hr)) { std::cerr << "CoInitializeEx Failed. Error: " << hr << std::endl; return -1; }
	

	//MFCreateSinkWriterFromMediaSink(encodedSink, NULL, &sinkWriter);
	hr = MFCreateSinkWriterFromURL(NULL, m_outNetStream, encodedAttr, &m_sinkWriter);
	if (FAILED(hr)) { std::cerr << "Couldn't create SinkWriter. Error: " << hr << std::endl; return -1; }
	
	//m_sinkWriter->GetServiceForStream()

	//Set SinkWriter output type
	DWORD encodedStreamIndex;
	hr = m_sinkWriter->AddStream(encodedType, &encodedStreamIndex);
	if (FAILED(hr)) { std::cerr << "Couldn't add stream to SinkWriter. Error: " << hr << std::endl; return -1; }
	
	//Set SinkWriter input type
	hr = m_sinkWriter->SetInputMediaType(encodedStreamIndex, sourceOutputVideo, NULL); //May need to use attr here instead of output media type
	if (FAILED(hr)) { std::cerr << "Couldn't set SinkWriter input type. Error: " << hr << std::endl; return -1; }

	//Initilize Sink Writer to begin writing
	hr = m_sinkWriter->BeginWriting();
	if (FAILED(hr)) 
	{ 
		std::cerr << "Couldn't set SinkWriter to begin writing. Error: " << hr << std::endl;
		return -1; 
	}

	return 0;
}


int VideoPlayer::SendEncodedData()
{
	//Get frame from SourceReader
	DWORD actualInd_v;
	LONGLONG timeStamp_v;
	DWORD sourceReaderFlagsOut_v;
	IMFSample* sample_v;


	DWORD actualInd_a;
	LONGLONG timeStamp_a;
	DWORD sourceReaderFlagsOut_a;
	IMFSample* sample_a;
	
	HRESULT hr;
	
	for (int i = 0; i < 1; i++)
	{
		hr = m_sourceReader->ReadSample(MF_SOURCE_READER_FIRST_VIDEO_STREAM, NULL, &actualInd_v, &sourceReaderFlagsOut_v, &timeStamp_v, &sample_v);
		if (FAILED(hr))
		{
			std::cerr << "Couldn't get video sample from sourceReader. Error: " << hr << std::endl
				<< "Source Reader Flags Out: " << sourceReaderFlagsOut_v << std::endl;
			return -1;
		}

		/*hr = m_sourceReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, NULL, &actualInd_a, &sourceReaderFlagsOut_a, &timeStamp_a, &sample_a);
		if (FAILED(hr))
		{
			std::cerr << "Couldn't get audio sample from sourceReader. Error: " << hr << std::endl
				<< "Source Reader Flags Out: " << sourceReaderFlagsOut_a << std::endl;
			return -1;
		}*/

		//Give frame to SinkWriter
		hr = m_sinkWriter->WriteSample(0, sample_v);
		if (FAILED(hr)) { std::cerr << "Couldn't write video sample. Error: " << hr << std::endl; return -1; }
		//hr = m_sinkWriter->WriteSample(0, sample_a);
		if (FAILED(hr)) { std::cerr << "Couldn't write audio sample. Error: " << hr << std::endl; return -1; }
	}

	BYTE* encodedData;
	IMFMediaBuffer* encodedDataBuf;
	DWORD bufferMax = 1024;
	hr = MFCreateMemoryBuffer(bufferMax, &encodedDataBuf);
	if (FAILED(hr)) { std::cerr << "Couldn't create buffer. Error: " << hr << std::endl; }
	hr = encodedDataBuf->Lock(&encodedData, NULL, NULL);
	if (FAILED(hr)) { std::cerr << "Couldn't lock encodedDataBuffer. Error: " << hr << std::endl; }
	ULONG bytesOutput = 0;

	BOOL EOS;
	int count = 0;
	do
	{
		ZeroMemory(encodedData, bufferMax);
		hr = m_outNetStream->Read(encodedData, bufferMax, &bytesOutput);
		if (FAILED(hr)) { std::cerr << "Can't read data. Error: " << hr << std::endl; return -1; }
		std::cout << "Bytes output: " << bytesOutput << std::endl;
		
		//for (int i = 0; i < bytesOutput; i++)
		{
			//std::cout << "Output: " << (encodedData + i) << std::endl;
			std::cout << "Output:" << (encodedData) << ":end" << std::endl;
		}
		m_outNetStream->IsEndOfStream(&EOS);
	} while (!EOS);

		std::cout << "Two frames should have been written" << std::endl;
	return 0;
}

