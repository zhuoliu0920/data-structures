int main(int argc, char *argv[]){
  
    int c;
    CommandLineParser clp(argv[1],',');   //instantiating the class CommandLineParser
    string line;

    if (argc ==1)  {
        cout << "One argument must be passed." << endl ;
        return -1;
    }
    //use CommandLineParser to get the script name
    char *fileA = clp.extract("script");

cout << "script file is: " << fileA << endl;

    //use ScriptParser to get the commands from script file
    ScriptParser SParser =  ScriptParser();
    ifstream indata(fileA);
    
     while(getline( indata, line ) ) {
        cout<<line << endl;
         SParser.ParseLine(line);
          cout<< SParser.cmd.listID<< " " <<SParser.cmd.value1<< "-"<< SParser.cmd.value2<<" Op.Code:";
          switch (SParser.operationCode())
          {
          case 1:
             
              cout << SParser.operationCode()<<endl;
               // Call your function to insert here
              break;
          case 2:
              
              cout << SParser.operationCode()<<endl;
               // Call your function to delete here
              break;
          case 3:
             
             cout << SParser.operationCode()<<endl;
               // Call your function to insert here
              break;
          case 4:
             cout << SParser.operationCode()<<endl;
             // Call your function to fwr write here
              break;
          case 5:
              cout << SParser.operationCode()<<endl;
             // Call your function to rev write here
              break;
           case 0:
              cout << SParser.operationCode()<<" ERROR "<<endl;
              break;
          }
 
    }
  
  cout << "end of parsing" <<endl;

  return  0;
}
