#pragma once

#include <string>
#include <Windows.h>

#include "Encoder.h"
#include <thread>
#include <chrono>
#include <future>
#include <algorithm>

#define VIDEO_DIR_EXT "/videos/"

class DashPackager {

public:

	DashPackager();
	~DashPackager();

	bool Init(std::string fileName, std::string libPath, std::string videoID);
	std::future<void> Run(bool* isDone);

private:

	std::string m_fileName;
	std::string m_videoTitle;
	std::string m_videoID;
	std::string m_libPath;

};