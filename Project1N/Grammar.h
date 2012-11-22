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
#include <stack>
#include <cstring>

using namespace std;

/***
 * We use std::multimap and not std::unordered_multimap because:
 * "unordered_map containers are faster than map containers to access individual 
 * elements by their key, although they are generally less efficient for range 
 * iteration through a subset of their elements. "
 * (source: http://www.cplusplus.com/reference/stl/unordered_map/ )
 * And we need to range-iterate a lot because we have multiple RHS's per LHS.
 */

 
class Grammar {
     
  public:
    /* attributes and other stuff */
    typedef pair <string, int> stringAndInt;
    typedef pair <string, double> stringAndDouble;
    typedef pair <string, stringAndDouble> tableKeyAndValue;

    /* constructors */
    Grammar(string treeBankFile);
    Grammar(const Grammar& orig);
    virtual ~Grammar();
    
    /* methods */
    vector<stringAndDouble> getRHS(string LHS);
    vector<stringAndDouble> getLHS(string RHS);
    
    void fillR2lTableFromL2rTable() ;
    void readGrammar();
    void printL2rTable();
    void init();
    
private:
    /* attributes and other stuff */
    multimap<string, stringAndDouble> l2rTable;
    multimap<string, stringAndDouble> r2lTable; // inverse ordering from what is "natural"

    pair<multimap<string, stringAndDouble>::iterator, multimap<string, stringAndDouble>::iterator> ruleRangeIterator;
    multimap<string, stringAndDouble>::iterator ruleIterator;

    string treeBankFileName;

    /* methods */

    void parseLine(string line);
    void parseLineRecursively (const char * line, int linePos, stack <stringAndInt> , int  level);
    void insertL2rTable(string key, string valueString) ;
    bool validCharacter(char nextChar);
};

#endif	/* GRAMMAR_H */

