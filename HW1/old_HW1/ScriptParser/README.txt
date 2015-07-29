UNIVERSITY OF HOUSTON

Class ScriptParser Documentation

How to compile
--------------

Your main program should contain the appropiate include file:
#include "MyParser.cpp"

You should copy MyParser.cpp and MyParser.h to your source code directory
and compile your program as follows:
compile: bcc32 -w-inl -eeditor.exe *.cpp

How to use the class.

1) Create one instance of the class
   ScriptParser SParser =  ScriptParser();
2) Call the method ParseLine with the line you want to parse ( type must be string)  
   SParser.ParseLine(line);
3) After that, the instance has updated its properties
		cmd.operation; // insert,delete,read,fwrwrite,revwrite
		cmd.listID;    // Identification of the list
		cmd.value1;    // parameter string
		cmd.value2;    // second parameter string (used only for insert operations)
4) Call your own C++ function using the parameters as needed

lparser.cpp is a sample program that opens a script file and invokes the
class ScriptParserfor each line of the file. For testing, you should compile
with MyParser.cpp and MyParser.h

compiling: bcc32 -w-inl -elparser.exe *.cpp
executing: lparser.exe scriptfile="sample2.txt"
