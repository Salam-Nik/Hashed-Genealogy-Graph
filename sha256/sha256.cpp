#include "sha256.h"
#include <cmath>
#include <iomanip>
#include <sstream>


namespace SHA256
{

    const unsigned int K[] = {
        0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
        0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
        0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
        0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
        0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
        0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
        0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
        0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

    unsigned int W[64] = {0}; // Initialize the static member
    unsigned int a, b, c, d, e, f, g, h, T1, T2;

    std::vector<unsigned char> SHA256::readInput(const std::string &input)
    {
        std::vector<unsigned char> inputVector;
        int size = input.length();
        for (int i = 0, pos = 0; i < size / 2; ++i, pos += 2)
        {
            unsigned char hexpair = std::stoi(input.substr(pos, 2), nullptr, 16);
            inputVector.push_back(hexpair);
        }
        return inputVector;
    }

    void SHA256::printHash(std::vector<std::vector<unsigned int>> H, int N)
    {
        for (int i = 0; i < 8; i++)
        {
            std::cout << std::hex << std::setw(8) << std::setfill('0') << H[N][i];
        }
        std::cout << "\n";
    }

    const int SHA256::getPadding(long long length)
    {
        int k = 0;
        while ((length + 1 + k) % 512 != 448)
        {
            ++k;
        }
        return k;
    }

    std::vector<unsigned char> SHA256::paddingMessage(std::vector<unsigned char> M)
    {
        int k = getPadding(M.size() * 8);
        long long l = (M.size() * 8);
        M.push_back(0x80);
        k = k - 7;
        for (int i = 0; i < (k / 8); ++i)
        {
            M.push_back(0);
        }
        for (int i = 1; i < 9; ++i)
        {
            M.push_back(l >> (64 - i * 8));
        }
        return M;
    }

    std::vector<std::vector<unsigned int>> SHA256::parsingMessage(std::vector<unsigned char> bytes)
    {
        std::vector<std::vector<unsigned int>> M;
        unsigned int n = 0;
        for (int i = 0; n < bytes.size() / 64; ++n)
        {
            std::vector<unsigned int> block(16);
            for (int j = 0; j < 16; ++j)
            {
                unsigned int word = 0;
                for (int k = 0; k < 4; ++k, ++i)
                {
                    word <<= 8;
                    word |= bytes[i];
                }
                block[j] = word;
            }
            M.push_back(block);
        }
        return M;
    }

    std::vector<std::vector<unsigned int>> SHA256::parsingMessage(std::vector<unsigned char> bytes)
    {
        std::vector<std::vector<unsigned int>> M;
        unsigned int n = 0;
        for (int i = 0; n < bytes.size() / 64; ++n)
        {
            std::vector<unsigned int> block(16);
            for (int j = 0; j < 16; ++j)
            {
                unsigned int word = 0;
                for (int k = 0; k < 4; ++k, ++i)
                {
                    word <<= 8;
                    word |= bytes[i];
                }
                block[j] = word;
            }
            M.push_back(block);
        }
        return M;
    }

    const unsigned int SHA256::SHR(const unsigned int &n, const unsigned int &x)
    {
        return x >> n;
    }

    const unsigned int SHA256::ROTR(const unsigned int &n, const unsigned int &x)
    {
        return (x >> n) | (x << (32 - n));
    }

    const unsigned int SHA256::Ch(const unsigned int &x, const unsigned int &y, const unsigned int &z)
    {
        return (x & y) ^ (~x & z);
    }

    const unsigned int SHA256::Maj(const unsigned int &x, const unsigned int &y, const unsigned int &z)
    {
        return (x & y) ^ (x & z) ^ (y & z);
    }

    const unsigned int SHA256::capitalSigma_0(const unsigned int &x)
    {
        return ROTR(2, x) ^ ROTR(13, x) ^ ROTR(22, x);
    }

    const unsigned int SHA256::capitalSigma_1(const unsigned int &x)
    {
        return ROTR(6, x) ^ ROTR(11, x) ^ ROTR(25, x);
    }

    const unsigned int SHA256::lowercaseSigma0(const unsigned int &x)
    {
        return ROTR(7, x) ^ ROTR(18, x) ^ SHR(3, x);
    }

    const unsigned int SHA256::lowercaseSigma1(const unsigned int &x)
    {
        return ROTR(17, x) ^ ROTR(19, x) ^ SHR(10, x);
    }

    std::vector<std::vector<unsigned int>> SHA256::getHash(std::vector<std::vector<unsigned int>> M)
    {
        std::vector<std::vector<unsigned int>> Hash;
        std::vector<unsigned int> H0 = {0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
                                        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19};
        Hash.push_back(H0);

        std::vector<unsigned int> currentBlock(8);
        int N = M.size();

        for (int i = 1; i <= N; ++i)
        {
            for (int t = 0; t <= 15; ++t)
                W[t] = M[i - 1][t];
            for (int t = 16; t <= 63; ++t)
                W[t] = lowercaseSigma1(W[t - 2]) + W[t - 7] + lowercaseSigma0(W[t - 15]) + W[t - 16];

            a = Hash[i - 1][0];
            b = Hash[i - 1][1];
            c = Hash[i - 1][2];
            d = Hash[i - 1][3];
            e = Hash[i - 1][4];
            f = Hash[i - 1][5];
            g = Hash[i - 1][6];
            h = Hash[i - 1][7];

            for (int t = 0; t <= 63; ++t)
            {
                T1 = h + capitalSigma_1(e) + Ch(e, f, g) + K[t] + W[t];
                T2 = capitalSigma_0(a) + Maj(a, b, c);
                h = g;
                g = f;
                f = e;
                e = d + T1;
                d = c;
                c = b;
                b = a;
                a = T1 + T2;
            }

            currentBlock[0] = a + Hash[i - 1][0];
            currentBlock[1] = b + Hash[i - 1][1];
            currentBlock[2] = c + Hash[i - 1][2];
            currentBlock[3] = d + Hash[i - 1][3];
            currentBlock[4] = e + Hash[i - 1][4];
            currentBlock[5] = f + Hash[i - 1][5];
            currentBlock[6] = g + Hash[i - 1][6];
            currentBlock[7] = h + Hash[i - 1][7];

            Hash.push_back(currentBlock);
        }

        return Hash;
    }
    std::string SHA256::getHashString(const std::string &input)
    {
        // Read the input
        std::vector<unsigned char> message = readInput(input);

        // Padding
        message = paddingMessage(message);

        // Parse the message into n blocks
        std::vector<std::vector<unsigned int>> parsed_message = parsingMessage(message);

        // Compute the hash value
        std::vector<std::vector<unsigned int>> hashResult = getHash(parsed_message);

        // Convert hash vector to string
        return hashVectorToString(hashResult);
    }

    std::string SHA256::hashVectorToString(const std::vector<std::vector<unsigned int>> &hashVector)
    {
        std::string hashString;
        for (int i = 0; i < 8; i++)
        {
            std::stringstream stream;
            stream << std::hex << std::setw(8) << std::setfill('0') << hashVector.back()[i];
            hashString += stream.str();
        }
        return hashString;
    }

} // namespace SHA256
