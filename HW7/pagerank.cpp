#include "commandlineparser.h"
#include "scriptparser.h"
#include "freqlist.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    ofstream log; // to record log file
    log.open("log.txt", std::fstream::app);
    CommandLineParser clp(argv[1],';');   //instantiating the class CommandLineParser

    if (argc == 1) 
    {
        log << "Error: no argument is passed.\n" << endl;
        log.close();
        return -1;
    }

    //use CommandLineParser to get the script name
    char *script = clp.extract("script");
    if (script == NULL || *script == '\0')
    {
        log << "Error: no script file specified.\n" << endl;
        log.close();
        return -1;
    }
    
    //use ScriptParser to get the commands from script file
    ScriptParser SParser = ScriptParser();
    ifstream indata(script);
    if (!indata.is_open()) // check if script file is correctly opened.
    {
        log << "Error: Script file \'" << script << "\' cannot be opened or does not exist.\n" << endl;
        log.close();
        return -1;
    }

    //start parsing script file    
    int i = 0;
    int n;
    string line; // store each line in script file
    FreqList *F; // array of frequency lists which store the information of words and its frequency
    string *S; // array of strings which store the name of lists
    F = new FreqList[1000];
    S = new string[1000];

    //initiate temporary lists and list nodes
    WordNode *tmpnode;
    WordList tmplist1;
    WordList tmplist2;
    ifstream input;

    log << "Begin parsing script file \'" << script << "\':\n" << endl;
    while(getline(indata, line))
    {
        log << line << '\n';
        SParser.ParseLine(line);

        //process to determine if listID exits
        int ID = -1; // cmd.listID, after following checking process, if it's still -1, then cmd.listID is a new list, otherwise cmd.listID is the word list L[ID] or frequency list F[ID]

        for (int j = 0; j < i; j++)
        {
            ID = (SParser.cmd.listID == S[j])?j:ID; // check if cmd.listID is already in S[]
        }

        if (SParser.operationCode() == 14 && ID == -1) 
        { 
            log << "Error: invalid operation, because list \'" << SParser.cmd.listID << "\' is not created yet.\n" << endl;
            ofstream output;
            output.open(SParser.cmd.value1.c_str());
            output << "List " << SParser.cmd.listID << " does not exist" << endl;
            output.close();
            continue;
        }

        if (SParser.operationCode() == 10 && ID > -1) 
        { 
            log << "Error: invalid operation, because list \'" << SParser.cmd.listID << "\' already exists.\n" << endl;
            continue;
        }

        // do all the list operations, such as read, insert, delete, write, intersection, union, load, filter, index and seach
        switch (SParser.operationCode())
        {
        case 1:
            log << "Error: this program does not support \'insert\' operation." << endl;
            break;
        case 2:
            log << "Error: this program does not support \'delete\' operation." << endl;
            break;
        case 3:
            log << "Error: this program does not support \'read\' operation." << endl;
            break;
        case 4:
            log << "Error: this program does not support \'write\' operation." << endl;
            break;
        case 5:
            log << "Error: this program does not support \'write\' operation." << endl;
            break;
        case 6:
            log << "Error: this program does not support \'union\' operation." << endl;
            break;
        case 7:
            log << "Error: this program does not support \'intersection\' operation." << endl;
            break;
        case 8:
            log << "Error: this program does not support \'arithmetic\' operation." << endl;
            break;
        case 9:
            log << "Error: this program does not support \'check\' operation." << endl;
            break;
        case 10:
            tmplist1.clear();
            tmplist2.clear();
            S[i] = SParser.cmd.listID; // create a new listname in S[]
            n = tmplist1.read_keepUpper(SParser.cmd.value1);
            if (n < 0) {
                log << "Error: fail to open input file \'" << SParser.cmd.value1 << "\'.\n" << endl;
                continue;
            }
            tmpnode = tmplist1.head;
            while (tmpnode) {
                input.open((tmpnode->word).c_str());
                if (input) {
                    tmplist2.insert_ordered(tmpnode->word);
                    F[i].insert(tmpnode->word,0,0,true);
                }
                else
                    F[i].insert(tmpnode->word,0,0,false);
                input.close();
                tmpnode = tmpnode->pnext;
            }
            tmpnode = tmplist2.head;
            while (tmpnode) {
                F[i].update(tmpnode->word,tmplist2);
                tmpnode = tmpnode->pnext;
            }
            log << "Graph based on the pages in \'" << SParser.cmd.value1 << "\' has been successfully stored in list \'" << S[i] << "\'.\n" << endl;
            i++;
            break;
        case 11:
            log << "Error: this program does not support \'filter\' operation." << endl;
            break;
        case 12:
            log << "Error: this program does not support \'index\' operation." << endl;
            break;
        case 13:
            log << "Error: this program does not support \'search\' operation." << endl;
            break;
        case 14:
            n = F[ID].output(SParser.cmd.value1);
            if (n == 0) {
                log << "Information about the graph stored in list \'" << S[ID] << "\' has been successfully written into file \'" << SParser.cmd.value1 << "\'.\n" << endl;
            }
            else {
                log << "Error: fail to create output file \'" << SParser.cmd.value1 << "\'.\n" << endl;
            }
            break;
        case 0:
            log << "Error: above line is not a valid script form.\n" << endl;
            break;
        }
    }
  
  log << "End parsing script file \'" << script << "\'.\n" <<endl;
  log.close();
  delete[] F;
  delete[] S;
  return 0;
}

