#include <vector>
#include <string>
#include <map>
using namespace std;

class Reader {
public:
    // ������ ��������� ����� � ������ ����
    static vector<unsigned char> loadBinaryFile(const string& filename);

    // �������� ������� �����: ���������� ������� ����� � �������������� ������
    static void loadCompressedFile(const string& filename, std::map<unsigned char, std::string>& codes, std::vector<unsigned char>& encoded);
};
