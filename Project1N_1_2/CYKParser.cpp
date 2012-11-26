
#include "CYKParser.h"



CYKParser::CYKParser(Grammar * aGrammar)  {
  myCFG = aGrammar;
  CYKTable2 = NULL;

}

CYKParser::CYKParser(const CYKParser &orig) {

}

CYKParser::~CYKParser() {
  for (int i = 0; i < nrTerms; i++) {
    for (int j = 0; j < nrTerms; j++) {
      tableEntryMap ().swap(CYKTable2[i][j]); // deletes content from memory
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
        tableEntryMap ().swap(CYKTable2[i][j]);
      }
      delete[] CYKTable2[i];
    }
    delete[] CYKTable2;
  }

  nrTerms = 0;
  lineTerms.clear();
  //CYKTable.clear();
 



}

void CYKParser::parseLine(const string line) {
  reset();

  lineTerms = split(line);
  //  for (int i = 0; i < lineTerms.size(); i++) {
  //    cout << "***" << lineTerms[i] << "***" << endl;
  //  }
  // reserve space in CYKTable
  nrTerms = lineTerms.size();

  CYKTable2 = new tableEntryMap*[nrTerms];
  for (int i = 0; i < nrTerms; i++) {
    CYKTable2[i] = new tableEntryMap[nrTerms];
  }

  //  CYKTable.resize(nrTerms);
  //  for (int i = 0; i < nrTerms; i++) {
  //    CYKTable[i].resize(nrTerms);
  //  }
  CYKLine();
}

void CYKParser::CYKLine() {
  location n = {-1, -1};
  emptyRHS = stringAndLocation({"", n});
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

      location back1 = (location{i, -1}); // but the terminal is the "back" of A_j
      CYKTable2[i][i][LHSs[j].first] = RHSEntry({stringAndLocation(lineTerms[i], back1), emptyRHS, LHSs[j].second , true}); // true = backIsTerminal
    }

    // handle unaries
    if (sAdded) {
      int cellSize = CYKTable2[i][i].size();
      int k = 0;

      while (k < LHSsSize) { // for each added entry to this cell

        vector<Grammar::stringAndDouble> LHSsRec;
       
        myCFG->getLHSs(LHSs[k].first, LHSsRec); // get all rules B --> A

        int LHSsRecSize = LHSsRec.size();
        for (int l = 0; l < LHSsRecSize; l++) { // for all B's
          // add the rule B--> A always, not just if their combined probability is higher
          Grammar::stringAndDouble lhsCurrentEntry = LHSsRec[l];
          double recProb = lhsCurrentEntry.second *  CYKTable2[i][i][LHSs[k].first].prob;
          double prob = LHSs[k].second;
          if (recProb >  prob) {
            prob = recProb;
          }
          location back1 = (location{i, i}); // k = index currentEntry
          CYKTable2[i][i][lhsCurrentEntry.first] = RHSEntry({stringAndLocation({LHSs[k].first, back1}), emptyRHS, recProb, false});  //(tableEntry{lhsCurrentEntry.first, recProb, back1,      {-1, -1, -1}, false});
          LHSs.push_back(Grammar::stringAndDouble(lhsCurrentEntry.first, prob));
          LHSsSize++;
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
        tableEntryMap Bs = CYKTable2[begin - 1][split - 1];
        tableEntryMap Cs = CYKTable2[split][end - 1];

        // this is needed for recursive case
        bool sAdded = false;
        int firstAddedIndex = CYKTable2[begin - 1][end - 1].size(); // from which index to check for recursion


        cellIterator iteratorB;
        cellIterator iteratorC;


        for (iteratorB = Bs.begin(); iteratorB != Bs.end(); iteratorB++) {
          for (iteratorC = Cs.begin(); iteratorC != Cs.end(); iteratorC++) {

            vector<Grammar::stringAndDouble> As;
            string RHS1 = iteratorB->first;
            string RHS2 = iteratorC->first; // Cs[c_i].nonTerm;
            myCFG->getLHSs(RHS1 + " " + RHS2, As); // get all rules A --> B C

            int AsSize = As.size();
            for (int a_i = 0; a_i < AsSize; a_i++) {
              sAdded = true;
              double prob = As[a_i].second * iteratorB->second.prob * iteratorC->second.prob;
              
              location locC_i = {split, end - 1};
              location locB_i = {begin - 1, split - 1};
              
              RHSEntry rightEntry = {stringAndLocation({iteratorB->first, locB_i}), stringAndLocation({iteratorC->first, locC_i}), prob, false};
              
              cellIterator findA_i = CYKTable2[begin - 1][end - 1].find(As[a_i].first);
              if (findA_i != CYKTable2[begin - 1][end - 1].end()) { // element exists already
                if (findA_i->second.prob <  As[a_i].second) {
                  CYKTable2[begin - 1][end - 1].erase(findA_i); // delete element
                  CYKTable2[begin - 1][end - 1][As[a_i].first] = rightEntry;
                }
              }
              else {
                CYKTable2[begin - 1][end - 1][As[a_i].first] = rightEntry;
              }

              //cout << "added " << As[a_i].first << " --> " << RHS1 + " " + RHS2 << endl;

            }
          }
        }
//        // handle unaries
//        /* actually should only be necessary for cell (0, nrTerms-1)!!
//           however we could, just to be sure, do for every cell ?
//         */
//        if (sAdded && (begin - 1) == 0 && (end - 1) == nrTerms - 1) { // if some nonterm was added in that cell this iteration
//          bool added = true;
//          int k = firstAddedIndex;
//          int cellSize = CYKTable2[begin - 1][end - 1].size();
//          cout << "sAdded = true, index is " << firstAddedIndex << " table size is " << cellSize << endl;
//
//          while (k < cellSize) { // for each entry in table (note CYKTable may grow inside loop)
//
//            vector<Grammar::stringAndDouble> LHSsRec;
//            tableEntry currentEntry = CYKTable2[begin - 1][end - 1][k];
//            myCFG->getLHSs(currentEntry.nonTerm, LHSsRec); // get all rules B --> A
//
//            int LHSsRecSize = LHSsRec.size();
//            for (int l = 0; l < LHSsRecSize; l++) { // for all B's
//              // add the rule B--> A
//              Grammar::stringAndDouble lhsCurrentEntry = LHSsRec[l];
//              double recProb = lhsCurrentEntry.second * currentEntry.prob;
//
//              location back1 = (location{begin - 1, end - 1, k}); // k = index currentEntry
//              CYKTable2[begin - 1][end - 1].push_back(tableEntry{lhsCurrentEntry.first, recProb, back1,
//                {-1, -1, -1}, false});
//              cellSize++;
//            }
//            k++;
 //         }
 //       }
      }
    }
  }
}

void CYKParser::printCYKTable() {
  cellIterator iterator;

  for (int i = 0; i < nrTerms; i++) {
    for (int j = 0; j < nrTerms; j++) {
      cout << "At cell " << i << ", " << j << ": ";

      for (iterator = CYKTable2[i][j].begin(); iterator != CYKTable2[i][j].end(); iterator++) {
      
        if (iterator != CYKTable2[i][j].begin()) {
          cout << ", ";
        }
        // print backs        
        cout << iterator->first << "(" << iterator->second.prob << ")";
        cout << "(";
        cout << "(" << iterator->second.RHS1.first << " " << iterator->second.RHS1.second.i << " " << iterator->second.RHS1.second.j << ")";
        if (iterator->second.RHS2.first != "") {
          cout << "(" << iterator->second.RHS2.first << " " << iterator->second.RHS2.second.i << " " << iterator->second.RHS2.second.j << ")";
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
