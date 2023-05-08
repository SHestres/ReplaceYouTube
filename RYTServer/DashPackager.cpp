#include "DashPackager.h"
#include <cctype>

DashPackager::DashPackager() {}

DashPackager::~DashPackager() {}

bool DashPackager::Init(std::string fileName, std::string videoTitle, bool resolutions[MAX_NUM_OF_RESOLUTIONS])
{
	m_fileName = fileName;
	m_videoTitle = videoTitle;

	for (int i = 0; i < MAX_NUM_OF_RESOLUTIONS; i++)
	{
		m_resolutions[i] = resolutions[i];
	}

	return true;
}

void DashPackager::Run() {
	/* Media foundation doesn't correctly process files bigger than a few MB
	Encoder encoder;
	std::string s = m_videoTitle;
	s.erase(std::remove_if(s.begin(), s.end(), ::isspace), s.end());
	std::string outputFile = "../" + s + "_480" + ".mp4";
	
	bool isRunning = true;

	std::cout << "Running Encoder" << std::endl;

	if (encoder.Init(m_fileName, &isRunning, outputFile) != 0)
		isRunning = false;

	while (isRunning) {
		;
	}
	*/


	std::cout << "Done encoding" << std::endl;

	/*
	std::string fragFileName = "../" + s + "-frag.mp4";
	std::string command = "dashProgs\\bin\\win32\\mp4fragment " + outputFile + " " + fragFileName;
	system(command.c_str());

	std::cout << "Finished fragmenting" << std::endl;
	*/

}