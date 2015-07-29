#ifndef HASH_H
#define HASH_H

#include "hashlist.h"

class Hash
{
public:
    HashList* Table;
    int size;

    Hash();
    Hash(int);
    ~Hash();

    int myhash(const string);
    void insert(const string, const string);
    WordList* search(const string);
};
int hashfn(const string);


Hash::Hash()
{
    size = 100003;
    Table = new HashList[100003];
};
    
Hash::Hash(int n)
{
    size = n;
    Table = new HashList[n];
};

Hash::~Hash()
{
    delete[] Table;
};

int Hash::myhash(const string s)
{
    int hashVal = hashfn(s);
    hashVal %= size;
    if (hashVal < 0) hashVal += size;
    return hashVal;
}

void Hash::insert(const string index, const string filename)
{
    int pos = Hash::myhash(index);
    WordList* indexlist = ((Table[pos]).search(index));

    if (indexlist->head == NULL) {
        (Table[pos]).insert(index,filename);
        return;
    }
    else {
        (*indexlist).insert(filename);
        return;
    }
}

WordList* Hash::search(const string index)
{
    int pos = Hash::myhash(index);
    return ((Table[pos]).search(index));
}

int hashfn(const string s)
{
    int A = 55001;
    int B = 84011;
    int val = 11;
    char temp;
    for (unsigned int i = 0; i < s.length(); i++) {
        temp = s.at(i);
        val = (val*A)^(int(temp)*B);
    }
    return val;
}

#endif
