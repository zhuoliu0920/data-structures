#include "hash.h"
#include "freqlist.h"

int main()
{
    WordList wl;
    FreqList fl;
    Hash corpus;
    wl.read_keepUpper("input.txt");
    wl.print();
/*    fl.frequency_unsorted(wl);
    fl.sort();

    FreqNode *index;
    Node *index2;
    index = fl.head;
    int j;
    int emptycount;
    while(index) {
        corpus.insert(index->word,"input.txt");
        index = index->pnext;
    }
    for (int i = 0; i < corpus.size; i++) {
        j = 0;
        index2 = (corpus.Table[i]).head;
        while(index2) {
            j++;
            index2 = index2->pnext;
        }
//        if (j > 1) cout << "collision occurs at position " << i << "," << j << endl;
        if (j == 0) {cout << "empty spot at position " << i << endl; emptycount++;}
    }
    cout << "size of input file is: " << fl.size() << endl;
    cout << "size of total spot is: " << corpus.size << endl;
    cout << "number of empty spot is: " << emptycount << endl; */
    return 0; 
}
