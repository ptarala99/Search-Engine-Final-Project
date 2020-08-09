//Owner: Pravik

//Citations:
    //https://stackoverflow.com/questions/612097/how-can-i-get-the-list-of-files-in-a-directory-using-c-or-c
        //Used the above link to figure out how to go through every "document" file in the directory
        //We used our own path for the string since it is our machines, not the owner of the StackOverflow question's.
        //We explicitly defined the namespace for directory_iterator'
        //The range-based for loop is what will loop through each document in our directory
        //However, we created our own original code within the for loop in order to parse information into new "Doc" objects.

    //https://www.geeksforgeeks.org/removing-punctuations-given-string/
        //We copied (and slightly modified) code for punctuation removal

    //https://www.fluentcpp.com/2017/04/21/how-to-split-a-string-in-c/
        //We copied (and slightly modified) code to help us split our strings
        //containing phrases into strings containing individual words!

//History:
//Nov 14, 8:14pm, figured out how to print first line from every json file
//Nov 18, 8:38pm, installed myhtml libarary
//Nov 18, 9:42pm, successfully printed all of medium dataset "html" text (with tags still, but newline characters stripped by myHTML)
//Nov 18, 11:01pm, successfully removed angle brackets (< and >) and everything between them!
//Nov 19, 10:27pm, Created a stem stop words list to read from, and now can stem the text from docs.
//Nov 20, 11:47am, updated doc class, removed uniqueWords vector from parser
//Nov 21, 12:45am, successfully extracted parties from relevant files (and inserted them into their respective docs)
//Nov 21, 12:04pm, implemented command line args and into where we use filepath appropriately
//Nov 21, 12:04pm, modified code to only select the first available container of opinion content (i.e. html, plaintext, etc.)
//Nov 21, 9:50pm, cleaned up parse() and got ID into each doc
//Nov 21, 10:58pm, got year into each doc
//Nov 22, 12:15am, got justices into some docs. started code for inserting into wordVector & wordSet.
//Nov 23, 9:06pm, re-added arguments
//Nov 25, 4:07am, fixed stemStops path (for now)
//Nov 25, 4:50am, calling createStopsSet() from constructor now
//Nov 25, 9:08pm, removed removeStops(), removed code for parties/year extraction,
                //and replaced stringstream loop with istringstream iteration.
//Nov 25, 10:54pm, created IndexInterface instance in parse(), which returns the pointer
                 //by reference to the Searcher class
//Nov 26, 4:50am, Got num nodes and totalOccur working, and set up loading output.

#include "docparser.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "json.hpp"
#include <experimental/filesystem>
using namespace std;

Docparser::Docparser(){
    //Primitive
    ignoreDoc = false;

    //Container
    createStopsSet();
}

//Creates Doc objects by parsing from the documents
void Docparser::parse(string folderPath, IndexInterface*& ii, bool onTheSpot){
    ifstream inFs;
    string path = folderPath;
    int c = 1;

    //For a single document...
    if(onTheSpot){
        //Opening file
        inFs.open(folderPath);
        if(!inFs.is_open()){
            cout << "Cannot open input file." << endl;
            exit(-6);
        }

        //Declaring temp variables and setting up json parser
        string k;
        ifstream i(folderPath);
        json j;
        i >> j;

        //Copies the contents of text from the following keys
        auto htmlplain = j.at("html");
        auto htmlcite = j.at("html_with_citations");
        auto htmllaw = j.at("html_lawbox");
        auto plaintext = j.at("plain_text");

        //Find a key with text in it, and extract the text
        string toProcess = "";
        if(!(htmlplain.empty())){
            toProcess = htmlplain;
        }
        else if(!(htmlcite.empty())){
            toProcess = htmlcite;
        }
        else if(!(htmllaw.empty())){
            toProcess = htmllaw;
        }
        else if(!(plaintext.empty())){
            toProcess = plaintext;
        }

        //Ignoring tags and parsing the first 300 words.
        htmlConvert(toProcess);

        //Close the current file stream
        inFs.close();
        //i.close() too?

        return;
    }

    //For every file in the directory...
    for(const auto& entry : std::experimental::filesystem::directory_iterator(path)){

        //Opening file
        inFs.open(entry.path());
        if(!inFs.is_open()){
            cout << "Cannot open input file." << endl;
            exit(-6);
        }

        //Declaring temp variables and setting up json parser
        Doc tempDoc;
        string k;
        ifstream i(entry.path());
        json j;
        i >> j;

        //Read ID
        unsigned id = j.at("id");
        tempDoc.setID(id);

        //Copies the contents of text from the following keys
        auto htmlplain = j.at("html");
        auto htmlcite = j.at("html_with_citations");
        auto htmllaw = j.at("html_lawbox");
        auto plaintext = j.at("plain_text");

        //Find a key with text in it, and extract the text
        string toProcess = "";
        if(!(htmlplain.empty())){
            toProcess = htmlplain;
        }
        else if(!(htmlcite.empty())){
            toProcess = htmlcite;
        }
        else if(!(htmllaw.empty())){
            toProcess = htmllaw;
        }
        else if(!(plaintext.empty())){
            toProcess = plaintext;
        }

        //Ignoring tags and parsing the necessary info from the text
        htmlConvert(toProcess, tempDoc, id, ii);

        //Print "loading" message
        if(c % 100 == 0) cout << c << endl;
        c++;
        //cout << id << ".json" << endl;

        //Close the current file stream
        inFs.close();
        //i.close() too?
    }
}

void Docparser::htmlConvert(string s, Doc& tempDoc,
                            unsigned id, IndexInterface*& ii){
    ignoreDoc = false;
    const char* html = s.data();

    //MyHTML driver code
    myhtml_t* myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);
    myhtml_tree_t* tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);
    myhtml_parse(tree, MyENCODING_UTF_8, html, strlen(html));
    mycore_string_raw_t str = {nullptr};
    myhtml_serialization_tree_buffer(myhtml_tree_get_document(tree), &str);

    //Using strtok to extract phrases, but excluding angle brackets and anything inside them.
    string partiesTemp = "";
    unsigned phraseNum = 1;
    char* tok = strtok(str.data, ">");
    while(tok != nullptr){
        bool restartLoop = false;
        string temp(tok);
        string strNew = "";
        unsigned delim = 0;

        delim = temp.find('<');
        strNew = temp.substr(0, delim);

        if(strNew == "")
            restartLoop = true;

        //If no restart, istringstream extracts individual words from the phrases.
        if(!restartLoop){

            //Next three lines inspired fluentcpp.com (see citations)
            std::istringstream iss(strNew);
            std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                             std::istream_iterator<std::string>());

            //Stems words, then removes stop words.
            unsigned numTotalWords = 0;
            for(int i = 0; i < results.size(); numTotalWords++){
                results[i] = stem(results[i]); //Stem the word first.
                if(stopWords.count(results[i]) > 0){ //If word is stop word...
                    results.erase(results.begin() + i); //...remove it from results vector.
                }else{//If word is NOT a stop word...
                    Word theWord(results[i], id);
                    ii->addWord(theWord, id); //...add it to the index.
                    i++;
                }
            }
            tempDoc.incTotalWords(numTotalWords);
            phraseNum = phraseNum + 1; //Moving on to the next phrase

            //Ignore certain documents (double-check this later to make sure we aren't ignoring good files)
            if(phraseNum >= 12 && phraseNum <= 15){
                bool found1 = partiesTemp.find("Supreme Court of United States.") != string::npos;
                bool found2 = partiesTemp.find("No. 01-1") != string::npos;
                bool found3 = partiesTemp.find("UNITED STATES.") != string::npos;
                //If either signal phrase is found...
                if(found1 || found2 || found3){
                    ignoreDoc = true;
                    //cout << "*";
                    break; //this means that the file is a certiorari denied doc, which we can ignore. Exit parsing of this file.
                }
            }
        }//end of "!restartLoop" if statement namespace code

        //Continue using strtok
        tok = strtok(nullptr, ">");
    }
    //cout << "EXITED TOK WHILE LOOP" << endl; //Debug statement

    //MyHTML: "Releasing resources"
    mycore_string_raw_destroy(&str, false);
    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);

    if(!ignoreDoc){
        ii->getDocVector().push_back(tempDoc);
        //cout << "Doc NOT ignored" << endl; //Debug statement
    }
}

void Docparser::htmlConvert(string s){
    int numWordsPrinted = 0;
    const char* html = s.data();

    //MyHTML driver code
    myhtml_t* myhtml = myhtml_create();
    myhtml_init(myhtml, MyHTML_OPTIONS_DEFAULT, 1, 0);
    myhtml_tree_t* tree = myhtml_tree_create();
    myhtml_tree_init(tree, myhtml);
    myhtml_parse(tree, MyENCODING_UTF_8, html, strlen(html));
    mycore_string_raw_t str = {nullptr};
    myhtml_serialization_tree_buffer(myhtml_tree_get_document(tree), &str);

    //Using strtok to extract phrases, but excluding angle brackets and anything inside them.
    string partiesTemp = "";
    unsigned phraseNum = 1;
    char* tok = strtok(str.data, ">");
    while(tok != nullptr){
        bool restartLoop = false;
        string temp(tok);
        string strNew = "";
        unsigned delim = 0;

        delim = temp.find('<');
        strNew = temp.substr(0, delim);

        if(strNew == "")
            restartLoop = true;

        //If no restart, istringstream extracts individual words from the phrases.
        if(!restartLoop){

            //Next three lines inspired fluentcpp.com (see citations)
            std::istringstream iss(strNew);
            std::vector<std::string> results((std::istream_iterator<std::string>(iss)),
                                             std::istream_iterator<std::string>());

            //Stems words, then removes stop words.
            for(int i = 0; i < results.size(); i++){
                cout << results[i] << " ";
                numWordsPrinted++;
            }
            if(numWordsPrinted >= 300){
                break;
            }
            phraseNum = phraseNum + 1; //Moving on to the next phrase
        }//end of "!restartLoop" if statement namespace code

        //Continue using strtok
        tok = strtok(nullptr, ">");
    }
    //cout << "EXITED TOK WHILE LOOP" << endl; //Debug statement
    cout << endl;

    //MyHTML: "Releasing resources"
    mycore_string_raw_destroy(&str, false);
    myhtml_tree_destroy(tree);
    myhtml_destroy(myhtml);
}

string Docparser::stem(string s){
    //Remove punctuation
    for(int i = 0, len = s.size(); i < len; i++){
        if(ispunct(s[i])){
            s.erase(i--, 1);
            len = s.size();
        }
    }

    //Stem the word
    Porter2Stemmer::stem(s);

    //Make string all lowercase
    locale loc;
    string newS;
    for(int i = 0; i < s.length(); i++){
        newS += tolower(s[i], loc);
    }

    //cout << newS << endl; //Debug statement
    return newS;
}

void Docparser::stemStops(){
    //Reading in from normal stop word list
    ifstream inFs("stopwords.txt");

    //Full path for Pravik's laptop:
    //"home/student/Desktop/CSE2341-F19-Pravik-Kade/Sprint5/build-Project5-Desktop_Qt_5_10_0_GCC_64bit-Debug/stopwords.txt"

    if(!inFs.is_open()){
        cout << "Cannot open input file." << endl;
        return;
    }

    string s;
    while(getline(inFs, s)){
        s = stem(s);
        //cout << s << endl; //Debug statement
        stopWords.emplace(s);
    }

    inFs.close();

    //Writing to a new file to hold stemmed stop words
    ofstream os("/home/student/Desktop/CSE2341-F19-Pravik-Kade/Sprint5/build-Project5-Desktop_Qt_5_10_0_GCC_64bit-Debug/stemStops.txt");

    for(auto& word : stopWords){
        os << word << endl;
    }

    os.close();
}

void Docparser::createStopsSet(){
    //Initializer list (would have to manually add them into {},
    //                  then do "stopWords = temp;")
    //set<string> temp {};

    //Open file stream
    ifstream inFs("../../Sprint5/Project5/stemStops.txt");

    //Did the file open?
    if(!inFs.is_open()){
        cout << "Cannot open input file." << endl;
        return;
    }

    string s;
    while(getline(inFs, s)){
        stopWords.emplace(s);
    }

    /*
    //Debug statements
    for(auto& word : stopWords){
        cout << word << endl;
    }
    cout << stopWords.size() << endl;
    */

    //Close file stream
    inFs.close();
}

//Read from persistent index (into AVL Tree)
void Docparser::readFromPIndex(IndexInterface*& ii){
    ifstream iFile;
    iFile.open("../../Sprint5/build-Project5-Desktop_Qt_5_10_0_GCC_64bit-Debug/persistIndex.txt");

    if(!iFile.is_open()){
        cout << "Could not open persistIndex.txt" << endl;
        return;
    }

    string line;
    getline(iFile, line); //Ignoring first line (empty string)
    while(getline(iFile, line)){
        cout << ".";
        stringstream ss(line);
        string w; //read until _
        string numTotalS; //read until _
        string id;
        string numInDoc;
        set<pair<unsigned, unsigned>> tempSet;

        //Getting word and numTotalOccurences from persistent index
        getline(ss, w, '_');
        getline(ss, numTotalS, '_');
        unsigned numTotal = stoi(numTotalS);

        while(ss.peek() != EOF){
            //Retrieving pairs info from persistent index
            getline(ss, id, ' ');
            unsigned idNum = stoi(id);
            getline(ss, numInDoc, '_');
            unsigned numInDocNum = stoi(numInDoc);

            //Putting pairs into set
            pair<unsigned, unsigned> tempPair;
            tempPair.first = idNum;
            tempPair.second = numInDocNum;
            tempSet.emplace(tempPair);
        }

        //Construct a word object and put it into the live index
        Word theWord(w, numTotal, tempSet);
        ii->addWord(theWord);

        //Add doc
        //Doc tempDoc;
        //tempDoc.setID()
    }
    cout << endl;
    iFile.close();
}

//Write to persistent index (from AVL Tree)
void Docparser::writeToPIndex(IndexInterface*& ii){
    vector<Word> v;
    ii->getTree().getAllVals(v); //Copies all Word objects into v
    ofstream oFile;
    oFile.open("../../Sprint5/build-Project5-Desktop_Qt_5_10_0_GCC_64bit-Debug/persistIndex.txt");

    if(!oFile.is_open()){
        cout << "Could not open persistIndex.txt" << endl;
        return;
    }

    for(auto& word : v){
        oFile << word.getW() << "_"; //string of word
        oFile << word.getNum() << "_"; //numTotalOccurs
        oFile << word.getStrOfPairs() << "_" << endl;
    }
    oFile.close();
}
