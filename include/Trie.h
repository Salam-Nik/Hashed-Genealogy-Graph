// Trie.h

#ifndef TRIE_H
#define TRIE_H

#include <unordered_map>
#include <string>
#include <vector>

using namespace std;

template <typename T>
struct TrieNode
{
    unordered_map<char, TrieNode *> children;
    T *data;

    TrieNode() : data(nullptr) {}
};

template <typename T>
class TrieIterator
{
private:
    const TrieNode<T> *current;
    bool is_end;

public:
    TrieIterator(const TrieNode<T> *node);

    const T &operator*() const;

    TrieIterator &operator++();

    bool operator!=(const TrieIterator<T> &other) const;

private:
    void advance();
};

template <typename T>
class Trie
{
public:
    TrieNode<T> *root;

    Trie();

    ~Trie();

    void insert(const string &key, const T &value);
    void insert(const pair<string, T> &keyValuePair);

    TrieIterator<T> begin() const;
    TrieIterator<T> end() const;

    T *operator[](const string &key);
    TrieNode<T> *find(const string &key);
    bool contains(const string &key) const;

    string getKey(const T &value) const;
    vector<T*> values();

private:
    void deleteSubtree(TrieNode<T> *node);
    void getAllValuesHelper(const TrieNode<T> *node, const string &currentKey, vector<T*> &result);

};

#endif // TRIE_H
