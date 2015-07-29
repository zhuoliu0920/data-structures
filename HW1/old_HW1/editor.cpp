#include "commandlineparser.h"
#include "wordlist.h"

int main(int argc, char** argv)
{
    ofstream log;
    log.open("editor_log.txt");

    if (argc == 1)
    {
        log << "Error: input file and output file are not specified.\n";
        log.close();
        return 1;
    }

    CommandLineParser clp(argv[1],',');
    char *v1 = clp.extract("inputfile");
    char *v2 = clp.extract("outputfile");
    char *v3 = clp.extract("order");
    char *v4 = clp.extract("insert");
    char *v5 = clp.extract("delete");
    WordList wl;
    string st1;
    string st2;
    string st3;
    if (v3 != NULL) st3 = v3;
    int n;

    if (v1 == NULL)
    {
        log << "Error: input file name is not specified.\n";
        log.close();
        return 1;
    }
    if (v2 == NULL)
    {
        log << "Error: output file name is not specified.\n";
        log.close();
        return 1;
    }
    if (v3 != NULL && st3 != "forward" && st3 != "backward")
    {
        log << "Error: order is not valid.\n";
        log.close();
        return 1;
    }

    st1 = v1;
    wl.read(st1);
    cout << "After reading input file, the number of bytes used is: " <<  wl.listSize() << endl;
    
    if (v4 == NULL) ;
    else {
        for (int i = 0; v4[i] != '\0'; i++)
        {
            if (v4[i] != '/')
                st2.push_back(v4[i]);
            else {
                st1 = st2;
                st2.clear();
            }
        }
    n = wl.insert(st2, st1);
    if (n == 0)
        log << "Warning: in insertion, " << '\'' << st1 << '\'' << " cannot be found, so insertion fails.\n";
    else
        log << '\'' << st2 << '\'' << " has been inserted after the first appeared word " << '\'' << st1 << '\'' << '.' << '\n';
    }

    if (v5 == NULL) ;
    else {
    st1 = v5;
    n = wl.erase(st1);
    log << n << " word(s) " << '\'' << st1 << '\'' << " have been deleted.\n";
    }

    if (v3 == NULL || st3 == "forward")
    {
        if (v3 == NULL)
            log << "Order is not specified, forward order is used as default.\n";

        wl.output_forward(v2);
        log << "Document is correctly written to file " << '\'' << v2 << '\'' << " in forward order.\n";
    }
    else if (st3 == "backward")
    {
        wl.output_backward(v2);
	log << "Document is correctly written to file " << '\'' << v2 << '\'' << " in backward order.\n";
    }

    cout << "After writing output file, the number of bytes used is: " <<  wl.listSize() << endl;
    log.close();
    return 0;
}
