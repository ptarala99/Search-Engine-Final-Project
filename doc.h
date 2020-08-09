#ifndef DOC_H
#define DOC_H
#include <vector>
#include <string>
#include <set>
using namespace std;


class Doc
{
private:
   unsigned id;
   unsigned year;
   unsigned numTotalWords;
   string parties;
   string majorJustice;

public:
    Doc();
    void clear();
    unsigned getID();
    void setID(unsigned myID);
    unsigned getYear();
    void setYear(unsigned myYear);
    unsigned getTotalWords();
    void incTotalWords(unsigned myTotalWords);
    string getParties();
    void setParties(string myparties);
    string getMajorJustice();
    void setMajorJustice(string myMajorJustice);
};

#endif // DOC_H
