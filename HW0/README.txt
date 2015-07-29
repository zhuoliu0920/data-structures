"topWord.cpp" is the source file for the program which can find the most repeating words in a text file, and show its (or their) frequency.


To execute the program, just simply type:

topWord filename=input.txt;

where "input.txt" is the text file where you want to find the most frequently words.


Since in the code, we save all the words in an array which is composed by 100 strings, so this program can only be used on a text which is composed by at most 100 different words. If assigned file does not exist or cannot be properly opened, there will be an "Error" showed. If the file is composed by no word, a "Warning" will show.

We will treat the words like "you're, i'm, boys', Tic-Tac-Toe" as a single word, and a capitalized word is the same as a non-capitalized word.