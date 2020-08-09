//Owner: Pravik

//Citations: None

//History:
//Nov 20, 11:47am, updated doc class: created clear function and getters/setters
//Nov 25, 9:10pm, added numTotalWords private variable (+get/set)

#include "doc.h"

Doc::Doc(){
    id = 0;
    year = 0;
    numTotalWords = 0;
    parties = " ";
    majorJustice = " ";
}

void Doc::clear(){
    id = 0;
    year = 0;
    parties = "";
    majorJustice = "";
}

unsigned Doc::getID(){
    return id;
}

void Doc::setID(unsigned myID){
    id = myID;
}

unsigned Doc::getYear(){
    return year;
}

void Doc::setYear(unsigned myYear){
    year = myYear;
}

unsigned Doc::getTotalWords(){
    return numTotalWords;
}

void Doc::incTotalWords(unsigned myTotalWords){
    numTotalWords += myTotalWords;
}

string Doc::getParties(){
    return parties;
}

void Doc::setParties(string myParties){
    parties = myParties;
}

string Doc::getMajorJustice(){
    return majorJustice;
}

void Doc::setMajorJustice(string myMajorJustice){
    majorJustice = myMajorJustice;
}


