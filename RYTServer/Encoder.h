#pragma once

#include <iostream>
#include <string>
#include <assert.h>
#include <thread>
#include <future>
#include <Windows.h>

#include <mfapi.h>  //Microsoft Media Foundation api
#include <mfidl.h>	//Source resolver
#include <mfreadwrite.h>
#include <codecapi.h> //h264 MPEG2 profile

#pragma comment(lib, "Mfplat.lib")  
#pragma comment(lib, "mfuuid.lib") //GUID symbols
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mf.lib")

#define MAX_NUMBER_OF_RESOLUTIONS 4

class Encoder
{

public:

	int Init(std::string fileName, bool* isRunning, std::string outputFilename = "output.mp4", int width = 0, int height = 0);
	int SendEncodedData();
	
private:
	//IMFSourceReader* m_sourceReader;
	//IMFSinkWriter* m_sinkWriter;
	IMFByteStream* m_outNetStream;

	std::string filename;

	//720, 1920, 1440, 4k. Video default will be preserved
	bool m_resolutions[MAX_NUMBER_OF_RESOLUTIONS];

	//Brooooo
	HRESULT OpenFileAndCreateSession(LPCWSTR sURL);
	HRESULT CreateMediaSource(LPCWSTR url, IMFMediaSource** ppMediaSource);
	HRESULT ConfigureAudioOutput();
	HRESULT ConfigureVideoOutput(LPCWSTR inFileName, LPCWSTR outFileName, int width = 0, int height = 0);
	HRESULT ConfigureContainer();
	HRESULT FinalizeMediaSession(IMFByteStream* ppByteStream, std::string outFileName, bool* isRunning);
	//HRESULT Transcode();
	HRESULT CreateOutputStream();

	HRESULT StartMediaSession();
	HRESULT ShutdownMediaSession();
	HRESULT ProcessMediaSessionEvents(bool* isRunning);
	HRESULT ChangeVideoResolution(int width = 0, int height = 0);

	IMFMediaSource* m_pSource;
	IMFMediaSource* m_pSourceDup; //Duplicate source to create SourceReader and get resolution
	IMFMediaSession* m_pSession;
	IMFTranscodeProfile* m_pProfile;
	IMFTopology* m_pTopology;

	GUID m_mediaSubtype = MFVideoFormat_H264;
};