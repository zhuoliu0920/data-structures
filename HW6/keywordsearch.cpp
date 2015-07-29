#include "commandlineparser.h"
#include "scriptparser.h"
#include "freqlist.h"
#include "arithmetic.h"
#include "hash.h"
#include <fstream>
#include <string>
#include <iostream>
#include <algorithm> //to use the transform() funtion which puts upper case strings into lower case ones

using namespace std;

int main(int argc, char *argv[])
{
    ofstream log; // to record log file
    log.open("log.txt", std::fstream::app);
    CommandLineParser clp(argv[1],',');   //instantiating the class CommandLineParser

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
    int size;
    int k;
    string line; // store each line in script file
    WordList *L; // array of word lists
    FreqList *F; // array of frequency lists which store the information of words and its frequency
    string *S; // array of strings which store the name of lists
    Hash HT; // hash table to store all the words
    L = new WordList[1000];
    F = new FreqList[1000];
    S = new string[1000];

    //initiate temporary lists and list nodes
    WordNode *tempWordNode;
    FreqNode *tempFreqNode;
    WordList tempWordList;
    WordList *tempWordListpt;
    FreqList tempFreqList;
    FreqList tempFreqList2;
    string tempString;
    Node *tempNode;

    int pos1;
    int pos2;

    log << "Begin parsing script file \'" << script << "\':\n" << endl;
    while(getline(indata, line))
    {
        log << line << '\n';
        SParser.ParseLine(line);

        //process to determine if listID exits
        int ID = -1; // cmd.listID, after following checking process, if it's still -1, then cmd.listID is a new list, otherwise cmd.listID is the word list L[ID] or frequency list F[ID]
        int ID2 = -1; // cmd.value1 (if it stores list name), same procesure as above
        int ID3 = -1; // cmd.value2 (if it stores list name), same procesure as above

        for (int j = 0; j < i; j++)
        {
            ID = (SParser.cmd.listID == S[j])?j:ID; // check if cmd.listID is already in S[]
        }

        if (SParser.operationCode() != 3 && SParser.operationCode() != 8 && SParser.operationCode() != 9 && SParser.operationCode() != 10 && SParser.operationCode() != 13 && SParser.operationCode() != 0 && ID == -1) 
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

        if ((SParser.operationCode() == 3 || SParser.operationCode() == 8 || SParser.operationCode() == 9 || SParser.operationCode() == 10 || SParser.operationCode() == 13) && ID > -1) 
        { 
            log << "Error: invalid operation, because list \'" << SParser.cmd.listID << "\' already exists.\n" << endl;
            continue;
        }

        // do all the list operations, such as read, insert, delete, write, intersection, union, load, filter, index and seach
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
            log << "Error: this program does not support \'check\' operation." << endl;
            break;
        case 10:
            S[i] = SParser.cmd.listID; // create a new listname in S[]
            n = L[i].read_keepUpper(SParser.cmd.value1); // create a new word list in L[]
            if (n != 0) {
                log << "Error: fail to open input file \'" << SParser.cmd.value1 << "\'.\n" << endl;
                continue;
            }
            tempWordNode = L[i].head;
            while (tempWordNode) {
                n = tempWordList.read(tempWordNode->word);
                if (n == 0) {
                    F[i].insert("*"+tempWordNode->word,1);
                    tempFreqList.frequency_unsorted(tempWordList);
                    tempFreqList.sort();
                    F[i].append(tempFreqList,tempFreqList.head);
                    tempWordList.clear();
                    tempFreqList.clear();
                }
                if (n != 0) {
                    log << "Error: fail to open file \'" << tempWordNode->word << "\', so words in this file are ignored." << endl;
                }
                tempWordNode = tempWordNode->pnext;
            }
/*cout << "At very beginning:" << endl;
F[i].print();
cout << endl;*/
            // then, eliminate all the nodes without any letter.
            tempFreqNode = F[i].head;
            while (tempFreqNode) {
                k = 0;
                tempString = tempFreqNode->word;
                for (int j = 0; j < tempString.length(); j++) {
                    if (isalpha(tempString[j])) {
                        k = 1;
                        break;
                    }
                }
                tempFreqNode = tempFreqNode->pnext;
                if (k == 0) F[i].deletenode(tempFreqNode->pprev);
            }
            log << "Files specified in \'" << SParser.cmd.value1 << "\' are successfully loaded into list \'" << SParser.cmd.listID << "\'.\n" << endl;
/*cout << "Before filter:" << endl;
F[i].print();
cout << endl;*/
            i++;
            break;
        case 11:
            n = tempWordList.read(SParser.cmd.value1);
            if (n != 0) {
                log << "Error: fail to open file \'" << SParser.cmd.value1 << "\'.\n" << endl;
                continue;
            }
            tempFreqList.frequency_unsorted(tempWordList);
            tempFreqList.sort();
            tempFreqNode = tempFreqList.head;
            while (tempFreqNode) {
                F[ID].erase(tempFreqNode->word);
                tempFreqNode = tempFreqNode->pnext;
            }
            tempWordList.clear();
            tempFreqList.clear();
            log << "Words from file \'" << SParser.cmd.value1 << "\' are successfully eliminated from list \'" << SParser.cmd.listID << "\'.\n" << endl;
/*cout << "After filter:" << endl;
F[ID].print();
cout << endl;*/
            break;
        case 12:
            tempFreqNode = F[ID].head;
            while (tempFreqNode) {
                if (tempFreqNode->word[0] == '*')
                    tempString = (tempFreqNode->word).substr(1,(tempFreqNode->word).length()-1);
                else
                    HT.insert(tempFreqNode->word,tempString);
                tempFreqNode = tempFreqNode->pnext;
            }
            log << "Hash table is sucessfully established (or updated).\n" << endl;

/*cout << "Words are stored in hash table as:" << endl;
for (int j = 0; j < HT.size; j++) {
    tempNode = (HT.Table[j]).head;
    if (tempNode == NULL) emp++;
    if (tempNode != NULL) occ++;
    while (tempNode) {
        if (tempNode->pnext != NULL) con++;
        cout << "position: " << j << ", word: " << tempNode->IndexWord << endl;
//        cout << "file(s): ";
//        (tempNode->FileName).print();
        tempNode = tempNode->pnext;
    }
}*/
            break;
        case 13:
            S[i] = SParser.cmd.listID;
            tempString = SParser.cmd.value1;
            transform(tempString.begin(),tempString.end(),tempString.begin(),::tolower); //transfer all the key words into lower case
            pos1 = 0;
            while (pos1 != string::npos) {
                pos2 = tempString.find(",");
                tempWordList.insert(tempString.substr(pos1,pos2));
                if (pos2 == string::npos) pos1 = pos2;
                else {
                    pos1 = 0;
                    tempString = tempString.substr(pos2+1,tempString.length()-pos2-1);
                }
            }
            tempWordNode = tempWordList.head;
            while (tempWordNode) {
                tempWordListpt = HT.search(tempWordNode->word);
                tempFreqList.frequency_unsorted(*tempWordListpt);
                tempFreqList.sort();
                if (tempWordNode->pprev == NULL) {
                    F[i].copy(tempFreqList);
                    tempFreqList.clear();
                }
                else {
                   tempFreqList2.copy(F[i]);
                   F[i].clear();
                   F[i].list_intersection_nonrec(tempFreqList,tempFreqList2);
                   tempFreqList.clear();
                   tempFreqList2.clear();
                }
                tempWordNode = tempWordNode->pnext;
            }
            tempWordList.clear();
            i++;
            log << "Names of those files which contain the words: '" << SParser.cmd.value1 << "' are successfully found and written into list " << SParser.cmd.listID << ".\n" << endl;
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
  return 0;
}

