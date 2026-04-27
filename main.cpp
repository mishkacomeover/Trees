#include <iostream>
#include <locale.h>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

struct BplusTree;
struct NodeBplus;
void split(BplusTree* tree, NodeBplus* node);
void insert_into_parent(BplusTree* tree, NodeBplus* left, NodeBplus* right, int up_key);

//=======================================================BST=================================================================//
// структура для BST
struct Node {
    int value;
    Node* left;
    Node* right;
};

// вставка узла для BST
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

//построение дерева BST
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

//=======================================================B+=================================================================//
// структура узла
struct NodeBplus {
    bool leaf;
    vector<int> keys;
    NodeBplus* parent;
    vector<NodeBplus*> children;
    NodeBplus* left;
    NodeBplus* right;
};

// структура дерева
struct BplusTree {
    int t;
    NodeBplus* root;
};

// нахождение листа для вставки
NodeBplus* find_leaf(BplusTree* tree, int key) {
    NodeBplus* current = tree -> root;
    
    while (!current -> leaf) {
        int i = 0;
        while (i < current -> keys.size() && key >= current -> keys[i]) i++;
        current = current -> children[i];
    }
    return current;
}

// вставка ключа в лист
void insertBplus(BplusTree* tree, int key) {
    // если дерево пустое - новый лист и в него ключ
    if (tree -> root == nullptr) {
        tree -> root = new NodeBplus();
        tree -> root -> leaf = true;
        tree -> root -> keys.push_back(key);
        return;
    }
    
    NodeBplus* leafchik = find_leaf(tree, key);
    
    // проверка вдруг уже есть такой ключ
    if (find(leafchik -> keys.begin(), leafchik -> keys.end(), key) != leafchik -> keys.end())
        return;
    
    // вставка на нужную позицию
    int pos = 0;
    while (pos < leafchik -> keys.size() && leafchik -> keys[pos] < key) pos++;
    leafchik -> keys.insert(leafchik -> keys.begin() + pos, key);
    
    // проверка на переполнение
    if (leafchik -> keys.size() == 2 * tree -> t) {
        split(tree, leafchik);
    }
}

// вставка ключа и правого ребенка в родителя
void insert_into_parent(BplusTree* tree, NodeBplus* left, NodeBplus* right, int up_key) {

    // если у левого узла нет родителя - ну грустно че, создаем новый корень
    if (left->parent == nullptr) {
        NodeBplus* new_root = new NodeBplus();
        new_root->leaf = false;
        new_root->keys = {up_key};
        new_root->children = {left, right};
        left->parent = new_root;
        right->parent = new_root;
        tree->root = new_root;
        return;
    }
    
    // если родитель есть
    NodeBplus* parent = left->parent;
    
    // ищем позицию для вставки
    int pos = 0;
    while (pos < parent->keys.size() && parent -> keys[pos] < up_key) {
        pos++;
    }
    
    // вставляем ключ и правого ребенка
    parent -> keys.insert(parent -> keys.begin() + pos, up_key);
    parent -> children.insert(parent -> children.begin() + pos + 1, right);
    right -> parent = parent;
    
    // если родитель переполнился - разбиваем его
    if (parent -> keys.size() == 2 * tree -> t) {
        split(tree, parent);
    }
}

// разбиение узла
void split(BplusTree* tree, NodeBplus* node) {
    int t = tree -> t;
    
    // создаем правый узел
    NodeBplus* right = new NodeBplus();
    right -> leaf = node -> leaf;
    right -> parent = node -> parent;
    
    // связываем листья в список
    right -> left = node;
    right -> right = node -> right;
    if (node -> right) node -> right -> left = right;
    node -> right = right;
    
    if (node -> leaf) {
        // ЛИСТ: забираем КЛЮЧИ с индекса t
        for (int i = t; i < node -> keys.size(); i++) {
            right -> keys.push_back(node -> keys[i]);
        }
        node -> keys.resize(t); // ОБРЕЗАНИЕ
        
        // ключ для поднятия - первый ключ правого узла
        int up_key = right -> keys[0];
        
        // вставляем в родителя
        insert_into_parent(tree, node, right, up_key);
        
    } else {
        // ВНУТРЕННИЙ УЗЕЛ: забираем КЛЮЧИ с индекса t
        for (int i = t; i < node -> keys.size(); i++) {
            right -> keys.push_back(node -> keys[i]);
        }
        
        // забираем ДЕТЕЙ с индекса t
        for (int i = t; i < node -> children.size(); i++) {
            right -> children.push_back(node -> children[i]);
            node -> children[i] -> parent = right;
        }
        
        // Ключ для поднятия - предпоследний ключ (t-1)
        int up_key = node -> keys[t - 1];
        
        // Обрезаем левый узел
        node -> keys.resize(t - 1);
        node -> children.resize(t);
        
        // Вставляем в родителя
        insert_into_parent(tree, node, right, up_key);
    }
}

// рисуем BST дерево
void printTree(Node* node, string prefix = "", bool isLeft = true) {
    if (node == nullptr) return;
    cout << prefix << (isLeft ? "├── " : "└── ") << node->value << endl;
    printTree(node->left, prefix + (isLeft ? "│   " : "    "), true);
    printTree(node->right, prefix + (isLeft ? "│   " : "    "), false);
}

// рисуем B+ дерево
void printBPlusTree(NodeBplus* node, string prefix = "", bool isLast = true) {
    if (node == nullptr) return;
    
    cout << prefix;
    cout << (isLast ? "└── " : "├── ");
    
    cout << "[";
    for (size_t i = 0; i < node->keys.size(); i++) {
        cout << node->keys[i];
        if (i < node->keys.size() - 1) cout << " ";
    }
    cout << "]";
    
    if (node->leaf) {
        cout << " (leaf)";
        if (node->right != nullptr) {
            cout << " →";
        }
    }
    cout << endl;
    
    if (!node->leaf) {
        for (size_t i = 0; i < node->children.size(); i++) {
            printBPlusTree(node->children[i], 
                          prefix + (isLast ? "    " : "│   "), 
                          i == node->children.size() - 1);
        }
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
    
    cout << "Введите значение корня BST (0 если нет): ";
    cin >> root_value;
    
    Node* tree = buildBST(numbers, root_value);
    cout << endl << "------ BST ------" << endl;
    printTree(tree);
    
    int t;
    cout << endl << "Введите степень B+ дерева (t): ";
    cin >> t;

    BplusTree bptree;
    bptree.t = t;
    bptree.root = nullptr;
    
    for (int i : numbers) {
        insertBplus(&bptree, i);
    }
    
    cout << endl << "------ B+ ДЕРЕВО ------" << endl;
    printBPlusTree(bptree.root);
    
    return 0;
}