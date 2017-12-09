#include "SignHandler.h"

using namespace std;

SignHandler::SignHandler(map<string,set<char>>* curSigns){
    this->curSigns = *curSigns;
    buildPlusTable();
    buildMinusTable();
    buildMultTable();
    buildDivTable();
}
// shunting yard implementation for expression evaluation
set<char> SignHandler::evaluateSigns(string expr){
    vector<string> tokens = getTokens(expr);
    //for (unsigned int i = 0; i<tokens.size(); i++)
    //    cout << tokens[i] << endl;

    stack<set<char>> values;
    stack<char> operators;

    for (unsigned int i = 0; i<tokens.size(); i++){
        if (tokens[i].length()>1 || !isOperator(tokens[i].at(0)))
            values.push(signsOfToken(tokens[i]));
        else if (tokens[i] == "(")
            operators.push(tokens[i].at(0));
        else if (tokens[i] == ")"){
            while (operators.top() != '('){
                values.push(applyOp(toppop(&operators), toppop(&values), toppop(&values)));
            }
            operators.pop();
        }
        else if (tokens[i].at(0) == '+' || tokens[i].at(0) == '-' ||
                 tokens[i].at(0) == '*' || tokens[i].at(0) == '/' ) {
            while (!operators.empty() && hasPrecedence(tokens[i].at(0), operators.top())){
               values.push(applyOp(toppop(&operators), toppop(&values), toppop(&values)));
            }
            operators.push(tokens[i].at(0));
        }

        //"a:=8+B[9-z]*5";
        else if (tokens[i].at(0) == '['){ // new stuf 29.11
            //cout << i << endl;
            stack<char> braces;
            braces.push('[');
            int locI = i+1;
            while (!braces.empty()){
                if (tokens[locI].at(0) == ']') braces.pop();
                else if (tokens[locI].at(0) == '[') braces.push('[');
                locI++;
            }
            i += locI-1-i; // i = locI-1;
        }
    }
    while (!operators.empty()){
        values.push(applyOp(toppop(&operators), toppop(&values), toppop(&values)));
    }
    set<char> res = toppop(&values);
    return res;
}

set<char> SignHandler::signsOfToken(string token){
    if (isNumericStr(token)) {
        set<char> singleSign;
        if (token == "0") singleSign.insert('0');
        else singleSign.insert('+');
        return singleSign;
    }
    else return curSigns[token];
}

bool SignHandler::isNumericStr(string argStr){
    for (unsigned int i=0; i<argStr.length(); i++) {
        if (!isdigit(argStr.at(i))) return false;
    }
    return true;
}

set<char> SignHandler::applyOp(char op, set<char> a, set<char> b){
    set<char> res;
    vector<char> av (a.begin(), a.end());
    vector<char> bv (b.begin(), b.end());
    map<char, map<char, set<char>>> opTable;
    if (op == '+') opTable = plusTable;
    else if (op == '-') opTable = minusTable;
    else if (op == '*') opTable = multTable;
    else if (op == '/') opTable = divTable;
    else cout << "Wrong operator" << endl;
    for (unsigned int i = 0; i<av.size(); i++){
        for (unsigned int j = 0; j<bv.size(); j++){
            set<char> mapEntry = opTable[av[i]][bv[j]];
            set_union(res.begin(),res.end(), mapEntry.begin(), mapEntry.end(),inserter(res,res.begin()));
        }
    }
    return res;
}
// specified version of getTokens from preproc class, see if can be replaced!
vector<string> SignHandler::getTokens(string expr){
    string rhs = expr.substr(expr.find(":=")+2, expr.length()-1);
    vector <string> tokens;
    for (unsigned int i = 0; i<rhs.length(); i++) {
        //chars.push_back(lhs.at(i));
        //cout << chars[chars.size()-1] << endl;

        char elem = rhs.at(i);
        if (isdigit(elem)) {
            ostringstream aggrToken;
            aggrToken << elem;
            int locI = i+1;
            while(locI < rhs.length() && isdigit(rhs.at(locI))){
                aggrToken << rhs.at(locI);
                locI++;
            }
            i += locI-1-i; // i = locI-1;
            tokens.push_back(aggrToken.str());
        }
        else if (isalpha(elem) || elem == '$'){
            ostringstream aggrToken;
            aggrToken << elem;
            int locI = i+1;
            while(locI < rhs.length() && (isalpha(rhs.at(locI)) || isdigit(rhs.at(locI)))) {

                aggrToken << rhs.at(locI);
                locI++;
            }
            i += locI-1-i;
            tokens.push_back(aggrToken.str());
        }
        else if (elem == '+' || elem == '*' || elem == '/' || elem == '(' || elem == ')' || elem == '[' || elem == ']'){
            string charStr = "";
            charStr += elem;
            tokens.push_back(charStr);
        }
        else if (elem == '-'){
            string charStr = "";
            charStr += elem;
            if (tokens.empty() || tokens[tokens.size()-1] == "(") tokens.push_back("0");
            tokens.push_back(charStr);
        }
        else {
            cout << "Wrong character in expression" << endl;
        }
    }
    return tokens;
}

bool SignHandler::hasPrecedence(char op1, char op2){
    if (op2 == '(' || op2 == ')') return false;
    if ((op1 == '*' || op1 == '/') && (op2 == '+' || op2 == '-')) return false;
    return true;
}

bool SignHandler::isOperator(char c){
     if (c == '+' || c == '-' || c == '*' || c == '/' || c == '[' || c == ']' || c == '(' || c == ')') return true;
     return false;
}

void SignHandler::buildPlusTable(){
    plusTable['-']['-'] = {'-'};
    plusTable['-']['0'] = {'-'};
    plusTable['-']['+'] = {'-','0','+'};

    plusTable['0']['-'] = {'-'};
    plusTable['0']['0'] = {'0'};
    plusTable['0']['+'] = {'+'};

    plusTable['+']['-'] = {'-','0','+'};
    plusTable['+']['0'] = {'+'};
    plusTable['+']['+'] = {'+'};
}

void SignHandler::buildMinusTable(){
    minusTable['-']['-'] = {'-','0','+'};
    minusTable['-']['0'] = {'-'};
    minusTable['-']['+'] = {'-','0','+'};

    minusTable['0']['-'] = {'+'};
    minusTable['0']['0'] = {'0'};
    minusTable['0']['+'] = {'-'};

    minusTable['+']['-'] = {'+'};
    minusTable['+']['0'] = {'+'};
    minusTable['+']['+'] = {'-','0','+'};
}

void SignHandler::buildMultTable(){
    multTable['-']['-'] = {'+'};
    multTable['-']['0'] = {'0'};
    multTable['-']['+'] = {'-'};

    multTable['0']['-'] = {'0'};
    multTable['0']['0'] = {'0'};
    multTable['0']['+'] = {'0'};

    multTable['+']['-'] = {'-'};
    multTable['+']['0'] = {'0'};
    multTable['+']['+'] = {'+'};
}

void SignHandler::buildDivTable(){
    divTable['-']['-'] = {'+'};
    divTable['-']['0'] = {};
    divTable['-']['+'] = {'-'};

    divTable['0']['-'] = {'0'};
    divTable['0']['0'] = {};
    divTable['0']['+'] = {'0'};

    divTable['+']['-'] = {'-'};
    divTable['+']['0'] = {};
    divTable['+']['+'] = {'+'};
}
