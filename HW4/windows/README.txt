This program can do the operations "read", "union", "intersection" and "write", and record time complexity for "union" and "intersection". Words in documents will be saved in a doubly linked list called word list first, then word list will be automatically transfered into a frequency list which contains information of words and their frequencies. If input file is an empty file or does not exit, it will be saved as a empty list, also can be outputed as a empty file. How to use each operation is completely same as in previous programs. The following is how this program works:

1. By "read" operation, program will save the words from input text file into a doubly linked list first, then automatically transfer into a frequency list, which is unsorted and may contain repeated words in different nodes.

2. When "union" or "intersection" is called, program will start couting # of operations. Frequency lists, which are going to be unioned or intersected, which be first sorted alphabetically by insertion sort method, then program will do the operation of "union" or "intersection" on these two sorted lists. "Union" and "intersection" will be implemented by recursive funcions in default. However, in order to prevent stack overflow, when list size is very large (n > 100), non-recursive type functions will be called.

3. Time complexity T(n). By counting # of operations in the program, we can get an actual T(n). Program will also provide an estimate T(n), which is obtained by using ¡±worst case scenario¡± conditions. Therefore, for any input size and any input data, it is always true that "T(n) actual <= T(n) estimate". Since T(n) is a function with respect to n, program will also determine a function g(n), such that T(n) ~ O(g(n)), and c and n0 are also provided which arise from the definiton of "Big O" notation. In particular, in this program, n is max(n1,n2), where n1 and n2 are sizes of lists L1 and L2. In insertion sort, T(n) ~ O(n^2), and in both union and intersection operation, T(n) ~ O(n), so T(n) ~ O(n^2) in total.

4. Worst case. Worst case occurs when: 
(1) sizes of two lists are equal. 
(2) both lists are reverse ordered. 
(3) In union, the words in two lists are one-by-one, for example, L1 is "a, c, e, g, i" and L2 is "b, d, f, h, j". 
(4) In intersection, L1 and L2 have completely same words.
If all of the above conditions hold, then "T(n) actual = T(n) estimate".

5. Since here we use ¡±worst case scenario¡± to estimate T(n), which is dominated by sorting operation, and T(n) average is approximately equal to half of T(n) worst, so in the result, "T(n) estimate" will be approximately twice larger than "T(n) actual", but they are both of O(n^2).

6. T(n) estimation (here, suppose sizes of list L1 and L2 are n1 and n2, correspondingly):
(1) In union, T(n) in sorting each list is 4n^2+28n-17, where n is the size of each list. T(n) in union operation is 17(n1+n2)-2. Therefore, in total, estimated T(n) = 4(n1^2+n2^2)+28(n1+n2)+17(n1+n2)-36.
(2) In intersection, T(n) in sorting each list is 4n^2+28n-17. T(n) in intersection operation is 8(n1+n2)+1. Therefore, in total, estimated T(n) = 4(n1^2+n2^2)+28(n1+n2)+8(n1+n2)-33.
(3) By letting n = max(n1,n2), we will have T(n) ~ O(n^2) for both cases.

7. "T(n) estimate", "T(n) actual" and c*g(n). When these three function are ploted in logarithm scale, they are 3 parellal lines, with slope k=2. However, "T(n) actual" is always below "T(n) estimate", and "T(n) estimate" is always below c*g(n).



All operations should be written in a script file, for example "script.script". Then program will run this script file as the following command:

complexity script="script.script";result="out.txt"

One log file named "log.txt" will also be created, which record informations, errors, warnings and size of memory used by list in each operation.
