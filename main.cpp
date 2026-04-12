#include <iostream>
#include <locale.h>
#include <vector>
#include <string>
#include <sstream>
using namespace std;

struct Node {
    int value;
    Node* left;
    Node* right;
};

// вставка узла
Node* InsertNodeBST(Node* node, int value) {
    if (node == nullptr) {
        Node* newNode = new Node{ value,nullptr,nullptr };
        return newNode;
    }

    if (value < node->value) {
        node->left = InsertNodeBST(node->left, value);
    }

    else {
        node->right = InsertNodeBST(node->right, value);
    }

    return node;
}

//построение дерева
Node* buildBST(vector<int> numbers, int root_value) {
    Node* root = nullptr;

    if (root_value != 0) {
        root = new Node{ root_value, nullptr, nullptr };
    }

    for (int value : numbers) {
        root = InsertNodeBST(root, value);
    }
    return root;
}

void printTree(Node* node, string prefix = "", bool isLeft = true) {
    if (node == nullptr) return;
    cout << prefix << (isLeft ? "├── " : "└── ") << node->value << endl;
    printTree(node->left, prefix + (isLeft ? "│   " : "    "), true);
    printTree(node->right, prefix + (isLeft ? "│   " : "    "), false);
}

int main() {
    setlocale(LC_ALL, "Russian");

    string input;
    vector<int> numbers;
    int root_value;

    cout << "Введите числа через пробел: ";
    getline(cin, input);

    stringstream ss(input);
    int num;
    while (ss >> num) {
        numbers.push_back(num);
    }

    cout << "Введите значение корня дерева: ";
    cin >> root_value;

    Node* tree = buildBST(numbers, root_value);

    printTree(tree); s

    return(0);
}