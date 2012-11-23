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

/* header */
class Main {
  public:
    void readTreeBank();
    void parse();
    void testStuff1();

  private:
    Grammar * myGrammar;

};

/* implementation */
void Main::testStuff1() {
  myGrammar = new Grammar("example.dat");
  myGrammar->init();

//  vector<Grammar::stringAndDouble> RHSs = myGrammar->getRHS("NP");
//  for (int i = 0; i < RHSs.size(); i++) {
//    cout << RHSs[i].first << " " << RHSs[i].second << endl;
//  }  cout << endl;
//
//  myGrammar->fillR2lTable();
//
//  vector<Grammar::stringAndDouble> LHSs = myGrammar->getLHS("VP NNP");
//  for (int i = 0; i < LHSs.size(); i++) {
//    cout << LHSs[i].first << " " << LHSs[i].second << endl;
//  }  cout << endl;

  myGrammar->printR2lTable();
//  vector<Grammar::stringAndDouble> LHSs = myGrammar->getRHSs("NP");
//  for (int i = 0; i < LHSs.size(); i++) {
//    cout << LHSs[i].first << " " << LHSs[i].second << endl;
//  }  cout << endl;

}

void Main::readTreeBank() {

  // myGrammar = new Grammar();
  // myGrammar->init();

}

void Main::parse() {
  // CYKParser * parser = new CYKParser(grammar);
  // parser->produceDerivations("Dit is een zin");
  // parser->printDerivations();
}

int main(int argc, const char * argv[]) {

  Main * main = new Main();

 // main->readTreeBank();
 // main->parse();
  main->testStuff1();
  return 0;

}

