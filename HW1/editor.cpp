#include "commandlineparser.h"
#include "scriptparser.h"
#include "wordlist.h"
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
    ofstream log; // to record log file
    log.open("log.txt", std::fstream::app);
    int i = 0;
    int n;
    CommandLineParser clp(argv[1],',');   //instantiating the class CommandLineParser
    string line; // store each line in script file
    string listID = "";
    WordList L1; // list class

    if (argc == 1) 
    {
        log << "Error: no argument is passed." << endl;
        log.close();
        return -1;
    }

    //use CommandLineParser to get the script name
    char *script = clp.extract("script");
    if (script == NULL)
    {
        log << "Error: no script file specified." << endl;
        log.close();
        return -1;
    }

    //use ScriptParser to get the commands from script file
    ScriptParser SParser =  ScriptParser();
    ifstream indata(script);
    if (!indata.is_open()) // check if script file is correctly opened.
    {
        log << "Error: Script file \'" << script << "\' cannot be opened or does not exist." << endl;
        log.close();
        return -1;
    }
    
    log << "Begin parsing:\n" << endl;
    while(getline(indata, line))
    {
        log << line << '\n';
        SParser.ParseLine(line);

// we need to use "map" container to map different 'listID' to corresponding 'list', here, we suppose only one list will be used (the first created one), the other 'listID' which are different from the first one will be invalid.

/*        if (SParser.operationCode() == 3 && i == 0) // this is the first time create listID
        { 
            listID = SParser.cmd.listID;
        }
        else if (listID == "") // this is the time when list is not created yet
        {
            log << "Error: invalid operation, because list \'" << SParser.cmd.listID << "\' is not created yet, program is terminated.\n\n" << "End of parsing.\n" << endl;
            return -1;
        }
        else if (listID != SParser.cmd.listID) // if we want to create or visit more list other than the first, omit this operation
        {
            log << "Error: only one list \'" << listID << "\' is permitted, \'" << SParser.cmd.listID << "\' is not or cannot be created, program is terminated.\n\n" << "End of parsing.\n" <<endl;
            return -1;
        } */

        // do all the list operations, such as read, insert, delete, write.
        switch (SParser.operationCode())
        {
        case 1:
            n = L1.insert(SParser.cmd.value2, SParser.cmd.value1);
            if (n == -1) { 
                log << "Warning: in insertion, " << '\'' << SParser.cmd.value1 << '\'' << " cannot be found, so insertion fails.\n";
            }
            else {
                log << '\'' << SParser.cmd.value2 << '\'' << " has been inserted after the first appeared word " << '\'' << SParser.cmd.value1 << "\'.\n";
            }
                log << "Number of bytes used by list is: " << L1.listSize() << ".\n" << endl;
            break;
        case 2:
            n = L1.erase(SParser.cmd.value1);
	    if (n == 0) log << "Waring: ";
            log << n << " word(s) " << '\'' << SParser.cmd.value1 << '\'' << " have been deleted.\n";
            log << "Number of bytes used by list is: " << L1.listSize() << ".\n" << endl;
            break;
        case 3:
            ++i;
            n = L1.read(SParser.cmd.value1);
            if (n == 0) {
                log << "Input file \'" << SParser.cmd.value1 << "\' is successfully read into list.\n";
            }
            else {
                log << "Error: fail to open input file \'" << SParser.cmd.value1 << "\', program is terminated.\n" << endl;
		log << "End of parsing.\n" << endl;
		return -1;
            }
                log << "Number of bytes used by list is: " << L1.listSize() << ".\n" << endl;
            break;
        case 4:
            n = L1.output_forward(SParser.cmd.value1);
            if (n == 0) {
                log << "List is successfully written to file " << '\'' << SParser.cmd.value1 << '\'' << " in forward order.\n";
            }
            else {
                log << "Error: fail to create output file " << SParser.cmd.value1 << ".\n";
            }
                log << "Number of bytes used by list is: " << L1.listSize() << ".\n" << endl;
            break;
        case 5:
            n = L1.output_backward(SParser.cmd.value1);
            if (n == 0) {
                log << "List is successfully written to file " << '\'' << SParser.cmd.value1 << '\'' << " in reverse order.\n";
            }
            else {
                log << "Error: fail to create output file " << SParser.cmd.value1 << ".\n";
            }
                log << "Number of bytes used by list is: " << L1.listSize() << ".\n" << endl;
            break;
        case 0:
            cout << SParser.cmd.listID << endl;
            cout << "Error: above line is not a valid script form.\n" << endl;
            log << "Error: above line is not a valid script form.\n" << endl;
            break;
        } 
    }
  
  log << "End of parsing.\n" <<endl;
  log.close();
  return 0;
}
