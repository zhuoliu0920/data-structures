#include "commandlineparser.h"
#include "scriptparser.h"
#include "wordlist.h"
#include "freqlist.h"
#include "arithmetic.h"
#include "global.h"
#include <fstream>
#include <string>
#include <iostream>

using namespace std;

long int count = 0;

long int union_estimate(int, int);
long int intersection_estimate(int, int);

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

    //use CommandLineParser to get the script name and output file name
    char *script = clp.extract("script");
    char *result = clp.extract("result");
    if (script == NULL)
    {
        log << "Error: no script file specified.\n" << endl;
        log.close();
        return -1;
    }
    if (result == NULL)
    {
        log << "Error: no output file specified.\n" << endl;
        log.close();
        return -1;
    }

    //use ScriptParser to get the commands from script file
    ScriptParser SParser =  ScriptParser();
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
    int size, size1, size2;
    string line; // store each line in script file
    WordList *L; // array of word lists
    FreqList *F; // array of frequency lists which store the information of words and its frequency
    string *S; // array of strings which store the name of lists
    L = new WordList[100];
    F = new FreqList[100];
    S = new string[100];
    ofstream result_out; // to record log file
    result_out.open(result);
    result_out << "size,size,operation,T(n) estimate,T(n) actual,c,n0,g(n)=n^2" << endl;

    log << "Begin parsing script file \'" << script << "\':\n" << endl;
    while(getline(indata, line))
    {
        log << line << '\n';
        SParser.ParseLine(line);

        // to decide if listID exits
        int ID = -1; // cmd.listID, after checking, if it's still -1, then cmd.listID is a new list, otherwise cmd.listID is the word list L[ID] or frequency list F[ID]
        int ID2 = -1; // cmd.value1 (if it stores list name), same procesure as above
        int ID3 = -1; // cmd.value2 (if it stores list name), same procesure as above

        for (int j = 0; j < i; j++)
        {
            ID = (SParser.cmd.listID == S[j])?j:ID; // check if cmd.listID is already in S[]
        }
    
        if (SParser.operationCode() != 3 && SParser.operationCode() != 8 && SParser.operationCode() != 0 && ID == -1) 
        { 
            log << "Error: invalid operation, because list \'" << SParser.cmd.listID << "\' is not created yet.\n" << endl;
            if (SParser.operationCode() == 4 || SParser.operationCode() == 5)
            {
                ofstream output;
                output.open(SParser.cmd.value1.c_str());
                output << "List " << SParser.cmd.listID << " does not exist" << endl;
                output.close();
            }
            continue;
        }

        if ((SParser.operationCode() == 3 || SParser.operationCode() == 8) && ID > -1) 
        { 
            log << "Error: invalid operation, because list \'" << SParser.cmd.listID << "\' already exists.\n" << endl;
            continue;
        }

        // do all the list operations, such as read, insert, delete, write.
        switch (SParser.operationCode())
        {
        case 1:
            n = L[ID].insert(SParser.cmd.value2, SParser.cmd.value1);
            if (n == -1) { 
                log << "Warning: in insertion, " << '\'' << SParser.cmd.value1 << '\'' << " cannot be found, so insertion fails.\n";
            }
            else {
                log << '\'' << SParser.cmd.value2 << '\'' << " has been inserted after the first appeared word " << '\'' << SParser.cmd.value1 << "\'.\n";
            }
                log << "Number of bytes used by list \'" << S[ID] << "\' is: " << L[ID].listSize() << ".\n" << endl;
            break;
        case 2:
            n = L[ID].erase(SParser.cmd.value1);
            if (n == 0) log << "Waring: ";
            log << n << " word(s) " << '\'' << SParser.cmd.value1 << '\'' << " have been deleted.\n";
            log << "Number of bytes used by list \'" << S[ID] << "\' is: " << L[ID].listSize() << ".\n" << endl;
            break;
        case 3:
            S[i] = SParser.cmd.listID; // create a new listname in S[]
            n = L[i].read(SParser.cmd.value1); // create a new word list in L[]
            if (n == 0) {
                log << "Input file \'" << SParser.cmd.value1 << "\' is successfully read into list.\n";
            }
            else {
                log << "Error: fail to open input file \'" << SParser.cmd.value1 << "\'.\n" << endl;
            continue;
            }
            F[i].frequency_unsorted(L[i]); // create a new frequency list in F[]
            log << "Number of bytes used by list \'" << S[i] << "\' is: " << F[i].listSize() << ".\n" << endl;
            i++;
            break;
        case 4:
            n = F[ID].output_forward(SParser.cmd.value1);
            if (n == 0) {
                log << "List is successfully written to file " << '\'' << SParser.cmd.value1 << '\'' << " in forward order.\n";
            }
            else {
                log << "Error: fail to create output file " << SParser.cmd.value1 << ".\n";
            }
                log << "Number of bytes used by list \'" << S[ID] << "\' is: " << F[ID].listSize() << ".\n" << endl;
            break;
        case 5:
            n = F[ID].output_backward(SParser.cmd.value1);
            if (n == 0) {
                log << "List is successfully written to file " << '\'' << SParser.cmd.value1 << '\'' << " in reverse order.\n";
            }
            else {
                log << "Error: fail to create output file " << SParser.cmd.value1 << ".\n";
            }
                log << "Number of bytes used by list \'" << S[ID] << "\' is: " << F[ID].listSize() << ".\n" << endl;
            break;
        case 6:
            for (int j = 0; j < i; j++) {
                ID2 = (SParser.cmd.value1 == S[j])?j:ID2;
                ID3 = (SParser.cmd.value2 == S[j])?j:ID3;
            }
            if (ID2 < 0 ) { 
                log << "Error: invalid operation, because list \'" << SParser.cmd.value1 << "\' is not created yet.\n" << endl;
                continue;
            }
            else if (ID3 != -1 ) { 
                log << "Error: invalid operation, because list \'" << SParser.cmd.value2 << "\' already exists.\n" << endl;
                continue;
            }
            else {
                S[i] = SParser.cmd.value2;
                size1 = F[ID].size();
                size2 = F[ID2].size();
                size = (size1 > size2)?size1:size2;
                count = 0;
                F[ID].sort();
                F[ID2].sort();
                if (size < 100) F[i].list_union(F[ID],F[ID2],F[ID].head,F[ID2].head);
                else F[i].list_union_nonrec(F[ID],F[ID2]);

                result_out << size1<< ',' << size2 << ",union," << union_estimate(size1, size2) << ',' << count << ",66,1," << size*size << endl;
                log << "Comlexity is successfully written to file " << '\'' << result << '\'' << ".\n";
                log << "Number of bytes used by list \'" << S[i] << "\' is: " << F[i].listSize() << ".\n" << endl;
                i++;
            }
            break;
        case 7:
            for (int j = 0; j < i; j++) {
                ID2 = (SParser.cmd.value1 == S[j])?j:ID2;
                ID3 = (SParser.cmd.value2 == S[j])?j:ID3;
            }
            if (ID2 < 0 ) { 
                log << "Error: invalid operation, because list \'" << SParser.cmd.value1 << "\' is not created yet.\n" << endl;
                continue;
            }
            else if (ID3 != -1 ) { 
                log << "Error: invalid operation, because list \'" << SParser.cmd.value2 << "\' already exists.\n"  << endl;
                continue;
            }
            else {
                S[i] = SParser.cmd.value2;
                size = ((F[ID].size() > F[ID2].size())? F[ID].size():F[ID2].size());
                size1 = F[ID].size();
                size2 = F[ID2].size();
                size = (size1 > size2)?size1:size2;
                count = 0;
                F[ID].sort();
                F[ID2].sort();
                if (size < 100) F[i].list_intersection(F[ID],F[ID2],F[ID].head,F[ID2].head);
                else F[i].list_intersection_nonrec(F[ID],F[ID2]);

                result_out << size1 << ',' << size2 << ",intersection," << intersection_estimate(size1, size2) << ',' << count << ",53,1," << size*size << endl;
                log << "Complexity is successfully written to file " << '\'' << result << '\'' << ".\n";
                log << "Number of bytes used by list \'" << S[i] << "\' is: " << F[i].listSize() << ".\n" << endl;
                i++;
            }
            break;
        case 8:
            string *expression;
            expression = analysis(SParser.cmd.value1);
            if (expression == NULL) {
                log << "Error: invalid or too long arithmetic expression.\n"  << endl;
                continue;
            }
            S[i] = SParser.cmd.listID;
            n = arithmetic(expression, S, F, i); 
            if (n == -1) {
                log << "Error: one or more lists in above arithmetic expression do not exist, or invalid arithmetic expression which makes stack overflow or underflow.\n"  << endl;
                continue;
            }
            log << "Number of bytes used by list \'" << S[i] << "\' is: " << F[i].listSize() << ".\n" << endl;
            i++;
            delete[] expression;
            break;
        case 0:
            log << "Error: above line is not a valid script form.\n" << endl;
            break;
        } 
    }
  
  result_out.close();
  log << "End parsing script file \'" << script << "\'.\n" <<endl;
  log.close();

  delete[] L;
  delete[] F;
  delete[] S;
  return 0;
}


long int union_estimate(int n1, int n2)
{
    int n = (n1>n2)?n1:n2;
    if (n1 < 0 || n2 < 0) return -1;
    else if (n == 0) return 4;
    else if (n == 1 && n1*n2 == 0) return 32;
    else if (n == 1) return 66;
    else return (4*n1*n1+28*n1+4*n2*n2+28*n2+17*(n1+n2)-36);
}

long int intersection_estimate(int n1, int n2)
{
    int n = (n1>n2)?n1:n2;
    if (n1 < 0 || n2 < 0) return -1;
    else if (n == 0) return 4;
    else if (n == 1 && n1*n2 == 0) return 22;
    else if (n == 1) return 53;
    else if (n > 1) return (4*n1*n1+28*n1+4*n2*n2+28*n2+8*(n1+n2)-33);
}
