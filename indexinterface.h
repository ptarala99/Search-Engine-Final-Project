#ifndef INDEXINTERFACE_H
#define INDEXINTERFACE_H
#include <iostream>
#include <vector>
#include "word.h"
#include "doc.h"
#include "AVLTree.h"
using namespace std;

class IndexInterface
{
private:
    //Rule of three
    IndexInterface(IndexInterface&) {}
    IndexInterface& operator= (const IndexInterface&) {}

protected:
    //Primitives
    vector<Doc> docs; //Size means the total number of non-ignored documents
    double wordsPerOpinion;

public:
    //C'tor and virtual d'tor
    IndexInterface();
    virtual ~IndexInterface();

    //Pure virtual functions
    virtual void addWord(Word& w, unsigned id) = 0;
    virtual void addWord(Word& w) = 0;
    virtual Word& getWord(string w) = 0;
    virtual set<pair<unsigned, unsigned>>& getDocs(string w) = 0;
    virtual unsigned getNumNodes() = 0;
    virtual AVLTree<Word>& getTree() = 0;
    virtual void rank(const string&, vector<pair<unsigned, double>>&) = 0;
    virtual void rankOr(const string&, vector<pair<unsigned, double>>&) = 0;
    virtual void addRatio(const string&, vector<pair<unsigned, double>>&) = 0;
    virtual void notRemoval(const string&, vector<pair<unsigned, double>>&) = 0;

    //Misc. functions
    vector<Doc>& getDocVector();
};

#endif // INDEXINTERFACE_H
