#ifndef WORDLIST_H
#define WORDLIST_H

#include <string> // to store each word
#include <iostream>
#include <fstream>

using namespace std;

class WordNode
{
public:
    string word;
    WordNode *pnext;
    WordNode *pprev;

    WordNode() {pnext = NULL; pprev = NULL;};
    ~WordNode() {}; // destructor
 
    friend class WordList;
};

class WordList
{
public:
    WordNode *head;
    WordNode *end;
    WordNode *curr;

    WordList();
    ~WordList();
    int read(const string); // read a document into a list. (return -1: fail to read, return 0: success)
    int read_keepUpper(const string); 
    int insert(const string, const string); // insert string1 after string2. (return -1: string2 is not found, string1 cannot be inserted; return 0: succeed)
    void insert(const string); // insert string to the end of list.
    void insert_ordered(const string); // insert string into list, and keep alphabetically order.
    int search(const string);
    int erase(const string); // erase the nodes whose value are string. (The value this function returns is the number of nodes deleted.)
    void clear(void);
    int output_forward(const string); // output list into a document in forward order. (return -1: fail to creat output file, return 0: success)
    int output_backward(const string); // output list into a document in backward order. (return -1: fail to creat output file, return 0: success)
    size_t listSize(void); // return size of the list.
    int print(void);
};

WordList::WordList() // constructor of class WordList
{
    head = NULL;
    end = NULL;
};

WordList::~WordList() // destrctor of class WordList (free memory from 'head' to 'end')
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
            WordNode *temp = head;
            head = head->pnext;
            delete temp;
        }  
        delete head; // free the last pointer "end->pnext"
    }
}

void WordList::clear() 
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
            WordNode *temp = head;
            head = head->pnext;
            delete temp;
        }  
        delete end->pnext;
        head = NULL;
        end = NULL;
    }
}

int WordList::read(const string filename) // function to read list from a document 'filename'.
{ 
    int inputSize; // to store the size of input file
    char *data; // to temporarily the data from input file
    ifstream input;

    input.open(filename.c_str());
    if(!input.is_open()) // check if file is correctly opened
    {
        cerr << "Input file cannot be correctly opened!" << endl;
        return -1;
    }

    // Get the input file size
    long begin = input.tellg();
    input.seekg(0,ios::end);    
    long end = input.tellg();    
    input.seekg(0,ios::beg);
    long size = end - begin;
    data = new char[size];
    int count = 0;
    while(input.get(data[count++])) ; // Store each character in the array and do error checking
    inputSize = count-1;
    input.close();

    // Transfer data into list form
    string temp1; // temp string to hold each word
    string temp2; // temp string to hold each separator after word

    for (int i = 0; i < inputSize-1; i++)
    {
        if (isspace(data[i]))
        {
            if (!temp1.empty()) {
                insert(temp1);
                temp1.clear();
            }
            temp2.clear();
        }
        else if (isalnum(data[i]))
        {
            temp1.push_back(tolower(data[i]));
        }
        else
        {
            temp2.push_back(data[i]);
            if (isalnum(data[i+1]) && !temp1.empty())
            {
                temp1 += temp2;
                temp2.clear();
            }
            else if (isalnum(data[i+1]) && temp1.empty()) temp2.clear();
            else if (isspace(data[i+1])) temp2.clear();
        }
    }

    if (isalnum(data[inputSize-1])) // if the last character is a letter, we push it into temp1 string, and insert into the list.
    {
        temp1.push_back(tolower(data[inputSize-1]));
        insert(temp1);
    }
    else if (!temp1.empty())
    {
        insert(temp1);
        temp1.clear();
    }

    delete[] data;
    return 0;
}

int WordList::read_keepUpper(const string filename) // function to read list from a document 'filename'.
{ 
    int inputSize; // to store the size of input file
    char *data; // to temporarily the data from input file
    ifstream input;

    input.open(filename.c_str());
    if(!input.is_open()) // check if file is correctly opened
    {
        cerr << "Input file cannot be correctly opened!" << endl;
        return -1;
    }

    // Get the input file size
    long begin = input.tellg();
    input.seekg(0,ios::end);    
    long end = input.tellg();    
    input.seekg(0,ios::beg);
    long size = end - begin;
    data = new char[size];
    int count = 0;
    while(input.get(data[count++])) ; // Store each character in the array and do error checking
    inputSize = count-1;
    input.close();

    // Transfer data into list form
    string temp1; // temp string to hold each word
    string temp2; // temp string to hold each separator after word

    for (int i = 0; i < inputSize-1; i++)
    {
        if (isspace(data[i]))
        {
            if (!temp1.empty()) {
                insert(temp1);
                temp1.clear();
            }
            temp2.clear();
        }
        else if (isalnum(data[i]))
        {
            temp1.push_back(data[i]);
        }
        else
        {
            temp2.push_back(data[i]);
            if (isalnum(data[i+1]) && !temp1.empty())
            {
                temp1 += temp2;
                temp2.clear();
            }
            else if (isalnum(data[i+1]) && temp1.empty()) temp2.clear();
            else if (isspace(data[i+1])) temp2.clear();
        }
    }

    if (isalnum(data[inputSize-1])) // if the last character is a letter, we push it into temp1 string, and insert into the list.
    {
        temp1.push_back(data[inputSize-1]);
        insert(temp1);
    }
    else if (!temp1.empty())
    {
        insert(temp1);
        temp1.clear();
    }

    delete[] data;
    return 0;
}

int WordList::search(const string st)
{
    curr = head;
    while(curr) {
        if (curr->word == st) return 1;
        curr = curr->pnext;
    }
    return 0;
}

void WordList::insert(const string st) // function to insert a string to the end of the list
{
    if (head == NULL) {
        head = new WordNode;
        head->word = st;
        head->pnext = NULL;
        head->pprev = NULL;
        return;
    }
    else if (end == NULL) {
        end = new WordNode;
        end->word = st;
        end->pprev = head;
        head->pnext = end;
        end->pnext = NULL;
        return;
    }
    else {
        WordNode* temp = new WordNode;
        temp->word = st;
        temp->pprev = end;
        temp->pnext = NULL;
        end->pnext = temp;
        end = temp;
        return;
    }
}

int WordList::insert(const string in, const string pos) // function to insert word 'in' after word 'pos'
{
    curr = head;
    while (curr && curr->pnext != end)
    {
        if (curr->word == pos)
        {
            curr = curr->pnext; // first, insert word 'in' after the seperator which is after word 'pos'
            WordNode* temp = new WordNode;
            temp->word = in;
            temp->pprev = curr;
            temp->pnext = curr->pnext;
            curr->pnext->pprev = temp;
            curr->pnext = temp;
            curr = curr->pnext; // curr node becomes the word 'in' 
            
            temp = new WordNode; // then, insert a space after the word 'in'
            temp->word = " ";
            temp->pprev = curr;
            temp->pnext = curr->pnext;
            curr->pnext->pprev = temp;
            curr->pnext = temp;
            curr = curr->pnext;
            return 0;
        }
        else
            curr = curr->pnext;
    }

    if (curr->word == pos && curr->pnext == end)
    {
        insert(in);
        insert(" ");
        return 0;
    }
    return -1;
}

void WordList::insert_ordered(const string st)
{
    if (head == NULL) { // if list is empty, insert the node as head
        head = new WordNode;
        head->word = st;
        head->pnext = NULL;
        head->pprev = NULL;
        return;
    }
    else if (end == NULL) // if end is NULL, but head is not NULL, first determine if this word appeared in head, then update the list
    {  
        if (st.compare(head->word) == 0)
        {
            return;
        }
        else if (st.compare(head->word) > 0)
        {
            end = new WordNode;
            end->word = st;
            end->pprev = head;
            head->pnext = end;
            end->pnext = NULL;
            return;
        }
        else if (st.compare(head->word) < 0)
        {
            end = new WordNode;
            end->word = head->word;
            end->pprev = head;
            end->pnext = NULL;
            head->word = st;
            head->pnext = end;
            return;
        }
    }

    WordNode* index = head;
    while (index)
    {
        if (index->pnext == NULL && st.compare(index->word) > 0) // insert to end
        {
            WordNode* temp = new WordNode;
            temp->word = st;
            temp->pprev = index;
            temp->pnext = NULL;
            index->pnext = temp;
            end = temp;
            return;
        }
        else if (st.compare(index->word) == 0) // already in list
        {
            return;
        }
        else if (st.compare(index->word) < 0 && index->pprev == NULL) // insert to head
        {
            WordNode* temp = new WordNode;
            temp->word = st;
            temp->pprev = NULL;
            temp->pnext = index;
            index->pprev = temp;
            head = temp;
            return;
        }
        else if (st.compare(index->word) < 0) // insert in the middle
        {
            WordNode* temp = new WordNode;
            temp->word = st;
            temp->pprev = index->pprev;
            temp->pnext = index;
            index->pprev->pnext = temp;
            index->pprev = temp;
            return;
        }
        else index = index->pnext;
    }
}

int WordList::erase(const string st) // function to delete all the words 'st' in the list
{
    if (head == NULL) return 0; // if both head and end are NULL, delete fails.
    else if (head->word == st && end == NULL) // if end is NULL, and head->word is 'st', we delete this 'head' node.
    {
        delete head->pprev;
        delete head->pnext;
        head = NULL;
        return 1;
    }

// else, we delete the word by traversing the list
    int n = 0;
    while (head->word == st && head->pnext != end)
    {
        delete head->pprev; // if the first node is the word we want to delete, we first delete the node which saves this word
        WordNode *temp = head;
        head->pnext->pprev = NULL;
        head = head->pnext;
        delete temp;
        ++n;

        delete head->pprev; // then we delete the space or character return after this word
        temp = head;
        head->pnext->pprev = NULL;
        head = head->pnext;
        delete temp;
    }
    if (head->word == st && head->pnext == end)
    {
        delete end->pnext;// if the word to be deleted is store in the node before end, we first delete the node which saves this word
	end->pprev = NULL;
	delete end->pprev;
	end = NULL;
	head->pnext = NULL;
 
        delete head->pnext; // then we delete the space or character return after this word
	delete head->pprev;
	head = NULL;
	++n;
	return n;
    }

    curr = head;
    while (curr->pnext != end)
    {
        if (curr->word == st)
        {
            WordNode *temp = curr; // if the word we want to delete is stored in one node in the middle, we first delete the node which stores this word
            curr->pprev->pnext = curr->pnext;
            curr->pnext->pprev = curr->pprev;
            curr = curr->pnext;
            delete temp;
            ++n;

            temp = curr; // then delete the seperator after the word 'st'
            curr->pprev->pnext = curr->pnext;
            curr->pnext->pprev = curr->pprev;
            curr = curr->pnext;
            delete temp;
       }
        else
            curr = curr->pnext;
    }

    if (curr->word == st && curr->pnext == end)
    {
        delete end->pnext;// if the word to be deleted is store in the node before end, we first delete the node which saves this word
        WordNode *temp = end;
        end->pprev->pnext = NULL;
        end = end->pprev;
        delete temp;
        ++n;
 
        delete end->pnext; // then we delete the space or character return after this word
        temp = end;
        end->pprev->pnext = NULL;
        end = end->pprev;
        delete temp;
    }
    return n;
}

size_t WordList::listSize() // function to return the size of the list
{
    size_t size = 0;

    curr = head;
    while (curr)
    {
        size += sizeof(*curr);
        size += (curr->word).capacity();
        curr = curr->pnext;
    }
    return(size);
}

int WordList::output_forward(const string fileName) // function to output list into document 'filename'
{
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
        output << curr->word;
        curr = curr->pnext;
    }
    output.close();
    return 0;
}

int WordList::output_backward(const string fileName) // function to output list into document 'filename'
{
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
        return 0;
    
    curr = end->pprev; // output starting without the last node which store a seperator character
    while (curr)
    {
        output << curr->word;
        curr = curr->pprev;
    }
    output.close();
    return 0;
}

int WordList::print() // function to output list into document 'filename'
{
    curr = head;
    while (curr)
    {
        cout << curr->word << " ";
        curr = curr->pnext;
    }
    cout << endl;
    return 0;
}

#endif