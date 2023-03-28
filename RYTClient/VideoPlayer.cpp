#include "VideoPlayer.h"

//Forward Declaration
HRESULT InitializeSinkWriter(IMFSinkWriter** ppWriter, DWORD* pStreamIndex, LPCWSTR filename, UINT32 width, UINT32 height, DWORD bitrate);

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

template <class T> void SafeRelease(T** ppT);

//Initializes glad and glfw, opens a window, and defines a viewport
int VideoPlayer::Init()
{
	
	if (InitEncoder() < 0)
	{
		std::cerr << "Couldn't initialize Encoder." << std::endl;
		return -1;
	}
	//DWORD streamNum;
	//InitializeSinkWriter(&m_sinkWriter, &streamNum, L"output.asf", 720, 480, 80000);

	/*
	if (InitRendering() < 0)
	{
		std::cerr << "Couldn't initialize rendering." << std::endl;
		return -1;
	}
	*/

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
	std::clog << "Running..." << std::endl;

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


int VideoPlayer::InitEncoderOld()
{
	ULONG startOK = MFStartup(MF_VERSION);
	if (startOK != S_OK)
	{
		std::cerr << "Couldn't start Media Foundation." << std::endl;
		return -1;
	}
	
	//std::cout << "Filename: " << filename << std::endl;

	//Create bytestream from file
	//IMFByteStream* videoStream;
	//std::wstring filename_w(filename.begin(), filename.end());
	/*HRESULT vsOK = MFCreateFile(MF_ACCESSMODE_READ, MF_OPENMODE_FAIL_IF_NOT_EXIST, MF_FILEFLAGS_NONE, filename_w.c_str(), &videoStream);
	if (vsOK != S_OK)
	{
		std::cerr << "Couldn't create bytestream from file. Error: " << vsOK << std::endl;
		return -1;
	}
	*/

	//Create the source reader
	HRESULT srOK = MFCreateSourceReaderFromURL(NULL/*filename*/, NULL, &m_sourceReader);
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

HRESULT VideoPlayer::CreateOutputStream()
{
	HRESULT hr = S_OK;

	//Create the output ByteStream
	IStream* strm;
	HGLOBAL hgl = GlobalAlloc(GHND, 4210688);
	hr = CreateStreamOnHGlobal(hgl, false, &strm); //Istream needed to ByteStream
	if (FAILED(hr)) { std::cerr << "Couldn't create output Istream. Error: " << hr << std::endl; return hr; }
	hr = MFCreateMFByteStreamOnStream(strm, &m_outNetStream); //Create the ByteStream
	if (FAILED(hr)) { std::cerr << "Couldn't create output bytestream. Error: " << hr << std::endl; return hr; }
	hr = m_outNetStream->SetLength(4210688);
	if (FAILED(hr)) { std::cerr << "Couldn't set out stream length. Error: " << hr << std::endl; }
	
	return hr;
}

int VideoPlayer::InitEncoder()
{
	HRESULT hr = S_OK;

	hr = MFStartup(MF_VERSION);
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	//Convert string to LPCWSTR
	std::wstring filename_w(filename.begin(), filename.end());
	LPCWSTR fileUrl(filename_w.c_str());

	std::clog << "Opening File..." << std::endl;
	hr = OpenFile(fileUrl);
	if (FAILED(hr)) { std::cerr << "Couldn't open file. Error " << hr << std::endl; return -1; }
	
	std::clog << "Configuring Audio Output..." << std::endl;
	hr = ConfigureAudioOutput();
	if (FAILED(hr)) { std::cerr << "Couldn't configure Audio. Error " << hr << std::endl; return -1; }

	std::clog << "Configuring Vido Output..." << std::endl;
	hr = ConfigureVideoOutput();
	if (FAILED(hr)) { std::cerr << "Couldn't configure Video. Error " << hr << std::endl; return -1; }

	std::clog << "Configuring Container Output..." << std::endl;
	hr = ConfigureContainer();
	if (FAILED(hr)) { std::cerr << "Couldn't configure container. Error " << hr << std::endl; return -1; }

	std::clog << "Creating Output Stream..." << std::endl;
	hr = CreateOutputStream();
	if (FAILED(hr)) { std::cerr << "Couldn't create output stream. Error " << hr << std::endl; return -1; }

	std::clog << "Finalizing Media Session..." << std::endl;
	hr = FinalizeMediaSession(m_outNetStream);
	if (FAILED(hr)) { std::cerr << "Couldn't finalize media session. Error " << hr << std::endl; return -1; }


	return 0;
}

HRESULT VideoPlayer::OpenFile(LPCWSTR sURL)
{
	if (!sURL)
	{
		return E_INVALIDARG;
	}

	HRESULT hr = S_OK;

	// Create the media source.
	hr = CreateMediaSource(sURL, &m_pSource);
	if (FAILED(hr)) { std::cerr << "Couldn't create Media Source. Error " << hr << std::endl; return hr; }

	//Create the media session.
	hr = MFCreateMediaSession(NULL, &m_pSession);

	// Create an empty transcode profile.
	if (SUCCEEDED(hr))
	{
		hr = MFCreateTranscodeProfile(&m_pProfile);
	}
	return hr;
}

HRESULT VideoPlayer::CreateMediaSource(LPCWSTR url, IMFMediaSource **ppMediaSource)
{
	HRESULT hr = S_OK;

	IMFSourceResolver* SourceResolver = NULL;


	// Create the source resolver.
	hr = MFCreateSourceResolver(&SourceResolver);

	//Create the source from the resolver
	MF_OBJECT_TYPE ObjType;
	IUnknown* UnknownSource = NULL;
	std::wstring urlOut_w(url);
	std::string urlOut(urlOut_w.begin(), urlOut_w.end());
	std::cout << "Url: " << urlOut << std::endl;
	hr = SourceResolver->CreateObjectFromURL(url, MF_RESOLUTION_MEDIASOURCE, NULL, &ObjType, &UnknownSource);
	if (FAILED(hr)) { std::cerr << "Couldn't create Object from URL. Error " << hr << std::endl; return hr; }

	//Get the MediaSource from the IUnknown obj
	hr = UnknownSource->QueryInterface(IID_PPV_ARGS(ppMediaSource));

	SafeRelease(&SourceResolver);
	SafeRelease(&UnknownSource);

	return hr;
}

HRESULT VideoPlayer::ConfigureAudioOutput()
{
	assert(m_pProfile);

	HRESULT hr = S_OK;
	DWORD dwMTCount = 0;

	IMFCollection* pAvailableTypes = NULL;
	IUnknown* pUnkAudioType = NULL;
	IMFMediaType* pAudioType = NULL;
	IMFAttributes* pAudioAttrs = NULL;

	// Get the list of output formats supported by the Windows Media 
	// audio encoder.

	hr = MFTranscodeGetAudioOutputAvailableTypes(
		MFAudioFormat_WMAudioV9,
		MFT_ENUM_FLAG_ALL,
		NULL,
		&pAvailableTypes
	);

	// Get the number of elements in the list.

	if (SUCCEEDED(hr))
	{
		hr = pAvailableTypes->GetElementCount(&dwMTCount);

		if (dwMTCount == 0)
		{
			hr = E_UNEXPECTED;
		}
	}

	// In this simple case, use the first media type in the collection.

	if (SUCCEEDED(hr))
	{
		hr = pAvailableTypes->GetElement(0, &pUnkAudioType);
	}

	if (SUCCEEDED(hr))
	{
		hr = pUnkAudioType->QueryInterface(IID_PPV_ARGS(&pAudioType));
	}

	// Create a copy of the attribute store so that we can modify it safely.
	if (SUCCEEDED(hr))
	{
		hr = MFCreateAttributes(&pAudioAttrs, 0);
	}

	if (SUCCEEDED(hr))
	{
		hr = pAudioType->CopyAllItems(pAudioAttrs);
	}

	// Set the encoder to be Windows Media audio encoder, so that the 
	// appropriate MFTs are added to the topology.

	if (SUCCEEDED(hr))
	{
		hr = pAudioAttrs->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_WMAudioV9);
	}

	// Set the attribute store on the transcode profile.
	if (SUCCEEDED(hr))
	{
		hr = m_pProfile->SetAudioAttributes(pAudioAttrs);
	}

	SafeRelease(&pAvailableTypes);
	SafeRelease(&pAudioType);
	SafeRelease(&pUnkAudioType);
	SafeRelease(&pAudioAttrs);

	return hr;
}

HRESULT VideoPlayer::ConfigureVideoOutput()
{
	assert(m_pProfile);

	HRESULT hr = S_OK;

	IMFAttributes* pVideoAttrs = NULL;

	// Configure the video stream

	// Create a new attribute store.
	if (SUCCEEDED(hr))
	{
		hr = MFCreateAttributes(&pVideoAttrs, 5);
	}

	// Set the encoder to be h264 encoder, so that the appropriate MFTs are added to the topology.
	if (SUCCEEDED(hr))
	{
		hr = pVideoAttrs->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264);
	}

	// Set the frame rate.
	if (SUCCEEDED(hr))
	{
		hr = MFSetAttributeRatio(pVideoAttrs, MF_MT_FRAME_RATE, 30, 1);
	}

	//Set the frame size.
	if (SUCCEEDED(hr))
	{
		hr = MFSetAttributeSize(pVideoAttrs, MF_MT_FRAME_SIZE, 320, 240);
	}

	//Set the pixel aspect ratio
	if (SUCCEEDED(hr))
	{
		hr = MFSetAttributeRatio(pVideoAttrs, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);
	}

	// Set the bit rate.
	if (SUCCEEDED(hr))
	{
		hr = pVideoAttrs->SetUINT32(MF_MT_AVG_BITRATE, 300000);
	}

	// Set the attribute store on the transcode profile.
	if (SUCCEEDED(hr))
	{
		hr = m_pProfile->SetVideoAttributes(pVideoAttrs);
	}

	SafeRelease(&pVideoAttrs);
	return hr;
}

HRESULT VideoPlayer::ConfigureContainer()
{
	assert(m_pProfile);

	HRESULT hr = S_OK;

	IMFAttributes* pContainerAttrs = NULL;

	//Set container attributes
	hr = MFCreateAttributes(&pContainerAttrs, 2);

	//Set the output container to be ASF type
	if (SUCCEEDED(hr))
	{
		hr = pContainerAttrs->SetGUID(
			MF_TRANSCODE_CONTAINERTYPE,
			MFTranscodeContainerType_ASF
		);
	}

	// Use the default setting. Media Foundation will use the stream 
	// settings set in ConfigureAudioOutput and ConfigureVideoOutput.

	if (SUCCEEDED(hr))
	{
		hr = pContainerAttrs->SetUINT32(
			MF_TRANSCODE_ADJUST_PROFILE,
			MF_TRANSCODE_ADJUST_PROFILE_DEFAULT
		);
	}

	//Set the attribute store on the transcode profile.
	if (SUCCEEDED(hr))
	{
		hr = m_pProfile->SetContainerAttributes(pContainerAttrs);
	}

	SafeRelease(&pContainerAttrs);
	return hr;
}

HRESULT VideoPlayer::FinalizeMediaSession(IMFByteStream* pByteStream)
{
	assert(m_pSession);
	assert(m_pSource);
	assert(m_pProfile);

	if (!pByteStream)
	{
		return E_INVALIDARG;
	}

	HRESULT hr = S_OK;

	//Create the transcode topology
	//hr = MFCreateTranscodeTopologyFromByteStream(m_pSource, pByteStream, m_pProfile, &m_pTopology);
	hr = MFCreateTranscodeTopology(m_pSource, L"output.h264", m_pProfile, &m_pTopology);
	if (FAILED(hr)) { std::cerr << "Couldn't create Topology" << std::endl; }

	// Set the topology on the media session.
	hr = m_pSession->SetTopology(0, m_pTopology);
	if (FAILED(hr)) { std::cerr << "Couldn't set Topology" << std::endl; }
	
	std::async([&] {VideoPlayer::ProcessMediaSessionEvents();}); //Lambda function for async
	std::clog << "Processing Media Session Events..." << std::endl;
	//hr = ProcessMediaSessionEvents();

	return hr;
}

HRESULT VideoPlayer::ProcessMediaSessionEvents()
{
	assert(m_pSession);

	IMFMediaEvent* pEvent = NULL;
	MediaEventType meType = MEUnknown;  // Event type

	HRESULT hr = S_OK;
	HRESULT hrStatus = S_OK;            // Event status


	//Get media session events (will be run asynchronously)
	while (meType != MESessionClosed)
	{
		hr = m_pSession->GetEvent(0, &pEvent);

		if (FAILED(hr)) { break; }

		// Get the event type.
		hr = pEvent->GetType(&meType);

		if (FAILED(hr)) { break; }

		hr = pEvent->GetStatus(&hrStatus);

		if (FAILED(hr)) { break; }

		if (FAILED(hrStatus))
		{
			wprintf_s(L"Failed. 0x%X error condition triggered this event.\n", hrStatus);
			hr = hrStatus;
			break;
		}

		switch (meType)
		{
		case MESessionTopologySet:
			hr = StartMediaSession();
			if (SUCCEEDED(hr))
			{
				std::clog << "Ready to start." << std::endl;
			}
			break;

		case MESessionStarted:
			std::clog << "Started encoding..." << std::endl;
			break;

		case MESessionEnded:
			hr = m_pSession->Close();
			if (SUCCEEDED(hr))
			{
				std::clog << "Finished encoding." << std::endl;
			}
			break;

		case MESessionClosed:
			std::clog << "Stream Ended." << std::endl;
			break;
		}

		if (FAILED(hr))
		{
			break;
		}

		SafeRelease(&pEvent);
	}

	SafeRelease(&pEvent);
	return hr;
}

HRESULT VideoPlayer::StartMediaSession()
{
	assert(m_pSession != NULL);

	HRESULT hr = S_OK;

	PROPVARIANT varStart;
	PropVariantInit(&varStart);

	hr = m_pSession->Start(NULL, &varStart);

	if (FAILED(hr))
	{
		std::cerr << "Failed to start the session..." << std::endl;
	}
	return hr;
}

HRESULT VideoPlayer::ShutdownMediaSession()
{
	HRESULT hr = S_OK;

	// Shut down the media source
	if (m_pSource)
	{
		hr = m_pSource->Shutdown();
	}

	// Shut down the media session.
	if (SUCCEEDED(hr))
	{
		if (m_pSession)
		{
			hr = m_pSession->Shutdown();
		}
	}

	if (FAILED(hr))
	{
		std::cerr << "Failed to close the session..." << std::endl;
	}

	return hr;
}

int VideoPlayer::SendEncodedDataOld()
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

int VideoPlayer::SendEncodedData()
{
	BYTE* encodedData;
	IMFMediaBuffer* encodedDataBuf;
	DWORD bufferMax = 1024;
	HRESULT hr = MFCreateMemoryBuffer(bufferMax, &encodedDataBuf);
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
		/*std::cout << "Bytes output: " << bytesOutput << std::endl;

		std::cout << "Output:";
		for (int i = 0; i < bytesOutput; i++)
		{
			//std::cout << "Output: " << (encodedData + i) << std::endl;
			std::cout << (encodedData + i);
		}
		std::cout << ":end" << std::endl;*/
		m_outNetStream->IsEndOfStream(&EOS);
	} while (!EOS);

	std::cout << "End of stream" << std::endl;
}


template <class T> void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}


HRESULT InitializeSinkWriter(IMFSinkWriter** ppWriter, DWORD* pStreamIndex, LPCWSTR filename, UINT32 width, UINT32 height, DWORD bitrate)
{
	HRESULT hr = S_OK;
	IMFMediaType* pMediaTypeOut = NULL;
	IMFSinkWriter* pSinkWriter = NULL;

	*ppWriter = NULL;
	*pStreamIndex = NULL;

	ULONG startOK = MFStartup(MF_VERSION);
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	if (FAILED(hr)) { std::cout << "-1" << std::endl; goto done; }

	// Create the sink writer
	hr = MFCreateSinkWriterFromURL(filename, NULL, NULL, &pSinkWriter);
	if (FAILED(hr)) { std::cout << "0" << std::endl; goto done; }

	// Set the output media type
	hr = MFCreateMediaType(&pMediaTypeOut);
	if (FAILED(hr)) { std::cout << "1" << std::endl; goto done; }

	hr = pMediaTypeOut->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
	if (FAILED(hr)) { std::cout << "2" << std::endl; goto done; }

	hr = pMediaTypeOut->SetGUID(MF_MT_SUBTYPE, MFVideoFormat_H264);
	if (FAILED(hr)) { std::cout << "3" << std::endl; goto done; }

	hr = pMediaTypeOut->SetUINT32(MF_MT_AVG_BITRATE, bitrate);
	if (FAILED(hr)) { std::cout << "4" << std::endl; goto done; }

	hr = pMediaTypeOut->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
	if (FAILED(hr)) { std::cout << "5" << std::endl; goto done; }

	hr = MFSetAttributeSize(pMediaTypeOut, MF_MT_FRAME_SIZE, width, height);
	if (FAILED(hr)) { std::cout << "6" << std::endl; goto done; }

	hr = MFSetAttributeRatio(pMediaTypeOut, MF_MT_FRAME_RATE, 30, 1);
	if (FAILED(hr)) { std::cout << "7" << std::endl; goto done; }

	hr = MFSetAttributeRatio(pMediaTypeOut, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);
	if (FAILED(hr)) { std::cout << "8" << std::endl; goto done; }

	hr = pSinkWriter->AddStream(pMediaTypeOut, pStreamIndex);
	if (FAILED(hr)) { std::cout << "9" << std::endl; goto done; }

	//hr = pSinkWriter->SetInputMediaType(*pStreamIndex, pMediaTypeOut, NULL);
	if (FAILED(hr)) { std::cout << "10" << std::endl; goto done; }

	// Tell the sink writer to start accepting data
	hr = pSinkWriter->BeginWriting();
	if (FAILED(hr)) { std::cout << "11" << std::endl; goto done; }

	// Return the pointer to the caller
	*ppWriter = pSinkWriter;
	(*ppWriter)->AddRef();

	std::cout << "Success" << std::endl;

done:
	std::cout << "Hr: " << hr << std::endl;
	SafeRelease((IUnknown**) &pSinkWriter);
	SafeRelease((IUnknown**) &pMediaTypeOut);
	return hr;
}


/*
// Format constants
const UINT32 VIDEO_WIDTH = 640;
const UINT32 VIDEO_HEIGHT = 480;
const UINT32 VIDEO_FPS = 30;
const UINT64 VIDEO_FRAME_DURATION = 10 * 1000 * 1000 / VIDEO_FPS;
const UINT32 VIDEO_BIT_RATE = 800000;
const GUID   VIDEO_ENCODING_FORMAT = MFVideoFormat_H265;
const GUID   VIDEO_INPUT_FORMAT = MFVideoFormat_IYUV;
const UINT32 VIDEO_PELS = VIDEO_WIDTH * VIDEO_HEIGHT;
const UINT32 VIDEO_FRAME_COUNT = 20 * VIDEO_FPS;





HRESULT InitializeSinkWriter(IMFSinkWriter** ppWriter, DWORD* pStreamIndex)
{
	*ppWriter = NULL;
	*pStreamIndex = NULL;

	IMFSinkWriter* pSinkWriter = NULL;
	IMFMediaType* pMediaTypeOut = NULL;
	IMFMediaType* pMediaTypeIn = NULL;
	DWORD           streamIndex;

	ULONG startOK = MFStartup(MF_VERSION);
	HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	hr = MFCreateSinkWriterFromURL(L"output.wmv", NULL, NULL, &pSinkWriter);
	if (FAILED(hr)) { std::cout << "Failed" << std::endl; return hr; }

	// Buffer to hold the video frame data.
	DWORD *videoFrameBufferTut = (DWORD*)malloc(VIDEO_PELS * sizeof(DWORD));

	// Set all pixels to green in input frame
	for (DWORD i = 0; i < VIDEO_PELS; i++)
	{
		videoFrameBufferTut[i] = 0x0000FF00;
	}

	// Set the output media type.
	if (SUCCEEDED(hr))
	{
		std::cout << "1" << std::endl;
		hr = MFCreateMediaType(&pMediaTypeOut);
	}
	if (SUCCEEDED(hr))
	{
		std::cout << "2" << std::endl;
		hr = pMediaTypeOut->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
	}
	if (SUCCEEDED(hr))
	{
		std::cout << "3" << std::endl;
		hr = pMediaTypeOut->SetGUID(MF_MT_SUBTYPE, VIDEO_ENCODING_FORMAT);
	}
	if (SUCCEEDED(hr))
	{
		std::cout << "4" << std::endl;
		hr = pMediaTypeOut->SetUINT32(MF_MT_AVG_BITRATE, VIDEO_BIT_RATE);
	}
	if (SUCCEEDED(hr))
	{
		std::cout << "5" << std::endl;
		hr = pMediaTypeOut->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
	}
	if (SUCCEEDED(hr))
	{
		std::cout << "6" << std::endl;
		hr = MFSetAttributeSize(pMediaTypeOut, MF_MT_FRAME_SIZE, VIDEO_WIDTH, VIDEO_HEIGHT);
	}
	if (SUCCEEDED(hr))
	{
		std::cout << "7" << std::endl;
		hr = MFSetAttributeRatio(pMediaTypeOut, MF_MT_FRAME_RATE, VIDEO_FPS, 1);
	}
	if (SUCCEEDED(hr))
	{
		std::cout << "8" << std::endl;
		hr = MFSetAttributeRatio(pMediaTypeOut, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);
	}
	if (SUCCEEDED(hr))
	{
		std::cout << "9" << std::endl;
		hr = pSinkWriter->AddStream(pMediaTypeOut, &streamIndex);
	}

	// Set the input media type.
	if (SUCCEEDED(hr))
	{
		std::cout << "10" << std::endl;
		hr = MFCreateMediaType(&pMediaTypeIn);
	}
	if (SUCCEEDED(hr))
	{
		std::cout << "11" << std::endl;
		hr = pMediaTypeIn->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
	}
	if (SUCCEEDED(hr))
	{
		std::cout << "12" << std::endl;
		hr = pMediaTypeIn->SetGUID(MF_MT_SUBTYPE, VIDEO_INPUT_FORMAT);
	}
	if (SUCCEEDED(hr))
	{
		std::cout << "13" << std::endl;
		//hr = pMediaTypeIn->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
	}
	if (SUCCEEDED(hr))
	{
		std::cout << "14" << std::endl;
		//hr = MFSetAttributeSize(pMediaTypeIn, MF_MT_FRAME_SIZE, VIDEO_WIDTH, VIDEO_HEIGHT);
	}
	if (SUCCEEDED(hr))
	{
		std::cout << "15" << std::endl;
		//hr = MFSetAttributeRatio(pMediaTypeIn, MF_MT_FRAME_RATE, VIDEO_FPS, 1);
	}
	if (SUCCEEDED(hr))
	{
		std::cout << "16" << std::endl;
		//hr = MFSetAttributeRatio(pMediaTypeIn, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);
	}
	if (SUCCEEDED(hr))
	{
		std::cout << "17" << std::endl;
		hr = pSinkWriter->SetInputMediaType(streamIndex, pMediaTypeIn, NULL);
	}
	else { std::cerr << "Hr: " << hr << std::endl; }

	// Tell the sink writer to start accepting data.
	if (SUCCEEDED(hr))
	{
		std::cout << "18" << std::endl;
		hr = pSinkWriter->BeginWriting();
	}
	else { std::cerr << "Hr: " << hr << std::endl; }

	// Return the pointer to the caller.
	if (SUCCEEDED(hr))
	{
		std::cout << "Success" << std::endl;
		*ppWriter = pSinkWriter;
		(*ppWriter)->AddRef();
		*pStreamIndex = streamIndex;
	}

	pSinkWriter->Release();
	pMediaTypeOut->Release();
	pMediaTypeIn->Release();
	return hr;
}
*/