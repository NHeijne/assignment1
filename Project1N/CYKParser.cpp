
#include "CYKParser.h"


CYKParser::CYKParser(Grammar * aGrammar) {
  myCFG = aGrammar;

  
}

CYKParser::CYKParser(const CYKParser &orig) {

}

CYKParser::~CYKParser() {

}

void CYKParser::parseLine(const string line) {
  vector<string> lineTerms = split(line);
  for (int i=0; i < lineTerms.size(); i++) {
    cout << "***" << lineTerms[i] << "***" << endl;
  }
}

void CYKParser::splitHelper(const string line, vector<string> &terms){  
  stringstream termStream(line);
  string term;
  while(getline(termStream, term, termDelimiter)) {
    if (!term.empty()) // we don't want the last space
      terms.push_back(term);
  }
}


vector<string> CYKParser::split(const string line) {
  vector<string> terms;
  splitHelper(line, terms);
  return terms;
}