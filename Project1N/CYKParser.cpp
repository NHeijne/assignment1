
#include "CYKParser.h"

CYKParser::CYKParser(Grammar * aGrammar) {
  myCFG = aGrammar;


}

CYKParser::CYKParser(const CYKParser &orig) {

}

CYKParser::~CYKParser() {

}

void CYKParser::reset() {
  nrTerms = 0;
  lineTerms.clear();
  CYKTable.clear();
  terminalEntries.clear();
}

void CYKParser::parseLine(const string line) {
  reset();

  lineTerms = split(line);
  //  for (int i = 0; i < lineTerms.size(); i++) {
  //    cout << "***" << lineTerms[i] << "***" << endl;
  //  }
  // reserve space in CYKTable
  nrTerms = lineTerms.size();
  CYKTable.resize(nrTerms);
  for (int i = 0; i < nrTerms; i++) {
    CYKTable[i].resize(nrTerms);
  }
  CYKLine();
}

void CYKParser::CYKLine() {
  CYKLineBaseCase();
  CYKLineRecursiveCase();
}

void CYKParser::CYKLineBaseCase() {

  for (int i = 0; i < nrTerms; i++) { // for each terminal

    vector<Grammar::stringAndDouble> LHSs;
    myCFG->getLHSs(lineTerms[i], LHSs); // get all rules A_j --> terminal_i
    //cout << "Nr LHS's for " << lineTerms[i] << ": " << LHSs.size() << endl;

    bool sAdded=  false;
    int firstAddedIndex  = CYKTable[i][i].size() ; // from which index to check for recursion
    for (int j = 0; j < LHSs.size(); j++) { // for each A_j(LHS), add it to table
      //cout << "Processed LHS " << j << endl;
      sAdded = true;
      
      vector<location> noBacks; // a terminal has no "backs"
      terminalEntries.push_back(tableEntry{lineTerms[i], 1, noBacks, false});
      vector<location> backs; // but the terminal is the "back" of A_j
      backs.push_back(location{i, -1, -1});
      CYKTable[i][i].push_back(tableEntry{LHSs[j].first, LHSs[j].second, backs, true}); // true = backIsTerminal
    }

    // handle unaries
     if (sAdded) {
      bool added = true;

      int k = firstAddedIndex;
      while (k < CYKTable[i][i].size()) { // for each entry in table (note CYKTable may grow inside loop)
        while (added) {
          added = false;
          vector<Grammar::stringAndDouble> LHSsRec;
          tableEntry currentEntry = CYKTable[i][i][k];
          myCFG->getLHSs(currentEntry.nonTerm, LHSsRec); // get all rules B --> A

          for (int l = 0; l < LHSsRec.size(); l++) { // for all B's
            // add the rule B--> A always, not just if their combined probability is higher
            Grammar::stringAndDouble lhsCurrentEntry = LHSsRec[l];
            double recProb = lhsCurrentEntry.second * currentEntry.prob;
            vector<location> backsRec;
            backsRec.push_back(location{i, i, k}); // k = index currentEntry
            CYKTable[i][i].push_back(tableEntry{lhsCurrentEntry.first, recProb, backsRec});
          }
          if (LHSsRec.size() > 0) { // if some recursive nonterm was added
            added = true;
          }
          else {
            added = false;
            k = CYKTable[i][i].size(); // stop the loop above this one
          }
          k++;
          //cout << endl;
        }
      }
     }
  }
}

void CYKParser::CYKLineRecursiveCase() {

  for (int span = 2; span <= nrTerms; span++) {
    for (int begin = 1; begin <= (nrTerms - span) + 1; begin++) {
      int end = (begin + span) - 1;
      for (int split = begin; split <= (end - 1); split++) {

        cout << "begin: " << begin - 1 << " end: " << end-1 << endl;
        // we do these indices minus 1 because or table starts at index 0
        vector<tableEntry> Bs = CYKTable[begin - 1][split - 1];
        vector<tableEntry> Cs = CYKTable[split][end - 1];

        // this is needed for recursive case
        bool sAdded=  false;
        int firstAddedIndex  = CYKTable[begin - 1][end - 1].size() ; // from which index to check for recursion

        for (int b_i = 0; b_i < Bs.size(); b_i++) {
          for (int c_i = 0; c_i < Cs.size(); c_i++) {

            vector<Grammar::stringAndDouble> As;
            string RHS1 = Bs[b_i].nonTerm;
            string RHS2 = Cs[c_i].nonTerm;
            myCFG->getLHSs(RHS1 + " " + RHS2, As);
            for (int a_i = 0; a_i < As.size(); a_i++) {
              sAdded = true;             
              
              double prob = As[a_i].second * Bs[b_i].prob * Cs[c_i].prob;
              vector <location> backsA_i;
              location locC_i = {split, end - 1, c_i};
              location locB_i = {begin - 1, split - 1, b_i};
              backsA_i.push_back(locB_i);
              backsA_i.push_back(locC_i);
              tableEntry tEa = {As[a_i].first, prob, backsA_i, false};
              CYKTable[begin - 1][end - 1].push_back(tEa);
              
              //cout << "added " << As[a_i].first << " --> " << RHS1 + " " + RHS2 << endl;
            }
          }
        }
        // handle unaries
        /* actually should only be necessary for cell (0, nrTerms-1)!!
           however we could, just to be sure, do for every cell ?
         */
        if (sAdded && (begin - 1) ==0 && (end-1) == nrTerms-1) { // if some nonterm was added in that cell this iteration
          cout << "sAdded = true, index is " << firstAddedIndex << " table size is " <<  CYKTable[begin - 1][end - 1].size() << endl;
          bool added = true;
          int k = firstAddedIndex;

          while (k < CYKTable[begin - 1][end - 1].size()) { // for each entry in table (note CYKTable may grow inside loop)
            while (added) {
              added = false;
              vector<Grammar::stringAndDouble> LHSsRec;
              tableEntry currentEntry = CYKTable[begin - 1][end - 1][k];
              myCFG->getLHSs(currentEntry.nonTerm, LHSsRec); // get all rules B --> A

              for (int l = 0; l < LHSsRec.size(); l++) { // for all B's
                // add the rule B--> A always, not just if their combined probability is higher
                Grammar::stringAndDouble lhsCurrentEntry = LHSsRec[l];                
                double recProb = lhsCurrentEntry.second * currentEntry.prob;
                vector<location> backsRec;
                backsRec.push_back(location{begin - 1, end - 1, k}); // k = index currentEntry
                CYKTable[begin - 1][end - 1].push_back(tableEntry{lhsCurrentEntry.first, recProb, backsRec});
              }
              if (LHSsRec.size() > 0) { // if some recursive nonterm was added
                added = true;
              }
              else {
                added = false;
                k = CYKTable[begin - 1][end - 1].size(); // stop the loop above this one
              }
              k++;
            }
          }
        }
      }
    }
  }
}

void CYKParser::printCYKTable() {
  for (int i = 0; i < nrTerms; i++) {
    for (int j = 0; j < nrTerms; j++) {
      cout << "At cell " << i << ", " << j << ": ";
      for (int k = 0; k < CYKTable[i][j].size(); k++) {
        if (k > 0) {
          cout << ", ";
        }
        // print backs
        cout << CYKTable[i][j][k].nonTerm << "(" << CYKTable[i][j][k].prob << ")";
        cout << "(";
        for (int l = 0; l < CYKTable[i][j][k].backs.size(); l++) {
          if (l > 0) {
            cout << ", ";
          }
          location loc = CYKTable[i][j][k].backs[l];
          cout << "back " << l << ": " << loc.i << " " << loc.j << " " << loc.k;
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
