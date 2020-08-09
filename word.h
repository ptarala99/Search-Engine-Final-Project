#ifndef WORD_H
#define WORD_H
#include <iostream>
#include <vector>
#include <string>
#include <iterator>
#include <set>
using namespace std;

class Word
{
private:
    string w;
    set<pair<unsigned, unsigned>> docs;
    unsigned numTotalOccurs;

public:
    //Rule of three
    Word(){}
    Word(string w);
    Word(string w, unsigned id);
    Word(string w, unsigned numTO, set<pair<unsigned, unsigned>> theDocs);
    Word(const Word&);
    Word& operator= (const Word&);
    ~Word() {}

    //Getter(s) and incrementers
    string getW();
    set<pair<unsigned, unsigned>>& getDocs();
    pair<unsigned, unsigned> getFirstDoc();
    unsigned getNum();
    void incNum();
    void incNumInDoc(unsigned);
    int getNumInDoc(unsigned);
    bool idExists(unsigned);

    //Operator overload(s)
    bool operator< (const Word& rhs) const;
    bool operator== (const Word& rhs) const;
    bool operator!= (const Word& rhs) const;
    friend std::ostream& operator<< (std::ostream&, const Word&);

    //Misc.
    void printDocs();
    string getStrOfPairs();
};

#endif // WORD_H
