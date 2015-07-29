#include "freqlist.h"

using namespace std;

int main()
{
    FreqList fq; 
    fq.insert("good",1);
    fq.print();
    fq.deletenode(fq.head);
    fq.print();
    fq.insert("good",1);
    fq.insert("bad",1);
    fq.print();
    fq.deletenode(fq.head);
    fq.print();
    fq.insert("and",1);
    fq.print();
    fq.deletenode(fq.end);
    fq.print();
    fq.insert("and",1);
    fq.insert("you",1);
    fq.print();
    fq.deletenode((fq.head)->pnext);
    fq.print();
    return 0;
}
