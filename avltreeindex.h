#ifndef AVLTREEINDEX_H
#define AVLTREEINDEX_H
#include <iostream>
#include "indexinterface.h"
#include "AVLTree.h"
using namespace std;

class AVLTreeIndex : public IndexInterface
{
private:
    AVLTree<Word> words;

    //Tried putting copy c'tor and op= overload here, but
    //error said "Must explicitly instantiate IndexInterface,
    //which has no default constructor".

public:
    //C'tor and d'tor
    AVLTreeIndex();
    ~AVLTreeIndex() {}

    //Function overrides
    void addWord(Word& w, unsigned id);
    void addWord(Word& w);
    Word& getWord(string w);
    set<pair<unsigned, unsigned>>& getDocs(string w);
    unsigned getNumNodes();
    AVLTree<Word>& getTree();
    void rank(const string&, vector<pair<unsigned, double>>&);
    void rankOr(const string&, vector<pair<unsigned, double>>&);
    void addRatio(const string&, vector<pair<unsigned, double>>&);
    void notRemoval(const string&, vector<pair<unsigned, double>>&);
};

#endif // AVLTREEINDEX_H
