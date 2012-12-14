/* 
 * File:   Parser.cpp
 * Author: agnes
 * 
 * Created on December 14, 2012, 7:24 PM
 */

#include "Parser.h"

#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstring>

Parser::Parser(string treebankFileName, string testSentencesFileName, string testSentencesTreesFileName, string outputFileName) {
  this->treebankFileName = treebankFileName;
  this->testSentencesFileName = testSentencesFileName;
  this->outputFileName = outputFileName;
  this->testSentencesTreesFileName = testSentencesTreesFileName;

  newTestSentencesTreesFileName = testSentencesTreesFileName + "new";

  grammar = new Grammar(treebankFileName);
  sentenceParser = new SentenceParser(grammar);
}

Parser::Parser(const Parser& orig) {
}

Parser::~Parser() {
}

void Parser::start() {
  grammar->init();
  parseTestSentences();
}



void Parser::parseTestSentences() {
  cout << "Parsing. " << endl;
  try {
    ifstream inputFile(testSentencesFileName.c_str());
    ofstream outputFile(outputFileName.c_str());
    ifstream treeInputFile(testSentencesTreesFileName.c_str());
    ofstream newTestTreeFile(newTestSentencesTreesFileName.c_str());
    
    string sentence;
    string sentenceTreeTestData;
    DerivationTree sentenceDerivationTree;
    string derivationString;

    if (inputFile.is_open() && treeInputFile.is_open()) {
      int numberParsed = 0;
      int numberSkipped = 0;

      while (getline(inputFile, sentence) && getline(treeInputFile, sentenceTreeTestData )) {
        if (!sentence.empty()) {
          //cout << sentence << endl;
          // system("pause");
         
          if (sentenceParser->parseLine(sentence)) {
            sentenceParser->getTree(sentenceDerivationTree);
           // TreeManager::printTree(sentenceDerivationTree);
            derivationString = TreeManager::getPennWSJstring(sentenceDerivationTree);
            outputFile << derivationString << endl;
            newTestTreeFile << sentenceTreeTestData << endl;
            numberParsed++;
           
            cout << "Parsed " << numberParsed << " lines, skipped " << numberSkipped << " (total " << numberParsed+numberSkipped << ") " <<  endl;
           
          }
          else {
            numberSkipped++;
          }
        }
      }
      inputFile.close();
      cout << "Processed " << numberParsed << " sentences." << endl;
      cout << "Skipped " << numberSkipped << " sentences (those had more than " << SentenceParser::maxTerms << " terms)." << endl;
      cout << "The corresponding test sentences file for \"" << outputFileName << "\" is \"" << newTestSentencesTreesFileName << "\". " << endl;
    }
    else {
      throw ("Unable to open test sentences file " + testSentencesFileName);
    }
  }
  catch (const char * e) {
    cerr << "Exception caught: " << e << endl;
    exit(1);
  }
}

