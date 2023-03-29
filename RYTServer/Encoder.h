#include <iostream>
#include <string>
#include <assert.h>
#include <thread>
#include <future>

#include <mfapi.h>  //Microsoft Media Foundation api
#include <mfidl.h>	//Source resolver
#include <mfreadwrite.h>
#include <codecapi.h> //h264 MPEG2 profile

#pragma comment(lib, "Mfplat.lib")  
#pragma comment(lib, "mfuuid.lib") //GUID symbols
#pragma comment(lib, "mfreadwrite.lib")
#pragma comment(lib, "mf.lib")

class Encoder
{

public:

	int Init(std::string fileName, std::string outputFilename = "output.h264");
	int SendEncodedData();
	
private:
	//IMFSourceReader* m_sourceReader;
	//IMFSinkWriter* m_sinkWriter;
	IMFByteStream* m_outNetStream;

	std::string filename;

	//Brooooo
	HRESULT OpenFileAndCreateSession(LPCWSTR sURL);
	HRESULT CreateMediaSource(LPCWSTR url, IMFMediaSource** ppMediaSource);
	HRESULT ConfigureAudioOutput();
	HRESULT ConfigureVideoOutput();
	HRESULT ConfigureContainer();
	HRESULT FinalizeMediaSession(IMFByteStream* ppByteStream, std::string outFileName);
	//HRESULT Transcode();
	HRESULT CreateOutputStream();

	HRESULT StartMediaSession();
	HRESULT ShutdownMediaSession();
	HRESULT ProcessMediaSessionEvents();
	HRESULT ChangeVideoResolution(int width = 0, int height = 0);

	IMFMediaSource* m_pSource;
	IMFMediaSource* m_pSourceDup; //Duplicate source to create SourceReader and get resolution
	IMFMediaSession* m_pSession;
	IMFTranscodeProfile* m_pProfile;
	IMFTopology* m_pTopology;

	GUID m_mediaSubtype = MFVideoFormat_H264;
};