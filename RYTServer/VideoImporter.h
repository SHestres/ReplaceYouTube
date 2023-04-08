#pragma once

#include <iostream>
#include "Trie.h"

//#include <WinBase.h>

//#pragma comment(lib, "Kernel32.lib")

#define META_FILE_OFFSET 100
#define MAX_VIDEO_TITLE_LENGTH 40

template <typename T> void fputobj(FILE* f, T* obj);
template <typename T> void fgetobj(FILE* f, T* obj);

typedef struct EncodedVideoMetadata
{
	int key;
	std::string name;
}encVid_t;

/*
struct TrieNode
{
	char l; //The letter represented by this node
	int n; //The number representation of this node's letter
	TrieNode* nextNodes[50];
	int numNexts;
	encVid_t video;
};*/

class VideoImporter
{
public:
	VideoImporter(std::string trieFile = "./trieFile", std::string metaFile = "./metaFile");

	bool LoadDatabase();
	bool StoreDatabase();

	bool createDemoData(); //For Testing

private:
	std::string m_trieFilePath;
	std::string m_metaFilePath;
	FILE* m_trieFile;
	FILE* m_metaFile;
	void* m_metaFileBase;
	void* m_trieFileBase;
	
	int m_numVids; //The number of encoded videos in the database
	encVid_t** m_videoPtrs;

	int m_numTrieNodes;
	TrieNode<encVid_t*>** m_trieNodePtrs;

	int VidpToInt(encVid_t* vidp);
	encVid_t* IntToVidp(int ind);
};