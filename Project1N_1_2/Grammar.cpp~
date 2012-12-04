/*
 * File:   Grammar.cpp
 * Author: agnes
 *
 * Created on November 20, 2012, 6:15 PM
 */

#include "Grammar.h"

const string Grammar::nonTerminalSymbol = "nt_";

/**
 * Constructor
 */
Grammar::Grammar(string treeBankFile) {
  treeBankFileName = treeBankFile;
  archiveName = treeBankFileName + "_archive.xml";
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

/**
 * Start
 */
void Grammar::init(bool print /* = true */) {

  if (!archiveExists()) {
    cout << "Will read and process treebank file..." << endl;
    readGrammar(print);
    cout << "Nr. rules: " << l2rTable.size() << endl;
    l2rTableCountToProbability();
    saveTreebankArchive();
    fillR2lTableFromL2rTable();
  }
  else {
    cout << "Will load existing treebank file..." << endl;
    loadTreebankArchive();   
    cout << "Done loading." << endl;
     cout << "Nr. rules: " << l2rTable.size() << endl;
    fillR2lTableFromL2rTable();
  }
}

bool Grammar::archiveExists() {
  fstream file;
  file.open(archiveName.c_str(), ios_base::out | ios_base::in); // will not create file
  if (file.is_open()) {
    file.close();
    return true;
  }
  else {
    file.clear();
    return false;
  }
}

void Grammar::saveTreebankArchive() {
  cout << "Writing treebank file to XML archive..." << endl;
  ofstream file(archiveName.c_str());
  boost::archive::xml_oarchive outputArchive(file);

  // "&" has same effect as "<<" in outputArchive & BOOST_SERIALIZATION_NVP(l2rTable);
  // The macro BOOST_SERIALIZATION_NVP expands to  boost::serialization::make_nvp
  // see http://www.boost.org/doc/libs/1_44_0/libs/serialization/doc/wrappers.html#nvp
  outputArchive & BOOST_SERIALIZATION_NVP(l2rTable);
}

void Grammar::loadTreebankArchive() {
  ifstream file(archiveName.c_str());
  boost::archive::xml_iarchive inputArchive(file);
  inputArchive & BOOST_SERIALIZATION_NVP(l2rTable);
}

/**
 *
 * @param string LHS
 * @return Vector with Pairs denoting <the right hand sides,  their accompanying probability (of LHS -> RHS)>
 */
void Grammar::getRHSs(string LHS, vector<stringAndDouble>& RHSs) {
  ruleRangeIterator = l2rTable.equal_range(LHS);

  for (ruleIterator = ruleRangeIterator.first; ruleIterator != ruleRangeIterator.second; ruleIterator++) {
    RHSs.push_back((*ruleIterator).second);
  }
}

/**
 *
 * @param string RHS
 * @return Vector with Pairs denoting <the left hand sides,  their accompanying probability (of LHS -> RHS)>
 *
 * Note: Intuitively each RHS should only have one LHS (?)
 * but this is likely not  true in practice
 */
void Grammar::getLHSs(string RHS, vector<stringAndDouble>& LHSs) {
  ruleRangeIterator = r2lTable.equal_range(RHS);

  for (ruleIterator = ruleRangeIterator.first; ruleIterator != ruleRangeIterator.second; ruleIterator++) {
    LHSs.push_back((*ruleIterator).second);
  }
}

/**
 * Print all rules in LHS->RHS format
 */
void Grammar::printL2rTable() {
  for (ruleIterator = l2rTable.begin(); ruleIterator != l2rTable.end(); ruleIterator++) {
    printf("%s \t ==>  \t %s  \t (prob/count %f ) \n", (*ruleIterator).first.c_str(), (*ruleIterator).second.first.c_str(), (*ruleIterator).second.second);
  }
}

/**
 * Print all rules in LHS->RHS format
 */
void Grammar::printR2lTable() {
  for (ruleIterator = r2lTable.begin(); ruleIterator != r2lTable.end(); ruleIterator++) {
    printf("%s \t <== \t %s  \t (prob/count %f ) \n", (*ruleIterator).first.c_str(), (*ruleIterator).second.first.c_str(), (*ruleIterator).second.second);
  }
}

/**
 * Transforms counts per rule in l2rTable to probability per rule
 */
void Grammar::l2rTableCountToProbability() {
  for (ruleIterator = l2rTable.begin(); ruleIterator != l2rTable.end(); ruleIterator++) {
    ruleIterator->second.second /= lhsCountTable.find(ruleIterator->first)->second;
  }
}

/**
 * Given the LHS->RHS lookup table (l2rTable), fill the table with the RHS->LHS lookup function (r2lTable)
 */
void Grammar::fillR2lTableFromL2rTable() {
  for (ruleIterator = l2rTable.begin(); ruleIterator != l2rTable.end(); ruleIterator++) {
    r2lTable.insert(tableKeyAndValue((*ruleIterator).second.first, stringAndDouble((*ruleIterator).first, (*ruleIterator).second.second)));
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


  ruleRangeIterator = l2rTable.equal_range(key);

  if (ruleRangeIterator.first == ruleRangeIterator.second) { // LHS didnt occur yet
    lhsCountTable.insert(stringAndInt(key, 1)); // keep track of number of rules per LHS
    l2rTable.insert(tableKeyAndValue(key, stringAndDouble(valueString, 1))); // insert rule in l2rTable
  }
  else { // LHS did occur
    map<string, int>::iterator it = lhsCountTable.find(key);
    it->second++;

    bool RHSfound = false;
    for (ruleIterator = ruleRangeIterator.first; ruleIterator != ruleRangeIterator.second; ruleIterator++) {
      if (ruleIterator->second.first == valueString) {
        ruleIterator->second.second++;
        RHSfound = true;
        break;
      }
    }
    if (!RHSfound)
      l2rTable.insert(tableKeyAndValue(key, stringAndDouble(valueString, 1)));
  }
}

/**
 * Check if the character is a character we need to process anyway
 * and not e.g. the TAB or DEL character
 *
 * @param nextChar
 * @return
 */
bool Grammar::validCharacter(char nextChar) {
  return !(static_cast<int> (nextChar) < 33 || static_cast<int> (nextChar) > 126);
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
  char nextChar = line[linePos]; // cout << "nextChar: " << nextChar << endl;
  while (nextChar == ' ' && linePos < strlen(line)) {
    linePos++;
    nextChar = line[linePos];
  }
  if ((linePos >= strlen(line) - 1) ) //|| !validCharacter(nextChar)) // check for e.g. tab-characters in input file
    return;
  else {
    if (nextChar == ')') { // ========> non-terminal rule (RHS end) found
      stringAndInt RHS1, RHS2, LHS;
      RHS2 = stringLevelStack.top();
      stringLevelStack.pop();

      if (stringLevelStack.top().second == RHS2.second) { // same level
        RHS1 = stringLevelStack.top();
        stringLevelStack.pop();
        RHS1.first += " " + nonTerminalSymbol;
      }
      else {
        RHS1.first = "";
      }
      LHS = stringLevelStack.top(); //stringLevelStack.pop();
      insertL2rTable(nonTerminalSymbol + LHS.first, nonTerminalSymbol + RHS1.first + RHS2.first);
      //cout << "insert " << LHS.first << " ==> " << RHS1.first + RHS2.first << endl;
      return parseLineRecursively(line, linePos + 1, stringLevelStack, level - 1);
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
    if (nextChar == '(') { // ========> begin new rule found (new LHS)
      return parseLineRecursively(line, linePos + 1, stringLevelStack, level + 1);
    }
    else { //  ========> terminal symbol + ')' found
      string term = "";
      while (line[linePos] != ')') {
        term += line[linePos];
        linePos++;
      }
      //cout << "term: " << term << endl;
      string nonTerm = stringLevelStack.top().first;
      insertL2rTable(nonTerminalSymbol + nonTerm, term);
      //cout << "insert " << nonTerm << " ==> " << term << endl;
      return parseLineRecursively(line, linePos + 1, stringLevelStack, level - 1);
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
  int linePos = 0;
  parseLineRecursively(line.c_str(), linePos, stringLevelStack, level);

  //cout << "line: " << line << endl;
}

/**
 * Read treebank file
 * parse it, fill l2rTable accordingly
 */
void Grammar::readGrammar(bool print) {
  try {
    ifstream myfile(treeBankFileName.c_str());
    string line;

    if (myfile.is_open()) {
      int numberLines = 0;
      while (getline(myfile, line)) {
        if (!line.empty()) {
          //cout << line << endl;
         // system("pause");
          line.append(" ");
          parseLine(line);
          numberLines++;
          if (print && numberLines % 100 == 0)
            cout << "processed " << numberLines << " lines " << endl;
        }
        line = "";
      }
      myfile.close();
    }
    else {
      throw ("Unable to open treebank file");
    }
  }
  catch (const char * e) {
    cerr << "Exception caught: " << e << endl;
    exit(1);
  }

}