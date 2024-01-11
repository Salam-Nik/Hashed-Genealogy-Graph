// Trie.h

#ifndef TRIE_H
#define TRIE_H

#include <unordered_map>
#include <string>
#include <vector>

template <typename T>
struct TrieNode {
    std::unordered_map<char, TrieNode*> children;
    T data;

    TrieNode() : data() {}
};

template <typename T>
class TrieIterator {
private:
    const TrieNode<T>* current;
    bool is_end;

public:
    TrieIterator(const TrieNode<T>* node);

    const T& operator*() const;

    TrieIterator& operator++();

    bool operator!=(const TrieIterator<T>& other) const;

private:
    void advance();
};

template <typename T>
class Trie {
public:
    TrieNode<T>* root;

    Trie();

    ~Trie();
    
    void insert(const std::string& key, const T& value);
    void insert(const std::pair<std::string, T>& keyValuePair); // Overloaded insert

    TrieIterator<T> begin() const;
    TrieIterator<T> end() const;

    T& operator[](const std::string& key);
    TrieNode<T>* find(const std::string& key); // Renamed from search to be consistent
    bool contains(const std::string& key) const;
    
    std::string getKey(const T& value) const;
    std::vector<T> values() const;

private:
    void deleteSubtree(TrieNode<T>* node); // Helper function for destructor
    std::string getKeyHelper(const TrieNode<T>* node, const T& value, const std::string& currentKey) const;
    void getAllValuesHelper(const TrieNode<T>* node, const std::string& currentKey, std::vector<T>& result) const;
};

#endif // TRIE_H
