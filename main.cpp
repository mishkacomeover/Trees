#include <iostream>
#include <locale.h>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cmath>
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

// высота дерева
int getHeight(Node* root) {
    if (!root) return 0;
    return 1 + max(getHeight(root->left), getHeight(root->right));
}

// длина числа (чтобы выравнивать)
int getWidth(int value) {
    return to_string(value).length();
}

// заполняем матрицу
void fill(vector<vector<string>>& canvas, Node* root,
    int row, int col, int offset, int maxWidth) {
    if (!root) return;

    string val = to_string(root->value);
    int start = col - val.length() / 2;

    for (int i = 0; i < val.length(); i++) {
        canvas[row][start + i] = val[i];
    }

    if (root->left) {
        canvas[row + 1][col - offset / 2] = "/";
        fill(canvas, root->left, row + 2, col - offset, offset / 2, maxWidth);
    }

    if (root->right) {
        canvas[row + 1][col + offset / 2] = "\\";
        fill(canvas, root->right, row + 2, col + offset, offset / 2, maxWidth);
    }
}

// главная функция
void printTree(Node* root) {
    if (!root) return;

    int h = getHeight(root);

    int maxWidth = pow(2, h) * 3;   // ширина холста
    int rows = h * 2;

    vector<vector<string>> canvas(rows, vector<string>(maxWidth, " "));

    fill(canvas, root, 0, maxWidth / 2, maxWidth / 4, maxWidth);

    // удаляем лишние пробелы справа
    for (auto& row : canvas) {
        int end = row.size() - 1;
        while (end >= 0 && row[end] == " ") end--;

        for (int i = 0; i <= end; i++) {
            cout << row[i];
        }
        cout << "\n";
    }
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

    printTree(tree);

    return(0);
}