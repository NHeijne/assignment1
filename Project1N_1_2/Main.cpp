/* 
 * File:   main.cpp
 * Author: agnes
 *
 * Created on November 20, 2012, 5:53 PM
 */
#include <iostream>
#include <cstdio>
#include <fstream>
#include<string>
#include<cstring>
#include "Grammar.h"
#include "CYKParser.h"
#include "TreeManager.h"

using namespace std;

// TODO: Iterators gebruiken in CYKParser.cpp

/* header */
class Main {
  public:
    void testStuff1();
    void askTreebankFilename();
    void topsTestSentences();
    void assignment2() ;
    void askTestSentencesFilename();

 // private:
    string treebankFilename;
    string testSentencesFilename;
    Grammar * myGrammar;
    CYKParser * parser;
};

/* implementation */
void Main::testStuff1() {
  myGrammar = new Grammar("treebank.dat");
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
 
// myGrammar->printL2rTable();
// cout <<"r 2 l: " << endl;
 //myGrammar->printR2lTable();
//  vector<Grammar::stringAndDouble> LHSs = myGrammar->getRHSs("NP");
//  for (int i = 0; i < LHSs.size(); i++) {
//    cout << LHSs[i].first << " " << LHSs[i].second << endl;
//  }  cout << endl;

  parser = new CYKParser(myGrammar);
  parser->parseLine("Ms. Haag plays Elianti . ");
// parser->parseLine("He plays , and he plays superbly . ");
  // will consume too much memory (std::bad_alloc error):
  //parser->parseLine("On the exchange floor , `` as soon as UAL stopped trading , we braced for a panic , '' said one top floor trader . ");
  parser->writeTOPs("toptest.dat"); 
}


void Main::topsTestSentences() {
  cout << endl << "will write to toptest.dat" << endl;

   try {
    ifstream myfile(testSentencesFilename.c_str());
    string line;

    if (myfile.is_open()) {
      int numberLines = 0;
      while (getline(myfile, line)) {
        if (!line.empty()) {
          parser->parseLine(line);
          parser->writeTOPs("toptest.dat"); 
          numberLines++;
          //if (numberLines % 100 == 0)
            cout << "processed " << numberLines << " lines from test file" << endl;
        }
        line = "";
      }
      myfile.close();
    }
    else {
      throw ("Unable to open test sentences file");
    }
  }
  catch (const char * e) {
    cerr << "Exception caught: " << e << endl;
    exit(1);
  }
}

void Main::assignment2 () {
  myGrammar = new Grammar("treebank.dat");
  myGrammar->init();
  parser = new CYKParser(myGrammar);
  topsTestSentences();
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

void Main::askTestSentencesFilename() {
  cout  <<  "A filename for the file containing the test sentences was not specified."  << endl;
  cout  <<  "Enter the filename. "                                                      << endl;
  do {
    cin >> testSentencesFilename;
    cin.ignore(100, '\n');
    cin.clear();
  } while(cin.fail());
}

int main(int argc, const char * argv[]) {
  Main * main = new Main();
    
  if (argc < 2) {
    main->askTreebankFilename();
  }
  else {
    main->treebankFilename.assign(argv[1]);
  }
  if (argc < 3) {
    main->askTestSentencesFilename();
  }
  else {
    main->testSentencesFilename.assign (argv[2]);
  }
  
  main->assignment2();

  return 0;
}

