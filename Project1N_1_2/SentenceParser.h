/* 
 * File:   CYKParser.h
 * Author: agnes
 *
 * Created on November 24, 2012, 2:02 PM
 */

#ifndef SENTENCEPARSER_H
#define	SENTENCEPARSER_H

using namespace std;

#include "Grammar.h"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <cmath>
#include <fstream>

#include "tree.hh" 
#include <boost/unordered_map.hpp> 



class SentenceParser {

  struct location;

    public:
      /* attributes and other stuff */
      
      // what seperates the terms/words in a sentence
      static const char termDelimiter = ' ';
      static const int  maxTerms = 16;
      
       /* constructors */
      SentenceParser(Grammar * aGrammar);
      SentenceParser(const SentenceParser &orig);
      virtual ~SentenceParser();

      /* methods */
      bool parseLine(const string givenLine);
      void printCYKTable();
      void printTOPs();
      void writeTOPs(string fileName);
      
      void reset();

      void recTree(tree<string>& myTree, tree<string>::iterator node, location locLHS, string lhsString);
      void makeTree();

      void getTree(tree<string>&);
      
    private:
      /* attributes and other stuff */
      Grammar * myCFG;

      struct location {
        int i;
        int j;
      };

      struct RHSEntry {
        pair<string, location> RHS1;
        pair<string, location> RHS2;
        double prob;
        bool backIsTerminal;
      };

      typedef pair<string, location> stringAndLocation;
      stringAndLocation emptyRHS;

      typedef boost::unordered_map<string, RHSEntry> tableEntryMap;
      typedef boost::unordered_map<string, RHSEntry>::iterator cellIterator;

      // will be changed per to-be-parsed line
      string line;
      vector<string> lineTerms;
      int nrTerms;      
      tableEntryMap * * CYKTable;
      vector <pair<string, RHSEntry> > allTOPs;
      tree<string> myTree;

      /* methods */
      void splitHelper(const string line, vector<string> &terms);
      vector<string> split(const string line);
      
      void CYKLine();
      void CYKLineBaseCase() ;
      void CYKLineRecursiveCase();
    
};


#endif	/* SENTENCEPARSER_H */

