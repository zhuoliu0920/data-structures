This is a editor program, which can do the operations "read", "insert", "delete" and "write". Words in documents will be saved as a doubly linked list ADT. If input file is an empty file or does not exit, then it will be saved as a empty list, also can be outputed as a empty file. The following is the instructions on each operation:


read(L1,'input.txt'):
Input a file into a list. If only one node needs to be used to save word, 'head' will save this word, and 'end' keeps NULL. After reading the whole file, program will check if 'end' node saves a seperator (space or return). If not, program will insert a return at the end. 
For this program, only one list is permitted in one script file. Therefore, if in one script, there are two command: read(L1,'input1.txt') and read(L2,'input2.txt'), program will record an error message, and be terminated.

insert(L1,'word1/word2'):
Insert word1 after word2. If word1 is followed by a space (no matter at the end of a line or in the middle), word2 will be inserted right after the space after word1. If word1 is followed by a return, then word2 will be inserted at the beginning of next line.
If word2 cannot be found in the list, program will record an error message, but countinue next line of script file.
If L1 is not created yet, program will record an error message, and be terminated.

delete(L1,'word')
Delete all 'word' in the list. The seperators right after 'word' will also be deleted, no matter is space or return.
If no 'word' can be found, program will record an warning message, and continue next line of script file.
If L1 is not created yet, program will record an error message, and be terminated.

write(L1,'output.txt',forward) & write(L1,'output.txt',reverse)
Write list into a output file. List can be written in forward and backward orders. If L1 is an empty list, output file will also be an empty file, but it will be created.
If L1 is not created yet, program will record an error message, and be terminated.


All the operations should be written in a script file, for example "script.script". Then program will run this script file as the following command:

editor script="script.script";

One log file named "log.txt" will also be created, which record informations, errors, warnings and size of memory used by list in each operation.