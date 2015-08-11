This program is called "keywordsearch", which can search the files that contains the key words specified (similar to what Google does). The program works as following steps:

1. load(corpus,'filelist.txt'):
Load the files specified in 'filelist.txt' into memory. In particular, file names will be first read into a temporary word list, and sorted alphabetically. Then, a list which is used to store both file names and file contents will be created. File names and files contents are stored in the list as:
*filename1->word1->word2->word3->......->wordn->*filename2->word1->word2->......
To make a difference between filename and words in a file, a '*' sign is added in front of filenames in the list manually. In order to avoid duplicated words stored for each file part, words in each file will also be first sorted.
Since numbers cannot be keywords, all the numbers and seperators+numbers will be removed from list. For example, "123" and "123-456" will be removed, but "123-456-keys" will be kept.

2. filter(corpus,'remove.txt')
Remove all the words in 'remove.txt' from 'corpus' list. Words in 'remove.txt' will be first read into a word list, then 'filter' is equivalent to find the difference of two sets (very similar to intersection). Therefore, a function 'difference' will be called in order to implement this feature.

3. Index(corpus)
'corpus' will be indexed into a hash table. When there is conflicts, hash table will do chaining. The elements in the hash table are lists. The nodes of each list are composed by two elements: a list to store filenames, and a string to store key word. So basically, it is a list of lists. Since as specified in Homework Requirement, there will be about 10000 words stored in hash table, the size of the hash table is chosen to be a prime number 100003. And the hash function is defined as:
Choose three different prime numbers: a = 55001, b = 84011, c = 11, for any string 'st', hash function will return 'val':

for (i=0; i<st.length(); i++)
{
   val = (val*a)^(int(st[i])*b);
}
val = val%100003;
if(val < 0) val += 100003;

The last line above is to make sure the value hash function returns is nonnegative.

4. search(R1,'keyword1,keyword2,...',corpus)
Search keywords in hash table, and store the result in list 'R1'. Keywords can be in upper case, but when it is read into memory, it will be converted into lower case. (We do this because all the words in hash table will also be converted into lower case).
The formation "search(R1,'keyword')" is not allowed, because no corpus specified. Under this case, an error message will show, and 'R1' will not be created.

5. VERY IMPORTANT! It is possible for this program to load different filelists into memory, and filter them. However, there will be only one hash table. Therefore, any word in any file, no matter in which corpus this file is specified, will be all stored in the same hash table after indexing. Moreover, it's not allowed to use same 'corpus' for different filelists. For example, in one script file, if there are the following commands:

load(corpus,'filelist1.txt')
filter(corpus,'remove.txt')
index(corpus)
load(corpus,'filelist2.txt')
filter(corpus,'remove.txt')
index(corpus)

There will be an error message showed for the second 'load' command, because 'corpus' already exists, and it is not allowed to overwrite it.


The four commands "load", "filter", "index" and "search" are new from previous homeworks. Also, this program still supports the operations "read", "write", "union", "intersection", "insert", "delete", arithmetic operations as in previous homeworks. 

The only difference is that in "write" function, the FREQUENCY will NOT be showed, only the word.

Moreover, in this program, all seperators in front and after a word will be ignored. For example, "you??", "..you" are equivalent to "you", but "you're" is considered as a single word, "you're".

All operations should be written in a script file, for example "script.script". Then program will run this script file as the following command:

keywordsearch script="script.script"

One log file named "log.txt" will also be created, which record informations, errors, warnings and size of memory used by list in each operation.
