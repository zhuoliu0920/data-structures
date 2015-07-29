#include "wordlist.h"
#include "freqlist.h"

using namespace std;

int main()
{
    int size;
    WordList L;
    L.read("input.txt");
    FreqList F1;
    FreqList F2;
    F1.frequency(L,L.head);
    size = F2.frequency_unsorted(L);
    F2.sort(size);
    cout << "F1 is:" << endl;
    F1.print();
    cout << "F2 is:" << endl;
    F2.print();
    return 0;
}
