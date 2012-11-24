/* 
 * File:   CYKParser.h
 * Author: agnes
 *
 * Created on November 24, 2012, 2:02 PM
 */

#ifndef CYKPARSER_H
#define	CYKPARSER_H

using namespace std;

#include "grammar.h"

#include <vector>
#include <string>
#include <sstream>
#include <iostream>




class CYKParser {

    public:
      /* attributes and other stuff */
      
      // what seperates the terms/words in a sentence
      static const char termDelimiter = ' ';

      vector<vector<vector<string>>> table; //3D vector

       /* constructors */
      CYKParser(Grammar * aGrammar);
      CYKParser(const CYKParser &orig);
      virtual ~CYKParser();

      /* methods */
      void parseLine(const string line);

      

    private:
      /* attributes and other stuff */
      Grammar * myCFG;

      struct tableEntry {
        string nonTerm;
        double prob;
        vector <tableEntry*> backs;
      };

      /* methods */
      void splitHelper(const string line, vector<string> &terms);
      vector<string> split(const string line);

    
};


#endif	/* CYKPARSER_H */

