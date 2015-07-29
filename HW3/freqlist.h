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
    void insert_ordered(const string, int, FreqNode*); // insert a node cotaining 'word' and 'frequency' into list, with alphabetical order
    void frequency(const WordList&, WordNode*); // transfer a word list into a frequency list
    void append(const FreqList&, FreqNode*); // append a frequency list F1 by the end of a frequency list F2, if F2 is empty, it copies F1 to F2
    void clear(void);
    void copy(const FreqList&); 
    int search(const string, FreqNode*) const; // search a word in a frequency list, and return its frequency
    void list_union(const FreqList&, FreqNode*); // take union of two frequency lists, and class members will be updated
    void list_intersection(const FreqList&, FreqNode*); // take intersection of two frequency lists, and class members will be updated
    int output_forward(const string); // output frequency list into a document in forward order. (return -1: fail to creat output file, return 0: success)
    int output_backward(const string); // output frequency list into a document in backward order. (return -1: fail to creat output file, return 0: success)
    size_t listSize(void); // return size of the list.
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

void FreqList::insert_ordered(const string st, int number, FreqNode *index) // function to insert a string to the end of the list
{
    if (head == NULL) { // if list is empty, insert the node as head
        head = new FreqNode;
        head->word = st;
        head->freq = number;
        head->pnext = NULL;
        head->pprev = NULL;
        return;
    }
    else if (end == NULL) // if end is NULL, but head is not NULL, first determine if this word appeared in head, then update the list
    {  
        if (st.compare(head->word) == 0)
        {
            head->freq += number;
            return;
        }
        else if (st.compare(head->word) > 0)
        {
            end = new FreqNode;
            end->word = st;
            end->freq = number;
            end->pprev = head;
            head->pnext = end;
            end->pnext = NULL;
            return;
        }
        else if (st.compare(head->word) < 0)
        {
            end = new FreqNode;
            end->word = head->word;
            head->word = st;
            end->freq = number;
            end->pprev = head;
            head->pnext = end;
            end->pnext = NULL;
            return;
        }
    }

    else // if neither head and end are NULL, by recursion, we compare inserted word with the words in list, and then insert it into proper position
    {
        if (index->pnext == NULL && st.compare(index->word) > 0) // insert to end
        {
            FreqNode* temp = new FreqNode;
            temp->word = st;
            temp->freq = number;
            temp->pprev = index;
            temp->pnext = NULL;
            index->pnext = temp;
            end = temp;
            return;
        }
        else if (st.compare(index->word) == 0) // already in list
        {
            index->freq += number;
            return;
        }
        else if (st.compare(index->word) < 0 && index->pprev == NULL) // insert to head
        {
            FreqNode* temp = new FreqNode;
            temp->word = st;
            temp->freq = number;
            temp->pprev = NULL;
            temp->pnext = index;
            index->pprev = temp;
            head = temp;
            return;
        }
        else if (st.compare(index->word) < 0) // insert in the middle
        {
            FreqNode* temp = new FreqNode;
            temp->word = st;
            temp->freq = number;
            temp->pprev = index->pprev;
            temp->pnext = index;
            index->pprev->pnext = temp;
            index->pprev = temp;
            return;
        }
        else // otherwise, compare to the word stored in next node
        {
            FreqList::insert_ordered(st, number, index->pnext);
        }
    }
}

void FreqList::frequency(const WordList &L, WordNode* index) // function to transfer a word list into a frequency list
{
    if (index == NULL) // base case
        return;
    else if (isalnum(index->word[0])) { // if the node stores a word, not a seperator, we insert into the frequency list, and go to next node
        FreqList::insert_ordered(index->word, 1, head);
        FreqList::frequency(L, index->pnext);
        return;
    }
    else { // otherwise, directly go to next node
        FreqList::frequency(L, index->pnext);
        return;
    }
}

void FreqList::append(const FreqList &F2, FreqNode* index2) // function to append a frequency list to the end of other frequency list (it is used for copying)
{
    if (index2 == NULL) return;
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
    if (index == NULL)
        return 0;
    else if (index->word == st)
        return (index->freq);
    else
        return (search(st, index->pnext));
}
        
void FreqList::list_union(const FreqList &F2, FreqNode *index2) // function to take union of two lists
{
    if (index2 == NULL) return;
    else {
        FreqList::insert_ordered(index2->word, index2->freq, head);
        FreqList::list_union(F2, index2->pnext);
        return;
    }
}

void FreqList::list_intersection(const FreqList &F2, FreqNode *index) // function to take intersection of two lists
{
    if (index == NULL) return; // base case
    else {
        int n = F2.search(index->word, F2.head); // search if 'word1' is in F2 or not
        if (n == 0) // if not, we need to remove the node containing 'word1' and go to next node
        {
            if (index == head && index->pnext == end && end != NULL)
            {
                delete index->pprev;
                FreqNode *temp = index;
                index->pnext->pprev = NULL;
                index = index->pnext;
                head = index;
                end = NULL;
                delete temp;
                FreqList::list_intersection(F2, index);
                return;
            }
            else if (index == head && index->pnext != NULL)
            {
                delete index->pprev;
                FreqNode *temp = index;
                index->pnext->pprev = NULL;
                index = index->pnext;
                head = index;
                delete temp;
                FreqList::list_intersection(F2, index);
                return;
            }
            else if (index == head)
            {
                delete head->pprev;
                delete head->pnext;
                head = NULL;
                index = NULL;
                FreqList::list_intersection(F2, index);
                return;
            }
            else if (index == end && index->pprev != head)
            {
                FreqList::list_intersection(F2, index->pnext);
                delete index->pnext;
                FreqNode *temp = index;
                index->pprev->pnext = NULL;
                index = index->pprev;
                end = index;
                delete temp;
                return;
            }
            else if (index == end)
            {
                FreqList::list_intersection(F2, index->pnext);
                delete index->pnext;
                FreqNode *temp = index;
                index->pprev->pnext = NULL;
                index = index->pprev;
                end = NULL;
                delete temp;
                return;
            }
            else
            {
                FreqNode *temp = index;
                index->pprev->pnext = index->pnext;
                index->pnext->pprev = index->pprev;
                index = index->pnext;
                delete temp;
                FreqList::list_intersection(F2, index);
                return;
            }
        }
        else // if yes, we need to update the frequency of this node to be 1, and go to next node
        {
            index->freq = 1;
            FreqList::list_intersection(F2, index->pnext);
            return;
        }
    }
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
        output << curr->word << " " << curr->freq << endl;
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
        output << curr->word << " " << curr->freq << endl;
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

void FreqList::print()
{
    curr = head;
    while (curr)
    {
        cout << curr->word << " " << curr->freq << endl;
        curr = curr->pnext;
    }
    return;
}

#endif
