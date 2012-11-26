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

#include <boost/unordered_map.hpp> 



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
      vector<string> lineTerms;
      int nrTerms;      
      tableEntryMap * * CYKTable2;
      

      /* methods */
      void splitHelper(const string line, vector<string> &terms);
      vector<string> split(const string line);
      
      void CYKLine();
      void CYKLineBaseCase() ;
      void CYKLineRecursiveCase();
    
};


#endif	/* CYKPARSER_H */

