//Owner: Kade

//Citations: None

//History:
//Nov 23, 8:04pm, created IndexInterface class
//Nov 23, 9:00pm, removed "vector<Doc> docs" from private in .h, b/c
                //we might move it to the Searcher class later.
//Nov 23, 9:06pm, re-added arguments
//Nov 24, 11:50pm, fixed destructors in IndexInterface and AVLTreeIndex
//Nov 25, 1:25am, added copy c'tor and op= in private for sake of rule of three

#include "indexinterface.h"

//C'tor with args
IndexInterface::IndexInterface()
{
    wordsPerOpinion = 0;
}

//Empty destructor b/c virtual
IndexInterface::~IndexInterface() {}

vector<Doc>& IndexInterface::getDocVector(){
    return this->docs;
}
