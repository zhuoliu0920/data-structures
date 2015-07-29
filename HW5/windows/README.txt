This program is called "spellchecker", which can check misspelled words in a document based on a specified dictionary, and output those misspelled words in an output file. The program works as following steps (time complexity for each step, suppose size of dictionary is n, and size of file to be checked is m):

1. Read dictionary file and save it into a doubly linked list. ~ O(n)

2. Based on the size of the list, allocate memory for a string array with exactly same size as the list, and copy the dictionary into this array. ~ O(n)

3. Do merge sort to this array (sorted alphabetically). (Now, there may be duplicate words in the array) ~ O(nlog(n))

4. Calculate the number of non-duplicate words in the array, and allocate a new array with updated size. ~ O(n)

5. Copy all the non-duplicate words into this new array. ~ O(n)

6. Deallocate memory for above linked list, and temporary array. (Only the final sorted, unrepeated array is left.)

7. Read the file to be checked into a doubly linked list, and sort this list. (Here, since I use the old code from last homework, which use insertion sort, so the time complexity for this step is O(m^2).)

8. Traverse this linked list, and by binary search method, determine if the word in the list exists in dictionary array. If it does, do nothing. Otherwise, insert this word and its frequency into another list. ~ O(mlog(n))

9. Output the list which store all the words that are not in dictionary. ~ O(m)


The commands in script file for the operation "read", "write", "union", "intersection", "insert", "delete", arithmetic operations are exactly same as in previous homeworks. There is one new command:

check(R1,L1)

which can check all the words in list L1, based on input dictionary file, and record into the list R1.

In this homework, all seperators will not be considered as part of the words. Therefore, "you??", "..you" are equivalent to "you", but "you're" is considered as two words, "you" and "re".


All operations should be written in a script file, for example "script.script". Then program will run this script file as the following command:

spellchecker script="script.script";dictionary="dictionary.txt"

One log file named "log.txt" will also be created, which record informations, errors, warnings and size of memory used by list in each operation.
