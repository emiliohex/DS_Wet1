#ifndef AVL_TREE_H_
#define AVL_TREE_H_

#include <stddef.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>

using namespace std;

template<typename KeyType, typename DataType, class CompareFunction>
class AvlTree {

private:

    class AvlTreeNode {
        KeyType key;
        DataType data;
        AvlTreeNode* left;
        AvlTreeNode* right;
        AvlTreeNode* parent;

        int heightLeft;
        int heightRight;

    public:
        AvlTreeNode(KeyType key, DataType data) {
            this->key = key;
            this->data = data;
            this->left = NULL;
            this->right = NULL;
            this->parent = NULL;

            this->heightLeft = 0;
            this->heightRight = 0;
        }

        void deepDelete() {
            if (this->left != NULL) {
                this->left->deepDelete();
            }

            if (this->right != NULL) {
                this->right->deepDelete();
            }

            delete (this);
        }

        int getNumOfChildern() const {
            int num = 0;
            if (this->left != NULL) {
                num++;
            }
            if (this->right != NULL) {
                num++;
            }

            return num;
        }

        void setData(DataType newData) {
            this->data = newData;
        }

        void setKey(KeyType newKey) {
            this->key = newKey;
        }

        DataType getData() const {
            return data;
        }

        KeyType getKey() const {
            return key;
        }

        void setThisAsParent(AvlTreeNode* child) {
            if (child != NULL) {
                child->setParent(this);
            }
        }

        AvlTreeNode* getLeft() const {
            return this->left;
        }

        void setLeft(AvlTreeNode* left) {
            this->left = left;
            setThisAsParent(left);
        }

        AvlTreeNode* getRight() const {
            return this->right;
        }

        void setRight(AvlTreeNode* right) {
            this->right = right;
            setThisAsParent(right);
        }

        void setParent(AvlTreeNode* parent) {
            this->parent = parent;
        }

        AvlTreeNode* getParent() const {
            return this->parent;
        }

        AvlTreeNode* getChild() const {
            if (this->left == NULL) {
                return this->right;
            }
            return this->left;
        }

        void replaceChild(AvlTreeNode* oldChild, AvlTreeNode* newChild) {
            if (this->left == oldChild) {
                setLeft(newChild);
            } else if (this->right == oldChild) {
                setRight(newChild);
            }
        }

        void removeChild(AvlTreeNode* child) {
            if (this->left == child) {
                this->left = NULL;
            } else if (this->right == child) {
                this->right = NULL;
            }
        }

        // This method will find the closest bigger node to this node
        AvlTreeNode* findNextBigger() const {
            AvlTreeNode* nextBigger = getRight(); // we go once right (bigger)
            AvlTreeNode* lastBigger = nextBigger;
            while (nextBigger != NULL) { // and all the way left (smallest bigger)
                lastBigger = nextBigger;
                nextBigger = nextBigger->getLeft();
            }

            return lastBigger;
        }

        int getHeight() {
            return ((
                            this->heightLeft > this->heightRight ?
                            this->heightLeft : this->heightRight) + 1);
        }

        int getHeightLeft() {
            return this->heightLeft;
        }

        int getHeightRight() {
            return this->heightRight;
        }

        void setHeightLeft(int heightLeft) {
            this->heightLeft = heightLeft;
        }

        void setHeightRight(int heightRight) {
            this->heightRight = heightRight;
        }

        int getBalanceFactor() const {
            return (this->heightLeft) - (this->heightRight);
        }

        int getHeightOnThisSide(AvlTreeNode* child) {
            CompareFunction comparer;
            if (comparer(this->left, child) == 0) {
                return this->heightLeft;
            } else if (comparer(this->right, child) == 0) {
                return this->heightRight;
            }
            return -1; // if the "child" is not real child
        }

        void addHeightOnThisSide(AvlTreeNode* child) {
            if (this->left == child) {
                setHeightLeft(getHeightLeft() + 1);
            } else if (this->right == child) {
                setHeightRight(getHeightRight() + 1);
            }
        }

        void subtractHeightOnThisSide(AvlTreeNode* child) {
            if (this->left == child) {
                setHeightLeft(getHeightLeft() - 1);
            } else if (this->right == child) {
                setHeightRight(getHeightRight() - 1);
            }
        }
    };

    int nodeCount;
    AvlTreeNode* head; // Pointer to the first node in the tree

    AvlTreeNode* findClosest(KeyType key, AvlTreeNode* head) { // This function will search for the given key, if not exists - will find the parent that should hold it
        // return NULL if this node is empty
        if (head == NULL) {
            return NULL;
        }

        CompareFunction comparer;

        // return this node if we have reached the desired key
        if (comparer(head->getKey(), key) == 0) {
            return head;
        }

        AvlTreeNode* returnedNode;
        if (comparer(head->getKey(), key) > 0) { // go left if this node is bigger then desired
            returnedNode = findClosest(key, head->getLeft());
        } else { 				// go right if this node is smaller then desired
            returnedNode = findClosest(key, head->getRight());
        }

        // if the returnedNode is empty, then this is the closest node
        if (returnedNode == NULL) {
            return head;
        } else {
            return returnedNode;
        }
    }

    void fixHeightsInsertion(AvlTreeNode* node) {
        // Go over all the tree from bottom up to increment the balance factors
        AvlTreeNode* parent = node->getParent();
        while (parent != NULL) {

            int heightBeforeInsertion = parent->getHeight();

            parent->addHeightOnThisSide(node);
            if (abs(parent->getBalanceFactor()) == 2) {
                roll(parent);

                // After rolling once, the tree heights are valid
                return;
            }

            // if the height hasn't changed, then no need to keep climbing up.
            if (heightBeforeInsertion == parent->getHeight()) {
                return;
            }

            node = parent;
            parent = parent->getParent();
        }
    }

    void fixHeightsDeletion(AvlTreeNode* node) {
        AvlTreeNode* parent = node->getParent();
        while (parent != NULL) {
            int heightBeforeDeletion = parent->getHeight();

            parent->subtractHeightOnThisSide(node);
            if (abs(parent->getBalanceFactor()) == 2) {
                roll(parent);

                // because we rolled, we need to skip the next node
                node = parent;
                parent = parent->getParent();
            }

            // if the height hasn't changed, then no need to keep climbing up
            if (heightBeforeDeletion == parent->getHeight()) {
                return;
            }

            node = parent;
            parent = parent->getParent();
        }
    }

    void roll(AvlTreeNode* node) {
        if (node->getBalanceFactor() == 2) {
            AvlTreeNode* leftChild = node->getLeft();
            int leftChildBF = leftChild->getBalanceFactor();

            if (leftChild == NULL) {
                return;
            }

            if (leftChildBF >= 0) {
                // ROLL LL - is a roll to the right (according to ppt)
                rollRight(node);
            } else if (leftChildBF == -1) {
                // ROLL LR
                rollLeft(leftChild);
                rollRight(node);
            }
        } else if (node->getBalanceFactor() == -2) {
            AvlTreeNode* rightChild = node->getRight();

            if (rightChild == NULL) {
                return;
            }
            int rightChildBF = rightChild->getBalanceFactor();

            if (rightChildBF <= 0) {
                // ROLL RR - is a roll to the left (according to ppt)
                rollLeft(node);
            }

            else if (rightChildBF == 1) {
                //ROLL RL
                rollRight(rightChild);
                rollLeft(node);
            }
        }
    }

    void rollRight(AvlTreeNode* node) {
        AvlTreeNode* leftNode = node->getLeft();
        node->setLeft(leftNode->getRight());

        AvlTreeNode* parentNode = node->getParent();

        leftNode->setRight(node);

        if (parentNode != NULL) {
            parentNode->replaceChild(node, leftNode);
        } else { // if parent is NULL then this is the head node
            head = leftNode;
            leftNode->setParent(NULL);
        }

        node->setHeightLeft(leftNode->getHeightRight());
        leftNode->setHeightRight(node->getHeight());
    }

    void rollLeft(AvlTreeNode* node) {
        AvlTreeNode* rightNode = node->getRight();
        node->setRight(rightNode->getLeft());

        AvlTreeNode* parentNode = node->getParent();

        rightNode->setLeft(node);

        if (parentNode != NULL) {
            parentNode->replaceChild(node, rightNode);
        } else { // if parent is NULL then this is the head node
            head = rightNode;
            rightNode->setParent(NULL);
        }

        node->setHeightRight(rightNode->getHeightLeft());
        rightNode->setHeightLeft(node->getHeight());
    }

    // This function will call "findClosest" and then validate that it is indeed the exact node we were looking for
    AvlTreeNode* findNode(KeyType key) {
        AvlTreeNode* closest = findClosest(key, head);
        CompareFunction comparer;
        if (closest != NULL && comparer(closest->getKey(), key) == 0) {
            return closest;
        }
        return NULL;
    }

    // Delete this node
    void Delete(AvlTreeNode* node) {
        int numOfChilds = node->getNumOfChildern();
        AvlTreeNode* parentNode = node->getParent();

        if (numOfChilds == 0) { // This node is a leaf, just remove it.
            fixHeightsDeletion(node); // fix heights from this node and up
            if (parentNode != NULL) {
                parentNode->removeChild(node);
            } else {
                head = NULL;
            }
        } else if (numOfChilds == 1) {
            fixHeightsDeletion(node); // fix heights from this node and up
            AvlTreeNode* childNode = node->getChild();
            if (parentNode != NULL) {
                parentNode->replaceChild(node, childNode);
            } else {
                head = childNode;
                childNode->setParent(NULL);
            }
        } else if (numOfChilds == 2) {
            AvlTreeNode* nextBigger = node->findNextBigger();
            AvlTreeNode* nextBiggerParent = nextBigger->getParent();
            AvlTreeNode* nextBiggerChild = nextBigger->getChild();
            fixHeightsDeletion(nextBigger); // fix heights from the next bigger and up
            nextBiggerParent->replaceChild(nextBigger, nextBiggerChild); // This will remove the nextBigger node from where it was

            // assign the left and right nodes from the deleted one to the nextBigger
            nextBigger->setLeft(node->getLeft());
            nextBigger->setRight(node->getRight());
            nextBigger->setHeightLeft(node->getHeightLeft());
            nextBigger->setHeightRight(node->getHeightRight());

            // refresh the parent node, maybe it's changed because of the rolls
            parentNode = node->getParent();
            if (parentNode != NULL) {
                parentNode->replaceChild(node, nextBigger); // put the nextBigger instead of the node
            } else {
                head = nextBigger;
                nextBigger->setParent(NULL);
            }
        }

        delete (node);
    }
    void auxTreeArrayByData(DataType** dataArray, int* index,AvlTreeNode* cNode) {
        if (cNode == NULL) {
            return;
        }
        auxTreeArrayByData(dataArray, index, cNode->getRight());
        (*dataArray)[*index] = cNode->getData();
        *index = *index + 1;
        auxTreeArrayByData(dataArray, index, cNode->getLeft());
    }

public:

    class Iterator {
    private:

    public:
        AvlTreeNode* currentNode;
        AvlTreeNode* lastNode;

        Iterator(AvlTreeNode* node) {
            currentNode = node;
            lastNode = node;
        }
        Iterator(const Iterator& sourceIterator) {
            this->currentNode = sourceIterator.currentNode;
            this->lastNode = sourceIterator.lastNode;
        }

        DataType operator*() {
            if (currentNode == NULL) {
                return NULL;
            }
            return currentNode->getData();
        }

        // This ++ operator will point at NULL when reaching the end,
        // should test for currentNode==NULL after every increment
        Iterator operator++(int) {


            if(currentNode->getLeft() != NULL){
                // go left and most right you can
                lastNode = currentNode;
                currentNode = currentNode->getLeft();
                while(currentNode->getRight() != NULL) {
                    lastNode = currentNode;
                    currentNode = currentNode->getRight();
                }
            }
            else {
                while(currentNode != NULL && currentNode->getParent() != NULL &&
                      currentNode == currentNode->getParent()->getLeft()) {
                    lastNode = currentNode;
                    currentNode = currentNode->getParent();
                }

                // we already handled this parent, so move another one up
                if(currentNode != NULL) {
                    lastNode = currentNode;
                    currentNode = currentNode->getParent();
                }
            }

            return *this;
        }

        KeyType key() {
            return this->currentNode->getKey();
        }

        DataType value() {
            return this->currentNode->getData();
        }
    };

    AvlTree() {
        head = NULL;
        nodeCount = 0;
    }

    ~AvlTree() {
        if (head != NULL) {
            head->deepDelete();
        }
    }

    Iterator GetIterator() {
        if(head == NULL) {
            return Iterator(NULL);
        }
        AvlTreeNode* node = head;
        while (node->getRight() != NULL) {
            node = node->getRight();
        }
        return Iterator(node);
    }

    DataType FindMax() {
        if(head == NULL) {
            return NULL;
        }

        AvlTreeNode* node = head;
        while (node != NULL && node->getRight() != NULL) {
            node = node->getRight();
        }
        return node->getData();
    }

    DataType FindMin() {
        if(head == NULL) {
            return NULL;
        }

        AvlTreeNode* node = head;
        while (node != NULL && node->getLeft() != NULL) {
            node = node->getLeft();
        }
        return node->getData();
    }

    // This method may seem weird, but it is necessary to delete this key's instance from memory
    KeyType GetKey(KeyType key) {
        AvlTreeNode* node = findNode(key);
        if(node != NULL) {
            return node->getKey();
        }
        return NULL;
    }

    // retrieved the data that this key is associated with
    DataType Find(KeyType key) {
        AvlTreeNode* node = findNode(key);
        if (node != NULL) {
            return node->getData();
        }
        return NULL;
    }

    bool Exists(KeyType key) {
        DataType keyExists = Find(key);
        if (keyExists != NULL) {
            return true;
        }
        return false;
    }

    // insert this data and key
    bool Insert(KeyType key, DataType data) {
        AvlTreeNode* newNode;
        try {
            newNode = new AvlTreeNode(key, data);
        } catch (std::bad_alloc&) {
            return false;
        }
        if (head == NULL) {
            head = newNode;
            nodeCount++;
            return true;
        }

        CompareFunction comparer;
        AvlTreeNode* parent = findClosest(key, head);
        /*if (comparer(parent->getKey(), key) == 0) { // This key already exists in this tree, replacing the data
            //parent->setData(data);
        } else { // This key doensn't exist in the tree
         */if (comparer(parent->getKey(), key) > 0) { //If the parent is bigger then inserted key, is should be assigned on the left
            parent->setLeft(newNode);
        } else { 			// else, it is smaller, then assign to the right
            parent->setRight(newNode);
        }


        fixHeightsInsertion(newNode);
        nodeCount++;

        return true;
    }

    // Delete the key and data that is associated with this key
    // returns true if deleted, false if not found
    bool Delete(KeyType key) {
        AvlTreeNode* node = findNode(key);
        if (node == NULL) {
            return false;
        }

        this->Delete(node);
        nodeCount--;
        return true;
    }

    int getNodesCount() {
        return nodeCount;
    }



    AvlTree(int size){
        head = FullTree((int)log2(size));
        nodeCount = size;
    }
    static AvlTreeNode* FullTree(int depth) {
        if (depth == -1)
            return NULL;
        AvlTreeNode* Root = new AvlTreeNode(0, NULL);
        Root->setHeightLeft(depth - 1);
        Root->setHeightRight(depth - 1);
        Root->setLeft(FullTree(depth - 1));
        if(Root->getLeft()!=NULL){
            Root->getLeft()->setParent(Root);
        }
        Root->setRight(FullTree(depth - 1));
        if(Root->getLeft()!=NULL){
            Root->getRight()->setParent(Root);
        }
        return Root;
    }
    void deleteFullTreeOrder(int num){
        //printf("num is - %d\n",num);
        return deletepOrder(this->head,num);
    }
    //deletes certain number of leafs in a complete tree
     void deletepOrder(AvlTreeNode* node,int num) {
        if (node->getNumOfChildern()==0) {
            return;
        }
        if ( num == 0) {
            return;
        }
        //printf("node->getRight()->getNumOfChildern() is - %d\n",node->getRight()->getNumOfChildern());
        if (node->getRight()->getNumOfChildern()==0 ) {
            Delete(node->getRight());
            (num)--;
            node->setRight(NULL);
        } else {
            deletepOrder(node->getRight(), num);
        }
        //printf("num is - %d\n",num);
        if(num <= 0) {
            return;
        }
        if (node->getLeft()->getNumOfChildern()==0 && num) {
            (num)--;
            Delete(node->getLeft());
            node->setLeft(NULL);
        } else {
            deletepOrder(node->getLeft(), num);
        }
    }
    void InOrderCopyArray(DataType* Array, int len, int index){
        return InOrderCopyArrayAux(this->head,Array,len,&index);
    }
    static void InOrderCopyArrayAux(AvlTreeNode* node, DataType* Array,int len, int *index){
        if (len == *index || node==NULL)
            return;
        //printf("InOrderCopyArrayAux\n");
        InOrderCopyArrayAux(node->getRight(), Array, len, index);
        //printf("OK12 - InOrderCopyArrayAux\n");
        if (len == *index || node == NULL)
            return;
        node->setData(Array[*index]);
        //printf("index-%d,array - %d\n",*index,Array[*index]->getId());
        node->setKey(Array[*index]->getRankAndId());
        (*index)++;

        InOrderCopyArrayAux(node->getLeft(), Array, len, index);
        return;
    }
    DataType* createDataArray() {
        if (nodeCount==0) return NULL;
        DataType* arr = new DataType[nodeCount];
        int i = 0;
        Iterator it = this->GetIterator();
        while(*it!=NULL){
            arr[i++] = (*it);
            //printf("arr[%d] - %d\n",i-1,arr[i-1]->getId());
            it++;
        }
        return arr;
    }

};

#endif
