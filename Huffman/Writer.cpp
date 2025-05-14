#include "pch.h"
#include "Writer.h"
#include <fstream>
#include <iostream>

using namespace std;

void Writer::saveBinaryFile(const string& filename, const vector<unsigned char>& data) {
    ofstream file(filename.c_str(), ios::binary);
    if (!file) {
        cerr << "Ошибка записи файла." << endl;
        exit(1);
    }
    file.write((char*)data.data(), data.size());
}

void Writer::saveCompressedFile(const string& filename, const map<unsigned char, string>& codes, const vector<unsigned char>& encoded) {
    ofstream file(filename.c_str(), ios::binary);
    if (!file) {
        cerr << "Ошибка записи сжатого файла." << endl;
        exit(1);
    }

    size_t tableSize = codes.size();
    file.write((char*)&tableSize, sizeof(tableSize));

    for (map<unsigned char, string>::const_iterator it = codes.begin(); it != codes.end(); ++it) {
        unsigned char sym = it->first;
        const string& code = it->second;
        size_t len = code.size();
        file.write((char*)&sym, sizeof(sym));
        file.write((char*)&len, sizeof(len));
        file.write(code.data(), len);
    }

    size_t encodedSize = encoded.size();
    file.write((char*)&encodedSize, sizeof(encodedSize));
    file.write((char*)encoded.data(), encodedSize);
}
