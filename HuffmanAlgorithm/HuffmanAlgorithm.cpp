#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>


using namespace std;


struct Node
{
    unsigned char symbol;
    long long freq;
    Node *left, *right;
};


Node* createNode(char symbol, int freq, Node* left, Node* right)
{
    Node* newNode = new Node();
    newNode->symbol = symbol;
    newNode->freq = freq;
    newNode->left = left;
    newNode->right = right;

    return newNode;
}


unordered_map<char, int> createModel(string str)
{
    unordered_map<char, int> freqs;
    for (char ch : str)
        freqs[ch]++;
    return freqs;
}


bool compareNodes(Node* a, Node* b)
{
    return (a->freq < b->freq);
}



Node* buildHuffmanTree(unordered_map<char, int> freqs)
{
    vector<Node*> nodes;
    for (const pair<char, int> pair : freqs)
    {
        Node* newNode = createNode(pair.first, pair.second, nullptr, nullptr);
        nodes.push_back(newNode);
    }

    while (nodes.size() > 1)
    {
        sort(nodes.begin(), nodes.end(), compareNodes);

        Node* left = nodes[0];
        Node* right = nodes[1];

        Node* merged = new Node();
        merged->symbol = '*';
        merged->freq = left->freq + right->freq;
        merged->left = left;
        merged->right = right;

        nodes.erase(nodes.begin());
        nodes.erase(nodes.begin());
        nodes.push_back(merged);
    }

    return nodes[0];
}



void generateHuffmanCodes(Node* root, string code, unordered_map<char, string> &codes)
{
    if (!root) return;

    if (!root->left && !root->right) {
        codes[root->symbol] = code;
    }

    generateHuffmanCodes(root->left, code + "0", codes);
    generateHuffmanCodes(root->right, code + "1", codes);
}


string encode(Node* root, string str, unordered_map<char, string> codes)
{
    string encodedText;
    for (char ch : str)
        encodedText += codes.at(ch);
    return encodedText;
}


string decode(const string& encodedText, Node* root)
{
    string decodedText;
    Node* current = root;

    for (char bit : encodedText)
    {
        if (bit == '0')
            current = current->left;
        else
            current = current->right;

        if (!current->left && !current->right)
        {
            decodedText += current->symbol;
            current = root;
        }
    }
    return decodedText;
}

void printHuffmanTree(Node* root)
{
    if (root == nullptr) return;

    cout << root->symbol << ": " << root->freq << endl;

    printHuffmanTree(root->left);
    printHuffmanTree(root->right);
}


void deleteTree(Node* root)
{
    if (root == nullptr) return;

    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}


double calculateR(string text, string encodedText)
{
    return (encodedText.size() - text.size()) / text.size() * 100.0;
}


int main()
{
    setlocale(LC_ALL, "ru");

    string text;
    cout << "Введите строку: ";
    getline(cin, text);

    unordered_map<char, int> freqs = createModel(text);

    Node* root = buildHuffmanTree(freqs);

    unordered_map<char, string> codes;
    generateHuffmanCodes(root, "", codes);

    cout << "Вектор частот:\n";
    for (const pair<char, int> pair : freqs)
    {
        cout << pair.first << ": " << pair.second << endl;
    }

    cout << "Коды Хаффмана:\n";
    for (const auto& pair : codes)
    {
        cout << pair.first << ": " << pair.second << endl;
    }

    string encodedStr = encode(root, text, codes);
    cout << "Закодированная строка: " << encodedStr << endl;

    string decodedStr = decode(encodedStr, root);
    cout << "Декодированная строка: " << decodedStr << endl;

    deleteTree(root);
    return 0;
}