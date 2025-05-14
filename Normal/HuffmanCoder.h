#ifndef HUFFMANCODER_H
#define HUFFMANCODER_H

#include <vector>
#include <map>
#include <string>

struct Node {
    unsigned char symbol;
    long long freq;
    Node* left;
    Node* right;
    Node(unsigned char s, long long f);
    Node(Node* l, Node* r);
};

struct CompressionStats {
    long long originalSize;
    long long compressedSize;
    double compressionRatio;
};

class HuffmanCoder {
private:
    Node* root;
    void freeTree(Node* node);
public:
    HuffmanCoder();
    ~HuffmanCoder();

    Node* createHuffmanTree(const std::vector<unsigned char>& data, std::map<unsigned char, std::string>& codes);
    void buildCodeTable(Node* root, std::string code, std::map<unsigned char, std::string>& codes);
    std::vector<unsigned char> encode(const std::vector<unsigned char>& data, std::map<unsigned char, std::string>& codes);
    std::vector<unsigned char> decode(const std::vector<unsigned char>& encoded, Node* root);
};

#endif // HUFFMANCODER_H
