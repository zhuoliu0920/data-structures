#ifndef FREQLIST_H
#define FREQLIST_H

#include "wordlist.h"
#include <string> // to store each word
#include <iostream>

using namespace std;

// this is the class of frequency list, which records the word and its frequency in each node
class FreqNode
{
public:
    string word;
    int freq; // to store the frequency of this word
    FreqNode *pnext;
    FreqNode *pprev;

    FreqNode() {pnext = NULL; pprev = NULL;};
    ~FreqNode() {}; // destructor
 
    friend class FreqList;
};

class FreqList
{
public:
    FreqNode *head;
    FreqNode *end;
    FreqNode *curr;

    FreqList();
    ~FreqList();
    void insert(const string, int); // insert a node containing 'word' and 'frequency' at the end of list

    void frequency_unsorted(const WordList&); // transfer a word list into a unsorted frequency list
    void sort(void);
    string* toarray(void);

    void append(const FreqList&, FreqNode*); // append a frequency list F1 by the end of a frequency list F2, if F2 is empty, it copies F1 to F2
    void clear(void);
    void copy(const FreqList&);
    void erase(const string); 
    void deletenode(FreqNode*); 
    int search(const string, FreqNode*) const; // search a word in a frequency list, and return its frequency

    void list_union(const FreqList&, const FreqList&, FreqNode*, FreqNode*);
    void list_union_nonrec(const FreqList&, const FreqList&); // function to take union of two lists
    void list_intersection(const FreqList&, const FreqList&, FreqNode*, FreqNode*);
    void list_intersectionwith(const FreqList&);
    void list_intersection_nonrec(const FreqList&, const FreqList&); // function to take union of two lists

    int output_forward(const string); // output frequency list into a document in forward order. (return -1: fail to creat output file, return 0: success)
    int output_backward(const string); // output frequency list into a document in backward order. (return -1: fail to creat output file, return 0: success)
    size_t listSize(void); // return size of the list.
    int size(void); // return numer of nodes in the list.
    void print(void);

};

FreqList::FreqList() // constructor of class WordList
{
    head = NULL;
    end = NULL;
};

FreqList::~FreqList() // destrctor of class WordList (free memory from 'head' to 'end')
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
            FreqNode *temp = head;
            head = head->pnext;
            delete temp;
        }  
        delete head; // free the last pointer "end->pnext"
    }
}

void FreqList::clear() 
{
    if (head == NULL){
        return;
    }
    else if (end == NULL)
    {
        delete head->pprev;
        delete head->pnext;
        head = NULL;
        return;
    }
    else
    {
        delete head->pprev; 
        while (head->pnext != end)
        {
            FreqNode *temp = head;
            head = head->pnext;
            delete temp;
        }  
        delete end->pnext;
        head = NULL;
        end = NULL;
    }
}

void FreqList::insert(const string st, int number) // function to insert a string and its frequency to the end of the list
{
    if (head == NULL) {
        head = new FreqNode;
        head->word = st;
        head->freq = number;
        head->pnext = NULL;
        head->pprev = NULL;
        return;
    }
    else if (end == NULL) {
        end = new FreqNode;
        end->word = st;
        end->freq = number;
        end->pprev = head;
        head->pnext = end;
        end->pnext = NULL;
        return;
    }
    else {
        FreqNode* temp = new FreqNode;
        temp->word = st;
        temp->freq = number;
        temp->pprev = end;
        temp->pnext = NULL;
        end->pnext = temp;
        end = temp;
        return;
    }
}

void FreqList::erase(const string st) // function to delete all the words 'st' in the list
{
    if (head == NULL) return; // if both head and end are NULL, delete fails.

// else, we delete the word by traversing the list
    while (head->word == st)
    {
        if (head->pnext != NULL) {
            delete head->pprev; // if the first node is the word we want to delete, we first delete the node which saves this word
            FreqNode *temp = head;
            head->pnext->pprev = NULL;
            head = head->pnext;
            delete temp;
        }
        else {
            delete head->pprev;
            delete head->pnext;
            head = NULL;
            return;
        }
    }
    
    curr = head->pnext;
    if (curr == NULL) return;

    while (curr != end)
    {
        if (curr->word == st)
        {
            FreqNode *temp = curr; // if the word we want to delete is stored in one node in the middle, we first delete the node which stores this word
            curr->pprev->pnext = curr->pnext;
            curr->pnext->pprev = curr->pprev;
            curr = curr->pnext;
            delete temp;
        }
        else
            curr = curr->pnext;
    }

    if (curr == end && curr->word == st)
    {
        delete end->pnext;// if the word to be deleted is store in the node before end, we first delete the node which saves this word
        FreqNode *temp = end;
        end->pprev->pnext = NULL;
        end = end->pprev;
        delete temp;
    }
    return;
}

void FreqList::deletenode(FreqNode* index) 
{
    if (head == NULL) return; 
    else if (index == head && end != NULL) {
        delete head->pprev;
        head->pnext->pprev = NULL;
        head = head->pnext;
        delete index;
        return;
    }
    else if (index == head && end == NULL) {
        delete head->pprev;
        delete head->pnext;
        head = NULL;
        return;
    }
    else if (index != end) {
        index->pprev->pnext = index->pnext;
        index->pnext->pprev = index->pprev;
        delete index;
        return;
    }
    else
    {
        delete end->pnext;
        end->pprev->pnext = NULL;
        end = end->pprev;
        delete index;
	return;
    }
}

void FreqList::frequency_unsorted(const WordList &L) // function to transfer a word list into a frequency list
{
    WordNode* index = L.head;
    while (index != NULL) // base case
    {
        if (isalnum(index->word[0])) { // if the node stores a word, not a seperator, we insert into the frequency list, and go to next node
            FreqList::insert(index->word, 1);
            index = index->pnext;
        }
        else { // otherwise, directly go to next node
            index = index->pnext;
        }
    }
    return;
}

string* FreqList::toarray()
{
    int size = FreqList::size();
    int i = 0;
    string *A;
    A = new string[size];

    curr = head;
    while (curr)
    {
        A[i++] = curr->word;
        curr = curr->pnext;
    }
    return A;
}

void FreqList::sort()
{
    int size = FreqList::size();
    string* Word;
    int* Freq;
    Word = new string[size];
    Freq = new int[size];
    FreqNode* index = head;

    for (int i = 0; i < size; i++) {
        Word[i] = index->word;
        Freq[i] = index->freq;
        index = index->pnext;
    }
    string temp;
    int temp2;
    for (int i = 1; i < size; i++) {
        temp = Word[i];
        temp2 = Freq[i];
        for (int j = i; j != 0; j--) {
            if ((temp.compare(Word[j-1]) < 0 && j != 1) || Word[j-1] == "") {
                Word[j] = Word[j-1];
                Freq[j] = Freq[j-1];
                continue;
            }
            else if (temp.compare(Word[j-1]) == 0) {
                Freq[j-1] += temp2;
                Word[j] = "";
                Freq[j] = 0;
                break;
            }
            else if (temp.compare(Word[j-1]) > 0) {
                Word[j] = temp;
                Freq[j] = temp2;
                break;
            }
            else if (j == 1) {
                Word[j] = Word[j-1];
                Freq[j] = Freq[j-1];
                Word[j-1] = temp;
                Freq[j-1] = temp2;
                break;
            }
        }
    }
        
    FreqList::clear();
    for (int i = 0; i < size; i++) {
        if (Word[i] != "") {
            FreqList::insert(Word[i], Freq[i]);
            }
    }
}
      
void FreqList::append(const FreqList &F2, FreqNode* index2) // function to append a frequency list to the end of other frequency list (it is used for copying)
{
    if (index2 == NULL) {
        return;
}
    else {
        FreqList::insert(index2->word, index2->freq);
        FreqList::append(F2, index2->pnext);
        return;
    }
}

void FreqList::copy(const FreqList &F)
{
    FreqList::clear();
    FreqList::append(F, F.head);
}

int FreqList::search(const string st, FreqNode* index) const // function to search a word in a frequency list
{
    if (index == NULL) {
        return 0;
    }
    else if (index->word == st) {
        return (index->freq);
    }
    else {
        return (search(st, index->pnext));
    }
}
        
void FreqList::list_union(const FreqList &F1, const FreqList &F2, FreqNode *index, FreqNode *index2) // function to take union of two lists
{
    if (index == NULL && index2 == NULL) {
        return;
    }
    else if (index == NULL) {
        FreqList::insert(index2->word, index2->freq);
        FreqList::list_union(F1, F2, index, index2->pnext);
        return;
    }
    else if (index2 == NULL) {
        FreqList::insert(index->word, index->freq);
        FreqList::list_union(F1, F2, index->pnext, index2);
        return;
    }
    else if (index->word == index2->word) {
        FreqList::insert(index->word, index->freq + index2->freq);
        FreqList::list_union(F1, F2, index->pnext, index2->pnext);
        return;
    }
    else if ((index->word).compare(index2->word) > 0) {
        FreqList::insert(index2->word, index2->freq);
        FreqList::list_union(F1, F2, index, index2->pnext);
        return;
    }
    else if ((index->word).compare(index2->word) < 0) {
        FreqList::insert(index->word, index->freq);
        FreqList::list_union(F1, F2, index->pnext, index2);
        return;
    }
}

void FreqList::list_union_nonrec(const FreqList &F1, const FreqList &F2) // function to take union of two lists
{
    FreqNode* index = F1.head;
    FreqNode* index2 = F2.head;

    while (index != NULL || index2 != NULL) {
        if (index == NULL) {
            FreqList::insert(index2->word, index2->freq);
            index2 = index2->pnext;
            continue;
        }
        else if (index2 == NULL) {
            FreqList::insert(index->word, index->freq);
            index = index->pnext;
            continue;
        }
        else if (index->word == index2->word) {
            FreqList::insert(index->word, index->freq + index2->freq);
            index = index->pnext;
            index2 = index2->pnext;
            continue;
        }
        else if ((index->word).compare(index2->word) > 0) {
            FreqList::insert(index2->word, index2->freq);
            index2 = index2->pnext;
            continue;
        }
        else if ((index->word).compare(index2->word) < 0) {
            FreqList::insert(index->word, index->freq);
            index = index->pnext;
            continue;
        }
    }
    return;
}

void FreqList::list_intersection(const FreqList &F1, const FreqList &F2, FreqNode *index, FreqNode *index2) // function to take intersection of two lists
{
    if (index == NULL || index2 == NULL) {
        return; // base case
    }
    else if ((index->word).compare(index2->word) > 0) {
        FreqList::list_intersection(F1, F2, index, index2->pnext);
        return;
    }
    else if ((index->word).compare(index2->word) < 0) {
        FreqList::list_intersection(F1, F2, index->pnext, index2);
        return;
    }
    else if (index->word == index2->word) {
        FreqList::insert(index->word, 1);
        FreqList::list_intersection(F1, F2, index->pnext, index2->pnext);
        return;
    }
}

void FreqList::list_intersection_nonrec(const FreqList &F1, const FreqList &F2) // function to take intersection of two lists
{
    FreqNode* index = F1.head;
    FreqNode* index2 = F2.head;

    while (index != NULL && index2 != NULL) {
        if ((index->word).compare(index2->word) > 0) {
            index2 = index2->pnext;
        } 
        else if ((index->word).compare(index2->word) < 0) {
            index = index->pnext;
        }
        else if (index->word == index2->word) {
        FreqList::insert(index->word, 1);
        index = index->pnext;
        index2 = index2->pnext;
        }
    }
    return;
}

int FreqList::output_forward(const string fileName) // function to output list into document 'filename'
{
    curr = NULL;
    if (fileName == "")
    {
        cerr << "Please specify output file name." << endl;
        return -1;
    }

    ofstream output;
    output.open(fileName.c_str());
    if (!output.is_open())
    {
        cerr << "Output file cannot be created." << endl;
        return -1;
    }

    curr = head;
    while (curr)
    {
        output << curr->word << endl;
        curr = curr->pnext;
    }
    output.close();
    return 0;
}

int FreqList::output_backward(const string fileName) // function to output list into document 'filename'
{
    curr = NULL;
    if (fileName == "")
    {
        cerr << "Please specify output file name." << endl;
        return -1;
    }

    ofstream output;
    output.open(fileName.c_str());
    if (!output.is_open())
    {
        cerr << "Output file cannot be created." << endl;
        return -1;
    }

    if (end == NULL && head == NULL) // if the list is empty, do nothing and return.
    {
        output.close();
        return 0;
    }
    else if (end == NULL) // if end is NULL, but head is not NULL, curr = head
    {
        curr = head;
    }
    else // if neither head and end are NULL, curr = end
    {
        curr = end;
    }
    
    while (curr)
    {
        output << curr->word << endl;
        curr = curr->pprev;
    }
    output.close();
    return 0;
}

size_t FreqList::listSize() // function to return the size of the list
{
    size_t size = 0;

    curr = head;
    while (curr)
    {
        size += sizeof(*curr);
        size += sizeof(curr->freq);
        size += (curr->word).capacity();
        curr = curr->pnext;
    }
    return(size);
}

int FreqList::size() // function to return number of nodes in the list
{
    int size = 0;

    curr = head;
    while (curr)
    {
        size++;
        curr = curr->pnext;
    }
    return(size);
}

void FreqList::print()
{
    curr = head;
    while (curr)
    {
        cout << curr->word << " ";
        curr = curr->pnext;
    }
    cout << endl;
    return;
}

#endif
