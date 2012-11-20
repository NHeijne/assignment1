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
Grammar::Grammar() {

  l2rTable.insert( l2rEntry("NP",stringAndDouble("VP NNP",0.9)));
  l2rTable.insert( l2rEntry("NP",stringAndDouble("NNP NP",0.1)));
  l2rTable.insert( l2rEntry("VP",stringAndDouble("V PP",0.3)));
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
  
}