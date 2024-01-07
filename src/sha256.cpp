#include "/home/salam/Documents/ds/Hashed-Genealogy-Graph/include/sha256.h"

const uint K[] = {0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
                 0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
                 0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
                 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
                 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
                 0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
                 0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
                 0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

uint W[64];

uint a, b, c, d, e, f, g, h, T1, T2;

string str_to_hex(const string &input)
{
    std::ostringstream hexStream;

    for (unsigned char ch : input)
    {
        hexStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(ch);
    }

    return hexStream.str();
}
vector<uchar> readInput(const string &input)
{
    vector<uchar> v;
    int size = input.length();
    for (int i = 0, pos = 0; i < size / 2; ++i, pos += 2)
    {
        uchar hexpair = stoi(input.substr(pos, 2), nullptr, 16);
        v.push_back(hexpair);
    }
    return v;
}

const int calPadding(ll size)
{
    int k = 0;
    while ((size + 1 + k) % 512 != 448)
        ++k;
    return k;
}

vector<uchar> padMessage(vector<uchar> bytes)
{
    int k = calPadding(bytes.size() * 8);
    ll l = (bytes.size() * 8);
    bytes.push_back(0x80);
    k = k - 7;

    for (int i = 0; i < (k / 8); ++i)
        bytes.push_back(0);

    for (int i = 1; i < 9; ++i)
        bytes.push_back(l >> (64 - i * 8));

    return bytes;
}

vector<vector<uint>> parseMessage(vector<uchar> bytes)
{
    vector<vector<uint>> M;
    int n = 0;
    for (int i = 0; n < bytes.size() / 64; ++n)
    {
        vector<uint> block(16);
        for (int j = 0; j < 16; ++j)
        {
            uint word = 0;
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

const uint SHR(const uint &n, const uint &x)
{
    return x >> n;
}

const uint ROTR(const uint &n, const uint &x)
{
    return (x >> n) | (x << (32 - n));
}

const uint Ch(const uint &x, const uint &y, const uint &z)
{
    return (x & y) ^ (~x & z);
}

const uint Maj(const uint &x, const uint &y, const uint &z)
{
    return (x & y) ^ (x & z) ^ (y & z);
}

const uint cSigma0(const uint &x)
{
    return ROTR(2, x) ^ ROTR(13, x) ^ ROTR(22, x);
}

const uint cSigma1(const uint &x)
{
    return ROTR(6, x) ^ ROTR(11, x) ^ ROTR(25, x);
}

const uint sSigma0(const uint &x)
{
    return ROTR(7, x) ^ ROTR(18, x) ^ SHR(3, x);
}

const uint sSigma1(const uint &x)
{
    return ROTR(17, x) ^ ROTR(19, x) ^ SHR(10, x);
}

vector<vector<uint>> calHash(const vector<vector<uint>> M)
{
    vector<vector<uint>> hash;
    vector<uint> h0 = {0x6a09e667,
                       0xbb67ae85,
                       0x3c6ef372,
                       0xa54ff53a,
                       0x510e527f,
                       0x9b05688c,
                       0x1f83d9ab,
                       0x5be0cd19};
    hash.push_back(h0);

    vector<uint> hashBlock(8);

    int size = M.size();

    for (int i = 1; i <= size; ++i)
    {

        for (int t = 0; t <= 15; ++t)
            W[t] = M[i - 1][t]; 
        for (int t = 16; t <= 63; ++t)
            W[t] = sSigma1(W[t - 2]) + W[t - 7] + sSigma0(W[t - 15]) + W[t - 16];

        a = hash[i - 1][0];
        b = hash[i - 1][1];
        c = hash[i - 1][2];
        d = hash[i - 1][3];
        e = hash[i - 1][4];
        f = hash[i - 1][5];
        g = hash[i - 1][6];
        h = hash[i - 1][7];

        for (int t = 0; t <= 63; ++t)
        {
            T1 = h + cSigma1(e) + Ch(e, f, g) + K[t] + W[t];
            T2 = cSigma0(a) + Maj(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + T1;
            d = c;
            c = b;
            b = a;
            a = T1 + T2;
        }

        hashBlock[0] = a + hash[i - 1][0];
        hashBlock[1] = b + hash[i - 1][1];
        hashBlock[2] = c + hash[i - 1][2];
        hashBlock[3] = d + hash[i - 1][3];
        hashBlock[4] = e + hash[i - 1][4];
        hashBlock[5] = f + hash[i - 1][5];
        hashBlock[6] = g + hash[i - 1][6];
        hashBlock[7] = h + hash[i - 1][7];
        hash.push_back(hashBlock);
    }

    return hash;
}

string getHashString(const string &input)
{

    vector<uchar> message = readInput(str_to_hex(input));

    message = padMessage(message);

    vector<vector<uint>> parsed_message = parseMessage(message);

    vector<vector<uint>> hashResult = calHash(parsed_message);

    return hashVectorToString(hashResult);
}

string hashVectorToString(const vector<vector<uint>> &hashVec)
{
    string hashString;
    for (int i = 0; i < 8; i++)
    {
        stringstream stream;
        stream << hex << setw(8) << setfill('0') << hashVec.back()[i];
        hashString += stream.str();
    }
    return hashString;
}

