#ifndef AVL_TREE_H_
#define AVL_TREE_H_

#include <stddef.h>
#include <stdlib.h>
#include <iostream>

template<typename KeyType, typename DataType, class CompareFunction>
class AvlTree {
private:


    struct Node {
        DataType data;
        KeyType key;
        Node* parent;
        Node* left;
        Node* right;
        int height;
        int factor;

        Node(): data(), key(), parent(NULL),
                left(NULL), right(NULL), height(0), factor(0) {}
        Node(int height): data(), key(), parent(NULL),
                          left(NULL), right(NULL), height(height), factor(0) {}
        Node(const DataType& data, const KeyType& key, Node* parent = NULL):
                data(data), key(key), parent(parent), left(NULL), right(NULL),
                height(0), factor(0) {}
        ~Node() {

            delete parent;
            delete left;
            delete right;
            parent = NULL;
            left=NULL;
            right=NULL;

        }


        void deleteData() {
            if (this->left) this->left->deleteData();
            if (this->right) this->right->deleteData();
            delete this->data;
        }


        void destroyShallow() {
            if (this->left) this->left->destroyShallow();
            if (this->right) this->right->destroyShallow();
            delete this;
        }

        void generateTree(int height) {
            if (!height) return;
            Node* left = new Node(height);
            Node* right = new Node(height);
            this->left = left;
            this->right = right;
            this->right->parent = this;
            this->left->parent = this;
            height--;
            this->left->generateTree(height);
            this->right->generateTree(height);
        }

        void cleanExtraNodes(int& extraNodesNum) {
            if (!extraNodesNum) return;
            if (this->left == NULL && this->right == NULL) { //leaf
                if (this->parent->left == this) {
                    this->parent->left = NULL;
                    delete this;
                } else {
                    this->parent->right = NULL;
                    delete this;
                }
                extraNodesNum--;
                return;
            }
            this->right->cleanExtraNodes(extraNodesNum);
            this->left->cleanExtraNodes(extraNodesNum);
        }
    };

    void swap_node(Node* a, Node* b) {
        KeyType key = a->key;
        DataType data = a->data;
        a->key = b->key;
        a->data = b->data;
        b->key = key;
        b->data = data;
    }

    Node* head;
    int size;

public:
    AvlTree(): head(NULL), size(0) {}

    ~AvlTree() {
        destroyTree();
//        head = NULL;
//        size = 0;
    }

    void setHead(Node* head){
        this->head = head;
    }
    Node* getHead(){return this->head;}
    void setSize(int size){
        this->size = size;
    }

    void makeIrrelevant() {
        this->head = NULL;
    }

    void destroyTree() {
        if (this->head==NULL) {
            return;
        }
        this->head->destroyShallow();
    }

    void deleteData() {
        if (this->head==NULL) {
            return;
        }
        this->head->deleteData();
    }

    class iterator {
    private:
        const AvlTree* tree;
        Node* current;
        friend class AvlTree;
    public:
        iterator(const AvlTree* tree): tree(tree), current(tree ? tree->head : NULL) {}
        iterator& operator=(const iterator& iterator) {
            current = iterator.current;
            tree = iterator.tree;
            return *this;
        }
        bool operator==(const iterator& i) const {
            return (current == i.current);
        }
        bool operator!=(const iterator& i) const {
            return !(*this == i);
        }
        iterator& operator++() {
            if (current->right) {
                current = current->right;
                while (current->left) {
                    current = current->left;
                }
                return *this;
            }
            while (current) {
                Node const* previous = current;
                current = current->parent;
                if (current && current->left == previous) return *this;
            }
            return *this;
        }
        iterator& operator--() {
            if (current->left) {
                current = current->left;
                while (current->right) {
                    current = current->right;
                }
                return *this;
            }
            while (current) {
                Node const* previous = current;
                current = current->parent;
                if (current && current->right == previous) return *this;
            }
            return *this;
        }
        DataType operator*() { return (this->current->data); }
    };

    iterator begin() {
        iterator it = iterator(this);
        while (it.current && it.current->left) {
            it.current = it.current->left;
        }
        return it;
    }

    iterator end() {
        iterator it = iterator(this);
        it.current = NULL;
        return it;
    }

    bool exists(const KeyType& key) {
        return find(key);
    }

    DataType find(const KeyType& key) {
        Node* current = head;
        while (current) {
            if (CompareFunction()(key, current->key)) {
                current = current->left;
            } else if (CompareFunction()(current->key, key)) {
                current = current->right;
            } else {
                return (current->data);
            }
        }
        return NULL;
    }


//    AVL& operator=(const AVL& avl) {
//        this->size = avl.size;
//        this->head = avl.head;
//        return *this;
//    }

    bool add(const KeyType& key,const DataType& data) {
        if (!head) {
            head = new Node(data, key);
            if(head==NULL){
                return false;
            }
            size++;
            return true;
        }

        bool fromLeft = true;
        Node* current = head;
        Node* previous = current->parent;
        while (current) {
            previous = current;
            if (CompareFunction()(key, current->key)) {
                fromLeft = true;
            } else {
                fromLeft = false;
            }
            current = fromLeft ? current->left : current->right;
        }

        Node* new_node = new Node(data, key, previous);
        if(new_node==NULL){
            return false;
        }
        if (fromLeft) {
            previous->left = new_node;
        } else {
            previous->right = new_node;
        }

        updatePath(new_node);
        roll(new_node);
        updateHead();
        size++;
        return true;
    }

    bool remove(const KeyType& key) {
        if (!this->exists(key)) return false;

        Node* current = head;
        while (CompareFunction()(key, current->key) || CompareFunction()(current->key, key)) {
            current = CompareFunction()(key, current->key) ? current->left : current->right;
        }

        Node* parent = current->parent;
        if (!current->left && !current->right) {
            removeLeaf(current);
        } else if (!current->right || !current->left) {
            removeSingle(current);
        } else {
            removeDouble(current);
        }

        updatePath(parent);
        roll(parent);
        updateHead();
        size--;
        return true;
    }

    void removeLeaf(Node* node) {
        if (!node->parent) {
            delete node;
            head = NULL;
            return;
        } else if (node == node->parent->left) {
            node->parent->left = NULL;
        } else {
            node->parent->right = NULL;
        }
        delete (node);
    }

    void removeSingle(Node* node) {
        if (node->left) { // has left
            if (!node->parent) {
                head = node->left;
                head->parent = NULL;
                delete node;
                return;
            }
            if (node == node->parent->left) {
                node->parent->left = node->left;
            } else {
                node->parent->right = node->left;
            }
            node->left->parent = node->parent;
        } else {
            if (!node->parent) {
                head = node->right;
                head->parent = NULL;
                delete node;
                return;
            }
            if (node == node->parent->left) {
                node->parent->left = node->right;
            } else {
                node->parent->right = node->right;
            }
            node->right->parent = node->parent;
        }
        delete node;
    }

    void removeDouble(Node* node) {
        Node* original = node;
        node = original->right;
        while (node->left) {
            node = node->left;
        }

        swap_node(original, node);

        if (!node->right) {
            removeLeaf(node);
        } else {
            removeSingle(node);
        }
    }

    void updatePath(Node* node) {
        while (node) {
            int left_h = node->left ? node->left->height : -1;
            int right_h = node->right ? node->right->height : -1;
            node->factor = left_h - right_h;
            node->height = std::max(left_h, right_h) + 1;
            node = node->parent;
        }
    }


    void updateHead() {
        if (!this->head) return;
        while (this->head->parent) {
            this->head = this->head->parent;
        }
    }

    void roll_LL(Node* b) {
        Node* a = b->left;
        if (!a) {
            return;
        }
        int height_b_right = b->right ? b->right->height : -1;
        int height_a_right = a->right ? a->right->height : -1;
        int height_a_left = a->left ? a->left->height : -1;

        a->parent = b->parent;

        if (b->parent && b == b->parent->left)
            b->parent->left = a;
        else if (b->parent) {
            b->parent->right = a;
        }

        b->parent = a;

        b->left = a->right;

        if (a->right) {
            a->right->parent = b;
        }

        a->right = b;

        b->height = std::max(height_b_right, height_a_right) + 1;
        b->factor = height_a_right - height_b_right;
        a->height = std::max(b->height, height_a_left) + 1;
        a->factor = height_a_left - b->height;
    }

    void roll_RR(Node* a) {
        Node* b = a->right;
        if (!b) {
            return;
        }
        int height_a_left = a->left ? a->left->height : -1;
        int height_b_right = b->right ? b->right->height : -1;
        int height_b_left = b->left ? b->left->height : -1;

        b->parent = a->parent;

        if (a->parent && a == a->parent->left)
            a->parent->left = b;
        else if (a->parent) {
            a->parent->right = b;
        }

        a->parent = b;

        a->right = b->left;

        if (b->left) {
            b->left->parent = a;
        }

        b->left = a;

        a->height = std::max(height_a_left, height_b_left) + 1;
        a->factor = height_a_left - height_b_left;
        b->height = std::max(a->height, height_b_right) + 1;
        b->factor = a->height - height_b_right;
    }

    void roll_LR(Node* c) {
        Node* b = c->left;
        roll_RR(b);
        roll_LL(c);
    }

    void roll_RL(Node* c) {
        Node* b = c->right;
        roll_LL(b);
        roll_RR(c);
    }


    void roll(Node* node) {
        Node* current = node;
        while (current) {
            int current_factor = current->factor;
            if (current_factor >= -1 && current_factor <= 1) {
                current = current->parent;
                continue;
            }

            int left_factor = current->left ? current->left->factor : -1;
            int right_factor = current->right ? current->right->factor : -1;

            if (current_factor == 2 && left_factor >= 0) {
                roll_LL(current);
            } else if (current_factor == 2 && left_factor == -1) {
                roll_LR(current);
            } else if (current_factor == -2 && right_factor <= 0) {
                roll_RR(current);
            } else if (current_factor == -2 && right_factor == 1) {
                roll_RL(current);
            }

            current = current->parent;
        }
    }

    int getSize() { return size; }



    DataType* getMax() {
        Node* current = this->head;
        if (!current) return NULL;
        while(current->right) {
            current = current->right;
        }
        return &(current->data);
    }

    DataType* createDataArray() {
        if (!size) return NULL;
        DataType* arr = new DataType[size];
        int i = 0;
        for (iterator it = this->begin(); it != this->end(); ++it) {
            arr[i++] = (*it);
        }
        return arr;
    }

    void fill(KeyType keys[], DataType data[]) {
        int i = 0;
        for (iterator it = this->begin(); it != this->end(); ++it) {
            it.current->data = data[i];
            it.current->key = keys[i];
            ++i;
        }
    }

    static AvlTree* createTree(int size){
        AvlTree* tree = new AvlTree;
        if (!size) return tree;

        int nodesFull = 0;
        int leavesNum = 1;
        int height = 0;

        while (nodesFull < size) {
            nodesFull += leavesNum;
            leavesNum *= 2;
            height++;
        }

        tree->head = new Node(height);
        tree->head->generateTree(height-1);
        tree->size = size;

        int extraNodesNum = nodesFull - size;
        tree->head->cleanExtraNodes(extraNodesNum);

        return tree;
    }
};

#endif

