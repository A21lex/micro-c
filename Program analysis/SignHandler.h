#ifndef SIGNHANDLER_H_INCLUDED
#define SIGNHANDLER_H_INCLUDED
#include <map>
#include <set>
#include <iterator>
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <stack>
using namespace std;

class SignHandler{

    public:
        //SignHandler(map<string,set<char>>* curSigns);
        SignHandler();
        set<char> evaluateSigns(string expr, map<string,set<char>> curSigns);
        vector<string> getTokens(string expr);

    protected:

    private:
        string expr;
        //map<string,set<char>> curSigns;
        map<char, map<char,set<char>>> plusTable;
        map<char, map<char,set<char>>> minusTable;
        map<char, map<char,set<char>>> multTable;
        map<char, map<char,set<char>>> divTable;

        set<char> signsOfToken(string token, map<string,set<char>> curSigns);
        bool isNumericStr(string argStr);
        set<char> applyOp(char op, set<char> a, set<char> b);

        bool hasPrecedence(char op1, char op2);
        bool isOperator(char c);

        template<typename T>
        T toppop(stack<T>* stackArg){
            T topElem = stackArg->top();
            stackArg->pop();
            return topElem;
        }

        void buildPlusTable();
        void buildMinusTable();
        void buildMultTable();
        void buildDivTable();

};



#endif // SIGNHANDLER_H_INCLUDED
