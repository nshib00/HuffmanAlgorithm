#pragma once
#include "pch.h"
#include "ExportDefs.h"

#include <vector>
#include <map>
#include <string>


struct HUFFMAN_API Node {
    unsigned char symbol;
    long long freq;
    Node* left;
    Node* right;
    Node(unsigned char s, long long f);
    Node(Node* l, Node* r);
};

struct HUFFMAN_API CompressionStats {
    size_t originalSize;
    size_t compressedSize;
    double compressionRatio;
};

class HUFFMAN_API HuffmanCoder {
private:
    Node* root;
    size_t originalSize;
    size_t compressedSize;

    void freeTree(Node* node);
    Node* createHuffmanTree(const std::vector<unsigned char>& data, std::map<unsigned char, std::string>& codes);
    void buildCodeTable(Node* root, std::string code, std::map<unsigned char, std::string>& codes);
    std::vector<unsigned char> huffmanEncode(const std::vector<unsigned char>& data, std::map<unsigned char, std::string>& codes);
    std::vector<unsigned char> huffmanDecode(const std::vector<unsigned char>& encoded, Node* root);
public:
    HuffmanCoder();
    ~HuffmanCoder();
    void encode(const std::string& inputFile, const std::string& compressedFile);
    void decode(const std::string& compressedFile, const std::string& outputFile);
    CompressionStats getStats();
};
