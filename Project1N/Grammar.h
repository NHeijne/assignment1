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

using namespace std;

class Grammar {
     
  public:
    /* attributes and other stuff */
    typedef pair <string, double> stringAndDouble;
    typedef pair <string, stringAndDouble> l2rEntry;

    /* constructors */
    Grammar();
    Grammar(const Grammar& orig);
    virtual ~Grammar();
    
    /* methods */
    vector<stringAndDouble> getRHS(string LHS);
    vector<stringAndDouble> getLHS(string RHS);
    
private:
    /* attributes and other stuff */
    multimap<string, stringAndDouble> l2rTable;
    multimap<string, stringAndDouble> r2lTable; // inverse ordering from what is "natural"

    pair<multimap<string, stringAndDouble>::iterator, multimap<string, stringAndDouble>::iterator> ruleRangeIterator;
    multimap<string, stringAndDouble>::iterator ruleIterator;

    /* methods */

    void fillR2lTable() ;

};

#endif	/* GRAMMAR_H */
