
#include "CYKParser.h"

CYKParser::CYKParser(Grammar * aGrammar) {
  myCFG = aGrammar;


}

CYKParser::CYKParser(const CYKParser &orig) {

}

CYKParser::~CYKParser() {

}

void CYKParser::parseLine(const string line) {
  lineTerms = split(line);
  for (int i = 0; i < lineTerms.size(); i++) {
    cout << "***" << lineTerms[i] << "***" << endl;
  }
  // reserve or free space in CYKTable
  nrTerms = lineTerms.size();
  CYKTable.clear();
  CYKTable.resize(nrTerms);
  for (int i = 0; i < nrTerms; i++) {
    CYKTable[i].clear(); // not sure if necessary
    CYKTable[i].resize(nrTerms);
  }
  terminalEntries.clear();
  CYKLine();
}

void CYKParser::CYKLine() {

  CYKLineBaseCase();
  
}

void CYKParser::CYKLineBaseCase() {

  for (int i=0; i < nrTerms; i++) { // for each terminal

    vector<Grammar::stringAndDouble> LHSs;
    myCFG->getLHSs(lineTerms[i], LHSs); // get all rules A_j --> terminal_i
    cout << "Nr LHS's for " << lineTerms[i] << ": " << LHSs.size() << endl;

    for (int j=0; j < LHSs.size(); j++) { // for each A_j(LHS), add it to table
      cout << "Processed LHS " << j << endl;
      vector<location> noBacks; // a terminal has no "backs"
      terminalEntries.push_back( tableEntry{ lineTerms[i], 1, noBacks, false });
      vector<location> backs; // but the terminal is the "back" of A_j
      backs.push_back(location{i, -1, -1});
      CYKTable[i][i].push_back(tableEntry { LHSs[j].first, LHSs[j].second, backs, true }); // true = backIsTerminal
      
    }

     // handle unaries
    bool added = true;
        int k=0;
        while( k < CYKTable[i][i].size()) { // for each entry in table (note CYKTable may grow inside loop!)
          while (added) {
            added = false;

            vector<Grammar::stringAndDouble> LHSsRec;
            tableEntry currentEntry = CYKTable[i][i][k];
            myCFG->getLHSs(currentEntry.nonTerm, LHSsRec); // get all rules B --> A

            for (int l=0; l < LHSsRec.size(); l++) { // for all B's
              //cout << "LHSsRec.size() = " << LHSsRec.size() << " for " << currentEntry.nonTerm << endl;
             // cout << l << ": " << LHSsRec[l].first << endl;
              Grammar::stringAndDouble lhsCurrentEntry = LHSsRec[l];

              // add the rule B--> A always, not just if their combined probability is higher
              double recProb = lhsCurrentEntry.second * currentEntry.prob;
              vector<location> backsRec;
              int curSize = CYKTable[i][i].size();
              backsRec.push_back(location{i,i,curSize}); // currentEntry
              CYKTable[i][i].push_back(tableEntry { lhsCurrentEntry.first, recProb, backsRec });  
                
              if (LHSsRec.size() > 0)
                added = true;
            }
            k++;
            cout << endl;
        }
      }
   }
}

void CYKParser::printCYKTable() {
  for (int i=0; i < nrTerms; i++) {
    for (int j=0; j < nrTerms; j++) {
      cout  << "At cell " << i << ", " << j << ": ";
      for (int k=0; k < CYKTable[i][j].size(); k++) {
        if (k > 0) {
          cout << ", ";
        }
        // print backs
        cout << CYKTable[i][j][k].nonTerm << "(" << CYKTable[i][j][k].prob << ")";
        cout << "(";
        for (int l=0; l < CYKTable[i][j][k].backs.size(); l++) {
          if (l > 0) {
            cout << ", ";
          }
          location loc = CYKTable[i][j][k].backs[l];
          cout << "back " << l << ": " << loc.i << " " << loc.j << " " << loc.k ;
        }
        cout << ")";
      }
      cout << endl;

    }
  }

}
void CYKParser::splitHelper(const string line, vector<string> &terms) {
  stringstream termStream(line);
  string term;
  while (getline(termStream, term, termDelimiter)) {
    if (!term.empty()) // we don't want the last space
      terms.push_back(term);
  }
}

vector<string> CYKParser::split(const string line) {
  vector<string> terms;
  splitHelper(line, terms);
  return terms;
}