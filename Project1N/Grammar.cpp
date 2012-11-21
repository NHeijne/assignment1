/* 
 * File:   Grammar.cpp
 * Author: agnes
 * 
 * Created on November 20, 2012, 6:15 PM
 */

#include "Grammar.h"


/**
 * Constructor
 */
Grammar::Grammar(string treeBankFile) {

  treeBankFileName = treeBankFile;

  l2rTable.insert( tableKeyAndValue("NP",stringAndDouble("VP NNP",0.9)));
  l2rTable.insert( tableKeyAndValue("NP",stringAndDouble("NNP NP",0.1)));
  l2rTable.insert( tableKeyAndValue("VP",stringAndDouble("V PP",0.3)));

  readGrammar();
}

/**
 * Copy constructor
 * @param orig
 */
Grammar::Grammar(const Grammar& orig) {
  
}

/**
 * Destructor
 */
Grammar::~Grammar() {
  
}


vector<Grammar::stringAndDouble> Grammar::getRHS(string LHS) {
  vector<stringAndDouble> result;
  ruleRangeIterator = l2rTable.equal_range(LHS);
  
  for (ruleIterator = ruleRangeIterator.first; ruleIterator != ruleRangeIterator.second; ruleIterator++) {
    result.push_back((*ruleIterator).second);
  }
  cout << endl;
  return result;

}

vector<Grammar::stringAndDouble> Grammar::getLHS(string RHS) {
  vector<stringAndDouble> result;
  ruleRangeIterator = r2lTable.equal_range(RHS);
  
  for (ruleIterator = ruleRangeIterator.first; ruleIterator != ruleRangeIterator.second; ruleIterator++) {
    result.push_back((*ruleIterator).second);
  }
  cout << endl;
  return result;
}

void Grammar::fillR2lTable() {
  for ( ruleIterator=l2rTable.begin() ; ruleIterator != l2rTable.end(); ruleIterator++ ) {
    r2lTable.insert( tableKeyAndValue((*ruleIterator).second.first, stringAndDouble((*ruleIterator).first,(*ruleIterator).second.second)));
  }
}


void Grammar::parseLine(string line) {
  // LIFO stack

}
void Grammar::readGrammar() {
   try {
    ifstream myfile (treeBankFileName.c_str());
    string line;
 
    if (myfile.is_open()){
      while (myfile.good()){
        getline (myfile,line);
        //cout << line << endl << endl;
        parseLine(line);
      }
      myfile.close();
    }
    else {
      throw ("Unable to open treebank file") ;
    }
  }
  catch(const char * e) {   
   cerr << "Exception caught: " << e << endl;
   exit(1);
  }
  
}