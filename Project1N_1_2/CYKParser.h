/* 
 * File:   CYKParser.h
 * Author: agnes
 *
 * Created on November 24, 2012, 2:02 PM
 */

#ifndef CYKPARSER_H
#define	CYKPARSER_H

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



class CYKParser {

  struct location;

    public:
      /* attributes and other stuff */
      
      // what seperates the terms/words in a sentence
      static const char termDelimiter = ' ';
      static const int  maxTerms = 15;
      
       /* constructors */
      CYKParser(Grammar * aGrammar);
      CYKParser(const CYKParser &orig);
      virtual ~CYKParser();

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
      tableEntryMap * * CYKTable2;
      vector <pair<string, RHSEntry> > allTOPs;
      tree<string> myTree;

      /* methods */
      void splitHelper(const string line, vector<string> &terms);
      vector<string> split(const string line);
      
      void CYKLine();
      void CYKLineBaseCase() ;
      void CYKLineRecursiveCase();
    
};


#endif	/* CYKPARSER_H */

