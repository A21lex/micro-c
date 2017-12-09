#include "MFP.h"
#include <iostream>
#include <sstream>
#include <string>
using namespace std;

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
// blocklist as a param instead of tfs - suited for SDA
MFP::MFP(vector<pair<int,int>> flow,
                              set<int> extrLabels, set<string> extrVal,
                              set<string> bottomElems, bool subsOrSups, bool unOrInters,
                              vector<flgnode> blockList, int numOfLabels, string worklistType){

    this->flow = flow;
    this->extrLabels = extrLabels;
    this->extrVal = extrVal;  // initial sigmaHat
    this->bottomElems = bottomElems;
    this->subsOrSups = subsOrSups;
    this->unOrInters = unOrInters;
    this->blockList = blockList;
    this->numOfLabels = numOfLabels;
    this->worklistType = worklistType;
}

// set input manually
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
    vector<pair<int,int>> worklist = flow;
    for (unsigned int i = 0; i<flow.size(); i++){
        int l1 = flow[i].first;
        int l2 = flow[i].second;
        if (extrLabels.count(l1)==1) Analysis[l1] = extrVal; //if l1 IN ext.labels, an=yo, else an=bot
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
        worklist.erase(worklist.begin()); // take 2 elems from the beginning of the worklist
        set<string> tfApplied = calcTFbvf(Analysis, l); //fl(analysis[l])
        set<string> intersResSet;
        set_intersection(tfApplied.begin(),tfApplied.end(),Analysis[lPr].begin(),Analysis[lPr].end(),inserter(intersResSet,intersResSet.begin()));
        // use the fact that: A INTERS B = A    < = >    A <= B
        //here: tfApplied(l) <= An(lpr) iff tfApplied(l) INTERS An(lpr) == tfApplied(l)
        set<string> subSet;
        set<string> superSet;
        set<string> expectResSet;
        if (subsOrSups){
            subSet = Analysis[lPr];
            superSet = tfApplied;
            expectResSet = Analysis[lPr];
        }
        else {
            subSet = tfApplied;
            superSet = Analysis[lPr];
            expectResSet = tfApplied;
        }

        //if (!(subSet.size() <= superSet.size() && tempSet == intersResSet)) {
        if (/*subSet.size() < superSet.size() ||*/ intersResSet != expectResSet){ // in essence, only 2nd cond is needed...
            set<string> tempSet;
            if (unOrInters)
                set_intersection(Analysis[lPr].begin(),Analysis[lPr].end(), tfApplied.begin(), tfApplied.end(),inserter(tempSet,tempSet.begin()));
            else
                set_union(Analysis[lPr].begin(),Analysis[lPr].end(), tfApplied.begin(), tfApplied.end(),inserter(tempSet,tempSet.begin()));
            Analysis[lPr] = tempSet;
            for (unsigned int i = 0; i<flow.size(); i++){
                //cout << flow[i].first << "....." << lPr << endl;
                //look in flow for pairs of (lPr, lDoublePr) which are not already in the worklist, add them
                if (flow[i].first == lPr && find(worklist.begin(), worklist.end(), flow[i]) == worklist.end()) {
                    if (worklistType == "FIFO") worklist.insert(worklist.begin(),flow[i]);
                    else if (worklistType == "LIFO") worklist.push_back(flow[i]);
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
   /* vector<map<string, set<char>>> Analysis;
    int numOfIterations = 0;
    Analysis.resize(numOfLabels+1);

    // Step 1
    vector<pair<int,int>> worklist = flow;
    for (unsigned int i = 0; i<flow.size(); i++){
        int l1 = flow[i].first;
        int l2 = flow[i].second;
        if (extrLabels.count(l1)==1) Analysis[l1] = extrVal;
        else Analysis[l1] = bottomElems;
        if (extrLabels.count(l2)==1) Analysis[l2] = extrVal;
        else Analysis[l2] = bottomElems;
    }

    int iters = 0;
    while (!worklist.empty()){
        numOfIterations++;
        int l = worklist[0].first;
        int lPr = worklist[0].second;
        worklist.erase(worklist.begin());
        bool changeHappened = false;

        // ---- beginning of map loop

        set<string> tfApplied = calcTFbvf(Analysis,l);
        set<string> intersResSet;
        set_intersection(tfApplied.begin(),tfApplied.end(),Analysis[lPr].begin(),Analysis[lPr].end(),inserter(intersResSet,intersResSet.begin()));

        set<string> subSet;
        set<string> superSet;
        set<string> expectResSet;
        if (subsOrSups){
            subSet = Analysis[lPr];
            superSet = tfApplied;
            expectResSet = Analysis[lPr];
        }
        else {
            subSet = tfApplied;
            superSet = Analysis[lPr];
            expectResSet = tfApplied;
        }
        //if (!(subSet.size() <= superSet.size() && tempSet == intersResSet)) {
        if (subSet.size() < superSet.size() || intersResSet != expectResSet){
            set<string> tempSet;
            if (unOrInters)
                set_intersection(Analysis[lPr].begin(),Analysis[lPr].end(), tfApplied.begin(), tfApplied.end(),inserter(tempSet,tempSet.begin()));
            else
                set_union(Analysis[lPr].begin(),Analysis[lPr].end(), tfApplied.begin(), tfApplied.end(),inserter(tempSet,tempSet.begin()));
            Analysis[lPr] = tempSet;

        }
        // ---- end of the map loop

        for (unsigned int i = 0; i<flow.size(); i++){
            //cout << flow[i].first << "....." << lPr << endl;
            if (flow[i].first == lPr && find(worklist.begin(), worklist.end(), flow[i]) == worklist.end()) {
                if (worklistType == "FIFO") worklist.insert(worklist.begin(),flow[i]);
                else if (worklistType == "LIFO") worklist.push_back(flow[i]);
            }
        }

        // debugging
        iters++;
    }
*/
}
//fl(l)=(l\kill[B]l]) U gen ([B]l)
set<string> MFP::calcTFbvf(vector<set<string>> Analysis, int label){
    set<string> killsApplied;
    set_difference( Analysis[label].begin(), Analysis[label].end(), tfs[label].first.begin(), tfs[label].first.end(), inserter(killsApplied, killsApplied.begin()));
    set<string> gensApplied = killsApplied;
    gensApplied.insert(tfs[label].second.begin(), tfs[label].second.end());
    return gensApplied;
}
// set some branching here to calculate tf depending on block type
set<string> MFP::calcTF(flgnode block, map<string, set<char>> sigmaHat){

}
