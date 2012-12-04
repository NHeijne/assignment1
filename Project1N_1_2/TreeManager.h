/* 
 * File:   TreeManager.h
 * Author: agnes
 *
 * Created on November 25, 2012, 8:35 PM
 */

#ifndef TREEMANAGER_H
#define	TREEMANAGER_H

#include <string>
#include <cstring>
#include <iostream>

#include "tree.hh" 
#include "CYKParser.h"

using namespace std;

class TreeManager {


public:
  
  TreeManager(CYKParser * p);
  TreeManager(const TreeManager& orig);
  virtual ~TreeManager();

  static void printTree(tree<string> myTree);

  void addTree(const string line);
  static void debinarize(tree<string>& theTree);

private:

  CYKParser * parser;
  vector <tree<string> > allTrees;

};

#endif	/* TREEMANAGER_H */

