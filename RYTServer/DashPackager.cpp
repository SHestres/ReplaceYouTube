#include "DashPackager.h"
#include <cctype>

DashPackager::DashPackager() {}

DashPackager::~DashPackager() {}

bool DashPackager::Init(std::string fileName, std::string libPath, std::string videoID)
{
	m_fileName = fileName;
	m_videoID = videoID;
	m_libPath = libPath;

	return true;
}

bool dirExists(LPCWSTR path, vidStat* packagingStatus) {
	if (INVALID_FILE_ATTRIBUTES == GetFileAttributes(path)) {
		std::cout << "Get attributes errored" << std::endl;
		DWORD err = GetLastError();
		if (err == ERROR_FILE_NOT_FOUND || err == ERROR_INVALID_NAME || err == ERROR_BAD_NETPATH || err == ERROR_PATH_NOT_FOUND) {
			std::cout << "Error was one of these" << std::endl;
			return false;
		}
		std::cout << err << std::endl;
		*packagingStatus = FAILED;
		return true;
	}
	*packagingStatus = BAD_ID;
	return true;
}

//Title should have no whitespaces
void runPackagingAsync(std::string inFile, std::string libPath, std::string videoID, vidStat* packagingStatus, int* step, std::string* errorMsg) {

	//Create filename for temp file
	std::string outFile = "temp" + videoID + ".mp4";

	//Create error checking vars
	std::string respStr;
	bool error;

	//Check final folder doesn't already exist
	std::string finalFolder = libPath + VIDEO_DIR_EXT + videoID;
	std::wstring wFol(finalFolder.begin(), finalFolder.end());
	if (dirExists(wFol.c_str(), packagingStatus)) {
		return;
	}

	while (*step < 1);

	*packagingStatus = FRAGMENTING;
	//Fragment process
	std::string command = "dashProgs\\mp4fragment " + inFile + " " + outFile + " > resp.txt 2> resp.err";
	system(command.c_str());
	std::ifstream fragResp("resp.err");
	std::stringstream fragRespBuf;
	fragRespBuf << fragResp.rdbuf();
	respStr = fragRespBuf.str();
	error = respStr.find("ERROR") != std::string::npos;
	if (error) {
		*packagingStatus = FAILED;
		*errorMsg = respStr;
		return;
	}
	
	while (*step < 2);

	*packagingStatus = PROCESSING;
	//Dash package process
	command = "dashProgs\\mp4-dash " + outFile + " > resp.txt 2> resp.err";
	system(command.c_str());
	std::ifstream packResp("resp.err");
	std::stringstream packRespBuf;
	packRespBuf << packResp.rdbuf();
	respStr = packRespBuf.str();
	error = respStr.find("ERROR") != std::string::npos;
	if (error) {
		*packagingStatus = FAILED;
		*errorMsg = respStr;
		return;
	}

	std::cout << "Done packaging" << std::endl;

	while (*step < 3);

	*packagingStatus = MOVING_FILES;
	//Rename output folder
	command = "REN output " + videoID + " > resp.txt 2> resp.err";
	system(command.c_str());
	std::ifstream renameResp("resp.err");
	std::stringstream renameRespBuf;
	renameRespBuf << renameResp.rdbuf();
	respStr = renameRespBuf.str();
	error = respStr.find("Error") != std::string::npos;
	if (error) {
		*packagingStatus = FAILED;
		*errorMsg = respStr;
		return;
	}

	while (*step < 4);

	//Move dir with mpd to be accessable by api
	command = "MOVE " + videoID + " " + finalFolder + " > resp.txt 2> resp.err";
	system(command.c_str());
	std::ifstream moveResp("resp.err");
	std::stringstream moveRespBuf;
	moveRespBuf << moveResp.rdbuf();
	respStr = moveRespBuf.str();
	error = respStr.find("Error") != std::string::npos;
	if (error) {
		*packagingStatus = FAILED;
		*errorMsg = respStr;
		return;
	}

	while (*step < 5);

	//Clean temp files
	try {
		DeleteFile(std::wstring(outFile.begin(), outFile.end()).c_str());
	}
	catch (std::exception e) { std::cerr << "Couldn't delete file" << std::endl; }

	*packagingStatus = FINISHING;
}


void doNothing(int i) {
	return;
}

std::future<void> DashPackager::Run(vidStat* packagingStatus, int* step, std::string* errorMsg) {
	
	*packagingStatus = STARTING;

	std::string safeFileName = "\"" + m_fileName + "\"";
	std::future<void> f = std::async(std::launch::async, runPackagingAsync, safeFileName, m_libPath, m_videoID, packagingStatus, step, errorMsg);

	return f;

	//Execute processes 
	

	/* Media foundation doesn't correctly process files bigger than a few MB
	Encoder encoder;
	
	
	bool isRunning = true;

	std::cout << "Running Encoder" << std::endl;

	if (encoder.Init(m_fileName, &isRunning, outputFile) != 0)
		isRunning = false;

	while (isRunning) {
		;
	}
	*/




	/*
	std::string fragFileName = "../" + s + "-frag.mp4";
	std::string command = "dashProgs\\bin\\win32\\mp4fragment " + outputFile + " " + fragFileName;
	system(command.c_str());

	std::cout << "Finished fragmenting" << std::endl;
	*/

}