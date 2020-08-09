//Owner: Kade

//Citations:
    //GeeksForGeeks Bubble Sort
        //copied and modified code to work for vector<pair>

//History:
//Nov 23, 8:40pm, created Searcher class
//Nov 25, 9:11pm, added IndexInterface pointer to private
//Nov 26, 12:26am, able to print number of AVLNodes!

#include "searcher.h"

Searcher::Searcher(){
    //Creating IndexInterface object here
    ii = new AVLTreeIndex();
}

Searcher::~Searcher(){
    delete ii;
}

void Searcher::run(){
    Docparser parser;
    string input;

    cout << "Welcome to the search engine!" << endl;
    cout << "Reading from persistent index..." << endl;
    //parser.readFromPIndex(ii);
    cout << "Done reading from persistent index." << endl << endl;

    while(input != "q"){
        cout << "Menu: Choose a mode (m/i)" << endl;
        cout << "(Type q to quit.)" << endl;
        cin >> input;

        //Maintenance Mode
        if(input == "m"){
            string isFolder;
            cout << "Want to supply a folder path with new opinions? (y/n)" << endl;
            cin >> isFolder;
            if(isFolder == "y"){
                cout << "Please supply the path now: " << endl;
                string folderPath;
                cin >> folderPath;

                cout << "Indexing new opinions..." << endl;
                //Parse these docs to the normal index (not persistent)
                parser.parse(folderPath, ii, false);
            }
        }

        //Interactive Mode
        if(input == "i"){
            //Get query from user
            cout << "Please enter a properly formatted Boolean query: " << endl;
            string query;
            cin.ignore();
            getline(cin, query);

            //vector<docId, tfidfRatio>
            vector<pair<unsigned, double>> relevantDocs;

            //Process query (includes stemming throughout)
            std::istringstream iss(query);
            std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                             std::istream_iterator<std::string>());
            int c = 0;
            results.at(c) = parser.stem(results.at(c));
            //If "and", get ALL docs that have first word
            if(results.at(c) == "and"){
               int andCount = 0;
               bool isNot = true;
               c++;
               results.at(c) = parser.stem(results.at(c));
               //Adding id-ratio pairs to vector
               ii->rank(results.at(c), relevantDocs);
               andCount++;
               //Then get ALL docs that have subsequent words
               while(results.at(c) != "not"){
                   c++;
                   if(c >= results.size()){
                       //no more words in query, so there is no "not"
                       isNot = false;
                       break;
                   }
                   //program knows it can stem b/c not out-of-bounds of vector
                   results.at(c) = parser.stem(results.at(c));
                   //search through relevantDocs (docIds)
                   for(int i = 0; i < relevantDocs.size(); i++){
                       //remove if id not found, or no occurences in the doc
                       int x = ii->getWord(results.at(c)).getNumInDoc(relevantDocs.at(i).first);
                       if(x <= 0){
                           //remove id-ratio pair from relevantDocs
                           vector<pair<unsigned, double>>::iterator it = relevantDocs.begin();
                           for(int j = 0; j < i; j++){
                               it++;
                           }
                           relevantDocs.erase(it);
                       }
                       else{
                           //Incrementer is here to avoid concurrent modifcation
                           i++;
                       }
                   }
                   //for all remaining IDs in relevantDocs...
                   ii->addRatio(results.at(c), relevantDocs);
                   //increment query word counter
                   andCount++;
               }
               //finish averaging process
               for(int i = 0; i < relevantDocs.size(); i++){
                   relevantDocs.at(i).second /= andCount;
               }

               if(isNot){
                   //found a "not", so go to next word
                   c++;
                   results.at(c) = parser.stem(results.at(c));
                   //remove all docs that have that word
                   ii->notRemoval(results.at(c), relevantDocs);
               }
            }
            else if(results.at(c) == "or"){
                //add all docs that have first word
                //add all docs that have second word, etc.

                int orCount = 0;
                bool isNot = true;
                c++;
                results.at(c) = parser.stem(results.at(c));
                //Adding id-ratio pairs to vector
                ii->rank(results.at(c), relevantDocs);
                orCount++;
                //Then get ALL docs that have subsequent words
                while(results.at(c) != "not"){
                    c++;
                    if(c >= results.size()){
                        //no more words in query, so there is no "not"
                        isNot = false;
                        break;
                    }
                    //program knows it can stem b/c not out-of-bounds of vector
                    results.at(c) = parser.stem(results.at(c));

                    //for all remaining IDs in relevantDocs...
                    ii->addRatio(results.at(c), relevantDocs);
                    //increment query word counter
                    orCount++;
                }

                if(isNot){
                    //found a "not", so go to next word
                    c++;
                    results.at(c) = parser.stem(results.at(c));
                    //remove all docs that have that word
                    ii->notRemoval(results.at(c), relevantDocs);
                }
            }
            //Otherwise, only one word (with potential "not" term)
            else{
                //Adding id-ratio pairs to vector
                ii->rank(results.at(c), relevantDocs);
                c++;
                if(c < results.size()){
                    //found a "not", so go to next word
                    c++;
                    results.at(c) = parser.stem(results.at(c));
                    //remove all docs that have that word
                    ii->notRemoval(results.at(c), relevantDocs);
                }
            }

            //Don't sort if relevantDocs only contains 0 or 1 pairs
            bool skipSorting = false;
            if(relevantDocs.size() <= 1){
                skipSorting = true;
            }

            if(!skipSorting){
                //Sort the vector here by the second val (tfidfRatio)
                //NOTE: Sorts by descending order of ratio
                int i, j;
                bool swapped;
                for(i = 0; i < relevantDocs.size()-1; i++){
                    swapped = false;
                    for(j = 0; j < relevantDocs.size()-i-1; j++){
                        if(relevantDocs.at(j).second < relevantDocs.at(j+1).second){
                            pair<unsigned, double> temp;
                            temp.first = relevantDocs.at(j).first;
                            temp.second = relevantDocs.at(j).second;
                            relevantDocs.at(j) = relevantDocs.at(j+1);
                            relevantDocs.at(j+1) = temp;
                            swapped = true;
                        }
                    }
                    if(swapped == false){
                        break;
                    }
                }
            }

            /* Debug:
            for(auto& pair : relevantDocs){
                cout << "id: " << pair.first;
                cout << ". tfidfRatio: " << pair.second << endl;
            }
            */

            //Display top 15 opinions (represented by IDs)
            int rankNum = 1;
            for(auto& pair : relevantDocs){
                cout << rankNum << ") ID: " << pair.first << endl;
                rankNum++;
                if(rankNum == 16){
                    //Show no more than top 15 opinions
                    break;
                }
            }

            //Let user pick a search result
            cout << "Type number of result you want: " << endl;
            string number;
            cin >> number;
            int selection = std::stoi(number);

            //Retrieve id of user selection
            int currId = relevantDocs.at(selection - 1).first;

            //Make string with path for specific doc (use id in path name)
            string docPath = "../../Sprint5/build-Project5-Desktop_Qt_5_10_0_GCC_64bit-Debug/scotus-med/";
            docPath += to_string(currId);
            docPath += ".json";

            //Print the first ~300 words for this doc
            parser.parse(docPath, ii, true);
        }
    }

    //Parse-speed-check-style debugging code
    /*
    string qWord = "platform";
    cout << "Dr. F's query word: " << qWord << endl;

    //A bunch of Words are now in the index
    cout << "Total number of nodes: " << ii->getNumNodes() << endl;

    //Inorder traversal (debug)
    //ii->getTree().inOrder();

    //Stem the query word!
    string stemmedQword = parser.stem(qWord);

    //Use getDocs for the stemmed query word
    cout << "Total number of docs the word appears in: "
         << ii->getDocs(stemmedQword).size()
         << endl;

    cout << "Total number of appearances of the word: "
         << ii->getWord(stemmedQword).getNum() << endl;
    */

    cout << endl;
    cout << "Thank you for using the search engine!" << endl;
    cout << "Writing to persistent index..." << endl; //Write to persistent index (from AVLTree).
    parser.writeToPIndex(ii);
    cout << "Done writing to persistent index." << endl;
}

//USER INTERFACE FUNCTIONS
void Searcher::addOpinions(){

}

void Searcher::clearIndex(){

}

void Searcher::setStructureType(int option){

}

void Searcher::printStats(){

}

void Searcher::printResults(){

}

void Searcher::printOpinion(){

}

//QUERY PROCESSING
void Searcher::readQ(){

}

void Searcher::breakIntoWords(){

}

void Searcher::stemQueryWords(){

}
