#include <vector>
#include <string>
#include <map>
using namespace std;

class Writer {
public:
    // Сохранение бинарного файла (например, восстановленного после декодирования)
    static void saveBinaryFile(const string& filename, const vector<unsigned char>& data);

    // Сохранение сжатого файла: запись таблицы кодов и закодированных данных
    static void saveCompressedFile(const string& filename, const std::map<unsigned char, std::string>& codes, const std::vector<unsigned char>& encoded);
};
