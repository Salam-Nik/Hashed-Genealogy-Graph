#pragma once
#include <iostream>
#include <vector>

class SHA256 {
public:
    static const unsigned int K[];
    static unsigned int W[64];
    static unsigned int a, b, c, d, e, f, g, h, T1, T2;

    static std::vector<unsigned char> readInput(const std::string &input);
    static void printHash(std::vector<std::vector<unsigned int>> H, int N);
    static const int getPadding(long long length);
    static std::vector<unsigned char> paddingMessage(std::vector<unsigned char> M);
    static std::vector<std::vector<unsigned int>> parsingMessage(std::vector<unsigned char> bytes);

    static const unsigned int SHR(const unsigned int &n, const unsigned int &x);
    static const unsigned int ROTR(const unsigned int &n, const unsigned int &x);
    static const unsigned int Ch(const unsigned int &x, const unsigned int &y, const unsigned int &z);
    static const unsigned int Maj(const unsigned int &x, const unsigned int &y, const unsigned int &z);
    static const unsigned int capitalSigma_0(const unsigned int &x);
    static const unsigned int capitalSigma_1(const unsigned int &x);
    static const unsigned int lowercaseSigma0(const unsigned int &x);
    static const unsigned int lowercaseSigma1(const unsigned int &x);

    static std::vector<std::vector<unsigned int>> getHash(std::vector<std::vector<unsigned int>> M);

    // Function to compute SHA256 hash and return as a string
    static std::string getHashString(const std::string &input);

private:
    static std::string hashVectorToString(const std::vector<std::vector<unsigned int>> &hashVector);
};


