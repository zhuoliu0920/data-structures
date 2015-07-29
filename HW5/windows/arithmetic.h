#include "freqlist.h"
#include "stack.h"
#include <string>

using namespace std;

string* analysis(const string); // function to analize arithmetic expression, output operands and operators as an array of strings and keep its order
int arithmetic(const string*, string*, FreqList*, int); // function to use stack to evaluate arithmetic expression

string* analysis(const string exp)
{
    string* out;
    out = new string[1000]; // here, we require the maximum number of operands plus operators in an expression is 1000
    size_t pos = 0;
    size_t pos2 = 0;
    int i = 0;

    while (pos2 != std::string::npos)
    {
         if (i == 999) {
         cerr << "error: expression is too long." << endl;
         return NULL;
         }

         pos2 = exp.find_first_of("+*()", pos);
         if (pos2 != std::string::npos && exp[pos2] == '(') {
             if (exp[pos2-1] != '+' && exp[pos2-1] != '*' && exp[pos2-1] != '(' && pos2 != 0) return NULL;
             if (exp[pos2+1] == '+' || exp[pos2+1] == '*' || exp[pos2] == ')') return NULL;
             if (exp.find(')', pos2+1) == std::string::npos) return NULL;
             else {
                 out[i++] = exp[pos2];
                 pos = pos2+1;
                 continue;
             }
         }
         else if (pos2 != std::string::npos && exp[pos2] == ')') {
             if (exp[pos2+1] != '+' && exp[pos2+1] != '*' && exp[pos2+1] != ')' && pos2 != exp.size()-1) return NULL;
             if (exp[pos2-1] == '+' || exp[pos2-1] == '*' || exp[pos2-1] == '(') return NULL;
             else if (exp[pos2-1] == ')') {
                 out[i++] = exp[pos2];
                 pos = pos2+1;
                 continue;
             }
             else {
                 out[i] = exp.substr(pos, pos2-pos);
                 out[i+1] = exp[pos2];
                 pos = pos2+1;
                 i+=2;
                 continue;
             }
         }
         else if (pos2 != std::string::npos) {
             if (exp[pos2+1] == '+' || exp[pos2+1] == '*' || pos2 == 0 || pos2 == exp.size()-1) return NULL;
             if (pos2 == pos) {
                 out[i++] = exp[pos2];
                 pos = pos2+1;
                 continue;
             }
             else {
                 out[i] = exp.substr(pos, pos2-pos);
                 out[i+1] = exp[pos2];
                 pos = pos2+1;
                 i+=2;
                 continue;
             }
         }
         else {
             if (pos == exp.size()) continue;
             else out[i++] = exp.substr(pos);
         }
    }


    string brackets = "";
    int left = 0;
    int right = 0;
    for (int j = 0; j < i; j++)
    {
        if (out[j] == "(" || out[j] == ")")
        {  
            brackets += out[j];
            left += (out[j] == "(")?1:0;
            right += (out[j] == ")")?1:0;
        }
    }
    if (brackets != "") {
        if (brackets[0] == ')') return NULL;
        if (brackets[brackets.size()-1] == '(') return NULL;
        if (left != right) return NULL;
    }

    return out;
}

int arithmetic(const string *exp, string *S, FreqList *F, int i)
{
    Stack<string> operators(100);
    Stack<FreqList*> operands(100);

    int ID;
    FreqList *temp;
    temp = new FreqList[1500];
    int fl = 0;

    for (int k = 0; exp[k] != ""; k++)
    {
        if (operators.isFull() || operands.isFull()) {
            cout << "error: stack is overflow, please check the expression." << endl;
            delete[] temp;
            return -1;
        }
        if (exp[k] == "+" || exp[k] == "*" || exp[k] == "(" || exp[k] == ")")
        {
            if (operators.isEmpty()) {
                operators.push(exp[k]);
            }
            else if (exp[k] == ")") {
                while (operators.top() != "(") {
                    if (operands.isEmpty()) {
                        delete[] temp;
                        return -1;
                    }
                    temp[fl++].copy(*operands.top());
                    operands.pop();
                    if (operands.isEmpty()) {
                        delete[] temp;
                        return -1;
                    }
                    temp[fl++].copy(*operands.top());
                    operands.pop();
                    if (operators.top() == "*") temp[fl].list_intersection_nonrec(temp[fl-1], temp[fl-2]);
                    if (operators.top() == "+") temp[fl].list_union_nonrec(temp[fl-1], temp[fl-2]);
                    operands.push(&temp[fl]);
                    fl++;
                    operators.pop();
                    if (operators.isEmpty()) {
                        delete[] temp;
                        return -1;
                    }
                 }
                 operators.pop();
            }
            else if (exp[k] == "+" && operators.top() != "(") {
                if (operands.isEmpty()) {
                    delete[] temp;
                    return -1;
                }
                temp[fl++].copy(*operands.top());
                operands.pop();
                if (operands.isEmpty()) {
                    delete[] temp;
                    return -1;
                }
                temp[fl++].copy(*operands.top());
                operands.pop();
                if (operators.top() == "*") temp[fl].list_intersection_nonrec(temp[fl-1], temp[fl-2]);
                if (operators.top() == "+") temp[fl].list_union_nonrec(temp[fl-1], temp[fl-2]);
                operands.push(&temp[fl]);
                fl++;
                operators.pop();
                operators.push(exp[k]);
            }
            else if (exp[k] == "*" && operators.top() == "*") {
                if (operands.isEmpty()) {
                    delete[] temp;
                    return -1;
                }
                temp[fl++].copy(*operands.top());
                operands.pop();
                if (operands.isEmpty()) {
                    delete[] temp;
                    return -1;
                }
                temp[fl++].copy(*operands.top());
                operands.pop();
                temp[fl].list_intersection_nonrec(temp[fl-1], temp[fl-2]);
                operands.push(&temp[fl]);
                fl++;
                operators.pop();
                operators.push(exp[k]);
            }
            else {
                operators.push(exp[k]);
            }
        }
        else
        {
            ID = -1;
            for (int j = 0; j < i; j++) {
                ID = (exp[k] == S[j])?j:ID;
            }
            if (ID == -1) {
                delete[] temp;            
                return -1;
            }
            else {
                operands.push(&F[ID]);
            }
        }
    }
    while (!operators.isEmpty()) {
        if (operands.isEmpty() || operators.top() == "(") {
            delete[] temp;
            return -1;
        }
        temp[fl++].copy(*operands.top());
        operands.pop();
        if (operands.isEmpty()) {
            delete[] temp;
            return -1;
        }
        temp[fl++].copy(*operands.top());
        operands.pop();
        if (operators.top() == "*") temp[fl].list_intersection_nonrec(temp[fl-1], temp[fl-2]);
        if (operators.top() == "+") temp[fl].list_union_nonrec(temp[fl-1], temp[fl-2]);
        operands.push(&temp[fl]);
        fl++;
        operators.pop();
    }
    F[i].copy(*operands.top());
    operands.pop();
    if (!operands.isEmpty()){
        F[i].clear();
        delete[] temp;
        return -1;
    }
    delete[] temp;
    return 0;
}
