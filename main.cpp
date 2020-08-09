//History:
//Nov 21, 12:04pm, added command line args code
//Nov 25, 3:33am, did more OOP: searcher calls parser calls its functions

#include <iostream>
#include "json.hpp"
#include <fstream>
#include "searcher.h"
#include "word.h"
#include "porter2_stemmer.h"
using namespace std;

int main(int argc, char* argv[])
{
    Searcher s;
    s.run();
    return 0;
}
