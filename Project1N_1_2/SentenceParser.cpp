
#include "SentenceParser.h"

SentenceParser::SentenceParser(Grammar * aGrammar) {
  myCFG = aGrammar;
  CYKTable = NULL;

  location n = {-1, -1};
  emptyRHS = stringAndLocation({"", n});

}

SentenceParser::SentenceParser(const SentenceParser &orig) {

}

SentenceParser::~SentenceParser() {
  for (int i = 0; i < nrTerms; i++) {
    for (int j = 0; j < nrTerms; j++) {
      tableEntryMap().swap(CYKTable[i][j]); // deletes content from memory
    }
    delete[] CYKTable[i];
  }
  delete[] CYKTable;
}

void SentenceParser::reset() {
  myTree.clear();
  allTOPs.clear();

  if (CYKTable != NULL) {
    for (int i = 0; i < nrTerms; i++) {
      for (int j = 0; j < nrTerms; j++) {
        tableEntryMap().swap(CYKTable[i][j]);
      }
      delete[] CYKTable[i];
    }
    delete[] CYKTable;
  }
  CYKTable = NULL;

  nrTerms = 0;
  lineTerms.clear();
}

bool SentenceParser::parseLine(const string givenLine) {
  reset();
  line = givenLine;
  lineTerms = split(line);
  nrTerms = lineTerms.size();


  if (nrTerms > maxTerms){
    allTOPs.push_back(pair<string, RHSEntry>(Grammar::nonTerminalSymbol+"TOP", RHSEntry({emptyRHS, emptyRHS, 1, false})));
    return false;
  }

  CYKTable = new tableEntryMap*[nrTerms];
  for (int i = 0; i < nrTerms; i++) {
    CYKTable[i] = new tableEntryMap[nrTerms];
  }
  CYKLine();
  return true;
}

void SentenceParser::CYKLine() {

  CYKLineBaseCase();
  if (nrTerms > 1) {
    CYKLineRecursiveCase();
  }
}

void SentenceParser::CYKLineBaseCase() {

  for (int i = 0; i < nrTerms; i++) { // for each terminal

    bool firstTerminal = (i==0);
    vector<Grammar::stringAndDouble> LHSs;

    myCFG->getLHSs(lineTerms[i], LHSs, true, firstTerminal); // true means it's a terminal. get all rules A_j --> terminal_i
    //cout << "Nr LHS's for " << lineTerms[i] << ": " << LHSs.size() << endl;

    bool sAdded = false;

    int LHSsSize = LHSs.size();
    for (int j = 0; j < LHSsSize; j++) { // for each A_j(LHS), add it to table
      //cout << "Processed LHS " << j << endl;
      sAdded = true;
      location back1 = (location{i, -1}); // the terminal is the "back"
      CYKTable[i][i][LHSs[j].first] = RHSEntry({stringAndLocation(lineTerms[i], back1), emptyRHS, LHSs[j].second, true}); // true = backIsTerminal
    }

    // handle unaries
    if (sAdded) {
      int k = 0;
      while (k < LHSsSize) { // for each added entry to this cell

        vector<Grammar::stringAndDouble> LHSsRec;
        myCFG->getLHSs(LHSs[k].first, LHSsRec); // get all rules A --> B

        int LHSsRecSize = LHSsRec.size();
        for (int l = 0; l < LHSsRecSize; l++) { // for all A's

          double prob = LHSsRec[l].second + CYKTable[i][i][LHSs[k].first].prob;
     
          location back1 = (location{i, i}); 
          RHSEntry rightEntry =  RHSEntry({stringAndLocation({LHSs[k].first, back1}), emptyRHS, prob, false});
          cellIterator findA_i = CYKTable[i][i].find(LHSsRec[l].first);
          if (findA_i != CYKTable[i][i].end()) {
            if (prob >  findA_i->second.prob) {
              findA_i->second = rightEntry;
              LHSs.push_back(Grammar::stringAndDouble(LHSsRec[l].first, prob));
              LHSsSize++;
            }
          }
          else {
            CYKTable[i][i][LHSsRec[l].first] = rightEntry;
          }
        }
        k++;
      }
    }
  }
}

void SentenceParser::CYKLineRecursiveCase() {

  for (int span = 2; span <= nrTerms; span++) {
    for (int begin = 1; begin <= (nrTerms - span) + 1; begin++) {
      int end = (begin + span) - 1;
      for (int split = begin; split <= (end - 1); split++) {

        //cout << "begin: " << begin - 1 << " end: " << end - 1 << endl;
        // we do these indices minus 1 because or table starts at index 0
        tableEntryMap Bs = CYKTable[begin - 1][split - 1];
        tableEntryMap Cs = CYKTable[split][end - 1];

        // this is needed for recursive case
        bool sAdded = false;

        cellIterator iteratorB;
        cellIterator iteratorC;
        for (iteratorB = Bs.begin(); iteratorB != Bs.end(); iteratorB++) {
          for (iteratorC = Cs.begin(); iteratorC != Cs.end(); iteratorC++) {
            vector<Grammar::stringAndDouble> addedToCell;

            vector<Grammar::stringAndDouble> As;
            string RHS1 = iteratorB->first;
            string RHS2 = iteratorC->first; // Cs[c_i].nonTerm;
            myCFG->getLHSs(RHS1 + " " + RHS2, As); // get all rules A --> B C
            int AsSize = As.size();

            if (iteratorB->first == "nt_NNP" && iteratorC->first == "nt_NNP") {
              cout << " entry " << begin-1 << ", " << end-1 << endl;
            }
            for (int a_i = 0; a_i < AsSize; a_i++) {
              sAdded = true;

              double prob = As[a_i].second + iteratorB->second.prob + iteratorC->second.prob;
              if (iteratorB->first == "nt_NNP" && iteratorC->first == "nt_NNP") {
                 cout << " prob for " << As[a_i].first << " = " << As[a_i].second + iteratorB->second.prob + iteratorC->second.prob << endl;
              }

              location locC_i = {split, end - 1};
              location locB_i = {begin - 1, split - 1};

              RHSEntry rightEntry = {stringAndLocation({iteratorB->first, locB_i}), stringAndLocation( {iteratorC->first, locC_i}), prob, false};

              cellIterator findA_i = CYKTable[begin - 1][end - 1].find(As[a_i].first);
              if (findA_i != CYKTable[begin - 1][end - 1].end()) { // element exists already
                if (findA_i->second.prob < prob /* As[a_i].second*/) { // if probability is higher
                   if (iteratorB->first == "nt_NNP" && iteratorC->first == "nt_NNP") {
                      cout << " prob for " << As[a_i].first << " is higher than for " << findA_i->first << ", " << prob << " is higher than " << findA_i->second.prob << endl;
                   }
                  findA_i->second = rightEntry; // swap right hand side entry
                }
              }
              else {
                 if (iteratorB->first == "nt_NNP" && iteratorC->first == "nt_NNP") {
                   cout <<  As[a_i].first << " didnt  exist yet "<< endl;
                 }
                CYKTable[begin - 1][end - 1][As[a_i].first] = rightEntry;
                addedToCell.push_back(Grammar::stringAndDouble({As[a_i].first, prob}));
              }

            }
            // recursive case
            if ((begin - 1) == 0 && ((end - 1) == (nrTerms - 1)) && sAdded) {
              int k = 0;
              int addedToCellSize = addedToCell.size();
              while (k < addedToCellSize) { // for each added entry to this cell
                vector<Grammar::stringAndDouble> LHSsRec;

                myCFG->getLHSs(addedToCell[k].first, LHSsRec); // get all rules B --> A

                int LHSsRecSize = LHSsRec.size();
                for (int l = 0; l < LHSsRecSize; l++) { // for all B's

				          Grammar::stringAndDouble lhsCurrentEntry = LHSsRec[l];

                  double recProb = lhsCurrentEntry.second + CYKTable[begin - 1][end - 1][addedToCell[k].first].prob;
                  double prob = addedToCell[k].second;
                  //if (recProb > prob) {
                    prob = recProb;
                 // }
                  location back1 = (location{begin - 1, end - 1});
                  RHSEntry rightEntry = RHSEntry({stringAndLocation({addedToCell[k].first, back1}), emptyRHS, prob, false});

                  cellIterator findAdded_i = CYKTable[begin - 1][end - 1].find(LHSsRec[l].first);

                  if (findAdded_i != CYKTable[begin - 1][end - 1].end()) { // element exists already
                    if (findAdded_i->second.prob < prob /*LHSsRec[l].second */) { // if probability is higher
                      findAdded_i->second = rightEntry; // swap right hand side entry
                    }
                  }
                  else {
                    CYKTable[begin - 1][end - 1][LHSsRec[l].first] = rightEntry;
                    addedToCell.push_back(Grammar::stringAndDouble({LHSsRec[l].first, prob}));
                    addedToCellSize++;
                  }
                  if (LHSsRec[l].first == Grammar::nonTerminalSymbol + "TOP") {
                    allTOPs.push_back(pair<string, RHSEntry>(LHSsRec[l].first,rightEntry ));
                  }
                }
                k++;
              }
            }
          }
        }
      }
    }
  }
}

void SentenceParser::printCYKTable() {
  cellIterator iterator;

  cout << "Table: " << endl;
  for (int i = 0; i < nrTerms; i++) {
    for (int j = 0; j < nrTerms; j++) {
      cout << "At cell " << i << ", " << j << ": ";
      for (iterator = CYKTable[i][j].begin(); iterator != CYKTable[i][j].end(); iterator++) {
        if (iterator != CYKTable[i][j].begin()) {
          cout << ", ";
        }
        cout << iterator->first << "(" << iterator->second.prob << ")";
         // print backs
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

void SentenceParser::printTOPs() {
  cout << "TOPs: " << endl;
  int allTOPsSize = allTOPs.size();
  for (int i=0; i< allTOPsSize; i++) {
    cout << allTOPs[i].first << "(" << allTOPs[i].second.prob << ")";
    // print backs
    cout << "(";
    cout << "(" << allTOPs[i].second.RHS1.first << " " << allTOPs[i].second.RHS1.second.i << " " << allTOPs[i].second.RHS1.second.j << ")";
    if ( allTOPs[i].second.RHS2.first != "") {
      cout << "(" << allTOPs[i].second.RHS2.first << " " << allTOPs[i].second.RHS2.second.i << " " << allTOPs[i].second.RHS2.second.j << ")";
    }
    cout << ")" << endl;
  }
}

void SentenceParser::writeTOPs(string fileName) {

  ofstream outputFile;
  outputFile.open(fileName.c_str());

  outputFile << "TOPs for \"" << line << "\"" << endl;
  int allTOPsSize = allTOPs.size();
  for (int i=0; i< allTOPsSize; i++) {
    outputFile << allTOPs[i].first << "(" << allTOPs[i].second.prob << ")";
    // print backs
    outputFile << "(";
    outputFile << "(" << allTOPs[i].second.RHS1.first << " " << allTOPs[i].second.RHS1.second.i << " " << allTOPs[i].second.RHS1.second.j << ")";
    if ( allTOPs[i].second.RHS2.first != "") {
      outputFile << "(" << allTOPs[i].second.RHS2.first << " " << allTOPs[i].second.RHS2.second.i << " " << allTOPs[i].second.RHS2.second.j << ")";
    }
    outputFile << ")" << endl ;
  }
  outputFile << endl;
  outputFile.close();
}

void SentenceParser::splitHelper(const string line, vector<string> &terms) {
  stringstream termStream(line);
  string term;
  while (getline(termStream, term, termDelimiter)) {
    if (!term.empty()) // we don't want the last space
      terms.push_back(term);
  }
}

vector<string> SentenceParser::split(const string line) {
  vector<string> terms;
  splitHelper(line, terms);
  return terms;
}

void SentenceParser::makeTree() {

  tree<string>::iterator node = myTree.begin();
  location locLHS = {0, nrTerms-1};
  string lhsString = Grammar::nonTerminalSymbol + "TOP" ;
  node = myTree.insert(node, lhsString);
  recTree(myTree, node, locLHS, lhsString);
  
}

void SentenceParser::recTree(tree<string>& myTree, tree<string>::iterator node, location locLHS, string lhsString){
  //cout << lhsString << endl;
  RHSEntry thisRHSentry = CYKTable[locLHS.i][locLHS.j][lhsString];
  //cout << ", " << thisRHSentry.RHS1.first << " & " << thisRHSentry.RHS2.first << endl;
  //system("pause");

  // first rhs
  tree<string>::iterator node2 = myTree.append_child(node, thisRHSentry.RHS1.first);

  if (! thisRHSentry.backIsTerminal){ // stop condition
      recTree(myTree, node2, thisRHSentry.RHS1.second, thisRHSentry.RHS1.first);
  }

  if (!(thisRHSentry.RHS2.first == "")) { // second rhs
    node2 = myTree.append_child(node, thisRHSentry.RHS2.first);
    recTree(myTree, node2, thisRHSentry.RHS2.second, thisRHSentry.RHS2.first);
  }
}

void SentenceParser::getTree(tree<string>& theTree) {

  if (myTree.empty()) {
    makeTree();
  }
  theTree = myTree;

}