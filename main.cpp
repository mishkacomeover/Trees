#include <iostream>
#include <locale.h>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
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

struct NodeBplus {
    vector<int> keys;
    vector<NodeBplus*> children;
    NodeBplus* next;
    bool is_leaf;
};

NodeBplus* createNode(bool is_leaf) {
    NodeBplus* node = new NodeBplus{{}, {}, nullptr, is_leaf};
    return node;
}

void InsertToLeaf(NodeBplus* leaf, int key) {
    leaf -> keys.push_back(key);
    sort(leaf -> keys.begin(), leaf -> keys.end());
}

int findChildrenIndex(NodeBplus* node, int key) {
    int i = 0;
    while (i < node -> keys.size() && key >= node -> keys[i]) i++;
    return i;
}

NodeBplus* insertRecursive(NodeBplus* node, int key, int max_keys) {
    if (node -> is_leaf) {
        InsertToLeaf(node, key);

        if (node -> keys.size() > max_keys) {
            //split
        }

        return node;
    }

    else {
        int index = findChildrenIndex(node, key);
        insertRecursive(node -> children[index], key, max_keys);
        return node;
    }
}

NodeBplus* Insert(NodeBplus* root, int key, int max_keys) {
    if (root == nullptr) {
        root = createNode(true);
        root -> keys.push_back(key);
        return root;
    }

    root = insertRecursive(root, key, max_keys);

    if (root -> keys.size() > max_keys) {
        //split
    }
    
    return root;
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
    cout << "Ну вот вам bst//" << endl;


    return(0);
}