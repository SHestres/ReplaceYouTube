#include "Trie.h"

//Must be a constant size
class TrieNode : TrieNodeBase<encVid_t*>
{
public:

    TrieNode(TrieNode*** pnodeArr, encVid_t*** pdataArr)
    {
        nodePtrsArr = pnodeArr;
        dataPtrsArr = pdataArr;
        numChildren = 0;
    }

    void updateArrPtrs(TrieNode*** pnodeArr, encVid_t*** pdataArr)
    {
        nodePtrsArr = pnodeArr;
        dataPtrsArr = pdataArr;
    }

    //Get the next node
    TrieNode* find(char c)
    {
        for (int i = 0; i < numChildren; i++)
        {
            if (nextChar[i] == c)
            {
                return (*nodePtrsArr)[nextNode[i]];
            }
        }
    }

    bool getIsEndOfWord()
    {
        return isEndNode;
    }

    void setIsEndOfWord(bool isEndOfWord)
    {
        isEndNode = isEndOfWord;
    }

    std::string getWord()
    {
        return title;
    }

    void setWord(const std::string& word)
    {
        strcpy(title, word.c_str());
    }

    encVid_t* getData()
    {
        return (*dataPtrsArr)[dataInd];
    }

    void setData(const encVid_t*& data)
    {
        int ind = ptrToArrInd((encVid_t*)data, dataPtrsArr);
        if (ind == -1) throw std::invalid_argument("Data not in dataPtrsArr");
        else
        {
            dataInd = ind;
        }
    }

    void addChild(char c, TrieNode* child)
    {
        int ind = ptrToArrInd(child, nodePtrsArr);
        if (ind == -1) throw std::invalid_argument("Child node not in dataPtrsArr");
        nextChar[numChildren] = c;

    }

    char getC()
    {
        return c;
    }

private:
    TrieNode*** nodePtrsArr;
    encVid_t*** dataPtrsArr;

    //int ind; //The index of this node in the nodePtrs array - Uneccessary?
    char c;
    int numChildren;
    
    char nextChar[40];
    trieNode_i nextNode[40];

    char title[MAX_VIDEO_TITLE_LENGTH];
    encVid_i dataInd; //Index of the data in the arrayPtr
    
    bool isEndNode = false;

};

template <typename T> int ptrToArrInd(T* ptr, T*** arr)
{
    int i = -1;
    bool done = false;
    try
    {
        do
        {
            i++;
            done = (*arr)[i] == data;
        } while (!done);
    }
    catch () { return -1; }
    return i;
}