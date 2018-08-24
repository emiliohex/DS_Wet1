#ifndef AVL_TREE_H_
#define AVL_TREE_H_

#include <stddef.h>
#include <stdlib.h>
#include <iostream>

template<typename KeyType, typename DataType, class CompareFunction>
class AvlTree {
private:


    struct avlNode {
        DataType data;
        KeyType key;
        avlNode* parent;
        avlNode* left;
        avlNode* right;
        int height;
        int factor;

        avlNode(): data(), key(), parent(NULL),left(NULL), right(NULL),
                 height(0), factor(0) {}
        avlNode(int height): data(), key(), parent(NULL),
                          left(NULL), right(NULL), height(height), factor(0) {}
        avlNode(const DataType& data, const KeyType& key, avlNode* parent = NULL):
                data(data), key(key), parent(parent), left(NULL), right(NULL),
                height(0), factor(0) {}
        ~avlNode() { }


        void dataDelete() {
            if (this->left) this->left->dataDelete();
            if (this->right) this->right->dataDelete();
            delete this->data;
        }


        void deleteShallow() {
            if (this->left) this->left->deleteShallow();
            if (this->right) this->right->deleteShallow();
            delete this;
        }

        void generateTree(int height) {
            if (!height) return;
            avlNode* left = new avlNode(height);
            avlNode* right = new avlNode(height);
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

    void swap_node(avlNode* a, avlNode* b) {
        KeyType key = a->key;
        DataType data = a->data;
        a->key = b->key;
        a->data = b->data;
        b->key = key;
        b->data = data;
    }

    avlNode* head;
    int size;

public:
    AvlTree(): head(NULL), size(0) {}

    ~AvlTree() {
        deleteTree();
//        head = NULL;
//        size = 0;
    }


    avlNode* getHead(){return this->head;}
    void setSize(int size){
        this->size = size;
    }
    void setHead(avlNode* head){
        this->head = head;
    }

    void emptyHead() {
        this->head = NULL;
    }

    void deleteTree() {
        if (this->head==NULL) {
            return;
        }
        this->head->deleteShallow();
    }

    void dataDelete() {
        if (this->head==NULL) {
            return;
        }
        this->head->dataDelete();
    }

    class iterator {
    private:
        const AvlTree* tree;
        friend class AvlTree;
        avlNode* currentNode;
    public:
        iterator(const AvlTree* tree): tree(tree), currentNode(tree ? tree->head : NULL) {}
        iterator& operator=(const iterator& iterator) {
            currentNode = iterator.currentNode;
            tree = iterator.tree;
            return *this;
        }
        bool operator==(const iterator& i) const {
            return (currentNode == i.currentNode);
        }
        bool operator!=(const iterator& i) const {
            return !(*this == i);
        }
        iterator& operator++() {
            if (currentNode->right) {
                currentNode = currentNode->right;
                while (currentNode->left) {
                    currentNode = currentNode->left;
                }
                return *this;
            }
            while (currentNode) {
                avlNode const* previous = currentNode;
                currentNode = currentNode->parent;
                if (currentNode && currentNode->left == previous) return *this;
            }
            return *this;
        }
        iterator& operator--() {
            if (currentNode->left) {
                currentNode = currentNode->left;
                while (currentNode->right) {
                    currentNode = currentNode->right;
                }
                return *this;
            }
            while (currentNode) {
                avlNode const* previous = currentNode;
                currentNode = currentNode->parent;
                if (currentNode && currentNode->right == previous) return *this;
            }
            return *this;
        }
        DataType operator*() { return (this->currentNode->data); }
    };

    iterator begin() {
        iterator it = iterator(this);
        while (it.currentNode && it.currentNode->left) {
            it.currentNode = it.currentNode->left;
        }
        return it;
    }

    iterator end() {
        iterator it = iterator(this);
        it.currentNode = NULL;
        return it;
    }

    bool exists(const KeyType& key) {
        return find(key);
    }

    DataType find(const KeyType& key) {
        avlNode* currentNode = head;
        while (currentNode) {
            if (CompareFunction()(key, currentNode->key)) {
                currentNode = currentNode->left;
            } else if (CompareFunction()(currentNode->key, key)) {
                currentNode = currentNode->right;
            } else {
                return (currentNode->data);
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
            head = new avlNode(data, key);
            if(head==NULL){
                return false;
            }
            size++;
            return true;
        }

        bool fromLeft = true;
        avlNode* currentNode = head;
        avlNode* previous = currentNode->parent;
        while (currentNode) {
            previous = currentNode;
            if (CompareFunction()(key, currentNode->key)) {
                fromLeft = true;
            } else {
                fromLeft = false;
            }
            currentNode = fromLeft ? currentNode->left : currentNode->right;
        }

        avlNode* new_node = new avlNode(data, key, previous);
        if(new_node==NULL){
            return false;
        }
        if (fromLeft) {
            previous->left = new_node;
        } else {
            previous->right = new_node;
        }

        updatePath(new_node);
        rollNode(new_node);
        updateHead();
        size++;
        return true;
    }

    bool remove(const KeyType& key) {
        if (!this->exists(key)) return false;

        avlNode* currentNode = head;
        while (CompareFunction()(key, currentNode->key) || CompareFunction()(currentNode->key, key)) {
            currentNode = CompareFunction()(key, currentNode->key) ? currentNode->left : currentNode->right;
        }

        avlNode* parent = currentNode->parent;
        if (!currentNode->left && !currentNode->right) {
            removeLeaf(currentNode);
        } else if (!currentNode->right || !currentNode->left) {
            removeSingle(currentNode);
        } else {
            removeDouble(currentNode);
        }

        updatePath(parent);
        rollNode(parent);
        updateHead();
        size--;
        return true;
    }

    void removeLeaf(avlNode* node) {
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

    void removeSingle(avlNode* node) {
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

    void removeDouble(avlNode* node) {
        avlNode* original = node;
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

    void updatePath(avlNode* node) {
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

    void rollLeftLeft(avlNode* b) {
        avlNode* a = b->left;
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

    void rollRightRight(avlNode* a) {
        avlNode* b = a->right;
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

    void rollLeftRight(avlNode* c) {
        avlNode* b = c->left;
        rollRightRight(b);
        rollLeftLeft(c);
    }

    void rollRightLeft(avlNode* c) {
        avlNode* b = c->right;
        rollLeftLeft(b);
        rollRightRight(c);
    }


    void rollNode(avlNode* node) {
        avlNode* currentNode = node;
        while (currentNode) {
            int currentFactor = currentNode->factor;
            if (currentFactor >= -1 && currentFactor <= 1) {
                currentNode = currentNode->parent;
                continue;
            }

            int left_factor = currentNode->left ? currentNode->left->factor : -1;
            int right_factor = currentNode->right ? currentNode->right->factor : -1;

            if (currentFactor == 2 && left_factor >= 0) {
                rollLeftLeft(currentNode);
            } else if (currentFactor == 2 && left_factor == -1) {
                rollLeftRight(currentNode);
            } else if (currentFactor == -2 && right_factor <= 0) {
                rollRightRight(currentNode);
            } else if (currentFactor == -2 && right_factor == 1) {
                rollRightLeft(currentNode);
            }

            currentNode = currentNode->parent;
        }
    }





    DataType getMax() {
        avlNode* currentNode = this->head;
        if (!currentNode) return NULL;
        while(currentNode->right) {
            currentNode = currentNode->right;
        }
        return (currentNode->data);
    }
    int getSize() { return size; }

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
            it.currentNode->data = data[i];
            it.currentNode->key = keys[i];
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

        tree->head = new avlNode(height);
        tree->head->generateTree(height-1);
        tree->size = size;

        int extraNodesNum = nodesFull - size;
        tree->head->cleanExtraNodes(extraNodesNum);

        return tree;
    }
};

#endif

