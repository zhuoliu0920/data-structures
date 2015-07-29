#include "commandlineparser.h"
#include "scriptparser.h"
#include "wordlist.h"
#include "freqlist.h"
#include "arithmetic.h"
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

    //use CommandLineParser to get the script name and output file name
    char *script = clp.extract("script");
    char *dictionary = clp.extract("dictionary");
    if (script == NULL || *script == '\0')
    {
        log << "Error: no script file specified.\n" << endl;
        log.close();
        return -1;
    }
    if (dictionary == NULL || *dictionary == '\0')
    {
        log << "Error: no dictionary specified.\n" << endl;
        log.close();
        return -1;
    }

    //save dictionary into an array
    int dic_size_temp; // size of temporary input dictionary (repeated words may appear)
    int dic_size; // size of sorted dictionary array (no repeated words)
    WordList *DIC_list1; // first, save dictionary into a word list
    FreqList *DIC_list2; // then, convert the word list into an unsorted frequency list
    string *DIC_temp; // temp array to store sorted dictionary (repeated word may appear)
    string *DIC; // array to store sorted dictionary (no repeated words)

    DIC_list1 = new WordList;
    if ((*DIC_list1).read(dictionary) != 0) {
        log << "Error: dictionary file \'" << dictionary << "\' cannot be correctly opened.\n" << endl;
        delete DIC_list1;
        log.close();
        return -1;
    }

    DIC_list2 = new FreqList;
    (*DIC_list2).frequency_unsorted(*DIC_list1);
    dic_size_temp = (*DIC_list2).size();
    DIC_temp = (*DIC_list2).toarray();
    delete DIC_list1;
    delete DIC_list2;

    if (checksorted(DIC_temp, 0, dic_size_temp-1) == 1) { // if array if sorted, then DIC is DIC_temp
        DIC = DIC_temp;
        dic_size = dic_size_temp;
    }
    if (checksorted(DIC_temp, 0, dic_size_temp-1) == 0) { // else, use merge sort method to sort DIC_temp, and eliminate duplicated words in DIC_temp and copy to DIC
        string *temparray;
        temparray = new string[dic_size_temp];
        msort(DIC_temp, 0, dic_size_temp-1, temparray);
        delete[] temparray; // finish sorting
        dic_size = 1;
        for (int i = 1; i < dic_size_temp; i++) {
            if (DIC_temp[i] != DIC_temp[i-1]) dic_size++;
        } // count unreapted words
        DIC = new string[dic_size];
        DIC[0] = DIC_temp[0];
        int ii = 1;
        for (int i = 1; i < dic_size_temp; i++) {
            if (DIC_temp[i] != DIC_temp[i-1]) DIC[ii++] = DIC_temp[i];
        }
        delete[] DIC_temp; // finish copying
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
    int size;
    string line; // store each line in script file
    WordList *L; // array of word lists
    FreqList *F; // array of frequency lists which store the information of words and its frequency
    string *S; // array of strings which store the name of lists
    L = new WordList[1000];
    F = new FreqList[1000];
    S = new string[1000];

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

        if (SParser.operationCode() != 3 && SParser.operationCode() != 8 && SParser.operationCode() != 9 && SParser.operationCode() != 0 && ID == -1) 
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

        if ((SParser.operationCode() == 3 || SParser.operationCode() == 8 || SParser.operationCode() == 9) && ID > -1) 
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
                F[ID].clear(); // update frequency list based on updated word list
                F[ID].frequency_unsorted(L[ID]);
                F[ID].sort();
            }
                log << "Number of bytes used by list \'" << S[ID] << "\' is: " << L[ID].listSize() << ".\n" << endl;
            break;
        case 2:
            n = L[ID].erase(SParser.cmd.value1);
            if (n == 0) log << "Waring: ";
            log << n << " word(s) " << '\'' << SParser.cmd.value1 << '\'' << " have been deleted.\n";
            F[ID].clear(); // update frequency list based on updated word list
            F[ID].frequency_unsorted(L[ID]);
            F[ID].sort(); 
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
            F[i].sort(); // sort the frequency list
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
                size = (F[ID].size() > F[ID2].size())?F[ID].size():F[ID2].size();
                if (size < 100) F[i].list_union(F[ID],F[ID2],F[ID].head,F[ID2].head);
                else F[i].list_union_nonrec(F[ID],F[ID2]);
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
                size = (F[ID].size() > F[ID2].size())?F[ID].size():F[ID2].size();
                if (size < 100) F[i].list_intersection(F[ID],F[ID2],F[ID].head,F[ID2].head);
                else F[i].list_intersection_nonrec(F[ID],F[ID2]);
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
        case 9:
            for (int j = 0; j < i; j++) {
                ID2 = (SParser.cmd.value1 == S[j])?j:ID2;
            }
            if (ID2 < 0 ) { 
                log << "Error: invalid operation, because list \'" << SParser.cmd.value1 << "\' is not created yet.\n" << endl;
                continue;
            }
            else {
                S[i] = SParser.cmd.listID;
                F[i].check(F[ID2], DIC, dic_size);
                log << "Number of bytes used by list \'" << S[i] << "\' is: " << F[i].listSize() << ".\n" << endl;
                i++;
            }
            break;
        case 0:
            log << "Error: above line is not a valid script form.\n" << endl;
            break;
        } 
    }
  
  log << "End parsing script file \'" << script << "\'.\n" <<endl;
  log.close();

  delete[] L;
  delete[] F;
  delete[] S;
  delete[] DIC;
  return 0;
}

