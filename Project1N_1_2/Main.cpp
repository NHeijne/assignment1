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
#include "SentenceParser.h"
#include "TreeManager.h"
#include "Parser.h"
#include "tree.hh"

using namespace std;


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
    SentenceParser * sentenceParser;
    Parser * parser;
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
 
//myGrammar->printL2rTable();
// myGrammar->printUnknownProbTable();

// cout <<"r 2 l: " << endl;
 //myGrammar->printR2lTable();
//  vector<Grammar::stringAndDouble> LHSs;
//  myGrammar->getLHSs("meal",LHSs);
//  for (int i = 0; i < LHSs.size(); i++) {
//    cout << LHSs[i].first << " " << LHSs[i].second << endl;
//  }  cout << endl;

  sentenceParser = new SentenceParser(myGrammar);

 //sentenceParser->parseLine("Ms. Haag plays Elianti . ");
 // sentenceParser->parseLine("Nobody likes me . ");
 //sentenceParser->parseLine("He believes in what he plays , and he plays superbly . ");
 sentenceParser->parseLine("No , it was n't Black Monday . ");
  //sentenceParser->parseLine("Exchange officials emphasized that the Big Board is trading . ");

  sentenceParser->writeTOPs("toptest.dat");

 //sentenceParser->printCYKTable();
  tree<string> thisTree;
  sentenceParser->getTree(thisTree);
  TreeManager::printTree(thisTree);
  TreeManager::debinarize(thisTree);
  TreeManager::removeSpecialUnaryRules(thisTree);
  TreeManager::printTree(thisTree);
  cout << TreeManager::getTreeString(thisTree);

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
          sentenceParser->parseLine(line);
          sentenceParser->writeTOPs("toptest.dat");
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
  sentenceParser = new SentenceParser(myGrammar);
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
    
//  if (argc < 2) {
//    main->askTreebankFilename();
//  }
//  else {
//    main->treebankFilename.assign(argv[1]);
//  }
//  if (argc < 3) {
//    main->askTestSentencesFilename();
//  }
//  else {
//    main->testSentencesFilename.assign (argv[2]);
//  }
//
//  main->assignment2();

 // main->testStuff1();

  main->parser = new Parser("treebank.dat", "testsentencespart.dat", "testtreespart.dat", "test_result.dat");
 main->parser->start();
  
  return 0;
}

