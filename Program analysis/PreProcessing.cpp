#include "PreProcessing.h"

using namespace std;

// vars are marked with $ by Sparser.java
bool isVar(string token){
    if (token.at(0) == '$') return true;
    return false;
}
// parse an expr to get tokens of it
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
                aggrToken << expr.at(locI); // fetch all the digits of a number
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

                aggrToken << expr.at(locI); //fetch all the digits or chars of a variable or an expression like "read.."
                locI++;
            }
            i += locI-1-i;
            tokens.push_back(aggrToken.str());
        }
        else if (elem == '+' || elem == '*' || elem == '/' || elem == '(' || elem == ')' || elem == '[' || elem == ']'){
            string charStr = "";
            charStr += elem;
            tokens.push_back(charStr); //fetch all single element operators and brackets
        }
        else if (elem == '-'){
            string charStr = "";
            charStr += elem; //-
            if (tokens.empty() || tokens[tokens.size()-1] == "(") tokens.push_back("0"); // so we have 0-something (for SDA)
            tokens.push_back(charStr);
        }
        else if (elem == '<' || elem == '>' || elem == '=' || elem == '!'){
            string charStr = "";
            charStr += elem;
            if (expr.at(i+1) == '=') { //check if double comparison operator
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
        killsGens.push_back(make_pair(dummySet, dummySet)); //start killsGens with dummy block to count intuitively from 1
        calcKillsGens();
    }
}

void PreProcessing::calcTokensOfLabels(){
    for (unsigned int i = 0; i<blocks.size(); i++){
        vector<string> tokens = getTokens(blocks[i].getVal());
        //tokensAtLabels.insert(blocks[i].getLabel(), tokens);
        tokensAtLabels[blocks[i].getLabel()] = tokens; // map labels to tokens of corresp. blocks
    }
}
// get fvs of tokens of a block
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
// get expressions of a block, not implemented yet
set<string> PreProcessing::getExpressions(flgnode block){

}
// get all elements of a complete lattice
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
         calcExtremalValues();
          for (unsigned int i = 0; i<blocks.size(); i++){

            set<string> elemsPerBlock;
            //elemsPerBlock = getFreeVariables(blocks[i]);
            // find variables in S being assigned to, those will be the elements of our complete lattice
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
                oss << varAssTo << "," << blocks[i].getLabel(); // get vars in format "var,label it is assigned in"
                elemsPerBlock.insert(oss.str());
            }
            set_union(res.begin(),res.end(), elemsPerBlock.begin(), elemsPerBlock.end(),inserter(res,res.begin()));
        }
    }
    set_union(res.begin(),res.end(), allElems.begin(), allElems.end(),inserter(allElems,allElems.begin()));
}
// works for RDA for now
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
                if ((i+2) <= tokens.size()-1){
                nextTok = tokens[i+2];
                }
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
                    res.insert(someElem); // go through all elems. If found variable we are assigning to, add it,label to list of kills.
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


