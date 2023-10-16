/* Author: Devin Lepur
 * RedID: 825148526
 * 9/7/23
 * Instructor: Prof Shen
 * SDSU CS 480
 * tree.h
 */


#ifndef TREE_H
#define TREE_H

class node {
public:
    //next houses the child nodes
    node* next[30];

    node() {
        for (int i = 0; i < 31; i++) {
            next[i] = nullptr;
        }
    }
};

class tree {
public:
    node* root;

    // Constructor
    tree();

    //Converts chars to int for array indexing
    int charToInt(char letter);

    bool insert(const char* word);

    bool search(const char* strBeingSearched);


};

#endif // TREE_H
