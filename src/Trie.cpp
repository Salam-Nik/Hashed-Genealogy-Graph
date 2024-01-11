// Trie.cpp

#include "../include/Trie.h"

template <typename T>
TrieIterator<T>::TrieIterator(const TrieNode<T>* node) : current(node), is_end(false) {
    if (current == nullptr) {
        is_end = true;
    }
}

template <typename T>
const T& TrieIterator<T>::operator*() const {
    return current->data;
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

    delete node;
}

template <typename T>
void Trie<T>::insert(const std::string& key, const T& value) {
    TrieNode<T>* node = root;

    for (char ch : key) {
        if (node->children.find(ch) == node->children.end()) {
            node->children[ch] = new TrieNode<T>();
        }
        node = node->children[ch];
    }

    node->data = value;
}

template <typename T>
void Trie<T>::insert(const std::pair<std::string, T>& keyValuePair) {
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
TrieNode<T>* Trie<T>::find(const std::string& key) {
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
T& Trie<T>::operator[](const std::string& key) {
    TrieNode<T>* node = root;

    for (char ch : key) {
        if (node->children.find(ch) == node->children.end()) {
            node->children[ch] = new TrieNode<T>();
        }
        node = node->children[ch];
    }

    return node->data;
}

template <typename T>
bool Trie<T>::contains(const std::string& key) const {
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
std::string Trie<T>::getKey(const T& value) const {
    return getKeyHelper(root, value, "");
}

template <typename T>
std::string Trie<T>::getKeyHelper(const TrieNode<T>* node, const T& value, const std::string& currentKey) const {
    if (node == nullptr) {
        return "";
    }

    if (node->data == value) {
        return currentKey;
    }

    for (const auto& [ch, child] : node->children) {
        std::string result = getKeyHelper(child, value, currentKey + ch);
        if (!result.empty()) {
            return result;
        }
    }

    return "";
}

template <typename T>
std::vector<T> Trie<T>::values() const {
    std::vector<T> result;
    std::string currentKey;

    getAllValuesHelper(root, currentKey, result);

    return result;
}

template <typename T>
void Trie<T>::getAllValuesHelper(const TrieNode<T>* node, const std::string& currentKey, std::vector<T>& result) const {
    if (node == nullptr) {
        return;
    }

    if (!currentKey.empty()) {
        result.push_back(node->data);
    }

    for (const auto& [ch, child] : node->children) {
        getAllValuesHelper(child, currentKey + ch, result);
    }
}
