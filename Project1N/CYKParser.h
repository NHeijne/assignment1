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




class CYKParser {

    public:
      /* attributes and other stuff */
      
      // what seperates the terms/words in a sentence
      static const char termDelimiter = ' ';

       /* constructors */
      CYKParser(Grammar * aGrammar);
      CYKParser(const CYKParser &orig);
      virtual ~CYKParser();

      /* methods */
      void parseLine(const string line);
      void printCYKTable();
      void reset();

    private:
      /* attributes and other stuff */
      Grammar * myCFG;

      struct location {
        int i;
        int j;
        int k;
      };

      struct tableEntry {
        string nonTerm;
        double prob;
        location back1;
        location back2;
        bool backIsTerminal;
      };
      
      // will be changed per to-be-parsed line
      vector<string> lineTerms;
      int nrTerms;      
     // vector<vector<vector<tableEntry> > > CYKTable; //3D vector
      vector<tableEntry> terminalEntries;

      vector<tableEntry> * * CYKTable2;

      /* methods */
      void splitHelper(const string line, vector<string> &terms);
      vector<string> split(const string line);
      
      void CYKLine();
      void CYKLineBaseCase() ;
      void CYKLineRecursiveCase();
    
};


#endif	/* CYKPARSER_H */

