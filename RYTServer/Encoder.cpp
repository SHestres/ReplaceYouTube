#include "Encoder.h"

//Encoder

template <class T> void SafeRelease(T** ppT);

HRESULT Encoder::CreateOutputStream()
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

int Encoder::Init(std::string fileName, std::string outputFilename)
{
	filename = fileName;

	HRESULT hr = S_OK;

	hr = MFStartup(MF_VERSION);
	hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	//Convert string to LPCWSTR
	std::wstring filename_w(filename.begin(), filename.end());
	LPCWSTR fileUrl(filename_w.c_str());

	std::clog << "Opening File..." << std::endl;
	hr = OpenFileAndCreateSession(fileUrl);
	if (FAILED(hr)) { std::cerr << "Couldn't open file. Error " << hr << std::endl; return -1; }

	std::clog << "Configuring Audio Output..." << std::endl;
	hr = ConfigureAudioOutput();
	if (FAILED(hr)) { std::cerr << "Couldn't configure Audio. Error " << hr << std::endl; return -1; }

	std::clog << "Configuring Video Output..." << std::endl;
	try {
		hr = ConfigureVideoOutput();
	}
	catch (std::runtime_error excpt) { std::cerr << excpt.what() << std::endl; return 2; }
	if (FAILED(hr)) { std::cerr << "Couldn't configure Video. Error " << hr << std::endl; return -1; }

	std::clog << "Configuring Container Output..." << std::endl;
	hr = ConfigureContainer();
	if (FAILED(hr)) { std::cerr << "Couldn't configure container. Error " << hr << std::endl; return -1; }

	std::clog << "Creating Output Stream..." << std::endl;
	hr = CreateOutputStream();
	if (FAILED(hr)) { std::cerr << "Couldn't create output stream. Error " << hr << std::endl; return -1; }

	std::clog << "Finalizing Media Session..." << std::endl;
	hr = FinalizeMediaSession(m_outNetStream, outputFilename);
	if (FAILED(hr)) { std::cerr << "Couldn't finalize media session. Error " << hr << std::endl; return -1; }


	return 0;
}

HRESULT Encoder::OpenFileAndCreateSession(LPCWSTR sURL)
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

HRESULT Encoder::CreateMediaSource(LPCWSTR url, IMFMediaSource** ppMediaSource)
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
	
	//Create duplicate for SourceReader
	/*hr = SourceResolver->CreateObjectFromURL(url, MF_RESOLUTION_MEDIASOURCE, NULL, &ObjType, &UnknownSource);
	if (FAILED(hr)) { std::cerr << "Couldn't create Object from URL. Error " << hr << std::endl; return hr; }
	hr = UnknownSource->QueryInterface(IID_PPV_ARGS(&m_pSourceDup));
	*/

	SafeRelease(&SourceResolver);
	SafeRelease(&UnknownSource);

	return hr;
}

HRESULT Encoder::ConfigureAudioOutput()
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

HRESULT Encoder::ConfigureVideoOutput()
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
		hr = pVideoAttrs->SetGUID(MF_MT_SUBTYPE, m_mediaSubtype);
	}

	//Get input video info
	UINT32 frameWidth, frameHeight, frameRateNum, frameRateDenom;
	IMFSourceReader* reader;
	IMFMediaType* fileType;
	hr = MFCreateMediaType(&fileType);
	hr = MFCreateSourceReaderFromURL(std::wstring(filename.begin(), filename.end()).c_str(), NULL, &reader);
	hr = reader->GetNativeMediaType(MF_SOURCE_READER_FIRST_VIDEO_STREAM, MF_SOURCE_READER_CURRENT_TYPE_INDEX, &fileType);
	hr = MFGetAttributeSize(fileType, MF_MT_FRAME_SIZE, &frameWidth, &frameHeight);
	if (FAILED(hr)) { std::cerr << "GettingAttribute failed" << std::endl; return hr; }
	hr = MFGetAttributeRatio(fileType, MF_MT_FRAME_RATE, &frameRateNum, &frameRateDenom);
	if (FAILED(hr)) { std::cerr << "GettingAttribute failed" << std::endl; return hr; }
	GUID subtype;
	hr = fileType->GetGUID(MF_MT_SUBTYPE, &subtype);
	if (subtype == m_mediaSubtype) { throw std::runtime_error("Video already in requested format"); }

	std::clog << "Frame size: " << frameWidth << "x" << frameHeight << std::endl;
	std::clog << "FrameRate: " << frameRateNum << "/" << frameRateDenom << std::endl;

	SafeRelease(&reader);
	SafeRelease(&fileType);

	// Set the frame rate.
	if (SUCCEEDED(hr))
	{
		hr = MFSetAttributeRatio(pVideoAttrs, MF_MT_FRAME_RATE, frameRateNum, frameRateDenom);
	}

	//Set the frame size.
	if (SUCCEEDED(hr))
	{
		hr = MFSetAttributeSize(pVideoAttrs, MF_MT_FRAME_SIZE, frameWidth, frameHeight);
	}

	//Set the pixel aspect ratio
	if (SUCCEEDED(hr))
	{
		hr = MFSetAttributeRatio(pVideoAttrs, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);
	}

	// Set the bit rate.
	if (SUCCEEDED(hr))
	{
		hr = pVideoAttrs->SetUINT32(MF_MT_AVG_BITRATE, 3000000);
	}

	// Set the attribute store on the transcode profile.
	if (SUCCEEDED(hr))
	{
		hr = m_pProfile->SetVideoAttributes(pVideoAttrs);
	}

	
	SafeRelease(&pVideoAttrs);


	return hr;
}

HRESULT Encoder::ChangeVideoResolution(int width, int height)
{
	HRESULT hr = S_OK;

	if (width == 0 || height == 0)
	{

	}
	else
	{

	}

	return hr;
}

HRESULT Encoder::ConfigureContainer()
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

HRESULT Encoder::FinalizeMediaSession(IMFByteStream* pByteStream, std::string outFileName)
{
	assert(m_pSession);
	assert(m_pSource);
	assert(m_pProfile);

	std::wstring outName_w(outFileName.begin(), outFileName.end());

	if (!pByteStream)
	{
		return E_INVALIDARG;
	}

	HRESULT hr = S_OK;

	//Create the transcode topology
	//hr = MFCreateTranscodeTopologyFromByteStream(m_pSource, pByteStream, m_pProfile, &m_pTopology);
	hr = MFCreateTranscodeTopology(m_pSource, outName_w.c_str(), m_pProfile, &m_pTopology);
	if (FAILED(hr)) { std::cerr << "Couldn't create Topology" << std::endl; }

	// Set the topology on the media session.
	hr = m_pSession->SetTopology(0, m_pTopology);
	if (FAILED(hr)) { std::cerr << "Couldn't set Topology" << std::endl; }

	std::async([&] {Encoder::ProcessMediaSessionEvents(); }); //Lambda function for async
	std::clog << "Processing Media Session Events..." << std::endl;
	//hr = ProcessMediaSessionEvents();

	return hr;
}

HRESULT Encoder::ProcessMediaSessionEvents()
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

HRESULT Encoder::StartMediaSession()
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

HRESULT Encoder::ShutdownMediaSession()
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

int Encoder::SendEncodedData()
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
