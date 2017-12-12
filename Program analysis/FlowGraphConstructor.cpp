#include "FlowGraphConstructor.h"

using namespace std;

FlowGraphConstructor::FlowGraphConstructor(){
    labelNum = 1;
}

vector<pair<int,int>> FlowGraphConstructor::prepareAST(string path){
    ifstream inputProg (path);
    string line;

    getline(inputProg,line);
    astnode rootNode ("root", intermediate);
    connectData.push_back(make_pair(0,&rootNode));


    while (getline(inputProg,line)){
        if (!line.empty()){
            int numOfEquals = calcTreeLevel(line);
            //cout << line << endl;
            pair<string, NodeType> args = convNames(line.substr(numOfEquals, line.size()-1));

            astnode* curNode = new astnode(args.first, args.second); // is it a memory leak?

            astnode* itsParent;
            itsParent = findParent(numOfEquals, connectData);
            curNode->setParent(itsParent);
            connectData.push_back(make_pair(numOfEquals,curNode));
        }
        else break;
    }
    cout << endl;
    for (unsigned int i = 0; i<connectData.size(); i++) cout << connectData[i].first << " : " << connectData[i].second->getVal() << endl;
    //for (unsigned int i = 0; i<connectData.size(); i++) delete connectData[i].second;
    rootNode.printTree();
    //constructFlowGraph(&rootNode);
    //return &rootNode;
    sStar = rootNode.getChildren();
    return constructFlowGraph(&rootNode);
}

vector<pair<int,int>> FlowGraphConstructor::constructFlowGraph(astnode* astroot){

    calcLabels(astroot->getChildren());
    for (unsigned int i = 0; i<blockList.size(); i++)blockList[i].printSingle();

    vector<int> finals = calcFinal(astroot->getChildren());
    //for (unsigned int i = 0; i<finals.size(); i++) cout << finals[i] << " ";

    cout << endl;

    vector<pair<int,int>> res = calcFlow(astroot->getChildren());
    for (unsigned int i = 0; i<res.size(); i++){
        cout << "(" << res[i].first << ":" << res[i].second << ") ";
    }
   return res;
}

vector<flgnode> FlowGraphConstructor::getBlockList(){
    return blockList;
}

void FlowGraphConstructor::calcLabels(vector<astnode*> prgseq){
    //cout << prgseq[0]->getVal() << endl;
    if (prgseq.empty()) cout << "Something went wrong in calcLabels" << endl;
    if (prgseq.size() == 1){
        if(prgseq[0]->getNodeType() == trivial) {
            flgnode flgn(prgseq[0]->getVal(), labelNum);
            blockList.push_back(flgn);
            prgseq[0]->setLabel(labelNum);
            labelNum++;
        }
        else calcLabels(prgseq[0]->getChildren()); //
      // else if (prgseq[0]->getChildren().size() != 0) calcLabels(prgseq[0]->getChildren()); // {} case handler
    }
    else {
        vector<astnode*> S1;
        S1.push_back(prgseq[0]);
        vector<astnode*> S2 = prgseq;
        S2.erase(S2.begin());
        calcLabels(S1);
        calcLabels(S2);
    }
}

int FlowGraphConstructor::calcInit(vector<astnode*> prgseq){
    if (prgseq.empty()) cout << "Something went wrong in calcInit" << endl;
    if (prgseq.size() == 1){
        if(prgseq[0]->getNodeType() == trivial) return prgseq[0]->getLabel();
        else return prgseq[0]->getChildren()[0]->getLabel();
    }
    else {
        vector<astnode*> S1;
        S1.push_back(prgseq[0]);
        return calcInit(S1);
    }
}

vector<int> FlowGraphConstructor::calcFinal(vector<astnode*> prgseq){
    if (prgseq.empty()) cout << "Something went wrong in calcFinal" << endl;
    if (prgseq.size() == 1){
        if(prgseq[0]->getNodeType() == trivial) {
            vector<int> tempVect;
            if (prgseq[0]->getVal() == "break;" || prgseq[0]->getVal() == "continue;") {
                // do nothing
            }
            else tempVect.push_back(prgseq[0]->getLabel());
            return tempVect;
        }

        else if (prgseq[0]->getNodeType() == branch) {
            if (prgseq[0]->getChildren().size() == 2){
                vector<int> tempVect;
                tempVect.push_back(prgseq[0]->getChildren()[0]->getLabel());
                vector<astnode*> S0 = prgseq[0]->getChildren()[1]->getChildren();
                return vectUnion(tempVect, calcFinal(S0));
            }
            else if (prgseq[0]->getChildren().size() == 3){
                vector<astnode*> S0 = prgseq[0]->getChildren()[1]->getChildren();
                vector<astnode*> S1 = prgseq[0]->getChildren()[2]->getChildren();
                return vectUnion(calcFinal(S0),calcFinal(S1));
            }
        }

        else if (prgseq[0]->getNodeType() == loop) {
            vector<int> tempVect;
            tempVect.push_back(prgseq[0]->getChildren()[0]->getLabel());
            return vectUnion(tempVect, calcBreakCont(prgseq[0]->getChildren()[1]->getChildren(), "break;"));
        }

        else return calcFinal(prgseq[0]->getChildren()); // for intermediate
    }
    else {
        vector<astnode*> S2;
        //cout << prgseq.size() << endl;
        S2.push_back(prgseq[prgseq.size()-1]);
        return calcFinal(S2);
    }
}

vector<int> FlowGraphConstructor::calcBreakCont(vector<astnode*> prgseq, string brOrCont){
    if (prgseq.empty()) cout << "Something went wrong in calcBreakCont" << endl;
    vector<int> tempVect;
     if (prgseq.size() == 1){
        if(prgseq[0]->getNodeType() == trivial) {
           if (prgseq[0]->getVal() == brOrCont) tempVect.push_back(prgseq[0]->getLabel());
           return tempVect;
        }
        else if (prgseq[0]->getNodeType() != loop) return calcBreakCont(prgseq[0]->getChildren(), brOrCont);
        else return tempVect;
     }
     else {
        vector<astnode*> S1;
        S1.push_back(prgseq[0]);
        vector<astnode*> S2 = prgseq;
        S2.erase(S2.begin());
        return vectUnion(calcBreakCont(S1, brOrCont), calcBreakCont(S2, brOrCont));
     }
}

vector<pair<int,int>> FlowGraphConstructor::calcFlow(vector<astnode*> prgseq){
    if (prgseq.empty()) cout << "Something went wrong in calcFlow" << endl;
    /*cout << "Running on S: ";
    for (unsigned int i = 0; i<prgseq.size(); i++){
        cout << " " << prgseq[i]->getVal() << " ";
    }
    cout << endl;*/
    vector<pair<int,int>> tempVect;
     if (prgseq.size() == 1){
        if(prgseq[0]->getNodeType() == trivial) {
           if (prgseq[0]->getVal() == "break;") {
                int leadsToL = -1;
                if ((whiles.top()->getChildOfNumber()+1) < (whiles.top()->getParent()->getChildren().size())){
                    leadsToL= whiles.top()->getParent()->getChildren()[whiles.top()->getChildOfNumber()+1]->getLabel();
                    whiles.pop();
                    tempVect.push_back(make_pair(prgseq[0]->getLabel(),leadsToL));
                }
                else {
                    // probably inconsistent program
                }
           }
           else if (prgseq[0]->getVal() == "continue;"){

           }
           else {
              // do nothing
           }
           return tempVect;
        }

        else if(prgseq[0]->getNodeType() == loop) {
            whiles.push(prgseq[0]);
            vector<astnode*> S0 = prgseq[0]->getChildren()[1]->getChildren();
            vector<pair<int,int>> flowS0 = calcFlow(S0);
            vector<int> finContS0 = vectUnion(calcFinal(S0), calcBreakCont(S0, "continue;"));
            int L = prgseq[0]->getChildren()[0]->getLabel();
            vector<pair<int,int>> S0toL;
            vector<pair<int,int>> LtoS0;
            LtoS0.push_back(make_pair(L, calcInit(S0)));
            for (unsigned int i = 0; i<finContS0.size(); i++) {
                S0toL.push_back(make_pair(finContS0[i],L));
            }
            tempVect = vectUnion(flowS0, vectUnion(LtoS0,S0toL));
            return tempVect;
        }

        else if(prgseq[0]->getNodeType() == branch){

             //cout << "branch detected" << endl;

             vector<astnode*> S0 = prgseq[0]->getChildren()[1]->getChildren();
             vector<pair<int,int>> flowS0 = calcFlow(S0);
             int L = prgseq[0]->getChildren()[0]->getLabel();
             vector<pair<int,int>> bToS0;
             bToS0.push_back(make_pair(L, calcInit(S0)));
             tempVect = vectUnion(flowS0, bToS0);
             if (prgseq[0]->getChildren().size() == 3){
                vector<astnode*> S1 = prgseq[0]->getChildren()[2]->getChildren();
                vector<pair<int,int>> flowS1 = calcFlow(S1);
                vector<pair<int,int>> bToS1;
                bToS1.push_back(make_pair(L, calcInit(S1)));
                tempVect = vectUnion(tempVect, vectUnion(flowS1,bToS1));
             }
             return tempVect;
        }

        else {
            // for intermediate node type, do nothing
            return tempVect;
        }

     }
     else {
        vector<astnode*> S1;
        S1.push_back(prgseq[0]);
        vector<astnode*> S2 = prgseq;
        S2.erase(S2.begin());
        vector<int> finalsS1 = calcFinal(S1);
        vector<pair<int,int>> S1toS2;
        int initS2 = calcInit(S2);
        for (unsigned int i = 0; i<finalsS1.size(); i++){
            S1toS2.push_back(make_pair(finalsS1[i],initS2));
        }
        return vectUnion(calcFlow(S1), vectUnion(calcFlow(S2),S1toS2));
     }
}

int FlowGraphConstructor::calcTreeLevel(string line){
    int numOfEquals = 0;
    for (unsigned int i=0; i<line.length(); i++){
        if (line.at(i) == '=') numOfEquals++;
        else break;
    }
    return numOfEquals;
}

astnode* FlowGraphConstructor::findParent(int numOfEquals, vector<pair<int,astnode*>> connectInfo){
    int i = connectInfo.size()-1;
    while (connectInfo[i].first>=numOfEquals && i>=0) i--;
    return connectInfo[i].second;
}

pair<string, NodeType> FlowGraphConstructor::convNames(string expr){
    string newName;
    NodeType newType;

    if (expr == "if") {
        newName = "ifcond";
        newType = branch;
    }
    else if (expr == "while") {
        newName = expr;
        newType = loop;
    }
     else if (expr == "if_interm"
             || expr == "else"
             || expr == "while_interm"){
        newName = "block";
        newType = intermediate;
    }
    else {
        newName = expr;
        newType = trivial;
    }
    return make_pair(newName, newType);
}
set<int> FlowGraphConstructor::getInit(){
    int initLabel = calcInit(sStar);
    vector<int> resv;
    resv.push_back(initLabel);
    set<int> ress (resv.begin(), resv.end());
    return ress;
}
set<int> FlowGraphConstructor::getFinal(){
    vector<int> resv = calcFinal(sStar);
    set<int> ress (resv.begin(), resv.end());
    return ress;
}
