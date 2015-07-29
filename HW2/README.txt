This is a editor program, which can do the operations "read", "union", "intersection" and "write". Words in documents will be saved as a doubly linked list ADT, and word list will be also automatically transfered into a frequency list which contains information of words and their frequencies. If input file is an empty file or does not exit, then it will be saved as a empty list, also can be outputed as a empty file. The following is the instructions on each operation:


read(L1,'input.txt'):
Input a file into a list. If only one node needs to be used to save word, 'head' will save this word, and 'end' keeps NULL. After reading the whole file into word list, program will create a new frequency list, whose nodes contain 'word' and 'frequency', nodes will be sorted as alphabetical order. Empty file will result both empty word list and frequency list.

union(L1,L2,L3):
Find the union of L1 and L2, and store in L3. If both L1 and L2 are empty, then L3 is empty. If L3 is a list which already exists, then program will record an error message, and jump to next line in script file.

intersection(L1,L2,L3)
Find the intersection of L1 and L2, and store in L3. If there is no common word in L1 and L2, then L3 is empty. If L3 is a list which already exists, then program will record an error message, and jump to next line in script file.

write(L1,'output.txt',forward) & write(L1,'output.txt',reverse)
Write list into a output file. List can be written in forward and backward orders. If L1 is an empty list, output file will also be an empty file, but it will be created.
If L1 is not created yet, program will record an error message, and be terminated.
Output formation is as follows:

me 1
you 2
he 4
...


All the operations should be written in a script file, for example "script.script". Then program will run this script file as the following command:

recursion script="script.script";

One log file named "log.txt" will also be created, which record informations, errors, warnings and size of memory used by list in each operation.
