#include <vector>
#include <string>
#include <map>
using namespace std;

class Reader {
public:
    // Чтение бинарного файла в вектор байт
    static vector<unsigned char> loadBinaryFile(const string& filename);

    // Загрузка сжатого файла: считывание таблицы кодов и закодированных данных
    static void loadCompressedFile(const string& filename, std::map<unsigned char, std::string>& codes, std::vector<unsigned char>& encoded);
};
