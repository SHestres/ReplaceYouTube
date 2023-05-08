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

//Title should have no whitespaces
void runPackagingAsync(std::string inFile, std::string libPath, std::string videoID, bool* isDone) {

	*isDone = false;

	//Create filename for temp file
	std::string outFile = "temp" + videoID + ".mp4";

	//Fragment process
	std::string command = "dashProgs\\mp4fragment " + inFile + " " + outFile;
	system(command.c_str());
	
	//Dash package process
	command = "dashProgs\\mp4-dash " + outFile;
	system(command.c_str());

	std::cout << "Done packaging" << std::endl;

	//Rename output folder
	command = "REN output " + videoID;
	system(command.c_str());

	//Move dir with mpd to be accessable by api
	command = "MOVE " + videoID + " " + libPath + VIDEO_DIR_EXT + videoID;
	system(command.c_str());

	//Clean temp files
	try {
		DeleteFile(std::wstring(outFile.begin(), outFile.end()).c_str());
	}
	catch (std::exception e) { std::cerr << "Couldn't delete file" << std::endl; }

	*isDone = true;
}


void doNothing(int i) {
	return;
}

std::future<void> DashPackager::Run(bool* isDone) {
	
	std::future<void> f = std::async(std::launch::async, runPackagingAsync, m_fileName, m_libPath, m_videoID, isDone);

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