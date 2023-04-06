#pragma once

#include <iostream>
#include <string>

#include "VideoImporter.h"

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
};

/*
class TrieNode : TrieNodeBase<encVid_t>
{
    int ind; //The index of this node in the nodePtrs array
    encVid_t* data;


};*/

template<typename T>
class Trie {
public:
    Trie() {
        root = new TrieNodeBase<T>();
    }

    ~Trie() {
        delete root;
    }

    void insert(TrieNodeBase<T>* node) {
        TrieNodeBase<T>* current = root;
        const std::string& word = node->getWord();
        for (char c : word) {
            TrieNodeBase<T>* child = current->find(c);
            if (child == nullptr) {
                child = new TrieNodeBase<T>();
                child->setWord(current->getWord() + c);
                //current->children[c] = child;
                current->addChild(c, child);
            }
            current = child;
        }
        current->setData(node->getData());
        current->setIsEndOfWord(true);
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