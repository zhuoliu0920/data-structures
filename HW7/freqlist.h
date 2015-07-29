#ifndef FREQLIST_H
#define FREQLIST_H

#include <string> 
#include <cstring> 
#include <iostream>
#include <fstream>
#include "wordlist.h"

using namespace std;

class FreqNode
{
public:
    string word;
    int indegree;
    int outdegree;
    bool Existence;

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

    int update(const string, WordList&);
    void insert(const string, int, int, bool); // insert a node containing 'word' and 'frequency' at the end of list
    int output(const string); // output frequency list into a document in forward order. (return -1: fail to creat output file, return 0: success)
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

int FreqList::update(const string page, WordList& pagelist)
{
    ifstream indata;
    string line;
    string tmp;
    string link;
    size_t pos1;
    size_t pos2;
    bool Inquotes = false;
    bool Assign = false;
    int outcount = 0;
    WordList tmplist;
    WordNode *tmpnode;

    // check whether page can be opened, if not, the corresponding flag "Existence" becomes 'false' 
    indata.open(page.c_str());
    if (!indata) {
        cerr << "Input file cannot be correctly opened!" << endl;
        curr = head;
        while (curr) {
            if (curr->word == page) {
                curr->Existence = false;
                break;
            }
            curr = curr->pnext;
        }
        indata.close();
        return -1;
    }
    
    // in order to avoid duplicated links in one page, we store links in an ordered list "tmplist"
    while (getline(indata,line)) {
        pos1 = 0;
        while (line.find("<a ",pos1) != string::npos) {
            pos1 = line.find("<a ",pos1)+3;
            pos2 = line.find(">",pos1);
            if (pos2 == string::npos) break;

            tmp = line.substr(pos1, pos2-pos1);
            if (tmp.find("href", 0) != string::npos) {
                tmp = tmp.substr(tmp.find("href",0)+4, tmp.length()-tmp.find("href",0)-4);
                for (unsigned int j = 0; j < tmp.length(); j++) {
                    if (isspace(tmp[j]) && !Inquotes) continue;
                    else if (tmp[j] == '=' && !Inquotes) Assign = true;
                    else if (tmp[j] == '\"') {
                        if (!Inquotes) Inquotes = true;
                        else {
                            Inquotes = false;
                            break;
                        }
                    }
                    else if (Inquotes && Assign) {
                        link.push_back(tmp[j]);
                    }
                }
            }

            if (Inquotes == false && Assign == true && link != "" && link != page) {
                tmplist.insert_ordered(link);
            }
            link = "";
            Inquotes = false;
            Assign = false;
            pos1 = pos2+1;
        }
    }
    
    // insert all the links in "tmplist" into freqlist, and update their indegrees and outdegrees
    tmpnode = tmplist.head;
    while (tmpnode) {
        FreqList::insert(tmpnode->word,1,0,false);
        if (pagelist.search(tmpnode->word))
            outcount++;
        tmpnode = tmpnode->pnext;
    }
    FreqList::insert(page,0,outcount,true);
    indata.close();
    return 0;
}

void FreqList::insert(const string st, int number1, int number2, bool exist)
{
    if (head == NULL) { // if list is empty, insert the node as head
        head = new FreqNode;
        head->word = st;
        head->indegree = number1;
        head->outdegree = number2;
        head->Existence = exist;
        head->pnext = NULL;
        head->pprev = NULL;
        return;
    }
    else if (end == NULL) // if end is NULL, but head is not NULL, first determine if this word appeared in head, then update the list
    {  
        if (st.compare(head->word) == 0)
        {
            head->indegree += number1;
            head->outdegree += number2;
            head->Existence = (!head->Existence && !exist)?false:true;
            return;
        }
        else if (st.compare(head->word) > 0)
        {
            end = new FreqNode;
            end->word = st;
            end->indegree = number1;
            end->outdegree = number2;
            end->Existence = exist;
            end->pprev = head;
            head->pnext = end;
            end->pnext = NULL;
            return;
        }
        else if (st.compare(head->word) < 0)
        {
            end = new FreqNode;
            end->word = head->word;
            end->indegree = head->indegree;
            end->outdegree = head->outdegree;
            end->Existence = head->Existence;
            end->pprev = head;
            end->pnext = NULL;
            head->word = st;
            head->indegree = number1;
            head->outdegree = number2;
            head->Existence = exist;
            head->pnext = end;
            return;
        }
    }

    FreqNode* index = head;
    while (index)
    {
        if (index->pnext == NULL && st.compare(index->word) > 0) // insert to end
        {
            FreqNode* temp = new FreqNode;
            temp->word = st;
            temp->indegree = number1;
            temp->outdegree = number2;
            temp->Existence = exist;
            temp->pprev = index;
            temp->pnext = NULL;
            index->pnext = temp;
            end = temp;
            return;
        }
        else if (st.compare(index->word) == 0) // already in list
        {
            index->indegree += number1;
            index->outdegree += number2;
            index->Existence = (!index->Existence && !exist)?false:true;
            return;
        }
        else if (st.compare(index->word) < 0 && index->pprev == NULL) // insert to head
        {
            FreqNode* temp = new FreqNode;
            temp->word = st;
            temp->indegree = number1;
            temp->outdegree = number2;
            temp->Existence = exist;
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
            temp->indegree = number1;
            temp->outdegree = number2;
            temp->Existence = exist;
            temp->pprev = index->pprev;
            temp->pnext = index;
            index->pprev->pnext = temp;
            index->pprev = temp;
            return;
        }
        else index = index->pnext;
    }
}

int FreqList::output(const string fileName) // function to output list into document 'filename'
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

    int i = 0;
    int max[5] = {-10};
    string link = "";
    string maxlink = "";
    bool initial = false;
    while (i < 5) {
        // find the first valid page as the pivot element
        curr = head;
        while (curr && !initial) {
            if (curr->word == maxlink)
                curr->indegree = i*(-1);
            else if (curr->Existence == true && curr->indegree >= 0) {
                max[i] = curr->indegree;
                link = curr->word;
                initial = true;
            }
            curr = curr->pnext;
        }
        if (!initial) break; 

        // traverse the list and find the maximum, and put the maximum indegree to be negative
        while (curr)
        {
            if (curr->word == maxlink)
                curr->indegree = i*(-1);
            else if (max[i] < curr->indegree && curr->Existence == true) {
                max[i] = curr->indegree;
                link = curr->word;
            }
            curr = curr->pnext;
        }
        output << link << " " << max[i] << endl;
        maxlink = link;
        initial = false;
        i++;
    }
    // restore the list (by restoring negative indegrees)
    curr = head;
    while (curr) {
        if (curr->indegree < 0)
            curr->indegree = max[-1*(curr->indegree)-1];
        curr = curr->pnext;
    }
    // print all the broken links
    output << endl;
    i = 0;
    curr = head;
    while (curr) {
        if (curr->indegree != 0 && curr->Existence == false) {
            output << curr->word << " Broken Link" << endl;
            i++;
        }
        curr = curr->pnext;
    }
    if (i == 0) output << "No Broken Links" << endl;
    // print all sinks
    output << endl;
    i = 0;
    curr = head;
    while (curr) {
        if (curr->outdegree == 0 && curr->Existence == true) {
            output << curr->word << " Sink" << endl;
            i++;
        }
        curr = curr->pnext;
    }
    if (i == 0) output << "No Sinks" << endl;

    output.close();
    return 0;
}

void FreqList::print()
{
    curr = head;
    while (curr)
    {
        cout << curr->word << " " << curr->indegree << " " << curr->outdegree << " " << curr->Existence << endl;
        curr = curr->pnext;
    }
    return;
}

#endif
