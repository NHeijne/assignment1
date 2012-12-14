/*
 * File:   TreeManager.cpp
 * Author: agnes
 *
 * Created on November 25, 2012, 8:35 PM
 */

#include "TreeManager.h"

using namespace std;

TreeManager::TreeManager(CYKParser * p) {
  parser = p;
}

TreeManager::TreeManager(const TreeManager& orig) {
}

TreeManager::~TreeManager() {
}

void TreeManager::printTree(tree<string> myTree) {

  cout << "Tree: " << endl;

  tree<string>::iterator it = myTree.begin();
  tree<string>::iterator end = myTree.end();
  while (it != end) {
    for (int i = 0; i < myTree.depth(it); i++) {
      cout << "    ";
    }
    cout << (*it) << endl;
    it++;
  }
}

string TreeManager::formatTree(tree<string> myTree) {

  string treeString = "";
  tree<string>::iterator it = myTree.begin();
  tree<string>::iterator end = myTree.end();
  int currentDepth = 0;
  int previousDepth = 0;
  while (it != end) {
    string thing;
    if (myTree.number_of_children(it) != 0) {
      treeString += "(";
    }
    if (myTree.number_of_children(it) != 0) {
      thing = (*it).substr(Grammar::nonTerminalSymbol.length(), (*it).length());
    }
    else {
      thing = (*it);
    }
    treeString += thing;
    if (myTree.number_of_children(it) != 0) {
      treeString += " ";
    }
    previousDepth = myTree.depth(it);
    it++;
    currentDepth = myTree.depth(it);

    for (int i = 0; i < previousDepth - currentDepth ; i++) {
      if (currentDepth == 0 && i == (previousDepth - currentDepth - 1)) {
        treeString += " ";
      }
      treeString += ")";
    }
    if (previousDepth - currentDepth > 0)
      treeString += " ";
  }
  return treeString;
}

void TreeManager::debinarize(tree<string>& theTree) {

  //cout << "debinarize: " << endl;

  tree<string>::iterator it = theTree.begin();
  while (it != theTree.end()) {

    if ((*it)[(*it).size() - 1] == '@') {//if '@' is at last in the name
      //cout << "has @: " << (*it) << endl;

      tree<string>::iterator thisAt = it;

      it++;
      tree<string>::iterator leftChild = it;
      tree<string>::iterator rightChild = theTree.next_sibling(it);
      //cout << "left child: " << (*leftChild) << ", ";
      // cout << "right child: " << (*rightChild) << endl;

      theTree.move_after(thisAt, rightChild);
      //cout << "move  " << (*rightChild) << " after " << (*thisAt) << endl;
      theTree.move_before(thisAt, leftChild);
      //cout << "move  " << (*leftChild) << " before " << (*thisAt) << endl;
      // cout << "current iter: " << (*it) << endl;
      tree<string>::iterator newAt = theTree.next_sibling(it);
      // cout << "erase " << (*newAt) << endl;
      theTree.erase(newAt);
      //cout << "current iter: " << (*it) << endl;
    }
    else {
      it++; // look at next node
    }
  }

}

void TreeManager::removeSpecialUnaryRules(tree<string>& theTree) {
  int sizeSpecialUnarySymbol = Grammar::specialUnarySymbol.size();

  // assume nt_A%%%%%B
  tree<string>::iterator it = theTree.begin();
  while (it != theTree.end()) {

    int pos = (*it).find(Grammar::specialUnarySymbol); //   position of the first occurrence in the string of the searched content
    if (!(pos == string::npos)) { // found
      cout << (*it) << " has " << Grammar::specialUnarySymbol << endl;

      // retrieve A and B from nt_A%%%%%B
      string str1 = (*it).substr(0, pos);
      string str2 = Grammar::nonTerminalSymbol + (*it).substr(pos + sizeSpecialUnarySymbol, string::npos);
      //cout << "str1: " << str1 << ", " << "str2: " << str2 << endl;

      // Make tree with root nt_B. nt_B's children will be nt_A%%%%%B's children.
      tree<string> tree2(it);
      tree<string>::iterator top2 = tree2.begin();
      (*top2) = str2;
      //cout << (*top2) << " has " << (*it) << " 's childs " << endl;

      // Make tree with root nt_A. nt_A's sole child will be nt_B.
      tree<string> tree1;
      tree<string>::iterator top1 = tree1.begin();
      tree<string>::iterator one1 = tree1.insert(top1, str1);
      theTree.append_child(one1, top2);
      //cout << (*one1) << " has " << (*top2) << " as child " << endl;

      theTree.move_ontop(it, one1);

      it = one1; // go to left-most side of %%%%% rule (works for additive %%%%%-rules, e.g. nt_A%%%%%B%%%%%C)
      //system("pause");
    }
    else {
      it++;
    }
  }

}

void TreeManager::addTree(const string line) {
  parser->parseLine(line);
  tree<string> currentTree;
  parser->getTree(currentTree);
  allTrees.push_back(currentTree);
}

