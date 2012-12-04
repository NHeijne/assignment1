/*
 * File:   TreeManager.cpp
 * Author: agnes
 *
 * Created on November 25, 2012, 8:35 PM
 */

#include "TreeManager.h"
#include "CYKParser.h"

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
      cout << " ";
    }
    cout << (*it) << endl;
    it++;
  }
}

void TreeManager::debinarize(tree<string>& theTree) {

  cout << "debinarize: " << endl;
  
  tree<string>::iterator it = theTree.begin();
  while (it != theTree.end()) {

    //cout << (*it) << endl;
    
    if ( (*it)[(*it).size()-1] == '@') {//if '@' is at last in the name
      
      cout << "has @: " << (*it) << endl;

      tree<string>::iterator thisAt = it;

      it++;
      tree<string>::iterator leftChild = it;
      tree<string>::iterator rightChild = theTree.next_sibling(it);

      cout << "left child: " << (*leftChild) << ", ";
      cout << "right child: " << (*rightChild) << endl;

      theTree.move_after(thisAt, rightChild);
      cout << "move  " << (*rightChild) << " after " << (*thisAt) << endl;


      theTree.move_before(thisAt, leftChild);
      cout << "move  " << (*leftChild) << " before " << (*thisAt) << endl;

       cout << "current iter: " << (*it) << endl;
       
       tree<string>::iterator newAt = theTree.next_sibling(it);
       cout << "newAt: " << (*newAt) << endl;

       cout << "erase " << (*newAt) << endl;
       theTree.erase(newAt);
       
       
        cout << "current iter: " << (*it) << endl;
       system("pause");
     
//      it++; //move right child after the non-term with @
//      theTree.move_ontop(theTree.parent(it), it);
//
//      cout << "replaced " << (*it) << " before "  << (*theTree.parent(it)) << endl;
//
//      it++; //next node is right child
//      theTree.move_ontop(theTree.parent(it), it);  //place the remaining right subtree on top of the non-term with @
//      cout << "moved " << (*it) << " on top of " << (*theTree.parent(it)) << endl;
    }
    else {
      it++; // look at next node
    }
  }

}
void TreeManager::addTree(const string line) {
  parser->parseLine(line);
  tree<string> currentTree;
  parser->getTree(currentTree);
  allTrees.push_back(currentTree);
}

