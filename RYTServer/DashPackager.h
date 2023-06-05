#pragma once

#include <string>
#include <Windows.h>
#include <fstream>
#include <sstream>

#include "Encoder.h"
#include <thread>
#include <chrono>
#include <future>
#include <algorithm>

#define VIDEO_DIR_EXT "/videos/"

typedef enum packageStatusEnum_t {
	IDLE,
	STARTING,
	FRAGMENTING,
	PROCESSING,
	MOVING_FILES,
	FINISHING,
	SUCCEEDED,
	BAD_ID,
	FAILED
} vidStat;

static const char* EnumStrings[] = {
	"IDLE",
	"STARTING",
	"FRAGMENTING",
	"PROCESSING",
	"MOVING_FILES",
	"FINISHING",
	"SUCCEEDED",
	"BAD_ID",
	"FAILED"
};

enum stepCounter {
	ZERO,
	ONE,
	TWO,
	THREE,
	FOUR,
	FIVE
};

class DashPackager {

public:

	DashPackager();
	~DashPackager();

	bool Init(std::string fileName, std::string libPath, std::string videoID);
	std::future<void> Run(vidStat* packagingStatus, int* step, std::string* errorMsg);

private:

	std::string m_fileName;
	std::string m_videoTitle;
	std::string m_videoID;
	std::string m_libPath;

};