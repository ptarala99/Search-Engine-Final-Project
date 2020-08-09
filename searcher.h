#ifndef SEARCHER_H
#define SEARCHER_H
#include <iostream>
#include <sstream>
#include "indexinterface.h"
#include "avltreeindex.h"
#include "docparser.h"
using namespace std;

class Searcher
{
private:
    //Index Interface (abbreviated as "ii")
    IndexInterface* ii;

    //Number to choose AVLTree or HashTable
    unsigned structureType;

public:
    Searcher();
    ~Searcher();
    void run();

    //User Interface Functions
    void addOpinions();
    void clearIndex();
    void setStructureType(int option);
    void printStats();
    void printResults();
    void printOpinion();

    //Query Processing
    void readQ();
    void breakIntoWords();
    void stemQueryWords();
};

#endif // SEARCHER_H
