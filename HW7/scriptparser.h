#ifndef SCRIPTPARSER_H
#define SCRIPTPARSER_H

#include <string>
#include <ctype.h>

using namespace std;

const string readTemplate  = "read(#,'#')";
const string insertTemplate  = "insert(#,'#/#')";
const string deleteTemplate  = "delete(#,'#')";
const string writeTemplate = "write(#,'#')";
const string writefTemplate  = "write(#,'#',forward)";
const string writerTemplate  = "write(#,'#',reverse)";
const string unionTemplate   =  "union(#,#,#)";
const string intersectionTemplate   =  "intersection(#,#,#)";
const string checkTemplate  =  "check(#,#)";
const string loadTemplate  =  "load(#,'#')";
const string filterTemplate  =  "filter(#,'#')";
const string indexTemplate  =  "index(#)";
const string searchTemplate  =  "search(#,'#',#)";
const string rankTemplate = "rank(#,'#')";
const string futureTemplate  =  "newoperation(#)";

class ScriptParser  
{
    public: struct cmdLine
    {
        string operation; // insert/delete/read
        string listID;    // Identification of the list
        string value1;    // parameter string
        string value2;    // second parameter string (used only for insert operations)
    };

// cmd: This property contains the parsed instruction
    public:  cmdLine cmd;

// constructor            
    public: ScriptParser() {cmd.operation = cmd.listID= cmd.value1 = cmd.value2 = "";}

// translate operation (string) to operationCode (int)
    public: int operationCode()
        {
            if  (cmd.operation == "insert") return 1;
            if  (cmd.operation == "delete") return 2;
            if  (cmd.operation == "read")   return 3;
            if  (cmd.operation == "fwrwrite") return 4;
            if  (cmd.operation == "revwrite") return 5;
            if  (cmd.operation == "union") return 6;
            if  (cmd.operation == "intersection") return 7;
            if  (cmd.operation == "arithmetic") return 8;
            if  (cmd.operation == "check") return 9;
            if  (cmd.operation == "load") return 10;
            if  (cmd.operation == "filter") return 11;
            if  (cmd.operation == "index") return 12;
            if  (cmd.operation == "search") return 13;
            if  (cmd.operation == "rank") return 14;
            return 0;
        }

// function Parse
// receives a string 
// updates the public property cmd
    public: void ParseLine(string stringtobeparsed)
    {    
         string variables, values;
         unsigned int pos, posSlash;
         cmd.operation = cmd.listID= cmd.value1 = cmd.value2="";
         variables = match(stringtobeparsed, insertTemplate); 
         if (variables!="") cmd.operation= "insert";
         else
         {
             variables = match(stringtobeparsed, deleteTemplate); 
             if (variables!="") cmd.operation= "delete";
             else
             {
                 variables = match(stringtobeparsed, readTemplate); 
                 if (variables!="") cmd.operation= "read";
                 else
                 {
                     variables = match(stringtobeparsed, writeTemplate); 
                     if (variables!="") cmd.operation= "fwrwrite";
                     else
                     {
                         variables = match(stringtobeparsed, writefTemplate); 
                         if (variables!="") cmd.operation= "fwrwrite";
                         else
                         {
                             variables = match(stringtobeparsed, writerTemplate); 
                             if (variables!="") cmd.operation= "revwrite";
                             else  
                             {
                                 variables = match(stringtobeparsed, unionTemplate); 
                                 if (variables!="") cmd.operation= "union";
                                 else  
                                 {
                                     variables = match(stringtobeparsed, intersectionTemplate); 
                                     if (variables!="") cmd.operation= "intersection";
                                     else 
                                     {
                                         variables = match(stringtobeparsed, checkTemplate);
                                         if (variables!="") cmd.operation= "check";
                                         else 
                                         {
                                             variables = match(stringtobeparsed, loadTemplate);
                                             if (variables!="") cmd.operation= "load";
                                             else
                                             {
                                                 variables = match(stringtobeparsed, filterTemplate);
                                                 if (variables!="") cmd.operation= "filter";
                                                 else
                                                 { 
                                                     variables = match(stringtobeparsed, indexTemplate);
                                                     if (variables!="") cmd.operation= "index";
                                                     else
                                                     { 
                                                         variables = match(stringtobeparsed, searchTemplate);
                                                         if (variables!="") cmd.operation= "search";
                                                         else
                                                         {
                                                             variables = match(stringtobeparsed, rankTemplate);
                                                             if (variables!="") cmd.operation= "rank";
                                                             else
                                                             {
                                                                 variables = stringtobeparsed;                       
                                                                 if (variables.find("=") != std::string::npos) cmd.operation= "arithmetic";
                                                                 else cmd.operation= "error";
                                                             }
                                                         }
                                                     }
                                                 }
                                             }
                                         }
                                     }
                                 }
                             }
                         }
                     }
                 }
             }
         }
         if (cmd.operation != "arithmetic" && cmd.operation != "index")
         {             
             pos = variables.find(",");
             if (pos == string::npos) cmd.operation ="error";
             else
             {
                cmd.listID = variables.substr(0,pos); 
                values = variables.substr(pos+1,variables.length()-pos+1);
                if (cmd.operation=="insert")
                {
                    posSlash = values.find("/");
                    if (posSlash ==string::npos) cmd.operation ="error";
                    else
                    {   cmd.value1=values.substr(0,posSlash);
                        cmd.value2=values.substr(posSlash+1,values.length()-posSlash+1);
                    }
                }
                else if (cmd.operation == "union" || cmd.operation == "intersection")
                {
                     variables=variables.substr(pos+1,variables.length()-pos+1);
                     pos = variables.find(",");
                     cmd.value1 = variables.substr(0,pos); 
                     variables=variables.substr(pos+1,variables.length()-pos+1);
                     //pos = variables.find(",");
                     //cmd.value2 = variables.substr(pos+1,variables.length()-pos+1);
                     cmd.value2= variables;
                }
                else if (cmd.operation == "search")
                {
                     variables=variables.substr(pos+1,variables.length()-pos+1);
                     pos = variables.find_last_of(",");
                     cmd.value1 = variables.substr(0,pos); 
                     variables=variables.substr(pos+1,variables.length()-pos+1);
                     cmd.value2= variables;
                }
                else
                    cmd.value1 =values;
             }
         }
         else if (cmd.operation == "index")
             cmd.listID = variables;
         else if (cmd.operation == "arithmetic")
         {
             pos = variables.find("=");
             cmd.listID = variables.substr(0,pos);
             cmd.value1 = variables.substr(pos+1, variables.length()-pos-1);
         }
    }

    public:  string match(string str1, string strtemplate)
              {   string strRet="";
                  if ( str1.length()<strtemplate.length()) return "";
                  int j=0;
                  bool flgp=false;
                  bool inquotes=false;
                  bool isWrite = false;
                  string oper="";
                  bool gotseparator = false;
                  for (unsigned int i=0; i<str1.length(); i++)
                  { 
                      if (str1[i] != strtemplate[j])
                      {
                           if (strtemplate[j]=='#' )
                           { 
                              if (inquotes)
                              {
                                  if (str1[i]!='\'') strRet+=str1[i];
                                   
                              }
                              else
                                  if (isalnum(str1[i])) 
                                  {
                                      strRet+=str1[i];
                                  }                                    
                              else return "";
                             if (str1[i+1] == strtemplate[j+1]) j++; 
                           }
                           else return "";
                      }
                      else
                      {
                              j++;
                              oper+=str1[i];

                              if (i==5 && oper=="search") {
                                  if (str1.find("(") < str1.find_last_of(")") && str1.find_last_of(")") != string::npos && str1.find("'") != string::npos && str1.find("'") != str1.find_last_of("'"))
                                  {
                                      str1.erase(str1.find("'"),1);
                                      str1.erase(str1.find_last_of("'"),1);
                                      return (str1.substr(str1.find("(")+1,str1.find_last_of(")")-1));
                                  }
                                  else return "";
                              }

                              if (i==4 && oper=="write") 
                                  isWrite=true;
                              if (str1[i] ==',' && !gotseparator )
                                 {
                                 strRet+=str1[i];
                                 if (isWrite) gotseparator = true;
                                 }
                              if ( str1[i] =='/')  strRet+=str1[i];                                 
                              if (str1[i] =='\'' ) 
                                   if (!inquotes) inquotes=true; else inquotes=false ;
                      }
                  }
                  return strRet;
              }
}
;

#endif
