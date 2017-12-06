#include "PreProcessing.h"

using namespace std;

bool isVar(string token){
    if (token.at(0) == '$') return true;
    return false;
}

vector<string> getTokens(string expr){
    //string rhs = expr.substr(expr.find(":=")+2, expr.length()-1);
    vector <string> tokens;
    for (unsigned int i = 0; i<expr.length(); i++) {
        //chars.push_back(lhs.at(i));
        //cout << chars[chars.size()-1] << endl;

        char elem = expr.at(i);
        if (isdigit(elem)) {
            ostringstream aggrToken;
            aggrToken << elem;
            int locI = i+1;
            while(locI < expr.length() && isdigit(expr.at(locI))){
                aggrToken << expr.at(locI);
                locI++;
            }
            i += locI-1-i; // i = locI-1;
            tokens.push_back(aggrToken.str());
        }
        else if (isalpha(elem) || elem == '$'){
            ostringstream aggrToken;
            aggrToken << elem;
            int locI = i+1;
            while(locI < expr.length() && (isalpha(expr.at(locI)) || isdigit(expr.at(locI)))) {

                aggrToken << expr.at(locI);
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
        else if (elem == '<' || elem == '>' || elem == '=' || elem == '!'){
            string charStr = "";
            charStr += elem;
            if (expr.at(i+1) == '=') {
                charStr += '=';
                i++;
            }
            tokens.push_back(charStr);
        }
        else if (elem != ';'){
            cout << elem << endl;
            cout << "Wrong character in expression" << endl;
        }
    }
    return tokens;
}

PreProcessing::PreProcessing(vector<flgnode> blocks, Analysis analysis){
    this->blocks = blocks;
    this->analysis = analysis;
    calcTokensOfLabels();
    calcAllElems();
    if (analysis == AE || analysis == VBE || analysis == RD || analysis == LV){
        set<string> dummySet;
        //kills.push_back(dummySet);
        //gens.push_back(dummySet);
        killsGens.push_back(make_pair(dummySet, dummySet));
        calcKillsGens();
    }
}

void PreProcessing::calcTokensOfLabels(){
    for (unsigned int i = 0; i<blocks.size(); i++){
        vector<string> tokens = getTokens(blocks[i].getVal());
        //tokensAtLabels.insert(blocks[i].getLabel(), tokens);
        tokensAtLabels[blocks[i].getLabel()] = tokens;
    }
}

set<string> PreProcessing::getFreeVariables(flgnode block){
    set<string> res;
    vector<string> tokens = tokensAtLabels[block.getLabel()];
    for (unsigned int i = 0; i<tokens.size(); i++){
        if (isVar(tokens[i])) {
            /*if (withLabel) {
                ostringstream oss1;
                oss1 << tokens[i] << "," << "?";
                string var1 = oss1.str();
                res.insert(var1);

                bool isAssignment = false;
                for (unsigned int i = 0; i<tokens.size(); i++){
                    if (tokens[i] == "="){
                        isAssignment = true;
                        break;
                    }
                }
                if (isAssignment){
                    ostringstream oss2;
                    oss2 << tokens[i] << "," << block.getLabel();
                    string var2 = oss2.str();
                    res.insert(var2);
                }
            }
            else*/ res.insert(tokens[i]);
        }
    }
   // cout << "res size " << res.size() << endl;
    return res;
}

set<string> PreProcessing::getExpressions(flgnode block){

}

void PreProcessing::calcAllElems(){
    set<string> res;
    if (analysis == AE || analysis == VBE){
        for (unsigned int i = 0; i<blocks.size(); i++){
            set<string> elemsPerBlock;
            elemsPerBlock = getExpressions(blocks[i]);
            set_union(res.begin(),res.end(), elemsPerBlock.begin(), elemsPerBlock.end(),inserter(res,res.begin()));
        }
    }
    else if (analysis == LV){
        for (unsigned int i = 0; i<blocks.size(); i++){
            set<string> elemsPerBlock;
            elemsPerBlock = getFreeVariables(blocks[i]);
            set_union(res.begin(),res.end(), elemsPerBlock.begin(), elemsPerBlock.end(),inserter(res,res.begin()));
        }
    }
    else if (analysis == RD){
         calcExtremalValues(); // for some reason does not add _,? to allElems, check it
          for (unsigned int i = 0; i<blocks.size(); i++){

            set<string> elemsPerBlock;
            //elemsPerBlock = getFreeVariables(blocks[i]);

            vector<string> tokens = tokensAtLabels[blocks[i].getLabel()];
            bool isAssignment = false;
            string varAssTo = "";
            for (unsigned int i = 0; i<tokens.size(); i++){
                if (tokens[i] == "="){
                    isAssignment = true;
                    varAssTo = tokens[0];
                    break;
                }
                else if (tokens[i] == "read" || tokens[i] == "int"){
                    isAssignment = true;
                    varAssTo = tokens[i+1];
                    break;
                }
            }

            if (isAssignment){
                ostringstream oss;
                oss << varAssTo << "," << blocks[i].getLabel();
                elemsPerBlock.insert(oss.str());
            }
            set_union(res.begin(),res.end(), elemsPerBlock.begin(), elemsPerBlock.end(),inserter(res,res.begin()));
        }
    }
    set_union(res.begin(),res.end(), allElems.begin(), allElems.end(),inserter(allElems,allElems.begin()));
}

void PreProcessing::calcExtremalValues(){
    set<string> res;
    for (unsigned int i = 0; i<blocks.size(); i++){
        set<string> fvPerBlock = getFreeVariables(blocks[i]); //get vars of a block as a set to avoid duplicates
        vector<string> fvPerBlockVec (fvPerBlock.begin(), fvPerBlock.end()); //convert them to vector
        for (unsigned int i = 0; i<fvPerBlockVec.size(); i++){
            fvPerBlockVec[i] += ",?"; // go through flow variables in the block and append ",?" to them (as we do in RDA)
        }
        set<string> fvPerBlockMod (fvPerBlockVec.begin(), fvPerBlockVec.end()); // convert vector to set<string> for unioning it with result to avoid duplicates
        set_union(res.begin(),res.end(), fvPerBlockMod.begin(), fvPerBlockMod.end(),inserter(res,res.begin()));
    }
    extremalValues = res;
    set_union(allElems.begin(),allElems.end(), extremalValues.begin(), extremalValues.end(),inserter(allElems,allElems.begin()));
}


//vector<set<string>> PreProcessing::calcKills (flgnode block){
set<string> PreProcessing::calcKillsForBlock (flgnode block){
    set<string> res;
    if (analysis == RD){
        vector<string> tokens = tokensAtLabels[block.getLabel()];

        bool isAssignment = false;
        string varAssTo = "";
        string nextTok = ""; // array checker
        for (unsigned int i = 0; i<tokens.size(); i++){
            if (tokens[i] == "="){
                isAssignment = true;
                varAssTo = tokens[0];
                nextTok = tokens[1];
                break;
            }
            else if (tokens[i] == "read" || tokens[i] == "int"){
                isAssignment = true;
                varAssTo = tokens[i+1];
                nextTok = tokens[i+2];
                break;
            }
        }

        if (isAssignment && nextTok != "[") {
            ostringstream oss;
            oss << varAssTo << ",?";
            res.insert(oss.str()); // add variable being assigned to to the list of kills with appended ",?"
            set<string>::iterator it;
            for (it = allElems.begin(); it != allElems.end(); ++it)
            {
                string someElem = *it;
                if (varAssTo == someElem.substr(0, someElem.find(","))) {
                    res.insert(someElem); // go through all elems. If found variable we are assigning to, add it to list of kills.
                }
            }
        }
    }
    else if (analysis == LV){

    }
    else if (analysis == AE){

    }
    else if (analysis == VBE){

    }
    return res;
}

//vector<set<string>> PreProcessing::calcGens (flgnode block){
set<string> PreProcessing::calcGensForBlock (flgnode block){
    set<string> res;
    if (analysis == RD){
        vector<string> tokens = tokensAtLabels[block.getLabel()];
        bool isAssignment = false;
        string varAssTo = "";
        for (unsigned int i = 0; i<tokens.size(); i++){
            if (tokens[i] == "="){
                isAssignment = true;
                varAssTo = tokens[0];
                break;
            }
            else if (tokens[i] == "read" || tokens[i] == "int"){
                isAssignment = true;
                varAssTo = tokens[i+1];
            }
        }
        if (isAssignment) {
            ostringstream oss;
            oss << varAssTo << "," << block.getLabel(); // at index 0, as there is one token before '=' - the variable being assigned to
            res.insert(oss.str()); // append gen in format var,label for an assignment
        }
    }
    else if (analysis == LV){

    }
    else if (analysis == AE){

    }
    else if (analysis == VBE){

    }
    return res;
}

void PreProcessing::calcKillsGens(){
    for (unsigned int i = 0; i<blocks.size(); i++){
        //kills.push_back(calcKillsForBlock(blocks[i]));
        //gens.push_back(calcGensForBlock(blocks[i]));
        killsGens.push_back(make_pair(calcKillsForBlock(blocks[i]), calcGensForBlock(blocks[i])));
    }
}

vector<pair<set<string>,set<string>>> PreProcessing::getKillsGens(){
    return killsGens;
}

set<string> PreProcessing::getAllElems(){
    return allElems;
}

set<string> PreProcessing::getExtremalValues(){
    return extremalValues;
}


