//None of this will link. Moved to header file

/*#include "Trie.h"

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
    strcpy(title, word.c_str());
}

template <typename T>
T TrieNode<T>::getData()
{
    return (*dataPtrsArr)[dataInd];
}

template <typename T>
void TrieNode<T>::setData(const T& data)
{
    int ind = ptrToArrInd((T)data, dataPtrsArr);
    if (ind == -1) throw std::invalid_argument("Data not in dataPtrsArr");
    else
    {
        dataInd = ind;
    }
}

template <typename T>
void TrieNode<T>::addChild(char c, T child)
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
template <typename T> int ptrToArrInd(T ptr, T** arr)
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
}*/