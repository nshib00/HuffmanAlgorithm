#include <vector>
#include <string>
#include <map>
using namespace std;

class Writer {
public:
    // ���������� ��������� ����� (��������, ���������������� ����� �������������)
    static void saveBinaryFile(const string& filename, const vector<unsigned char>& data);

    // ���������� ������� �����: ������ ������� ����� � �������������� ������
    static void saveCompressedFile(const string& filename, const std::map<unsigned char, std::string>& codes, const std::vector<unsigned char>& encoded);
};
