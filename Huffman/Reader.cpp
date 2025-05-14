#include "pch.h"
#include "Reader.h"
#include <fstream>
#include <iostream>
#include <iterator>

using namespace std;

vector<unsigned char> Reader::loadBinaryFile(const string& filename) {
    ifstream file(filename.c_str(), ios::binary);
    if (!file) {
        cerr << "Ошибка чтения файла!" << endl;
        exit(1);
    }
    // Считываем файл в вектор байт
    vector<unsigned char> data((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    return data;
}

void Reader::loadCompressedFile(const string& filename, map<unsigned char, string>& codes, vector<unsigned char>& encoded) {
    ifstream file(filename.c_str(), ios::binary);
    if (!file) {
        cerr << "Ошибка открытия сжатого файла!" << endl;
        exit(1);
    }

    size_t tableSize;
    file.read((char*)&tableSize, sizeof(tableSize));

    for (size_t i = 0; i < tableSize; ++i) {
        unsigned char sym;
        size_t len;
        file.read((char*)&sym, sizeof(sym));
        file.read((char*)&len, sizeof(len));
        string code(len, ' ');
        file.read(&code[0], len);
        codes[sym] = code;
    }

    size_t encodedSize;
    file.read((char*)&encodedSize, sizeof(encodedSize));
    encoded.resize(encodedSize);
    file.read((char*)encoded.data(), encodedSize);
}
