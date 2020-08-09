#ifndef DOCPARSER_H
#define DOCPARSER_H
#include "doc.h"
#include "indexinterface.h"
#include "avltreeindex.h"
#include "word.h"
#include <vector>
#include <string>
#include <myhtml/api.h>
#include "json.hpp"
#include <set>
#include "porter2_stemmer.h"
#include <locale>
using nlohmann::json;
using namespace std;

class Docparser
{
private:
    //Containers and primitives
    set<string> stopWords;
    bool ignoreDoc;

public:
    Docparser();

    void parse(string folderPath, IndexInterface*& ii, bool onTheSpot);
    void htmlConvert(string s, Doc& tempDoc, unsigned id, IndexInterface*& ii);
    void htmlConvert(string s);
    string stem(string s);
    void stemStops(); //Only call this function if you want to stem a new stop-words file.
    void createStopsSet();

    //readFromPIndex (into AVL Tree)
    void readFromPIndex(IndexInterface*& ii);
    //writeToPIndex (from AVL Tree)
    void writeToPIndex(IndexInterface*& ii);
};
#endif // DOCPARSER_H
