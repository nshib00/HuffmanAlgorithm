#include "HuffmanCoder.h"
#include "Reader.h"
#include "Writer.h"
#include <queue>
#include <bitset>
#include <locale>
#include <iostream>

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

Node::Node(unsigned char s, long long f)
    : symbol(s), freq(f), left(nullptr), right(nullptr) {
}

Node::Node(Node* l, Node* r)
    : symbol(0), freq(l->freq + r->freq), left(l), right(r) {
}

HuffmanCoder::HuffmanCoder() : root(nullptr) {}

HuffmanCoder::~HuffmanCoder() {
    freeTree(root);
}

void HuffmanCoder::freeTree(Node* node) {
    if (!node) return;
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

Node* HuffmanCoder::createHuffmanTree(const std::vector<unsigned char>& data, std::map<unsigned char, std::string>& codes) {
    std::map<unsigned char, long long> freq;
    for (unsigned char byte : data)
        freq[byte]++;

    std::priority_queue<Node*, std::vector<Node*>, Compare> pq;
    for (const auto& it : freq)
        pq.push(new Node(it.first, it.second));

    while (pq.size() > 1) {
        Node* l = pq.top(); pq.pop();
        Node* r = pq.top(); pq.pop();
        pq.push(new Node(l, r));
    }

    root = pq.top();
    buildCodeTable(root, "", codes);
    return root;
}

void HuffmanCoder::buildCodeTable(Node* node, std::string code, std::map<unsigned char, std::string>& codes) {
    if (!node) return;
    if (!node->left && !node->right) {
        codes[node->symbol] = code;
        return;
    }
    buildCodeTable(node->left, code + "0", codes);
    buildCodeTable(node->right, code + "1", codes);
}

std::vector<unsigned char> HuffmanCoder::encode(const std::vector<unsigned char>& data, std::map<unsigned char, std::string>& codes) {
    std::string bits;
    for (unsigned char byte : data)
        bits += codes[byte];

    int pad = (8 - bits.size() % 8) % 8;
    bits.append(pad, '0');

    std::vector<unsigned char> out;
    out.push_back(static_cast<unsigned char>(pad));
    for (size_t i = 0; i < bits.size(); i += 8) {
        std::bitset<8> bs(bits.substr(i, 8));
        out.push_back(static_cast<unsigned char>(bs.to_ulong()));
    }

    return out;
}

std::vector<unsigned char> HuffmanCoder::decode(const std::vector<unsigned char>& encoded, Node* root) {
    if (encoded.empty())
        return {};

    int pad = encoded[0];
    std::string bits;
    for (size_t i = 1; i < encoded.size(); ++i)
        bits += std::bitset<8>(encoded[i]).to_string();

    bits.resize(bits.size() - pad);

    std::vector<unsigned char> out;
    Node* current = root;
    for (char bit : bits) {
        current = (bit == '0') ? current->left : current->right;
        if (!current->left && !current->right) {
            out.push_back(current->symbol);
            current = root;
        }
    }
    return out;
}


int main() {
    setlocale(LC_ALL, "ru");

    string inputFile = "../files/input.txt";
    string compressedFile = "../files/input.hfmn";
    string outputFile = "../files/output.txt";

    // Создание объекта кодера
    HuffmanCoder coder;

    // Чтение исходного файла
    vector<unsigned char> data = Reader::loadBinaryFile(inputFile);
    size_t originalSize = data.size();

    // Построение дерева Хаффмана и таблицы кодов
    map<unsigned char, string> codes;
    Node* root = coder.createHuffmanTree(data, codes);

    // Сжатие данных и сохранение сжатого файла
    vector<unsigned char> encoded = coder.encode(data, codes);
    Writer::saveCompressedFile(compressedFile, codes, encoded);

    size_t compressedSize = encoded.size();
    double compressionRatio = 100.0 * (1.0 - static_cast<double>(compressedSize) / originalSize);

    // Загрузка сжатого файла и восстановление таблицы кодов
    map<unsigned char, string> loadedCodes;
    vector<unsigned char> loadedEncoded;
    Reader::loadCompressedFile(compressedFile, loadedCodes, loadedEncoded);

    // Декодирование данных и сохранение восстановленного файла
    vector<unsigned char> decoded = coder.decode(loadedEncoded, root);
    Writer::saveBinaryFile(outputFile, decoded);

    cout << "Файл " << inputFile << " успешно сжат и декодирован." << endl
        << "Результат кодирования: " << compressedFile << endl
        << "Результат декодирования: " << outputFile << endl
        << "Исходный размер файла: " << originalSize << " байт" << endl
        << "Размер после сжатия: " << compressedSize << " байт" << endl
        << "Степень сжатия: " << compressionRatio << "%\n";

    return 0;
}
