/* 
 * File:   Grammar.h
 * Author: agnes
 *
 * Created on November 20, 2012, 6:15 PM
 */

#ifndef GRAMMAR_H
#define	GRAMMAR_H

#include <iostream>
#include <cstdio>
#include <utility>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cassert>
#include <cstdlib>

using namespace std;

class Grammar {
     
  public:
    /* attributes and other stuff */
    typedef pair <string, double> stringAndDouble;
    typedef pair <string, stringAndDouble> tableKeyAndValue;

    /* constructors */
    Grammar(string treeBankFile);
    Grammar(const Grammar& orig);
    virtual ~Grammar();
    
    /* methods */
    vector<stringAndDouble> getRHS(string LHS);
    vector<stringAndDouble> getLHS(string RHS);
    
    void fillR2lTable() ;
    void readGrammar();

private:
    /* attributes and other stuff */
    multimap<string, stringAndDouble> l2rTable;
    multimap<string, stringAndDouble> r2lTable; // inverse ordering from what is "natural"

    pair<multimap<string, stringAndDouble>::iterator, multimap<string, stringAndDouble>::iterator> ruleRangeIterator;
    multimap<string, stringAndDouble>::iterator ruleIterator;

    string treeBankFileName;

    /* methods */

    void parseLine(string line);

};

#endif	/* GRAMMAR_H */

