
#include "CYKParser.h"

CYKParser::CYKParser(Grammar * aGrammar) {
  myCFG = aGrammar;
  CYKTable2 = NULL;

}

CYKParser::CYKParser(const CYKParser &orig) {

}

CYKParser::~CYKParser() {
  for (int i = 0; i < nrTerms; i++) {
    for (int j = 0; j < nrTerms; j++) {
      vector<tableEntry > ().swap(CYKTable2[i][j]); // deletes content from memory
    }
    delete[] CYKTable2[i];
  }
  delete[] CYKTable2;
}

void CYKParser::reset() {
  nrTerms = 2;

  if (CYKTable2 != NULL) {
    for (int i = 0; i < nrTerms; i++) {
      for (int j = 0; j < nrTerms; j++) {
        vector<tableEntry > ().swap(CYKTable2[i][j]);
      }
      delete[] CYKTable2[i];
    }
    delete[] CYKTable2;
  }

  nrTerms = 0;
  lineTerms.clear();
  //CYKTable.clear();
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

  CYKTable2 = new vector<tableEntry>*[nrTerms];
  for (int i = 0; i < nrTerms; i++) {
    CYKTable2[i] = new vector<tableEntry>[nrTerms];
    for (int j = 0; j < nrTerms; j++) {
      if (j >= 1)
        CYKTable2[i][j].reserve(10000); // reserve space for rules
    }
  }

  //  CYKTable.resize(nrTerms);
  //  for (int i = 0; i < nrTerms; i++) {
  //    CYKTable[i].resize(nrTerms);
  //  }
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

    bool sAdded = false;
    int firstAddedIndex = CYKTable2[i][i].size(); // from which index to check for recursion
    int LHSsSize = LHSs.size();
    for (int j = 0; j < LHSsSize; j++) { // for each A_j(LHS), add it to table
      //cout << "Processed LHS " << j << endl;
      sAdded = true;

      terminalEntries.push_back(tableEntry{lineTerms[i], 1,
        {-1, -1, -1},
        {-1, -1, -1}, false});
      location back1 = (location{i, -1, -1}); // but the terminal is the "back" of A_j
      CYKTable2[i][i].push_back(tableEntry{LHSs[j].first, LHSs[j].second, back1,
        {-1, -1, -1}, true}); // true = backIsTerminal
    }

    // handle unaries
    if (sAdded) {
      int cellSize = CYKTable2[i][i].size();
      int k = firstAddedIndex;

      while (k < cellSize) { // for each entry in table (note CYKTable may grow inside loop)

        vector<Grammar::stringAndDouble> LHSsRec;
        tableEntry currentEntry = CYKTable2[i][i][k];
        myCFG->getLHSs(currentEntry.nonTerm, LHSsRec); // get all rules B --> A

        int LHSsRecSize = LHSsRec.size();
        for (int l = 0; l < LHSsRecSize; l++) { // for all B's
          // add the rule B--> A always, not just if their combined probability is higher
          Grammar::stringAndDouble lhsCurrentEntry = LHSsRec[l];
          double recProb = lhsCurrentEntry.second * currentEntry.prob;
          location back1 = (location{i, i, k}); // k = index currentEntry
          CYKTable2[i][i].push_back(tableEntry{lhsCurrentEntry.first, recProb, back1,
            {-1, -1, -1}, false});
          cellSize++;
        }
        k++;
        //cout << endl;
      }
    }
  }
}

void CYKParser::CYKLineRecursiveCase() {

  for (int span = 2; span <= nrTerms; span++) {
    for (int begin = 1; begin <= (nrTerms - span) + 1; begin++) {
      int end = (begin + span) - 1;
      for (int split = begin; split <= (end - 1); split++) {

        cout << "begin: " << begin - 1 << " end: " << end - 1 << endl;
        // we do these indices minus 1 because or table starts at index 0
        vector<tableEntry> Bs = CYKTable2[begin - 1][split - 1];
        vector<tableEntry> Cs = CYKTable2[split][end - 1];

        // this is needed for recursive case
        bool sAdded = false;
        int firstAddedIndex = CYKTable2[begin - 1][end - 1].size(); // from which index to check for recursion

        int BsSize = Bs.size();
        int CsSize = Cs.size();

        for (int b_i = 0; b_i < BsSize; b_i++) {
          for (int c_i = 0; c_i < CsSize; c_i++) {

            vector<Grammar::stringAndDouble> As;
            string RHS1 = Bs[b_i].nonTerm;
            string RHS2 = Cs[c_i].nonTerm;
            myCFG->getLHSs(RHS1 + " " + RHS2, As); // get all rules A --> B C

            int AsSize = As.size();
            for (int a_i = 0; a_i < AsSize; a_i++) {
              sAdded = true;
              double prob = As[a_i].second * Bs[b_i].prob * Cs[c_i].prob;

              vector <location> backsA_i;
              location locC_i = {split, end - 1, c_i};
              location locB_i = {begin - 1, split - 1, b_i};
              backsA_i.push_back(locB_i);
              backsA_i.push_back(locC_i);
              tableEntry tEa = {As[a_i].first, prob, locB_i, locC_i, false};
              CYKTable2[begin - 1][end - 1].push_back(tEa);

              //cout << "added " << As[a_i].first << " --> " << RHS1 + " " + RHS2 << endl;

            }
          }
        }
        // handle unaries
        /* actually should only be necessary for cell (0, nrTerms-1)!!
           however we could, just to be sure, do for every cell ?
         */
        if (sAdded && (begin - 1) == 0 && (end - 1) == nrTerms - 1) { // if some nonterm was added in that cell this iteration
          bool added = true;
          int k = firstAddedIndex;
          int cellSize = CYKTable2[begin - 1][end - 1].size();
          cout << "sAdded = true, index is " << firstAddedIndex << " table size is " << cellSize << endl;

          while (k < cellSize) { // for each entry in table (note CYKTable may grow inside loop)

            vector<Grammar::stringAndDouble> LHSsRec;
            tableEntry currentEntry = CYKTable2[begin - 1][end - 1][k];
            myCFG->getLHSs(currentEntry.nonTerm, LHSsRec); // get all rules B --> A

            int LHSsRecSize = LHSsRec.size();
            for (int l = 0; l < LHSsRecSize; l++) { // for all B's
              // add the rule B--> A
              Grammar::stringAndDouble lhsCurrentEntry = LHSsRec[l];
              double recProb = lhsCurrentEntry.second * currentEntry.prob;

              location back1 = (location{begin - 1, end - 1, k}); // k = index currentEntry
              CYKTable2[begin - 1][end - 1].push_back(tableEntry{lhsCurrentEntry.first, recProb, back1,
                {-1, -1, -1}, false});
              cellSize++;
            }
            k++;
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
      for (int k = 0; k < CYKTable2[i][j].size(); k++) {
        if (k > 0) {
          cout << ", ";
        }
        // print backs        
        cout << CYKTable2[i][j][k].nonTerm << "(" << CYKTable2[i][j][k].prob << ")";
        cout << "(";
        if (CYKTable2[i][j][k].back1.i >= 0) {
          
          cout << "(" << CYKTable2[i][j][k].back1.i << " " << CYKTable2[i][j][k].back1.j << " " << CYKTable2[i][j][k].back1.k << ")";
        }
        if (CYKTable2[i][j][k].back2.i >= 0) {
          cout << ",";
          cout << "(" << CYKTable2[i][j][k].back1.i << " " << CYKTable2[i][j][k].back1.j << " " << CYKTable2[i][j][k].back1.k << ")";
        }
        cout << ")";
      }
      cout << endl;
    }
    cout << endl;


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
