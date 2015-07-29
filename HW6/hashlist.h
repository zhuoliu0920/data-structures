#ifndef HASHLIST_H
#define HASHLIST_H

#include "wordlist.h"
#include <string> // to store each word
#include <iostream>

using namespace std;

// this is the class of frequency list, which records the word and its frequency in each node
class Node
{
public:
    string IndexWord;
    WordList FileName;
    Node *pnext;
    Node *pprev;

    Node() {pnext = NULL; pprev = NULL;};
    ~Node() {}; // destructor
 
    friend class HashList;
};

class HashList
{
public:
    Node *head;
    Node *end;
    Node *curr;

    HashList();
    ~HashList();
    void insert(const string, const string); // insert a node containing 'index word' and corresponding 'file' at the end of list
    WordList* search(const string); // search an index word in a list, and output its 'file names'
    
    friend class Hash;
};

HashList::HashList() // constructor of class HashList
{
    head = NULL;
    end = NULL;
};

HashList::~HashList() // destrctor of class HashList (free memory from 'head' to 'end')
{
    if (head == NULL) // if both 'head' and 'end' are NULL, we simply free these two pointers
    {
        delete head;
        delete end;
    }
    else if (end == NULL) // if 'end' is NULL, but 'head' is not, we free pointer 'end' first, then free the contents of 'head' and 'head' itself.
    {
        delete end;
        delete head->pprev;
        delete head->pnext;
        delete head;
    }
    else // if neither 'head' and 'end' are NULL, we free memory by traversing the list
    {
        delete head->pprev; // free the first pointer "head->pprev"
        while (head) // free all the memory occupied by each node
        {
            Node *temp = head;
            head = head->pnext;
            delete temp;
        }  
        delete head; // free the last pointer "end->pnext"
    }
}

void HashList::insert(const string index, const string file) // function to insert a string and its frequency to the end of the list
{
    if (head == NULL) {
        head = new Node;
        head->IndexWord = index;
        (head->FileName).insert(file);
        head->pnext = NULL;
        head->pprev = NULL;
        return;
    }
    else if (end == NULL) {
        if (head->IndexWord == index) {
            (head->FileName).insert(file);
            return;
        }
        else {
            end = new Node;
            end->IndexWord = index;
            (end->FileName).insert(file);
            end->pprev = head;
            head->pnext = end;
            end->pnext = NULL;
            return;
        }
    }
    else {
        curr = head;
        while (curr) {
            if (curr->IndexWord == index) {
                (curr->FileName).insert(file);
                return;
            }
        curr = curr->pnext;
        }
        Node* temp = new Node;
        temp->IndexWord = index;
        (temp->FileName).insert(file);
        temp->pprev = end;
        temp->pnext = NULL;
        end->pnext = temp;
        end = temp;
        return;
    }
}

WordList* HashList::search(const string index) 
{
    curr = head;
    while (curr) {
        if (curr->IndexWord == index) return (&(curr->FileName));
        curr = curr->pnext;
    }
    WordList *emptylist;
    emptylist = new WordList;
    return emptylist;
}

#endif
