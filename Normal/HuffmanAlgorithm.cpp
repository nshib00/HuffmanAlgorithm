#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <queue>
#include <bitset>
#include <string>
#include "Reader.h"
#include "Writer.h"

using namespace std;

// ��������� ���� ������ ��������
struct Node {
    unsigned char symbol;
    long long freq;
    Node* left;
    Node* right;
    Node(unsigned char s, long long f) : symbol(s), freq(f), left(NULL), right(NULL) {}
    Node(Node* l, Node* r) : symbol(0), freq(l->freq + r->freq), left(l), right(r) {}
};

struct Compare {
    bool operator()(Node* a, Node* b) { return a->freq > b->freq; }
};

// ����������� ������� ���������� ������� ����� �� ������ ��������
void buildCodeTable(Node* root, string code, map<unsigned char, string>& codes) {
    if (!root) return;
    if (!root->left && !root->right)
        codes[root->symbol] = code;
    buildCodeTable(root->left, code + "0", codes);
    buildCodeTable(root->right, code + "1", codes);
}

// �������� ������ �������� � ���������� ������� �����
Node* createHuffmanTree(const vector<unsigned char>& data, map<unsigned char, string>& codes) {
    map<unsigned char, long long> freq;
    for (size_t i = 0; i < data.size(); ++i)
        freq[data[i]]++;

    priority_queue<Node*, vector<Node*>, Compare> pq;
    for (map<unsigned char, long long>::const_iterator it = freq.begin(); it != freq.end(); ++it)
        pq.push(new Node(it->first, it->second));

    while (pq.size() > 1) {
        Node* l = pq.top();
        pq.pop();
        Node* r = pq.top();
        pq.pop();
        pq.push(new Node(l, r));
    }

    Node* root = pq.top();
    buildCodeTable(root, "", codes);
    return root;
}

// ������� ������ ������: ��������� ������� ������, ��������� �� ������� ����� � ����������� � ������ ����
vector<unsigned char> encode(const vector<unsigned char>& data, map<unsigned char, string>& codes) {
    string bits;
    for (size_t i = 0; i < data.size(); ++i)
        bits += codes[data[i]];

    int pad = (8 - bits.size() % 8) % 8;
    bits.append(pad, '0');

    vector<unsigned char> out;
    out.push_back(static_cast<unsigned char>(pad));
    for (size_t i = 0; i < bits.size(); i += 8) {
        bitset<8> bs(bits.substr(i, 8));
        out.push_back(static_cast<unsigned char>(bs.to_ulong()));
    }
    return out;
}

// ������� �������������� ������: ���������� ������� ������ � �������� �����, ��������� ������ ��������
vector<unsigned char> decode(const vector<unsigned char>& encoded, Node* root) {
    if (encoded.empty())
        return vector<unsigned char>();

    int pad = encoded[0];
    string bits;
    for (size_t i = 1; i < encoded.size(); ++i)
        bits += bitset<8>(encoded[i]).to_string();

    bits.resize(bits.size() - pad);

    vector<unsigned char> out;
    Node* cur = root;
    for (size_t i = 0; i < bits.size(); ++i) {
        cur = (bits[i] == '0') ? cur->left : cur->right;
        if (!cur->left && !cur->right) {
            out.push_back(cur->symbol);
            cur = root;
        }
    }
    return out;
}

int main() {
    setlocale(LC_ALL, "ru");

    string inputFile = "../files/input.txt";
    string compressedFile = "../files/input.hfmn";
    string outputFile = "../files/output.txt";

    // ������ ��������� �����
    vector<unsigned char> data = Reader::loadBinaryFile(inputFile);
    size_t originalSize = data.size();

    // ���������� ������ �������� � ������� �����
    map<unsigned char, string> codes;
    Node* root = createHuffmanTree(data, codes);

    // ������ ������ � ���������� ������� �����
    vector<unsigned char> encoded = encode(data, codes);
    Writer::saveCompressedFile(compressedFile, codes, encoded);

    size_t compressedSize = encoded.size();
    double compressionRatio = 100.0 * (1.0 - static_cast<double>(compressedSize) / originalSize);

    // �������� ������� ����� � �������������� ������� �����
    map<unsigned char, string> loadedCodes;
    vector<unsigned char> loadedEncoded;
    Reader::loadCompressedFile(compressedFile, loadedCodes, loadedEncoded);

    // ������������� ������ � ���������� ���������������� �����
    vector<unsigned char> decoded = decode(loadedEncoded, root);
    Writer::saveBinaryFile(outputFile, decoded);

    cout << "���� " << inputFile << " ������� ���� � �����������." << endl
        << "��������� �����������: " << compressedFile << endl
        << "��������� �������������: " << outputFile << endl
        << "�������� ������ �����: " << originalSize << " ����" << endl
        << "������ ����� ������: " << compressedSize << " ����" << endl
        << "������� ������: " << compressionRatio << "%\n";

    return 0;
}
