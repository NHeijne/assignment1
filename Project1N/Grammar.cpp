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

//  l2rTable.insert( tableKeyAndValue("NP",stringAndDouble("VP NNP",0.9)));
//  l2rTable.insert( tableKeyAndValue("NP",stringAndDouble("NNP NP",0.1)));
//  l2rTable.insert( tableKeyAndValue("VP",stringAndDouble("V PP",0.3)));

  init();
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

void Grammar::init() {
  readGrammar();
  fillR2lTableFromL2rTable();
}

/**
 *
 * @param string LHS
 * @return Vector with Pairs denoting <the right hand sides,  their accompanying probability (of LHS -> RHS)>
 */
vector<Grammar::stringAndDouble> Grammar::getRHS(string LHS) {
  vector<stringAndDouble> result;
  ruleRangeIterator = l2rTable.equal_range(LHS);
  
  for (ruleIterator = ruleRangeIterator.first; ruleIterator != ruleRangeIterator.second; ruleIterator++) {
    result.push_back((*ruleIterator).second);
  }
  cout << endl;
  return result;

}

/**
 *
 * @param string RHS
 * @return Vector with Pairs denoting <the left hand sides,  their accompanying probability (of LHS -> RHS)>
 */
vector<Grammar::stringAndDouble> Grammar::getLHS(string RHS) {
  vector<stringAndDouble> result;
  ruleRangeIterator = r2lTable.equal_range(RHS);
  
  for (ruleIterator = ruleRangeIterator.first; ruleIterator != ruleRangeIterator.second; ruleIterator++) {
    result.push_back((*ruleIterator).second);
  }
  cout << endl;
  return result;
}

/**
 * Print all rules in LHS->RHS format
 */
void Grammar::printL2rTable() {
  for(ruleIterator = l2rTable.begin(); ruleIterator != l2rTable.end(); ruleIterator++) {
    printf("%s \t ==>  \t %s  \t (prob/count %f ) \n", (*ruleIterator).first.c_str() , (*ruleIterator).second.first.c_str() ,(*ruleIterator).second.second);
  }

}

/**
 * Given the LHS->RHS lookup table (l2rTable), fill the table with the RHS->LHS lookup function (r2lTable)
 */
void Grammar::fillR2lTableFromL2rTable() {
  for ( ruleIterator=l2rTable.begin() ; ruleIterator != l2rTable.end(); ruleIterator++ ) {
    r2lTable.insert( tableKeyAndValue((*ruleIterator).second.first, stringAndDouble((*ruleIterator).first,(*ruleIterator).second.second)));
  }
}

/**
 * Insert a key (string) and value (stringAndDouble) in l2rTable.
 * If the element already exists the double in the stringAndDouble value is increased by one.
 *
 * @param key
 * @param valueString
 */
void Grammar::insertL2rTable(string key, string valueString) {
  double count = 0;
  ruleRangeIterator = l2rTable.equal_range(key);
  for (ruleIterator = ruleRangeIterator.first; ruleIterator != ruleRangeIterator.second; ruleIterator++) {
    if (ruleIterator->second.first == valueString) {    
     count = ruleIterator->second.second;
     l2rTable.erase(ruleIterator); // erase occurence - multimap allows duplicate <key,value> pairs!
     break;
    }
  }
  l2rTable.insert( tableKeyAndValue(key,stringAndDouble(valueString,count+1)));
}


bool Grammar::validCharacter(char nextChar) {
  return !(static_cast<int>(nextChar) < 33 ||  static_cast<int>(nextChar) > 126);
}

/**
 * Parse a line, fill l2rTable accordingly
 * Recursive function
 *
 * @param line
 * @param linePos
 * @param stringLevelStack
 * @param level
 */
void Grammar::parseLineRecursively(const char * line, int linePos, stack <stringAndInt> stringLevelStack, int level) {
  char nextChar = line[linePos];// cout << "nextChar: " << nextChar << endl;
  while (nextChar == ' ' && linePos < strlen(line)) {
      linePos++;
    nextChar = line[linePos];
  }
  if ((linePos >= strlen(line)-1) || !validCharacter(nextChar)) // check for e.g. tab-characters in input file
    return;
  else {
   
    if (nextChar == ')') {                   // ========> non-terminal rule (RHS end) found
      stringAndInt RHS1, RHS2, LHS;
      RHS2 = stringLevelStack.top(); stringLevelStack.pop();
     
      if (stringLevelStack.top().second == RHS2.second) { // same level    
        RHS1 = stringLevelStack.top(); stringLevelStack.pop();
        RHS1.first += " ";
      }
      else {
        RHS1.first = "";
      }
      LHS = stringLevelStack.top(); //stringLevelStack.pop();
      insertL2rTable(LHS.first, RHS1.first + RHS2.first );
      //cout << "insert " << LHS.first << " ==> " << RHS1.first + RHS2.first << endl;
      return parseLineRecursively(line, linePos+1, stringLevelStack, level-1);
    }
    if (nextChar != ')' && nextChar != '(') { //  ========> non-terminal LHS found
      string nonTerm = "";
      while (line[linePos] != ' ') {
        nonTerm += line[linePos];
        linePos++;
      }
      //cout << "nonterm: " << nonTerm << endl;
      stringLevelStack.push(stringAndInt(nonTerm, level));
      //cout << "enter " << nonTerm << " at stack at level " << level << endl;
      linePos++; // go beyond space
      nextChar = line[linePos]; 
    }
    if (nextChar == '(') {                    // ========> begin new rule found (new LHS)
      return parseLineRecursively(line, linePos+1, stringLevelStack, level+1);
    }
    else {                                    //  ========> terminal symbol + ')' found
      string term = "";
      while (line[linePos] != ')') {
        term += line[linePos];
        linePos++;
      }
      //cout << "term: " << term << endl;
      string nonTerm = stringLevelStack.top().first;
      insertL2rTable(nonTerm, term);
      //cout << "insert " << nonTerm << " ==> " << term << endl;
      return parseLineRecursively(line, linePos+1, stringLevelStack, level-1);
    }
  }
}

/**
 * Helper for parseLineRecursively
 * Parse a line, fill l2rTable accordingly
 *
 * @param line
 */
void Grammar::parseLine(string line) {

  stack<stringAndInt> stringLevelStack;
  int level = 0;
  int linePos= 0;
  parseLineRecursively(line.c_str(), linePos, stringLevelStack, level);

  //cout << "line: " << line << endl;
}

/**
 * Read treebank file
 * parse it, fill l2rTable accordingly
 */
void Grammar::readGrammar() {
   try {
    ifstream myfile (treeBankFileName.c_str());
    string line;
 
    if (myfile.is_open()){
      int numberLines = 0;
      while (!myfile.eof()){
        getline (myfile,line);
        if (!line.empty()) {
          parseLine(line);
          numberLines++;
          if (numberLines % 100 == 0)
            cout << "processed " << numberLines << " lines " << endl;
        }
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