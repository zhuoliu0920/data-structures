#include "hashlist.h"

int main()
{
    HashList hl;
    WordList *wl;
    hl.insert("you","1.txt");
    hl.insert("you","2.txt");
    hl.insert("you","100.txt");
    hl.insert("me","100.txt");
    hl.insert("he","500.txt");
cout << "The following files contains \'you\': " << endl;
    wl = hl.search("you");
    wl->print();
cout << "The following files contains \'me\': " << endl;
    wl = hl.search("me");
    wl->print();
cout << "The following files contains \'he\': " << endl;
    wl = hl.search("he");
    wl->print();
cout << "The following files contains \'she\': " << endl;
    wl = hl.search("she");
    wl->print();
    return 0;
}
    
