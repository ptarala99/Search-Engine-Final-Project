//Owner: Kade

//Citations: None

//History:
//Nov 24, 4:57pm, created AVLTreeIndex class
//Nov 25, 6:03am, finished implementing addWord (yet to be tested electronically)
//Nov 25, 7:29am, finished implementing getDocs (yet to be tested electronically)
//Nov 26, 3:19am, FINALLY fixed numTotalOccurences output. Logic error was
                //due to doing incNum() on a Word not in the AVLTree.
//Nov 26, 5:29am, modified getDocs() here in AVLTreeIndex

#include "avltreeindex.h"

AVLTreeIndex::AVLTreeIndex()
    : IndexInterface() {}

void AVLTreeIndex::addWord(Word& w, unsigned id){
    //Inserts word into tree if not a duplicate
    bool isDup = words.insert(w);

    if(isDup){
        bool idExistsAlready = words.search(w).idExists(id);
        if(!idExistsAlready){
            //Search for equivalent word in tree
            //Emplace the docId of w INTO the word in the tree
            words.search(w).getDocs().emplace(w.getFirstDoc());
        }
    }

    //Increment numTotalOccurences of the word in the tree
    words.search(w).incNum();

    //Increment numOccurInDoc of the word in the tree
    words.search(w).incNumInDoc(id);
}

//Function for adding words built from persistent index
void AVLTreeIndex::addWord(Word& w){
    //No duplicates in persistent index
    words.insert(w);
}

Word& AVLTreeIndex::getWord(string w){
    return words.search(w);
}

//Important for inverted file index functionality
set<pair<unsigned, unsigned>>& AVLTreeIndex::getDocs(string w){
    return words.search(w).getDocs();
}

unsigned AVLTreeIndex::getNumNodes(){
    return words.getNumNodes();
}

AVLTree<Word>& AVLTreeIndex::getTree(){
    return this->words;
}

void AVLTreeIndex::rank(const string& word,
                        vector<pair<unsigned, double>>& relevantDocs){
    //increment through each doc id
    for(int i = 0; i < docs.size(); i++){
        int numInOneDoc = this->getWord(word).getNumInDoc(docs.at(i).getID());
        int numInAllDocs = this->getWord(word).getNum();

        //Debug: cout << "numInOneDoc: " << numInOneDoc << endl;
        //Debug: cout << "numInAllDocs: " << numInAllDocs << endl;

        if(numInOneDoc == -1){
            continue;
        }
        //make a temp pair then push_back it into the vector
        pair<unsigned, double> temp;
        temp.first = docs.at(i).getID();
        temp.second = static_cast<double>(numInOneDoc) / numInAllDocs;
        //Debug: cout << "temp.second: " << temp.second << endl;
        relevantDocs.push_back(temp);
    }
}

void AVLTreeIndex::rankOr(const string& word,
                          vector<pair<unsigned, double>>& relevantDocs){
    //increment through each doc id
    for(int i = 0; i < docs.size(); i++){
        int numInOneDoc = this->getWord(word).getNumInDoc(docs.at(i).getID());
        int numInAllDocs = this->getWord(word).getNum();

        //Debug: cout << "numInOneDoc: " << numInOneDoc << endl;
        //Debug: cout << "numInAllDocs: " << numInAllDocs << endl;

        if(numInOneDoc == -1){
            continue;
        }

        unsigned tempNum = docs.at(i).getID();
        for(int j = 0; j < relevantDocs.size(); j++){
            if(tempNum == relevantDocs.at(j).first){
                //if id found, add ratio to id
                double newRatio = static_cast<double>(numInOneDoc) / numInAllDocs;
                relevantDocs.at(j).second += newRatio;
            }
        }
        //if id not found, push into vector w/ id and ratio
        pair<unsigned, double> temp;
        temp.first = tempNum;
        temp.second = static_cast<double>(numInOneDoc) / numInAllDocs;
        relevantDocs.push_back(temp);
    }
}

void AVLTreeIndex::addRatio(const string& word,
                            vector<pair<unsigned, double>>& relevantDocs){
    //increment through each doc id
    for(int i = 0; i < relevantDocs.size(); i++){
        int numInOneDoc = this->getWord(word).getNumInDoc(relevantDocs.at(i).first);
        int numInAllDocs = this->getWord(word).getNum();

        //Debug: cout << "numInOneDoc: " << numInOneDoc << endl;
        //Debug: cout << "numInAllDocs: " << numInAllDocs << endl;

        if(numInOneDoc == -1){
            continue;
        }
        //calculate ratio of new word, then add it to current ratio
        double newRatio = static_cast<double>(numInOneDoc) / numInAllDocs;
        relevantDocs.at(i).second += newRatio;
    }
}

void AVLTreeIndex::notRemoval(const string& word,
                              vector<pair<unsigned, double>>& relevantDocs){
    //iterate through relevantDocs
    for(int i = 0; i < relevantDocs.size(); ){
        //access id (save in temp variable?)
        unsigned tempId = relevantDocs.at(i).first;
        //getWord
        int numInDoc = this->getWord(word).getNumInDoc(tempId);
        //if word contains <id, not zero>
        if(numInDoc > 0){
            vector<pair<unsigned, double>>::iterator it = relevantDocs.begin();
            for(int j = 0; j < i; j++){
                it++;
            }
            //remove current pair
            relevantDocs.erase(it);
        }
        else{
            //Incrementer is here to avoid concurrent modifcation
            i++;
        }
    }
}

