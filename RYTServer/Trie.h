#pragma once

#include <iostream>
#include <string>

//#include "VideoImporter.h"

typedef int encVid_i;
typedef int trieNode_i;

//extern struct EncodedVideoMetadata;

#define MAX_VIDEO_TITLE_LENGTH 40

/* This was a nice idea, but ultimately uneeded for this project. I'll pull a previous commit for my personal library
template<typename T>
class TrieNodeBase {
public:
    virtual TrieNodeBase<T>* find(char c) = 0;
    virtual bool getIsEndOfWord() const = 0;
    virtual void setIsEndOfWord(bool isEndOfWord) = 0;
    virtual std::string getWord() const = 0;
    virtual void setWord(const std::string& word) = 0;
    virtual T getData() const = 0;
    virtual void setData(const T& data) = 0;
    virtual void addChild(char c, TrieNodeBase<T>* child) = 0;
    virtual char getC() const = 0;
};
*/

//Must be a constant size
template <typename T>
class TrieNode //: public TrieNodeBase<T>
{
public:

    TrieNode(TrieNode<T>*** pnodeArr, T** pdataArr);

    void updateArrPtrs(TrieNode<T>*** pnodeArr, T** pdataArr);

    //Get the next node
    TrieNode<T>* find(char c);

    bool getIsEndOfWord();

    void setIsEndOfWord(bool isEndOfWord);

    std::string getWord();

    void setWord(const std::string& word);

    T getData();

    void setData(T data);

    void addChild(char c, TrieNode<T>* child);

    char getC();

private:
    TrieNode<T>*** nodePtrsArr;
    T** dataPtrsArr;

    //int ind; //The index of this node in the nodePtrs array - Uneccessary?
    char c;
    int numChildren;

    char nextChar[40];
    trieNode_i nextNode[40];

    char title[MAX_VIDEO_TITLE_LENGTH];
    encVid_i dataInd; //Index of the data in the arrayPtr

    bool isEndNode = false;

};

/*
* The trie will handle reallocating the nodePtrsArr
*/
template<typename T>
class Trie {
public:

    /*
    Trie(TrieNode<T>*** nodeArr, T** dataArr, int* numOfNodes, int* nodeAllocatedSpace)
        : Trie((TrieNodeBase<T)nodeArr, dataArr, numOfNodes, nodeAllocatedSpace)
    {}*/

    Trie(TrieNode<T>*** nodeArr, T** dataArr, int* numOfNodes, int* nodeAllocatedSpace) {
        root = new TrieNode<T>(nodeArr, dataArr);
        dataPtrsArr = dataArr;
        nodePtrsArr = nodeArr;
        numNodes = numOfNodes;
        nodeAllocSpace = nodeAllocatedSpace;
    }

    ~Trie() {
        delete root;
    }

    //Data must already be in the dataPtrsArray
    void insert(T data)
    {
        TrieNode<T>* node = new TrieNode<T>(nodePtrsArr, dataPtrsArr);
        node->setData(data);
        insertNode(node);
    }

    void insertNode(TrieNode<T>* node) {
        TrieNode<T>* current = root;
        const std::string& word = node->getWord();
        for (char c : word) {
            TrieNode<T>* child = current->find(c);
            if (child == nullptr) {
                child = new TrieNode<T>(nodePtrsArr, dataPtrsArr);
                child->setWord(current->getWord() + c);
                //current->children[c] = child;
                current->addChild(c, child);
            }
            current = child;
        }
        if (current->getIsEndOfWord()) throw std::invalid_argument("Node already exists in trie");
        current->setData(node->getData());
        current->setIsEndOfWord(true);
        
        //Ensure space in the nodes array
        if (*numNodes > *nodeAllocSpace - 2)
        {
            *nodePtrsArr = (TrieNode<T>**)realloc(*nodePtrsArr, *nodeAllocSpace + 10);
            *nodeAllocSpace += 10;
        }

        (*nodePtrsArr)[*numNodes] = node;

        *numNodes++;

    }

    bool contains(TrieNode<T>* node) const {
        const TrieNode<T>* current = root;
        const std::string& word = node->getWord();
        for (char c : word) {
            current = current->find(c);
            if (current == nullptr) {
                return false;
            }
        }
        return current->getIsEndOfWord();
    }

    T getData(std::string word) const {
        TrieNode<T>* current = root;
        //const std::string& word = node->getWord();
        for (char c : word) {
            current = current->find(c);
            if (current == nullptr) {
                throw std::invalid_argument("Word not in trie.");
            }
        }
        return current->getData();
    }

    void remove(TrieNode<T>* node) {
        if (!contains(node)) {
            return;
        }

        TrieNode<T>* current = root;
        const std::string& word = node->getWord();
        for (char c : word) {
            TrieNode<T>* child = current->find(c);
            if (child->children.empty()) {
                current->children.erase(c);
                delete child;
                return;
            }
            current = child;
        }
        current->setIsEndOfWord(false);
    }

    TrieNode<T>** list(int x, TrieNode<T>* node, const std::string& prefix) const {
        TrieNode<T>** result = new TrieNode<T>*[x]();
        int i = 0;
        const TrieNode<T>* current = node;
        const std::string& word = prefix;
        for (char c : word) {
            current = current->find(c);
            if (current == nullptr) {
                return result;
            }
        }
        listHelper(x, current, result, i);
        return result;
    }

private:
    TrieNode<T>* root;

    T** dataPtrsArr;

    TrieNode<T>*** nodePtrsArr;
    
    int* numNodes; //Tracks the number of nodes in the array
    int* nodeAllocSpace; //Tracks the maximum number of nodes before the nodePtrsArr needs to be reallocated

    void listHelper(int& x, const TrieNode<T>* node, TrieNode<T>** result, int& i) const {
        if (node->getIsEndOfWord()) {
            result[i] = const_cast<TrieNode<T>*>(node);
            ++i;
            if (--x == 0) {
                return;
            }
        }
        for (auto& child : node->children) {
            listHelper(x, child.second, result, i);
            if (x == 0) {
                return;
            }
        }
    }

    
};






//Idk why, but the rest of this won't link in a cpp file

template <typename T>
TrieNode<T>::TrieNode<T>(TrieNode<T>*** pnodeArr, T** pdataArr)
{
    nodePtrsArr = pnodeArr;
    dataPtrsArr = pdataArr;
    numChildren = 0;
}

template <typename T>
void TrieNode<T>::updateArrPtrs(TrieNode<T>*** pnodeArr, T** pdataArr)
{
    nodePtrsArr = pnodeArr;
    dataPtrsArr = pdataArr;
}


//Get the next node
template <typename T>
TrieNode<T>* TrieNode<T>::find(char c)
{
    for (int i = 0; i < numChildren; i++)
    {
        if (nextChar[i] == c)
        {
            return (*nodePtrsArr)[nextNode[i]];
        }
    }
    return nullptr;
}

template <typename T>
bool TrieNode<T>::getIsEndOfWord()
{
    return isEndNode;
}

template <typename T>
void TrieNode<T>::setIsEndOfWord(bool isEndOfWord)
{
    isEndNode = isEndOfWord;
}

template <typename T>
std::string TrieNode<T>::getWord()
{
    return title;
}

template <typename T>
void TrieNode<T>::setWord(const std::string& word)
{
    strcpy_s(title, word.c_str());
}

template <typename T>
T TrieNode<T>::getData()
{
    return (*dataPtrsArr)[dataInd];
}

template <typename T>
void TrieNode<T>::setData(T data)
{
    int ind = ptrToArrInd(data, dataPtrsArr);
    if (ind == -1) throw std::invalid_argument("Data not in dataPtrsArr");
    else
    {
        dataInd = ind;
    }
}

template <typename T>
void TrieNode<T>::addChild(char c, TrieNode<T>* child)
{
    int ind = ptrToArrInd(child, nodePtrsArr);
    if (ind == -1) throw std::invalid_argument("Child node not in dataPtrsArr");
    nextChar[numChildren] = c;
    nextNode[numChildren] = ind;
    numChildren++;
}

template <typename T>
char TrieNode<T>::getC()
{
    return c;
}

//T must be a pointer type
template <typename type> int ptrToArrInd(type ptr, type** arr)
{
    int i = -1;
    bool done = false;
    try
    {
        do
        {
            i++;
            done = (*arr)[i] == ptr;
        } while (!done);
    }
    catch (std::exception e) { return -1; }
    return i;
}