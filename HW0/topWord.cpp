#include <iostream>
#include <fstream>
#include <cctype> // for tolower(), and isalnum()
#include <string>

using namespace std;

class CommandLineParser
{
private:
    char *input;
    char delimiter;
    char* data;  // to store input data
    int inputSize; //size of input data 

public:
    char* extract(char*); // Method to extract the variable value
    CommandLineParser(char*,char); //constructor
    void read_inputData(char* fileName); // to read input data from file

    void print_topword(void);// to print the most repeating word stored in data and show its frequency
};

//Assigning the argument & delimiter value when the object of the class is instantiated

CommandLineParser::CommandLineParser(char *argument,char delimit)
{
    input = argument;
    delimiter = delimit;
}

void CommandLineParser::read_inputData(char* fileName)
{
    ifstream input;
    input.open(fileName);
    if(!input.is_open()) // check if file is correctly opened
    {
        cerr << "Error: no such file or file cannot be correctly opened!" << endl;
        data = NULL;
        return;
    }

    // Get the input file size
    long begin = input.tellg();
    input.seekg(0,ios::end);    
    long end = input.tellg();    
    input.seekg(0,ios::beg);
    long size = end - begin;

    data = new char[size];

    int count = 0;
    while(input.get(data[count++])); // Store each character in the array and do error checking
    data[count-1] = '\n'; // Pad the input to help check validity

    inputSize = count;

    input.close();
}



//Function to extract the value for a given variable from the command line input

char* CommandLineParser::extract(char *variableName)
{
    bool variableFound = false;
    bool variableNameFound = true;
    char *variable = new char[50]; // Char array to hold the value of the variable
    char *temp = new char[50]; // Temp array to hold a partial string to find the variable name
    int variableLength; // Length of the variable name
    int variableCount; // Counter for the variable array
    int inputLength; // Length of the command line argument
    int equalCount; // Index to find '='

    for(inputLength = 0; input[inputLength] != '\0'; inputLength++); // Get the length of the command line argument
    for(variableLength = 0; variableName[variableLength] != '\0'; variableLength++); // Get the length of the variable

    // Loop through the command line argument and find the variable name and get the variable value
    for(int i = 0; (input[i] != '\0' || input[i] != delimiter) && !variableFound && i < inputLength - variableLength + 1; i+=equalCount) {
        // Clear the temp array
        for(int j = 0; j < 50; j++) {
            temp[j] = '\0';
        }

        // Find the variable name
        for(equalCount = 0; equalCount < variableLength && variableNameFound; equalCount++) {
            temp[equalCount] = input[i+equalCount];
	
            // If the equalCount'th element of temp and variable name are not equal, we did not find the variable and we move on
            if(temp[equalCount] != variableName[equalCount]) {
                variableNameFound = false;
            }
        }

        // If we have a variable name match, we check the next char to see if it is '=', otherwise we have not found the variable
        if(variableNameFound && input[i+equalCount] != '=') {
            variableNameFound = false;
        }

        // If we found the variable name followed by '=', we get the value of the variable
        if(variableNameFound) {
            // Loop through the input to get the variable value
            for(variableCount = 0; input[i+equalCount+1+variableCount] != '\0' && input[i+equalCount+1+variableCount] != delimiter; variableCount++) {
                variable[variableCount] = input[i+equalCount+1+variableCount];
            }
            variable[variableCount] = '\0'; // Set the end line flag

            return variable;
        } else {
            variableNameFound = true; // Reset the variable name found flag to restart the search
        }
    }

    return NULL; // Return NULL if the variable name is not in the command line argument
}

//Funtion to find and print most frequent word and its frequency

void CommandLineParser::print_topword()
{    
     if (data == NULL) return; // to check if file is correctly read

     string word[100]; // to store all different words appeared in data
     int freq[100] = {0}; // to store frequency for corresponding word
     string temp; // temp string to hold each word
     int max[100] = {-1}; // to store the positions of most frequet words
     int maxfreq; // to store the maximum frequency
     int j = 0;
     int j1;
     int k;

     for (int i = 0; i < inputSize; i++)
     {
         if (isalnum(data[i]) || (data[i] == '\'' && isalnum(data[i+1])) || (data[i] == '-' && isalnum(data[i+1])) || (data[i] == '.' && isalnum(data[i+1]))) temp.push_back(tolower(data[i])); // if the data we scan is (numbers, a-z, A-Z, ' or -), then we store them in temp string. (In case we meet: you're, 50-counts, boys', e.g.)
         else if (j == 0 && !temp.empty()) // store the first word into string "word"
         {
              word[j] = temp;
              freq[j] += 1;
              j++;
              temp.clear();
         }
         else if (!temp.empty()) // if the data we scan is space, comma, period or some other punctation, and if at this time string "temp" is not empty, we first check if "temp" already exists in "word", if it does, its frequency +1, otherwise, create a new word in "word", and creat a new frequency = 1.
         {
             j1 = j;
             for (k = 0; k < j1; k++)
             {
                 if (temp == word[k])
                 {
                     freq[k]++;
                     temp.clear();
                     break;
                 }
                 else if (k == j-1)
                 {
                 word[j] = temp;
                 freq[j] ++;
                 j++;
                 temp.clear();
                 }
             }
         }
     }

     if (j == 0) // to check if there is any words stored in "word" array
     {
         cerr << "Warning: No word in this file." << endl;
         return;
     }
     
     k = 1;
     max[0] = 0;
     maxfreq = freq[0];
     for (int i = 1; i < j ; i++) // find the positions of the most frequent words and record the maximum frequency
     {
         if (freq[i] == maxfreq) max[k++] = i;
         else if (freq[i] > maxfreq)
         {
             maxfreq = freq[i];
             for (int l = 1; l < k; l++) max[l] = -1;
             k = 1;
             max[0] = i;
         }
     }

     cout << "Result: ";
     for (int i = 0; i < k-1; i++) cout << word[max[i]] << ", ";
     cout << word[max[k-1]] << "=" << maxfreq << endl;
}            



int main(int argc,char** argv)
{
    CommandLineParser clp(argv[1],';');   //instantiating the class
    char *v1 = clp.extract("filename");
    clp.read_inputData(v1);
    clp.print_topword();
	
return 0;
}
