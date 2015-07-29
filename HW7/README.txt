This program is called "pagerank", which can find the most "popular" pages in a "network", find the broken links and sinks.

1. load(W1,'page.txt'):
Load the pages specified in the input file 'page.txt', and save into a doubly linked list called 'W1'. Each node in the list contains 4 variables: page name(string), indegree(int), outdegree(int) and Existence(bool). Each time when a new page specified in 'page.txt' is read, the doubly linked list will be automatically updated. Program will find the 'hyperlinks' in the page, so all the information will be obtained.

2. rank(W1,'out.txt')
Rank the pages in the list 'W1' by indegree, find the broken links and sinks, and save the result into the file 'out.txt'.
At most 5 pages will be showed in the result based on the indegree, if several pages have same indegrees, they will be ranked by alphabetically order. Broken links are defined as "Do Not exist, and indegree is non-zero", and sinks are defined as "Exist, and outdegree is zero".

In this homework, all the commands appeared in old homeworks will not be supported.

All operations should be written in a script file, for example "script.script". Then program will run this script file as the following command:

pagerank script="script.script"

One log file named "log.txt" will also be created, which record informations, errors, warnings and size of memory used by list in each operation.
