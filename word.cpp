//Owner: Kade

//Citations: None

//History:
//Nov 23, 3:34pm, Created full implementation of this class (.h and .cpp)
//Nov 25, 1:59am, in getDocs(): now passing docs by reference
//Nov 25, 6:25am, made op== overload

#include "word.h"
#include <iostream>
using namespace std;

//Constructed from query terms
Word::Word(string w){
    //Primitives
    this->w = w;
    numTotalOccurs = 0;
}

//Constructed from opinion docs
Word::Word(string w, unsigned id){
    //Primitives
    this->w = w;
    numTotalOccurs = 0;

    //Pair of sets
    pair<unsigned, unsigned> wordPair;
    wordPair.first = id;
    wordPair.second = 0;
    docs.emplace(wordPair);
}

Word::Word(string w, unsigned numTO, set<pair<unsigned, unsigned>> theDocs){
    this->w = w;
    numTotalOccurs = numTO;
    docs = theDocs;
}

Word::Word(const Word& theWord){
    this->w = theWord.w;
    this->docs = theWord.docs;
    this->numTotalOccurs = theWord.numTotalOccurs;
}

Word& Word::operator= (const Word& rhs){
    this->w = rhs.w;
    this->docs = rhs.docs;
    this->numTotalOccurs = rhs.numTotalOccurs;
    return *this;
}

string Word::getW(){
    return this->w;
}

//Passing by reference so index can add a docId
set<pair<unsigned, unsigned>>& Word::getDocs(){
    return this->docs;
}

//Returns the first docId, useful for handling duplicate words in index
pair<unsigned, unsigned> Word::getFirstDoc(){
    if(this->docs.empty()){
        cout << "Word set is empty when trying to get first doc" << endl;
        pair<unsigned, unsigned> emptyPair;
        emptyPair.first = 0;
        emptyPair.second = 0;
        return emptyPair;
    }
    set<pair<unsigned, unsigned>>::iterator it = this->docs.begin();
    return *it;
}

void Word::incNumInDoc(unsigned id){
    //Iterator
    set<pair<unsigned, unsigned>>::iterator it;

    //Linear search for id
    for(it = this->docs.begin(); it != this->docs.end(); it++){
        if((*it).first == id){
            //Increment num in a copy
            pair<unsigned, unsigned> temp = *it;
            temp.second++;

            //Replace into the docs set
            this->docs.erase(it);
            this->docs.emplace(temp);

            //We found the doc, so exit this function
            return;
        }
    }
    cout << "In incNumInDoc:" << endl;
    cout << "ID " << id << " not found in set... error." << endl;
}

int Word::getNumInDoc(unsigned id){
    //Iterator
    set<pair<unsigned, unsigned>>::iterator it;

    //Linear search for id
    for(it = this->docs.begin(); it != this->docs.end(); it++){
        if((*it).first == id){
            //We found the doc, so exit this function
            return (*it).second;
        }
    }
    return -1; //Doc not found, so send -1 to remove id from vector
}

bool Word::idExists(unsigned id){
    //Iterator
    set<pair<unsigned, unsigned>>::iterator it;

    //Linear search for id
    for(it = this->docs.begin(); it != this->docs.end(); it++){
        if((*it).first == id){
            //We found the doc, so exit this function
            return true;
        }
    }
    return false;
}

unsigned Word::getNum(){
    return this->numTotalOccurs;
}

void Word::incNum(){
    numTotalOccurs++;
}

bool Word::operator< (const Word& rhs) const{
    return this->w < rhs.w ? true : false;
}

bool Word::operator== (const Word& rhs) const{
    return this->w == rhs.w ? true : false;
}

bool Word::operator!= (const Word& rhs) const{
    return !(this->w == rhs.w ? true : false);
}

std::ostream& operator<< (std::ostream& OS, const Word& theWord)
{
    OS << theWord.w;
    return OS;
}

void Word::printDocs(){
    cout << "Word: " << w << endl;
    cout << "Docs: ";
    for(auto& id : this->docs)
        cout << id.first << " ";
    cout << endl;
}

string Word::getStrOfPairs(){
    string s = "";

    //Iterating through docs set to make string
    set<pair<unsigned, unsigned>>::iterator it;
    for(it = docs.begin(); it != docs.end(); it++){
        s += to_string((*it).first);
        s += " ";
        s += to_string((*it).second);
        s += "_";
    }
    //Take off trailing underscore
    s = s.substr(0, s.size() - 1);

    return s;
}
