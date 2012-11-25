/* 
 * File:   main.cpp
 * Author: agnes
 *
 * Created on November 20, 2012, 5:53 PM
 */
#include <iostream>
#include <cstdio>

#include "Grammar.h"
#include "CYKParser.h"
#include "TreeManager.h"

using namespace std;

/* header */
class Main {
  public:
    void readTreeBank();
    void parse();
    void testStuff1();
    void askTreebankFilename();

  private:
    string treebankFilename;
    Grammar * myGrammar;
    CYKParser * parser;
};

/* implementation */
void Main::testStuff1() {
  myGrammar = new Grammar("example.dat");
  myGrammar->init();

 // myGrammar->save();
  //myGrammar->load();
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

// myGrammar->printL2rTable();
// cout <<"r 2 l: " << endl;
// myGrammar->printR2lTable();
//  vector<Grammar::stringAndDouble> LHSs = myGrammar->getRHSs("NP");
//  for (int i = 0; i < LHSs.size(); i++) {
//    cout << LHSs[i].first << " " << LHSs[i].second << endl;
//  }  cout << endl;

  parser = new CYKParser(myGrammar);
 parser->parseLine("Ms. Haag plays Elianti . ");
  //parser->parseLine("He plays , and he plays superbly . ");
  // will consume too much memory (std::bad_alloc error):
  //parser->parseLine("On the exchange floor , `` as soon as UAL stopped trading , we braced for a panic , '' said one top floor trader . ");
  parser->printCYKTable();

  
 
}

void Main::readTreeBank() {

  // myGrammar = new Grammar(treebankFilename);
  // myGrammar->init();

}

void Main::parse() {
  // CYKParser * parser = new CYKParser(grammar);
  // parser->produceDerivations("On the exchange floor , `` as soon as UAL stopped trading , we braced for a panic , '' said one top floor trader .  ");
  // parser->printDerivations();
}

void Main::askTreebankFilename() {
  cout  <<  "A treebank filename was not specified."                   << endl;
  cout  <<  "Enter the treebank filename. If you already have an "
            "archived treebank (.xml) file, enter the original treebank "
            "filename too, then the program will open the corresponding archive "
            "(e.g. for \"treebank.dat\" it will open \"treebank.dat_archive\"). ";
  do {
    cin >> treebankFilename;
    cin.ignore(100, '\n');
    cin.clear();
  } while(cin.fail());
}

int main(int argc, const char * argv[]) {

  Main * main = new Main();

  
  main->testStuff1();

//  if (!argv[1]) {
//    main->askTreebankFilename();
//  }
  // main->readTreeBank();
   // main->parse();
  return 0;

}

