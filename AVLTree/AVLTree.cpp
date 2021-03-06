#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>

template<typename T>
class BST {
    struct Node {
        T value;
        int height = 1;

        Node *left = nullptr;
        Node *right = nullptr;

        explicit Node(const T &value) :
            value(value) {}
    };

    Node *root = nullptr;
    int getHeight(Node *node) {
        if (node == nullptr) return 0;
        return node->height;
    }

    void RRotate(Node *&node) {
        // 这个函数保证传入的node和node->left不为空
        Node *left = node->left;
        node->left = left->right;
        left->right = node;
        node = left;
        // 先获取下方结点的高度，最后处理node
        Node *right = node->right;
        // 此函数中只有这两个的高度应该修改一下
        right->height = std::max(getHeight(right->left), getHeight(right->right)) + 1;
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    void LRotate(Node *&node) {
        Node *right = node->right;
        node->right = right->left;
        right->left = node;
        node = right;
        Node *left = node->left;
        left->height = std::max(getHeight(left->left), getHeight(left->right)) + 1;
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }

    bool insert(const T &value, Node *&node) {
        // 空结点直接上，高度默认为1
        if (node == nullptr) {
            node = new Node(value);
            return true;
        }
        // 得到插入结果的
        bool result = false;
        if (value > node->value) {
            result = insert(value, node->right);
        }
        else if (value < node->value) {
            result = insert(value, node->left);
        }
        else {
            return false;
        }
        // 插入失败直接返回false，不需要调整高度
        if (!result) return false;
        // 得到左右高度，判断是否失衡
        int left_height = getHeight(node->left);
        int right_height = getHeight(node->right);
        // 不平衡了,左边比右边高两个，说明插到左边
        if (left_height - right_height == 2) {
            // 插到左左，想右转
            if (value < node->left->value) {
                RRotate(node);
            }
            else {
                // 先左后右
                // LRRotate(node);
                LRotate(node->left);
                RRotate(node);
            }
        }
        else
            // 右边比左边高两个
            if (left_height - right_height == -2) {
                if (value > node->right->value) {
                    // 插到右右，往左边转一下就行了
                    LRotate(node);
                }
                else {
                    // 先右后左
                    RRotate(node->right);
                    LRotate(node);
                }
            }
        // 旋转后才可能到这里？
        // 改进一下调整高度的函数
        // 在函数最后获取一下当前的结点的应有高度
        // 没有旋转也需要调整高度
        node->height = std::max(getHeight(node->left), getHeight(node->right)) + 1;

        return true;
    }

    // 中序遍历，不多说
    void InOrder(const Node *node) const {
        if (node != nullptr) {
            InOrder(node->left);
            std::cout << node->value << "\n";
            InOrder(node->right);
        }
    }
    // 递归清理空间
    void free(Node *&node) {
        if (node != nullptr) {
            free(node->left);
            free(node->right);
            delete node;
            node = nullptr;
        }
    }

    bool isBalance(const Node *node) {
        if (node == nullptr) return true;
        int left = getHeight(node->left);
        int right = getHeight(node->right);
        if (isBalance(node->left) && isBalance(node->right)) {
            return std::abs(left - right) < 2;
        }
        return false;
    }

    void printLastLevel(Node *&node, int depth) {
        if (node == nullptr) return;

        if (node->left == nullptr && node->right == nullptr) {
            std::cout << depth << " ";
            return;
        }

        printLastLevel(node->left, depth + 1);
        printLastLevel(node->right, depth + 1);
    }

public:
    // 释放空间函数
    void free() {
        free(root);
    }
    // 插入函数
    bool insert(const T &value) {
        return insert(value, root);
    }
    // 中序遍历
    void inOrder() const {
        InOrder(root);
    }

    bool isBalance() {
        return isBalance(root);
    }

    void printLastLevel() {
        int depth = 0;
        printLastLevel(root, 1);
    }
};


int main() {
    // 测试函数
    for (int j = 0; j < 10; ++j) {
        std::cout << j + 1 << "th test:\n";
        BST<size_t> tree{};
        srand(clock());
        int size = 1000000;
        for (int i = 0; i < size; ++i) {
            tree.insert(static_cast<const size_t>(rand() % 1024));
        }
        std::cout << "Tree is balance: " << (tree.isBalance() ? "true" : "false") << "\n";
        // 释放空间
        //tree.free();
        //tree.printLastLevel();
    }

    int s = 0;
    std::cin >> s;

    return 0;
}