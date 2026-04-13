#include <iostream>
#include <locale.h>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

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

// структура для B+
struct NodeBplus {
    vector<int> keys;
    vector<NodeBplus*> children;
    NodeBplus* next;
    bool is_leaf;
};

// структура для вывода результата сплита для B+
struct splitresult {
    NodeBplus* brother;
    int key;
};

// создание узла для B+
NodeBplus* createNode(bool is_leaf) {
    NodeBplus* node = new NodeBplus{{}, {}, nullptr, is_leaf};
    return node;
}

// вставка в лист B+
void InsertToLeaf(NodeBplus* leaf, int key) {
    leaf->keys.push_back(key);
    sort(leaf->keys.begin(), leaf->keys.end());
}

// наход индекса ребенка B+
int findChildrenIndex(NodeBplus* node, int key) {
    int i = 0;
    while (i < node->keys.size() && key >= node->keys[i]) i++;
    return i;
}

// функция сплита (убрал max_keys из параметров - он не нужен)
splitresult split(NodeBplus* node) {
    int mid = node->keys.size() / 2;
    NodeBplus* brother = createNode(node->is_leaf);

    // копируем половину ключей в братуху
    for (int i = mid; i < node->keys.size(); i++) {
        brother->keys.push_back(node->keys[i]);
    }
    // удаляем скопированные ключи из первого братухи
    node->keys.erase(node->keys.begin() + mid, node->keys.end());

    // копируем детей если это не лист
    if (!node->is_leaf) {
        for (int i = mid + 1; i < node->children.size(); i++) {
            brother->children.push_back(node->children[i]);
        }
        node->children.erase(node->children.begin() + mid + 1, node->children.end());
    }

    // если лист, то связываем через next
    if (node->is_leaf) {
        brother->next = node->next;
        node->next = brother;
    }

    // найдем ключик который будем поднимать
    int upkey;
    if (node->is_leaf) {
        upkey = brother->keys[0]; // первый ключ в братане
    }
    else {
        upkey = brother->keys[0];
        brother->keys.erase(brother->keys.begin());
        // детей тоже надо перекинуть
        if (!brother->children.empty()) {
            brother->children.erase(brother->children.begin());
        }
    }

    return {brother, upkey};
}

// ВСТАВКА КЛЮЧА И БРАТА В РОДИТЕЛЯ (новая функция)
void insertIntoParent(NodeBplus* parent, int key, NodeBplus* brother, NodeBplus* oldChild) {
    // Находим позицию старого ребенка
    int pos = 0;
    while (pos < parent->children.size() && parent->children[pos] != oldChild) {
        pos++;
    }
    
    // Вставляем ключ
    parent->keys.insert(parent->keys.begin() + pos, key);
    // Вставляем нового брата
    parent->children.insert(parent->children.begin() + pos + 1, brother);
}

// рекурсивная функция вставки B+ (теперь возвращает splitresult)
splitresult insertRecursive(NodeBplus* node, int key, int max_keys) {
    if (node->is_leaf) {
        InsertToLeaf(node, key);
        
        if (node->keys.size() > max_keys) {
            return split(node);
        }
        return {nullptr, -1};
    }
    else {
        int index = findChildrenIndex(node, key);
        splitresult res = insertRecursive(node->children[index], key, max_keys);
        
        // Если ребенок сплитанулся
        if (res.brother != nullptr) {
            insertIntoParent(node, res.key, res.brother, node->children[index]);
            
            // Проверяем переполнение текущего узла
            if (node->keys.size() > max_keys) {
                return split(node);
            }
        }
        return {nullptr, -1};
    }
}

// Не рекурсивная функция вставки B+
NodeBplus* Insert(NodeBplus* root, int key, int max_keys) {
    if (root == nullptr) {
        root = createNode(true);
        root->keys.push_back(key);
        return root;
    }

    splitresult res = insertRecursive(root, key, max_keys);

    // Если корень сплитанулся - создаем новый корень
    if (res.brother != nullptr) {
        NodeBplus* newRoot = createNode(false);
        newRoot->keys.push_back(res.key);
        newRoot->children.push_back(root);
        newRoot->children.push_back(res.brother);
        root = newRoot;
    }
    
    return root;
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
    
    if (node->is_leaf) {
        cout << " (leaf)";
        if (node->next != nullptr) {
            cout << " →";
        }
    }
    cout << endl;
    
    if (!node->is_leaf) {
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

    int max_keys;
    cout << endl << "Введите максимальное количество ключей в узле B+ дерева:";
    cin >> max_keys;
    
    NodeBplus* bptree = nullptr;
    for (int i : numbers) {
        bptree = Insert(bptree, i, max_keys);
    }
    
    cout << endl << "------ B+ ДЕРЕВО ------" << endl;
    printBPlusTree(bptree);

    return 0;
}