#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    char getBalance () const;
    void setBalance (char balance);
    void updateBalance(char diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    char balance_;
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
char AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(char balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(char diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    /* HELPER FUNCTIONS:
     *  rotate left / right
     *  zig or zag?
     *  zig zig / zig zag
     * isLeftChild / isRightChild (OVERRIDE)
     * insertFix /removeFix
     */
    int static zigorzag(AVLNode<Key, Value>* n); //1 = zig-zig | 2 = zig-zag | 0 = fail
    void rotateRight(AVLNode<Key, Value>* g);
    void rotateLeft(AVLNode<Key, Value>* g);
    void insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void removeFix(AVLNode<Key, Value>* n, char diff);

};

/************************* INSERT *************************
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    if(this->root_ == NULL) { //empty tree
        AVLNode<Key, Value> *temp = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        temp->setBalance(0);
        this->root_ = temp;
        return;
    }
    AVLNode<Key, Value> *temp = static_cast<AVLNode<Key, Value>*>(this->internalFind(new_item.first));
    if(temp != NULL) { //key exist. updating value
        temp->setValue(new_item.second);
        return;
    }
    //otherwise insert node elsewhere
    AVLNode<Key, Value> *curr = static_cast<AVLNode<Key, Value>*>(this->root_);
    Key k = new_item.first;
    while(curr != NULL) {
        if(k < curr->getKey()) { //go left
            if(curr->getLeft() == NULL) { //stop at node b4 NULL
                //create new node
                AVLNode<Key, Value> *temp = new AVLNode<Key, Value>(k, new_item.second, curr);
                temp->setBalance(0);
                curr->setLeft(temp);

                //checking parent balance
                if(curr->getBalance() == -1) { curr->setBalance(0); } //don't think this is necessary
                else if(curr->getBalance() == 1) { curr->setBalance(0); }
                else { //should be 0
                    curr->updateBalance(-1); //because added left child
                    insertFix(curr, temp);
                }
                break; 
            } 
            else { curr = curr->getLeft(); } //move ptr left
        }
        else if(k > curr->getKey()) { //go right
            if(curr->getRight() == NULL) { //stop at node b4 NULL
                //create new node
                AVLNode<Key, Value> *temp = new AVLNode<Key, Value>(k, new_item.second, curr);
                temp->setBalance(0);
                curr->setRight(temp);

                //checking parent balance
                if(curr->getBalance() == -1) { curr->setBalance(0); } 
                else if(curr->getBalance() == 1) { curr->setBalance(0); } //don't think this is necessary
                else { //should be 0
                    curr->updateBalance(1); //because added right child
                    insertFix(curr, temp);
                }
                break; 
            } 
            else { curr = curr->getRight(); } //move ptr right
        }
    }
}

template<class Key, class Value>
void AVLTree<Key,Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n) {
    if(p == NULL) { return; } //parent null
    AVLNode<Key, Value>* g = p->getParent(); //grandparent
    if(g == NULL) { return; } //grandparent null

    //if left child
    if(BinarySearchTree<Key, Value>::isLeftChild(p)) { //parent is left child
        g->updateBalance(-1);
        if(g->getBalance() == 0) { return; } //case 1
        else if(g->getBalance() == -1) { insertFix(g, p); } //case 2
        else if(g->getBalance() == -2) { //case 3
            if(zigorzag(n) == 1) { //zig zig
                this->rotateRight(g); 
                p->setBalance(0);
                g->setBalance(0);
            } else if(zigorzag(n) == 2) { //zig zag
                this->rotateLeft(p);
                this->rotateRight(g);
                if(n->getBalance() == -1) { //case 3a
                    p->setBalance(0);
                    g->setBalance(1);
                    n->setBalance(0);
                } else if(n->getBalance() == 0) { //case 3b
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                } else if(n->getBalance() == 1) { //case 3c
                    p->setBalance(-1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
        }
    }

    //if right child
    else {
        g->updateBalance(1);
        if(g->getBalance() == 0) { return; } //case 1
        else if(g->getBalance() == 1) { insertFix(g, p); } //case 2
        else if(g->getBalance() == 2) { //case 3
            if(zigorzag(n) == 1) { //zig zig
                this->rotateLeft(g); 
                p->setBalance(0);
                g->setBalance(0);
            } else if(zigorzag(n) == 2) { //zig zag
                this->rotateRight(p);
                this->rotateLeft(g);
                if(n->getBalance() == 1) { //case 3a
                    p->setBalance(0);
                    g->setBalance(-1);
                    n->setBalance(0);
                } else if(n->getBalance() == 0) { //case 3b
                    p->setBalance(0);
                    g->setBalance(0);
                    n->setBalance(0);
                } else if(n->getBalance() == -1) { //case 3c
                    p->setBalance(1);
                    g->setBalance(0);
                    n->setBalance(0);
                }
            }
        }
    }
}


/************************* REMOVE *************************
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */

template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    AVLNode<Key, Value> *n = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
    if(n == NULL) { return; } //key no exist

    if(n->getLeft() != NULL && n->getRight() != NULL) { //2 children
        nodeSwap(n, static_cast<AVLNode<Key,Value>*>(this->predecessor(n)));
    }

    AVLNode<Key, Value> *p = n->getParent();
    char diff = 0;
    if(p != NULL) { 
        if(this->isLeftChild(n)) { diff = 1; }
        if(this->isRightChild(n)) { diff = -1; }
    }

    //deleting n and updating pointers
    if(n->getLeft() != NULL && n->getRight() == NULL) { //1 left child
        if(p == NULL) {
            this->root_ = n->getLeft();
            n->setParent(NULL);
        } else {
            if(this->isLeftChild(n)) { p->setLeft(n->getLeft()); }
            else if(this->isRightChild(n)) { p->setRight(n->getLeft()); }
        }
        n->getLeft()->setParent(p);
        
    }
    else if(n->getLeft() == NULL && n->getRight() != NULL) { //1 right child
        if(p == NULL) {
            this->root_ = n->getRight();
            n->setParent(NULL);
        } else {
            if(this->isLeftChild(n)) { p->setLeft(n->getRight()); }
            else if(this->isRightChild(n)) { p->setRight(n->getRight()); }
        }
        n->getRight()->setParent(p);

    } else { //0 children
        if(p == NULL) { this->root_ = NULL; }
        else if(this->isLeftChild(n)) { p->setLeft(NULL); }
        else{ p->setRight(NULL); }
    }
    delete n;

    removeFix(p, diff);
}

template<class Key, class Value>
void AVLTree<Key,Value>::removeFix(AVLNode<Key, Value>* n, char diff) {
    if(n == NULL) { return; }
    AVLNode<Key, Value>* p = n->getParent();
    //calculating next recursion's diff
    char ndiff = 0;
    if(p != NULL) {
        if(this->isLeftChild(n)) { ndiff = 1; }
        else { ndiff = -1; }
    }
    
    //----------------- diff=-1 -----------------//
    AVLNode<Key, Value>* c;
    if(diff == -1) {
        if(n->getBalance() + diff == -2) { //case 1
            c = n->getLeft(); //taller of the children
            if(c->getBalance() == -1) { //1a: zig zig
                rotateRight(n);
                n->setBalance(0); c->setBalance(0);
                removeFix(p, ndiff);
            } else if(c->getBalance() == 0) { //1b: zig zig
                rotateRight(n);
                n->setBalance(-1); c->setBalance(1);
                return;
            } else if(c->getBalance() == 1) { //1c: zig zag
                AVLNode<Key, Value>* gc = c->getRight();
                rotateLeft(c); rotateRight(n);
                if(gc->getBalance() == 1) { n->setBalance(0); c->setBalance(-1); gc->setBalance(0); }
                else if(gc->getBalance() == 0) { n->setBalance(0); c->setBalance(0); gc->setBalance(0); }
                else { n->setBalance(1); c->setBalance(0); gc->setBalance(0); }
                removeFix(p, ndiff);
            }
        }
        else if(n->getBalance() + diff == -1) { n->setBalance(-1); return; } //case 2
        else if(n->getBalance() + diff == 0) { //case 3
            n->setBalance(0);
            removeFix(p, ndiff); 
        }
    }
    
    //----------------- diff=1 -----------------//
    if(diff == 1) {
        if(n->getBalance() + diff == 2) { //case 1
            c = n->getRight(); //taller of the children
            if(c->getBalance() == 1) { //1a: zig zig
                rotateLeft(n);
                n->setBalance(0); c->setBalance(0);
                removeFix(p, ndiff);
            } else if(c->getBalance() == 0) { //1b: zig zig
                rotateLeft(n);
                n->setBalance(1); c->setBalance(-1);
                return;
            } else if(c->getBalance() == -1) { //1c: zig zag
                AVLNode<Key, Value>* gc = c->getLeft();
                rotateRight(c); rotateLeft(n);
                if(gc->getBalance() == -1) { n->setBalance(0); c->setBalance(1); gc->setBalance(0); }
                else if(gc->getBalance() == 0) { n->setBalance(0); c->setBalance(0); gc->setBalance(0); }
                else { n->setBalance(-1); c->setBalance(0); gc->setBalance(0); }
                removeFix(p, ndiff);
            }
        }
        else if(n->getBalance() + diff == 1) { n->setBalance(1); return; } //case 2
        else if(n->getBalance() + diff == 0) { //case 3
            n->setBalance(0);
            removeFix(p, ndiff);
        }
    }
}

// ---------------------------------------------- //
template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    char tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}
// ---------------------------------------------- //
template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key, Value>* g) {
    AVLNode<Key, Value>* p = g->getLeft();
    if(p == NULL || p->getParent() == NULL) { return; }

    //rotating
    AVLNode<Key, Value>* temp = p->getRight(); //getting irrelevent children
    p->setRight(g);
    p->setParent(g->getParent());
    if(g->getParent() == NULL){ this->root_ = p; } //potentially update root_
    else { 
        if(this->isLeftChild(g)) { g->getParent()->setLeft(p); }
        else { g->getParent()->setRight(p); }
    }
    g->setParent(p);
    if(temp != NULL) { temp->setParent(g); }
    g->setLeft(temp);
}


template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key, Value>* g) {
    AVLNode<Key, Value>* p = g->getRight();
    if(p == NULL || p->getParent() == NULL) { return; }

    //rotating
    AVLNode<Key, Value>* temp = p->getLeft(); //getting irrelevent children
    p->setLeft(g);
    p->setParent(g->getParent());
    if(g->getParent() == NULL){ this->root_ = p; } //potentially update root_
    else { 
        if(this->isLeftChild(g)) { g->getParent()->setLeft(p); }
        else { g->getParent()->setRight(p); }
    }
    g->setParent(p);
    if(temp != NULL) { temp->setParent(g); }
    g->setRight(temp);
}

template<class Key, class Value>
int AVLTree<Key, Value>::zigorzag(AVLNode<Key, Value>* n) {
    AVLNode<Key, Value>* p = n->getParent();
    if(p == NULL || p->getParent() == NULL) { return 0; } //just in case
    if(BinarySearchTree<Key, Value>::isLeftChild(n) && BinarySearchTree<Key, Value>::isLeftChild(p)) { return 1; } //left left
    if(BinarySearchTree<Key, Value>::isRightChild(n) && BinarySearchTree<Key, Value>::isRightChild(p)) { return 1; } //right right
    if(BinarySearchTree<Key, Value>::isLeftChild(n) && BinarySearchTree<Key, Value>::isRightChild(p)) { return 2; } //left right
    if(BinarySearchTree<Key, Value>::isRightChild(n) && BinarySearchTree<Key, Value>::isLeftChild(p)) { return 2; } //right left
    return 0;
}


#endif
