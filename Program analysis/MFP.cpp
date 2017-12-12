#include "MFP.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

string getBlockType(vector<string> tokens){ // not the most efficient conditioning, but let's test the worklist algorithm first
    if ((find(tokens.begin(), tokens.end(), "read") != tokens.end())
        && (find(tokens.begin(), tokens.end(), "[") == tokens.end())) {
            return "regread";
    }
    else if ((find(tokens.begin(), tokens.end(), "read") != tokens.end())
        && (find(tokens.begin(), tokens.end(), "[") != tokens.end())) {
            return "arrread";
    }
    else if ((find(tokens.begin(), tokens.end(), "int") != tokens.end())
        && (find(tokens.begin(), tokens.end(), "[") == tokens.end())) {
            return "regdec";
    }
    else if ((find(tokens.begin(), tokens.end(), "int") != tokens.end())
        && (find(tokens.begin(), tokens.end(), "[") != tokens.end())) {
            return "arrdec";
    }
    else if ((find(tokens.begin(), tokens.end(), "=") != tokens.end())
        && (tokens[1] == "[")) {
            return "arrass";
    }
    else if ((find(tokens.begin(), tokens.end(), "=") != tokens.end())
        && (tokens[1] != "[")) {
            return "regass";
    }
    else return "othertype";
}

MFP::MFP(set<string> allElems, int numOfLabels){
    InstMonFramework(allElems);
    worklistType = "FIFO";
    this->numOfLabels = numOfLabels;
}

MFP::MFP(vector<pair<int,int>> flow,
                              set<int> extrLabels, set<string> extrVal,
                              set<string> bottomElems, bool subsOrSups, bool unOrInters,
                              vector<pair<set<string>,set<string>>> tfs, int numOfLabels, string worklistType){

    this->flow = flow;
    this->extrLabels = extrLabels;
    this->extrVal = extrVal;
    this->bottomElems = bottomElems;
    this->subsOrSups = subsOrSups;
    this->unOrInters = unOrInters;
    this->tfs = tfs;
    this->numOfLabels = numOfLabels;
    this->worklistType = worklistType;
}

MFP::MFP(vector<pair<int,int>> flow,
                              set<int> extrLabels, map<string,set<char>> extrVal,
                              map<string,set<char>> bottomElems, bool subsOrSups, bool unOrInters,
                              vector<flgnode> blockList, int numOfLabels, string worklistType,
                              map<int,vector<string>> tokensAtLabels){

    this->flow = flow;
    this->extrLabels = extrLabels;
    this->extrValSDA = extrVal;  // initial sigmaHat
    this->bottomElemsSDA = bottomElems;
    this->subsOrSups = subsOrSups;
    this->unOrInters = unOrInters;
    this->blockList = blockList;
    this->numOfLabels = numOfLabels;
    this->worklistType = worklistType;
    this->tokensAtLabels = tokensAtLabels;
}

// for now input is set manually
void MFP::InstMonFramework(set<string> allElems){
    //AE analysis example
    /*flow.push_back(make_pair(1,2));
    flow.push_back(make_pair(2,3));
    flow.push_back(make_pair(3,4));
    flow.push_back(make_pair(4,5));
    flow.push_back(make_pair(5,3));
    extrLabels.insert(1); // call calcInit on analyzed program
    extrVal.clear();
    bottomElems = allElems;
    subsOrSups = true;
    unOrInters = true;
    set<T> k0;
    set<T> g0;
    set<T> k1;
    set<T> g1;
    g1.insert("a+b");
    set<T> k2;
    set<T> g2;
    g2.insert("a*b");
    set<T> k3;
    set<T> g3;
    g3.insert("a+b");
    set<T> k4;
    k4.insert("a+b");
    k4.insert("a*b");
    k4.insert("a+1");
    set<T> g4;
    set<T> k5;
    set<T> g5;
    g5.insert("a+b");
    tfs.push_back(make_pair(k0,g0));
    tfs.push_back(make_pair(k1,g1));
    tfs.push_back(make_pair(k2,g2));
    tfs.push_back(make_pair(k3,g3));
    tfs.push_back(make_pair(k4,g4));
    tfs.push_back(make_pair(k5,g5));*/

    //RD analysis example
    /*flow.push_back(make_pair(1,2));
    flow.push_back(make_pair(2,3));
    flow.push_back(make_pair(3,4));
    flow.push_back(make_pair(4,5));
    flow.push_back(make_pair(5,3));
    flow.push_back(make_pair(3,6));
    extrLabels.insert(1); // call calcInit on analyzed program
    extrVal.insert("x,?"); // this should be automatized!
    extrVal.insert("y,?");
    bottomElems.clear();
    subsOrSups = false;
    unOrInters = false;
    set<T> k0;
    set<T> g0;
    set<T> k1;
    k1.insert("x,?");
    k1.insert("x,1");
    k1.insert("x,5");
    k1.insert("x,6");
    set<T> g1;
    g1.insert("x,1");
    set<T> k2;
    k2.insert("y,?");
    k2.insert("y,2");
    k2.insert("y,4");
    set<T> g2;
    g2.insert("y,2");
    set<T> k3;
    set<T> g3;
    set<T> k4;
    k4.insert("y,?");
    k4.insert("y,2");
    k4.insert("y,4");
    set<T> g4;
    g4.insert("y,4");
    set<T> k5;
    k5.insert("x,?");
    k5.insert("x,1");
    k5.insert("x,5");
    k5.insert("x,6");
    set<T> g5;
    g5.insert("x,5");
    set<T> k6;
    k6.insert("x,?");
    k6.insert("x,1");
    k6.insert("x,5");
    k6.insert("x,6");
    set<T> g6;
    g6.insert("x,6");
    tfs.push_back(make_pair(k0,g0));
    tfs.push_back(make_pair(k1,g1));
    tfs.push_back(make_pair(k2,g2));
    tfs.push_back(make_pair(k3,g3));
    tfs.push_back(make_pair(k4,g4));
    tfs.push_back(make_pair(k5,g5));
    tfs.push_back(make_pair(k6,g6)); */


    //LV analysis example 1
    /*flow.push_back(make_pair(1,2));
    flow.push_back(make_pair(2,3));
    flow.push_back(make_pair(3,4));
    flow.push_back(make_pair(4,5));
    flow.push_back(make_pair(4,6));
    flow.push_back(make_pair(5,7));
    flow.push_back(make_pair(6,7));
    for (unsigned int i = 0; i<flow.size(); i++){
        int tempLabel = flow[i].first;
        flow[i].first = flow[i].second;
        flow[i].second = tempLabel;
    }
    reverse(flow.begin(),flow.end());
    extrLabels.insert(7); // call calcFinal on analyzed program
    extrVal.clear();
    bottomElems.clear();
    subsOrSups = false;
    unOrInters = false;
    set<T> k0;
    set<T> g0;
    set<T> k1;
    k1.insert("x");
    set<T> g1;
    set<T> k2;
    k2.insert("y");
    set<T> g2;
    set<T> k3;
    k3.insert("x");
    set<T> g3;
    set<T> k4;
    set<T> g4;
    g4.insert("x");
    g4.insert("y");
    set<T> k5;
    k5.insert("z");
    set<T> g5;
    g5.insert("y");
    set<T> k6;
    k6.insert("z");
    set<T> g6;
    g6.insert("y");
    set<T> k7;
    k7.insert("x");
    set<T> g7;
    g7.insert("z");
    tfs.push_back(make_pair(k0,g0));
    tfs.push_back(make_pair(k1,g1));
    tfs.push_back(make_pair(k2,g2));
    tfs.push_back(make_pair(k3,g3));
    tfs.push_back(make_pair(k4,g4));
    tfs.push_back(make_pair(k5,g5));
    tfs.push_back(make_pair(k6,g6));
    tfs.push_back(make_pair(k7,g7));*/

    //LV analysis example 2
    /*flow.push_back(make_pair(1,2));
    flow.push_back(make_pair(2,3));
    flow.push_back(make_pair(3,4));
    flow.push_back(make_pair(4,5));
    flow.push_back(make_pair(5,6));
    flow.push_back(make_pair(6,7));
    flow.push_back(make_pair(7,8));
    flow.push_back(make_pair(8,9));
    flow.push_back(make_pair(8,10));
    flow.push_back(make_pair(10,7));
    flow.push_back(make_pair(9,11));
    flow.push_back(make_pair(7,11));
    for (unsigned int i = 0; i<flow.size(); i++){
        int tempLabel = flow[i].first;
        flow[i].first = flow[i].second;
        flow[i].second = tempLabel;
    }
    reverse(flow.begin(),flow.end());
    extrLabels.insert(7); // call calcFinal on analyzed program
    extrVal.clear();
    bottomElems.clear();
    subsOrSups = false;
    unOrInters = false;
    set<T> k0;
    set<T> g0;
    set<T> k1;
    set<T> g1;
    set<T> k2;
    set<T> g2;
    set<T> k3;
    set<T> g3;
    set<T> k4;
    k4.insert("x");
    set<T> g4;
    set<T> k5;
    k5.insert("y");
    set<T> g5;
    set<T> k6;
    k6.insert("x");
    set<T> g6;
    set<T> k7;
    set<T> g7;
    g7.insert("y");
    g7.insert("x");
    set<T> k8;
    set<T> g8;
    g8.insert("z");
    set<T> k9;
    set<T> g9;
    set<T> k10;
    k10.insert("z");
    set<T> g10;
    g10.insert("y");
    set<T> k11;
    k11.insert("x");
    set<T> g11;
    g11.insert("y");
    tfs.push_back(make_pair(k0,g0));
    tfs.push_back(make_pair(k1,g1));
    tfs.push_back(make_pair(k2,g2));
    tfs.push_back(make_pair(k3,g3));
    tfs.push_back(make_pair(k4,g4));
    tfs.push_back(make_pair(k5,g5));
    tfs.push_back(make_pair(k6,g6));
    tfs.push_back(make_pair(k7,g7));
    tfs.push_back(make_pair(k8,g8));
    tfs.push_back(make_pair(k9,g9));
    tfs.push_back(make_pair(k10,g10));
    tfs.push_back(make_pair(k11,g11));*/

    //VBE analysis example
    /*flow.push_back(make_pair(1,2));
    flow.push_back(make_pair(2,3));
    flow.push_back(make_pair(1,4));
    flow.push_back(make_pair(4,5));
    for (unsigned int i = 0; i<flow.size(); i++){
        int tempLabel = flow[i].first;
        flow[i].first = flow[i].second;
        flow[i].second = tempLabel;
    }
    reverse(flow.begin(),flow.end());
    extrLabels.insert(3); // call calcFinal on analyzed program
    extrLabels.insert(5);
    extrVal.clear();
    bottomElems = allElems;
    subsOrSups = true;
    unOrInters = true;
    set<T> k0;
    set<T> g0;
    set<T> k1;
    set<T> g1;
    set<T> k2;
    set<T> g2;
    g2.insert("b-a");
    set<T> k3;
    set<T> g3;
    g3.insert("a-b");
    set<T> k4;
    set<T> g4;
    g4.insert("b-a");
    set<T> k5;
    set<T> g5;
    g5.insert("a-b");
    tfs.push_back(make_pair(k0,g0));
    tfs.push_back(make_pair(k1,g1));
    tfs.push_back(make_pair(k2,g2));
    tfs.push_back(make_pair(k3,g3));
    tfs.push_back(make_pair(k4,g4));
    tfs.push_back(make_pair(k5,g5));*/
}

void MFP::SolveEquationsBvf(){
    vector<set<string>> Analysis;
    int numOfIterations = 0;
    Analysis.resize(numOfLabels+1);

    // Step 1
    vector<pair<int,int>> worklist = flow; // make this dependent on LIFO/FIFO
    if (worklistType == "FIFO") reverse(worklist.begin(), worklist.end());
    for (unsigned int i = 0; i<flow.size(); i++){
        int l1 = flow[i].first;
        int l2 = flow[i].second;
        if (extrLabels.count(l1)==1) Analysis[l1] = extrVal;
        else Analysis[l1] = bottomElems;
        if (extrLabels.count(l2)==1) Analysis[l2] = extrVal;
        else Analysis[l2] = bottomElems;
    }

    // Step 2
    int iters = 0;
    while (!worklist.empty()){
        numOfIterations++;
        int l = worklist[0].first;
        int lPr = worklist[0].second;
        worklist.erase(worklist.begin());
        set<string> tfApplied = calcTFbvf(Analysis, l);
        set<string> intersResSet;
        set_intersection(tfApplied.begin(),tfApplied.end(),Analysis[lPr].begin(),Analysis[lPr].end(),inserter(intersResSet,intersResSet.begin()));

        set<string> expectResSet;
        if (subsOrSups){
            expectResSet = Analysis[lPr];
        }
        else {
            expectResSet = tfApplied;
        }

        //if (!(subSet.size() <= superSet.size() && tempSet == intersResSet)) {
        //if (subSet.size() < superSet.size() || intersResSet != expectResSet){
        if (intersResSet != expectResSet){
            set<string> tempSet;
            if (unOrInters)
                set_intersection(Analysis[lPr].begin(),Analysis[lPr].end(), tfApplied.begin(), tfApplied.end(),inserter(tempSet,tempSet.begin()));
            else
                set_union(Analysis[lPr].begin(),Analysis[lPr].end(), tfApplied.begin(), tfApplied.end(),inserter(tempSet,tempSet.begin()));
            Analysis[lPr] = tempSet;
            for (unsigned int i = 0; i<flow.size(); i++){
                //cout << flow[i].first << "....." << lPr << endl;
                if (flow[i].first == lPr && find(worklist.begin(), worklist.end(), flow[i]) == worklist.end()) {
                    if (worklistType == "LIFO") worklist.insert(worklist.begin(),flow[i]);
                    else if (worklistType == "FIFO") worklist.push_back(flow[i]);
                }
            }
        }

        // debugging
        iters++;
        for (unsigned i = 0; i<worklist.size(); i++){
            cout << "(" << worklist[i].first << " " << worklist[i].second << ")  ";
        }
        cout << endl;


        for (unsigned i = 0; i<Analysis.size(); i++){
            cout << "Analysis[ " << i << " ]: ";
            copy(
                Analysis[i].begin(),
                Analysis[i].end(),
                ostream_iterator<string>(cout, " ")
            );
            cout << endl;
        }
        cout << endl;
    }

    // Step 3
    for (unsigned int i = 0; i<Analysis.size(); i++){
        enterSols.push_back(Analysis[i]);
        exitSols.push_back(calcTFbvf(Analysis, i));
    }

    cout << endl;

    for (unsigned int i = 0; i<Analysis.size(); i++){
        cout << "Exit solution[ " << i << " ]: ";
            copy(
                exitSols[i].begin(),
                exitSols[i].end(),
                ostream_iterator<string>(cout, " ")
            );
            cout << endl;
    }

    cout << endl;
    cout << "Iterations: " << numOfIterations << endl;
}

void MFP::SolveEquations(){

    vector<map<string, set<char>>> Analysis;
    int numOfIterations = 0;
    Analysis.resize(numOfLabels+1);

    // Step 1
    vector<pair<int,int>> worklist = flow;
    if (worklistType == "LIFO") reverse(worklist.begin(), worklist.end());
    for (unsigned int i = 0; i<flow.size(); i++){
        int l1 = flow[i].first;
        int l2 = flow[i].second;
        if (extrLabels.count(l1)==1) Analysis[l1] = extrValSDA;
        else Analysis[l1] = bottomElemsSDA;
        if (extrLabels.count(l2)==1) Analysis[l2] = extrValSDA;
        else Analysis[l2] = bottomElemsSDA;
    }

    int iters = 0;
    while (!worklist.empty()){
        numOfIterations++;
        int l = worklist[0].first;
        int lPr = worklist[0].second;
        worklist.erase(worklist.begin());

        bool changeHappened = false;
        map<string, set<char>>::iterator it;

        for ( it = Analysis[l].begin(); it != Analysis[l].end(); it++ )
        {
            //set<string> tfApplied = calcTF(Analysis,l);
            map<string, set<char>> tfApplied = calcTF(blockList[l-1], Analysis[l]);
            set<char> tfAppliedEntry = tfApplied[it->first];
            set<char> intersResSet;
            set_intersection(tfAppliedEntry.begin(),tfAppliedEntry.end(),
                             Analysis[lPr][it->first].begin(),Analysis[lPr][it->first].end(),
                             inserter(intersResSet,intersResSet.begin()));
            set<char> expectResSet;
            if (subsOrSups) expectResSet = Analysis[lPr][it->first];
            else expectResSet = tfAppliedEntry;

            if (intersResSet != expectResSet){
                changeHappened = true;
                set<char> tempSet;
                if (unOrInters)
                    set_intersection(Analysis[lPr][it->first].begin(),Analysis[lPr][it->first].end(),
                                      tfAppliedEntry.begin(), tfAppliedEntry.end(),
                                      inserter(tempSet,tempSet.begin()));
                else
                    set_union(Analysis[lPr][it->first].begin(),Analysis[lPr][it->first].end(),
                               tfAppliedEntry.begin(), tfAppliedEntry.end(),
                               inserter(tempSet,tempSet.begin()));
                Analysis[lPr][it->first] = tempSet;
            }
        }

        if (changeHappened){
            for (unsigned int i = 0; i<flow.size(); i++){
                //cout << flow[i].first << "....." << lPr << endl;
                if (flow[i].first == lPr && find(worklist.begin(), worklist.end(), flow[i]) == worklist.end()) {
                    if (worklistType == "FIFO") worklist.insert(worklist.begin(),flow[i]);
                    else if (worklistType == "LIFO") worklist.push_back(flow[i]);
                }
            }
        }

        iters++;

    }

    // Step 3
    for (unsigned int i = 1; i<Analysis.size(); i++){
        enterSolsSDA.push_back(Analysis[i]);
         map<string, set<char>> tfApplied = calcTF(blockList[i-1], Analysis[i]);
         exitSolsSDA.push_back(tfApplied);
    }
    //  print solution
    cout << endl;
    cout << "Iterations: " << iters << endl;
    cout << "Exit solutions at block " << exitSolsSDA.size() << endl;
    map<string, set<char>>::iterator it;
    for ( it = exitSolsSDA[exitSolsSDA.size()-1].begin(); it != exitSolsSDA[exitSolsSDA.size()-1].end(); it++ )
    {
        cout << it->first;
        vector<char> signs (it->second.begin(), it->second.end());
        for (unsigned int i =0; i<signs.size(); i++) cout << " " << signs[i];
        cout << endl;
    }
}

set<string> MFP::calcTFbvf(vector<set<string>> Analysis, int label){
    set<string> killsApplied;
    set_difference( Analysis[label].begin(), Analysis[label].end(), tfs[label].first.begin(), tfs[label].first.end(), inserter(killsApplied, killsApplied.begin()));
    set<string> gensApplied = killsApplied;
    gensApplied.insert(tfs[label].second.begin(), tfs[label].second.end());
    return gensApplied;
}

 map<string, set<char>> MFP::calcTF(flgnode block, map<string, set<char>> sigmaHat){
    string blockType = getBlockType(tokensAtLabels[block.getLabel()]);
    cout << block.getVal() << " : " << blockType << endl;
    if (blockType == "regread" || blockType == "arrread"){
        string varKey = tokensAtLabels[block.getLabel()][1];
        sigmaHat[varKey] = {'-','0','+'};
    }
    else if (blockType == "regass"){
        set<char> res = signHandler.evaluateSigns(block.getVal(), sigmaHat);
        string varKey = tokensAtLabels[block.getLabel()][0];
        sigmaHat[varKey] = res;
    }
    else if (blockType == "arrass"){
        set<char> res = signHandler.evaluateSigns(block.getVal(), sigmaHat);
        string varKey = tokensAtLabels[block.getLabel()][0];
        set_union(sigmaHat[varKey].begin(),sigmaHat[varKey].end(),
                  res.begin(), res.end(),inserter(sigmaHat[varKey],sigmaHat[varKey].begin()));
    }
    else if (blockType == "regdec" || blockType == "arrdec"){
        string varKey = tokensAtLabels[block.getLabel()][1];
        sigmaHat[varKey] = {'0'};
    }
    return sigmaHat;
 }
