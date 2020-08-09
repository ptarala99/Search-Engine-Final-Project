//Owner: Kade

//Citations: None

//History:
//Nov 13, 7:45pm, finished initial class declarations.
//Nov 13, 8:38pm, implemented functions (except I stubbed "visit()")
//Nov 13, 9:13pm, fixed insert bug and implemented the public insert function
//Nov 20, 12:03am, starting from scratch since we went over avl code in class
//Nov 20, 1:02am, wrote the templated header code
//Nov 20, 4:42pm, changed some "int"s to "T"s, added private twin functions
//Nov 20, 5:53pm, all implemented except private twins and rotate functions
//Nov 20, 7:47pm, imp'd all rotate functions (I hope rotateWithRight works!)
//Nov 24, 5:20pm, removed inclusion of word.h, since IndexInterface includes it
                //and this is a basic data structure
//Nov 24, 10:50pm, made insert() return bool regarding presence of duplicate
//Nov 25, 5:13am, created and stubbed search()
//Nov 25, 6:05am, finished search()
//Nov 25, 6:22am, removed some consts from search() functions
//Nov 30, 12:53am, BUG FIX: added assignment of isDup to recursive calls of insert()

#ifndef AVLTREE_H
#define AVLTREE_H
#include <iostream>
#include <vector>
#include "word.h"
using namespace std;

template <class T>
class AVLTree{
private:
    class AVLNode{
        template <class U> friend class AVLTree;
    public:
        T element;
        AVLNode *left;
        AVLNode *right;
        int height;

        AVLNode(const T& theElement, AVLNode *lt, AVLNode *rt, int h = 0)
            : element(theElement), left(lt), right(rt), height(h) {}
    };

    AVLNode *root;
    unsigned numNodes;
    AVLTree& operator= (const AVLTree<T>&); //never called

public:
    //C'tors and destructor
    AVLTree() : root(nullptr), numNodes(0) {}
    AVLTree(const AVLTree& rhs) : root(nullptr), numNodes(rhs.numNodes) { *this = rhs; }
    ~AVLTree() { makeEmpty(); }

    //Functions for user (no node argument)
    const T& findMin() const;
    const T& findMax() const;
    T& search(const T& x) { return search(x, root); }
    bool contains(const T& x) const;
    void makeEmpty() { makeEmpty(root); }
    bool insert(const T& x) { return insert(x, root); }
    void inOrder() { inOrder(root); }
    void getAllVals(vector<T>& v) { getAllVals(root, v); }
    bool isEmpty() const;
    unsigned getNumNodes();

private:
    //Node passed in from its public twin
    const T& findMin(AVLNode*& t) const;
    const T& findMax(AVLNode*& t) const;
    T& search(const T& x, AVLNode*& t);
    bool contains(const T& x, AVLNode*& t) const;
    void makeEmpty(AVLNode*& t);
    bool insert(const T& x, AVLNode*& t);
    void inOrder(AVLNode*& t);
    void getAllVals(AVLNode*& t, vector<T>& v);

    //Internal functions
    void visit(AVLNode*& t);
    T returnVal(AVLNode*& t);
    void rotateWithLeftChild(AVLNode*& k2);
    void rotateWithRightChild(AVLNode*& k1);
    void doubleWithLeftChild(AVLNode*& k3);
    void doubleWithRightChild(AVLNode*& k1);
    int height(AVLNode *t) const;
    int max(int lhs, int rhs) const;

    //Added function(s)
};

//BEGIN OF USER FUNCTIONS (NO NODE ARG)
template <typename T>
const T& AVLTree<T>::findMin() const{
    if(isEmpty())
        std::exit(-1);
    return findMin(root)->element;
}

template <typename T>
const T& AVLTree<T>::findMax() const{
    if(isEmpty())
        std::exit(-2);
    return findMax(root)->element;
}

template <typename T>
bool AVLTree<T>::contains(const T& x) const{
    return contains(x, root);
}

template <typename T>
bool AVLTree<T>::isEmpty() const{
    return root == nullptr;
}

template <typename T>
unsigned AVLTree<T>::getNumNodes(){
    return this->numNodes;
}


//BEGIN OF PRIVATE TWIN FUNCTIONS (WITH NODE ARG)
template <typename T>
const T& AVLTree<T>::findMin(AVLNode*& t) const{

}

template <typename T>
const T& AVLTree<T>::findMax(AVLNode*& t) const{

}

template <typename T>
T& AVLTree<T>::search(const T& x, AVLNode*& t){
    if(t == nullptr)
        exit(-3); //There must be a duplicate in the tree
    if(x == t->element)
        return t->element;
    else if(x < t->element){
        return search(x, t->left);
    }else if(t->element < x){
        return search(x, t->right);
    }else{
        //cout << "Query term not found in any document: " << x << endl;
        exit(-4); //Potential failures:
                  //Query term not found in any document
                  //Word class "<" and/or "==" op overload
    }
}

template <typename T>
bool AVLTree<T>::contains(const T& x, AVLNode*& t) const{

}

template <typename T>
void AVLTree<T>::makeEmpty(AVLNode*& t){
    //IMPORTANT TO-DO TO AVOID MEMORY LEAKS:
    //Make tree empty:
    //do post-order traversal and delete nodes, while decrementing numNodes
}

template <typename T>
bool AVLTree<T>::insert(const T& x, AVLNode*& t){
    //Instantiating bool
    bool isDup = false;

    //Actually inserting
    if(t == nullptr){
        t = new AVLNode(x, nullptr, nullptr);
        numNodes++;
    }
    else if(x < t->element){
        isDup = insert(x, t->left);
        if(height(t->left) - height(t->right) == 2){
            if(x < t->left->element)
                rotateWithLeftChild(t); //case 1
            else
                doubleWithLeftChild(t); //case 2
        }
    }else if(t->element < x){
        isDup = insert(x, t->right);
        if(height(t->right) - height(t->left) == 2){
            if(t->right->element < x)
                rotateWithRightChild(t); //case 4
            else
                doubleWithRightChild(t); //case 3
        }
    }else
        isDup = true; //Handling duplicates

    //Updating height
    t->height = max(height(t->left), height(t->right)) + 1;

    //Returning bool
    return isDup;
}

template <typename T>
void AVLTree<T>::inOrder(AVLNode*& t){
    if(t != nullptr){
        inOrder(t->left);
        visit(t);
        inOrder(t->right);
    }
}

template <typename T>
void AVLTree<T>::getAllVals(AVLNode*& t, vector<T>& v){
    if(t != nullptr){
        getAllVals(t->left, v);
        v.push_back(returnVal(t));
        getAllVals(t->right, v);
    }
}

template <typename T>
void AVLTree<T>::visit(AVLNode*& t){
    cout << t->element << endl;
}

template <typename T>
T AVLTree<T>::returnVal(AVLNode*& t){
    return t->element;
}

//BEGIN OF INTERNAL FUNCTIONS
template <typename T>
void AVLTree<T>::rotateWithLeftChild(AVLNode*& k2){
    AVLNode* k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), k2->height) + 1;
    k2 = k1; //changing root ptr of tree or subtree to the new "top"
}

template <typename T>
void AVLTree<T>::rotateWithRightChild(AVLNode*& k1){
    AVLNode* k2 = k1->right;
    k1->right = k2->left;
    k2->left = k1;
    k1->height = max(height(k1->right), height(k1->left)) + 1;
    k2->height = max(height(k2->right), k1->height) + 1;
    k1 = k2; //changing root ptr of tree or subtree to the new "top"
}

template <typename T>
void AVLTree<T>::doubleWithLeftChild(AVLNode*& k3){
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3); //case 1
}

template <typename T>
void AVLTree<T>::doubleWithRightChild(AVLNode*& k1){
    rotateWithLeftChild(k1->right);
    rotateWithRightChild(k1); //case 4
}

template <typename T>
int AVLTree<T>::height(AVLNode *t) const{
    return t == nullptr ? -1 : t->height;
}

template <typename T>
int AVLTree<T>::max(int lhs, int rhs) const{
    return lhs > rhs ? lhs : rhs;
}

//ADDED FUNCTION(S)

#endif // AVLTREE_H
