// Trie.cpp

#include "../include/Trie.h"
#include "../include/GenealogyGraph.h"

template <typename T>
TrieIterator<T>::TrieIterator(const TrieNode<T>* node) : current(node), is_end(false) {
    if (current == nullptr) {
        is_end = true;
    }
}

template <typename T>
const T& TrieIterator<T>::operator*() const {
    return *(current->data);
}

template <typename T>
TrieIterator<T>& TrieIterator<T>::operator++() {
    advance();
    return *this;
}

template <typename T>
bool TrieIterator<T>::operator!=(const TrieIterator<T>& other) const {
    return is_end != other.is_end;
}

template <typename T>
void TrieIterator<T>::advance() {
    if (current == nullptr || current->children.empty()) {
        is_end = true;
    } else {
        auto it = current->children.begin();
        current = it->second;
    }
}

template <typename T>
Trie<T>::Trie() : root(new TrieNode<T>()) {}

template <typename T>
Trie<T>::~Trie() {
    deleteSubtree(root);
}

template <typename T>
void Trie<T>::deleteSubtree(TrieNode<T>* node) {
    if (node == nullptr) {
        return;
    }

    for (auto& [ch, child] : node->children) {
        deleteSubtree(child);
    }

    delete node->data; 
    delete node;
}

template <typename T>
void Trie<T>::insert(const string& key, const T& value) {
    TrieNode<T>* node = root;

    for (char ch : key) {
        if (node->children.find(ch) == node->children.end()) {
            node->children[ch] = new TrieNode<T>();
        }
        node = node->children[ch];
    }

    node->data = new T(value); 
}

template <typename T>
void Trie<T>::insert(const pair<string, T>& keyValuePair) {
    insert(keyValuePair.first, keyValuePair.second);
}

template <typename T>
TrieIterator<T> Trie<T>::begin() const {
    return TrieIterator<T>(root);
}

template <typename T>
TrieIterator<T> Trie<T>::end() const {
    return TrieIterator<T>(nullptr);
}

template <typename T>
TrieNode<T>* Trie<T>::find(const string& key) {
    TrieNode<T>* node = root;

    for (char ch : key) {
        auto it = node->children.find(ch);
        if (it == node->children.end()) {
            return nullptr;
        }
        node = it->second;
    }

    return node;
}

template <typename T>
T* Trie<T>::operator[](const string& key) {
    TrieNode<T>* node = root;

    for (char ch : key) {
        if (node->children.find(ch) == node->children.end()) {
            node->children[ch] = new TrieNode<T>();
        }
        node = node->children[ch];
    }

    if (node->data == nullptr) {
        node->data = new T();  
    }

    return node->data;
}

template <typename T>
bool Trie<T>::contains(const string& key) const {
    TrieNode<T>* node = root;

    for (char ch : key) {
        auto it = node->children.find(ch);
        if (it == node->children.end()) {
            return false;
        }
        node = it->second;
    }

    return true;
}

template <typename T>
vector<T*> Trie<T>::values() {
    vector<T*> result;
    string currentKey;

    getAllValuesHelper(root, currentKey, result);

    return result;
}

template <typename T>
void Trie<T>::getAllValuesHelper(const TrieNode<T>* node, const string& currentKey, vector<T*>& result) {
    if (node == nullptr) {
        return;
    }

    if (node->data != nullptr) {
        result.push_back(node->data);
    }

    for (const auto& [ch, child] : node->children) {
        getAllValuesHelper(child, currentKey + ch, result);
    }
}

template class Trie<GenealogyGraph::Person>;
