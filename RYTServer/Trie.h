#pragma once

#include <iostream>
#include <string>

#include "VideoImporter.h"

typedef int encVid_i;
typedef int trieNode_i;


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





/*
* The trie will handle reallocating the nodePtrsArr
*/
template<typename T, /*TrieNodeBase<T>*/>
class Trie {
public:
    Trie(TrieNodeBase*** nodeArr, encVid_t*** dataArr, int* numOfNodes, int* nodeAllocatedSpace) {
        root = new TrieNodeBase<T>(nodeArr, dataArr);
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
        TrieNodeBase<T>* node = new TrieNodeBase<T>(nodePtrsArr, dataPtrsArr);
        node->setData(T);
        insertNode(node);
    }

    void insertNode(TrieNodeBase<T>* node) {
        TrieNodeBase<T>* current = root;
        const std::string& word = node->getWord();
        for (char c : word) {
            TrieNodeBase<T>* child = current->find(c);
            if (child == nullptr) {
                child = new TrieNodeBase<T>(nodePtrsArr, dataPtrsArr);
                child->setWord(current->getWord() + c);
                //current->children[c] = child;
                current->addChild(c, child);
            }
            current = child;
        }
        current->setData(node->getData());
        current->setIsEndOfWord(true);
        
        (*nodePtrsArr)[numNodes] = node;

        *numNodes++;
        if (*numNodes > *nodeAllocSpace - 2)
        {
            nodePtrsArr = realloc(*nodePtrsArr, nodeAllocSpace + 10);
            *nodeAllocSpace += 10;
        }
    }

    bool contains(TrieNodeBase<T>* node) const {
        const TrieNodeBase<T>* current = root;
        const std::string& word = node->getWord();
        for (char c : word) {
            current = current->find(c);
            if (current == nullptr) {
                return false;
            }
        }
        return current->getIsEndOfWord();
    }

    T getData(TrieNodeBase<T>* node) const {
        const TrieNodeBase<T>* current = root;
        const std::string& word = node->getWord();
        for (char c : word) {
            current = current->find(c);
            if (current == nullptr) {
                throw std::invalid_argument("Word not in trie.");
            }
        }
        return current->getData();
    }

    void remove(TrieNodeBase<T>* node) {
        if (!contains(node)) {
            return;
        }

        TrieNodeBase<T>* current = root;
        const std::string& word = node->getWord();
        for (char c : word) {
            TrieNodeBase<T>* child = current->find(c);
            if (child->children.empty()) {
                current->children.erase(c);
                delete child;
                return;
            }
            current = child;
        }
        current->setIsEndOfWord(false);
    }

    TrieNodeBase<T>** list(int x, TrieNodeBase<T>* node, const std::string& prefix) const {
        TrieNodeBase<T>** result = new TrieNodeBase<T>*[x]();
        int i = 0;
        const TrieNodeBase<T>* current = node;
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
    TrieNodeBase<T>* root;
    encVid_t*** dataPtrsArr;
    TrieNodeBase*** nodePtrsArr;

    int* numNodes; //Tracks the number of nodes in the array
    int* nodeAllocSpace; //Tracks the maximum number of nodes before the nodePtrsArr needs to be reallocated

    void listHelper(int& x, const TrieNodeBase<T>* node, TrieNodeBase<T>** result, int& i) const {
        if (node->getIsEndOfWord()) {
            result[i] = const_cast<TrieNodeBase<T>*>(node);
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