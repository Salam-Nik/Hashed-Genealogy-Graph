#ifndef SHA256
#define SHA256

#include <iomanip>
#include <iostream>
#include <sstream>
#include <vector>
#include <cmath>

using namespace std;

typedef unsigned long long ll;
typedef unsigned int uint;
typedef unsigned char uchar;


extern const uint K[64];
extern uint W[64];
extern uint a, b, c, d, e, f, g, h, T1, T2;

string str_to_hex(const string &input);
vector<uchar> readInput(const string &input);
const int calPadding(ll size);
vector<uchar> padMessage(vector<uchar> bytes);
vector<vector<uint>> parseMessage(vector<uchar> bytes);

const uint SHR(const uint &n, const uint &x);
const uint ROTR(const uint &n, const uint &x);
const uint Ch(const uint &x, const uint &y, const uint &z);
const uint Maj(const uint &x, const uint &y, const uint &z);
const uint cSigma0(const uint &x);
const uint cSigma1(const uint &x);
const uint sSigma0(const uint &x);
const uint sSigma1(const uint &x);

vector<vector<uint>> calHash(const vector<vector<uint>> M);

string getHashString(const string &input);

string hashVectorToString(const vector<vector<uint>> &hashVec);

#endif
