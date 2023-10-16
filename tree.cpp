/* Author: Devin Lepur
 * RedID: 825148526
 * 9/7/23
 * Instructor: Prof Shen
 * SDSU CS 480
 * tree.cpp
 */

#include "tree.h"
#include <iostream>

//constructor;
tree::tree() {
    root = new node();
}


// charToInt converts given characters to a set int for indexing
int tree::charToInt(char letter)
{
    // convert char to ascii
    int conversion = tolower(letter);
    // handle special cases
    if (conversion == 32)                       // space
    { 
        return 26;
    }
    if (conversion == 39)                       // apostrophe
    { 
        return 27;
    }
    if (conversion == 45)                       // hyphen
    { 
        return 28;
    }
    if (conversion == 95)                       // underscore
    { 
        return 29;
    }
    if (conversion == 0)                        // null char
    { 
        return 30;
    }
    if (conversion > 96 && conversion < 123)    // alhpabetic
    { 
        return (conversion - 97);
    }

    // return val of -1 indicates an invalid character
    return -1;
}


// insert operation takes a string and inserts character by character
// until string is inserted and is followed with a null char insertion to
// mark the end of the word
bool tree::insert(const char *word)
{
    node *currNode = root;

    // loop until end of word is found
    while (*word != '\0')
    {
        int conversion = charToInt(*word);
        // check if valid char and insert new node
        if (conversion >= 0)
        {
            if (!currNode->next[conversion])
            {
                currNode->next[conversion] = new node();
            }
        }
        else
        {
            word++;

            continue;
        }

        // go to next node and next char in string
        currNode = currNode->next[conversion];
        word++;
    }
    // Insert end of word
    currNode->next[30] = new node();

    return true;
}

// given a string, returns true if string exists in the tree
bool tree::search(const char *strBeingSearched)
{
    node *currNode = root;

    // loop through string
    while (*strBeingSearched != '\0')
    {
        int conversion = charToInt(*strBeingSearched);

        if (conversion == -1)
        {
            return false;
        }

        if (!currNode->next[conversion])
        {
            return false;
        }
        // go to next node and next char
        currNode = currNode->next[conversion];
        strBeingSearched++;
    }
    // Check for end of word char
    if (currNode->next[30])
    {
        return true;
    }
    else
    {
        return false;
    }
}
