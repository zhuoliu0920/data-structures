#include <string>
#include <iostream>

using namespace std;

void merge(string*, int, int, int, string*);
void msort(string*, int, int, string*);
int bsearch(string*, int, int, string);
int checksorted(string*, int, int);

void merge(string *A, int lpos, int lend, int rend, string *B)
{
    int rpos = lend + 1;
    int count = lpos;
    int num = rend - lpos + 1;

    while (lpos <= lend || rpos <= rend) {
        if (lpos > lend) B[count++] = A[rpos++] ;
        else if (rpos > rend) B[count++] = A[lpos++];
        else if (A[lpos].compare(A[rpos]) < 0) B[count++] = A[lpos++];
        else B[count++] = A[rpos++];
    }

    for (int i = 0; i < num; i++, rend--) {
        A[rend] = B[rend];
    }
}

void msort(string *A, int l, int r, string *B)
{
    if (l < r) {
        int m = l + (r-l)/2;
        msort(A, l, m, B);
        msort(A, m+1, r, B);
        merge(A, l, m, r, B);
    }
}

int bsearch(string *A, int l, int r, string key)
{
    if (l > r) return -1;
    else {
        int m = l + (r-l)/2;
        if (A[m] == key) return m;
        else if (A[m] > key) return bsearch(A, l, m-1, key);
        else if (A[m] < key) return bsearch(A, m+1, r, key);
    }
}

int checksorted(string *A, int l, int r)
{
    int i = l;
    while (i < r) {
        if (A[i].compare(A[i+1]) > 0) return 0;
        else i++;
    }
    return 1;
}
