/* 
 * File:   main.cpp
 * Author: agnes
 *
 * Created on November 20, 2012, 5:53 PM
 */
#include <iostream>
#include <cstdio>

#include "Grammar.h"

using namespace std;

class Main {

  public:
    void readTreeBank();
    void parse();
};

void Main::readTreeBank() {

   Grammar * myGrammar = new Grammar();
   vector<Grammar::stringAndDouble> LHSs = myGrammar->getRHS("NP");
   
   for(int i=0; i < LHSs.size(); i++) {
     cout << LHSs[i].first << " " << LHSs[i].second << endl;
   }

  // corpusReader * reader = new CorpusReader(grammar);
  // reader->makeGrammar();

}

void Main::parse() {
    // PCYKParser * parser = new PCYKParser(grammar);
  // parser->produceDerivations("Dit is een zin");
  // parser->printDerivations();
}

int main (int argc, const char * argv[]) {

  Main * main = new Main();

  main->readTreeBank();
  main->parse();
  return 0;

}

