#include "VideoImporter.h"

VideoImporter::VideoImporter(std::string trieFile, std::string metaFile)
	: m_trieFilePath(trieFile), m_metaFilePath(metaFile)
{

}

bool VideoImporter::LoadDatabase()
{
	FILE* testFile;
	//Testing
	errno_t err = fopen_s(&testFile, "test.dbst", "r");
	if (err)
	{
		std::cerr << "Couldn't open file to read. Err: " << err << std::endl;
		return false;
	}
	int num;
	fgetobj(testFile, &num);
	std::cout << num << std::endl;
	fgetobj(testFile, &num);
	std::cout << num << std::endl;
	fgetobj(testFile, &num);
	std::cout << num << std::endl;
	fgetobj(testFile, &num);
	std::cout << num << std::endl;

	fclose(testFile);
	//Open metaFile
	

	//Get number of encVids

	//Allocate vidPtrs array

	//Import encVids and place in vidPtrs array

	//Open trieFile
	//m_trieFile = fopen(m_trieFilePath.c_str(), "")

	//Get number of trieNodes

	//Allocate trieNodePtrs array

	//Import trieNodes and place in trieNodePtrs array

	//(Maybe) Update all ptrs in trieNodes based on indicies and Ptrs arrays

	return true;
}


bool VideoImporter::StoreDatabase()
{
	FILE* testFile;
	//Testing
	errno_t err = fopen_s(&testFile, "test.dbst", "w+");
	if (err)
	{
		std::cerr << "Couldn't open file to write" << std::endl;
		return false;
	}

	int num = 93;
	fputobj(testFile, &num);
	num = 92;
	fputobj(testFile, &num);
	num = 3;
	fputobj(testFile, &num);
	num = 4;
	fputobj(testFile, &num);

	fclose(testFile);
	//Create file to write encVids
	//errno_t err = fopen_s(&m_trieFile, m_trieFilePath.c_str(), "w"); //This assumes that a home video library won't be prohibitively long. If that ever changes maybe I'll change this

	//Write each encVid to file based on vidPtrs array order
	
	//Create file to write trieNodes

	//Write each trieNode based on trieNodePtrs array order

	//Cleanup

	return true;
}

bool VideoImporter::createDemoData() //For Testing
{


	return true;
}

int VideoImporter::VidpToInt(encVid_t* vidp)
{
	return (char*)vidp - m_metaFileBase;
}

encVid_t* VideoImporter::IntToVidp(int ind)
{
	return (encVid_t*)((char*)m_metaFileBase + ind);
}

template <typename T> void fputobj(FILE* f, T* obj)
{
	for (int i = 0; i < sizeof(T); i++)
	{
		fputc(*((char*)obj + i), f);
	}
}

template <typename T> void fgetobj(FILE* f, T* obj)
{
	for (int i = 0; i < sizeof(T); i++)
	{
		*((char*)obj + i) = (char)fgetc(f);
	}
}