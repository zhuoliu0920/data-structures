#include "hash.h"

int main()
{
    Hash H;
    WordList* wl;
    H.insert("me","1.txt");
    H.insert("me","1.txt");
    H.insert("you","2.txt");
    H.insert("you","100.txt");
    H.insert("me","100.txt");
    H.insert("he","500.txt");
cout << "The positon of the word \'you\' in hash table is: " << H.myhash("you") << endl;
cout << "The following files contains \'you\': " << endl;
    wl = H.search("you");
    wl->print();
cout << "The positon of the word \'me\' in hash table is: " << H.myhash("me") << endl;
cout << "The following files contains \'me\': " << endl;
    wl = H.search("me");
    wl->print();
cout << "The positon of the word \'he\' in hash table is: " << H.myhash("he") << endl;
cout << "The following files contains \'he\': " << endl;
    wl = H.search("he");
    wl->print();
cout << "The positon of the word \'she\' in hash table is: " << H.myhash("she") << endl;
cout << "The following files contains \'she\': " << endl;
    wl = H.search("she");
    wl->print();
    return 0;
}
