This program is called "expression", which can evaluate arithmetic expressions, such as "L1=L2+L3*L4*(L5+L6)", etc. Here, L1-L6 are doubly linked lists, which store words and their frequencied in a document. Operator "+" means the union of two sets, and "*" means the intersection of two set. Parenthese "()" will change the priority of operations. Program will first detect whether input arithmetic expression is valid, then by using two stacks (one transfers infix expression into postfix, and the other one does the evaluation), result will be assigned to "L1" as in the example. This program will allow an expression will at most 1000 operators and operands in total (because they are stored in an array composed by 1000 strings). The sizes of stacks to store operators and operands are 100, under some circumstances, these two stacks may overflow or underflow, then error message will be showed.

The following are valid commands in script file:

read(L1,'input.txt'):
Input a file into a list. If only one node needs to be used to save word, 'head' will save this word, and 'end' keeps NULL. After reading the whole file into word list, program will create a new frequency list, whose nodes contain 'word' and 'frequency', nodes will be sorted as alphabetical order. Empty file will result both empty word list and frequency list.

union(L1,L2,L3):
Find the union of L1 and L2, and store in L3. If both L1 and L2 are empty, then L3 is empty. If L3 is a list which already exists, then program will record an error message, and jump to next line in script file.

intersection(L1,L2,L3)
Find the intersection of L1 and L2, and store in L3. If there is no common word in L1 and L2, then L3 is empty. If L3 is a list which already exists, then program will record an error message, and jump to next line in script file.

L1=L2+L3*L4*(L5+L6)
Evaluate the arithmetic expression, and assign to list L1. If L1 already exists or L2-L6 does not exist, program will record an error message and jump to next command. If arithmetic expression is not valid, or it makes stack overflow or underflow, program will record an error message and jump to next command.

write(L1,'output.txt') & write(L1,'output.txt',forward) & write(L1,'output.txt',reverse)
Write list into a output file. List can be written in forward and backward orders (forward in default). If L1 is an empty list, output file will also be an empty file, but it will be created.
If L1 is not created yet, program will write "List L1 does not exist" into 'output.txt'. And it also records an error message into log file.
Output formation is as follows:

me 1
you 2
he 4
...


All the operations should be written in a script file, for example "script.script". Then program will run this script file as the following command:

expression script="script.script";

One log file named "log.txt" will also be created, which record informations, errors, warnings and size of memory used by list in each operation.
