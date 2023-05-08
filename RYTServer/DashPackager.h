#pragma once

#include <string>

#include "Encoder.h"

#define MAX_NUM_OF_RESOLUTIONS 4

class DashPackager {

public:

	DashPackager();
	~DashPackager();

	bool Init(std::string fileName, std::string videoTitle, bool resolutions[MAX_NUM_OF_RESOLUTIONS]);
	void Run();

private:

	std::string m_fileName;
	std::string m_videoTitle;
	bool m_resolutions[MAX_NUM_OF_RESOLUTIONS];

};