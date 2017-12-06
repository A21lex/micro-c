#include "FlowGraphConstructor.h"
#include "MFP.h"
#include "SignHandler.h"
#include "PreProcessing.h"
#include <iostream>

using namespace std;

vector<pair<int, int>> reverseFlow (vector<pair<int, int>> flow){
    for (unsigned int i = 0; i<flow.size(); i++){
        int tempLabel = flow[i].first;
        flow[i].first = flow[i].second;
        flow[i].second = tempLabel;
    }
    reverse(flow.begin(),flow.end());
    return flow;
}

void performAnalysis(Analysis analysis, string worklistType){
    FlowGraphConstructor fgConstr;

    //vector<pair<int, int>> flow = fgConstr.prepareAST("C:\\PA_BENCHMARKS\\anytree.txt");
    vector<pair<int, int>> flow = fgConstr.prepareAST("C:\\Users\\dklevale\\Documents\\CodeBlockProjects\\anytree.txt");
    vector<flgnode> blockList = fgConstr.getBlockList();
    cout << endl;
    cout << endl;

    if (analysis == RD) {
        PreProcessing preproc (blockList, RD);
        set<string> allElems = preproc.getAllElems();
        set<string> bottomElems;
        set<int> extrLabels = fgConstr.getInit();
        set<string> extrVal = preproc.getExtremalValues();
        bool subsOrSups = false;
        bool unOrInters = false;
        preproc.calcKillsGens();
        vector<pair<set<string>,set<string>>>  killsGens = preproc.getKillsGens();
        MFP mfpobj(flow, extrLabels, extrVal, bottomElems, subsOrSups, unOrInters, killsGens, blockList.size(), worklistType);
        mfpobj.SolveEquationsBvf();

     //set<string> tempSet = preproc.getGens();
    /* vector<set<string>> something = preproc.getKills();
        for (unsigned int i = 0; i<something.size(); i++){
            set<string> tempSet = something[i];
            vector<string> v(tempSet.begin(), tempSet.end());
            for (unsigned int i = 0; i<v.size(); i++) cout << v[i] << " ";
            cout << endl;
        } */

    }
    else if (analysis == LV){

    }
    else if (analysis == VBE){

    }
    else if (analysis == AE){

    }
    else if (analysis == SDA){

    }
}

int main()
{
    // shift the stuff to corresponding function

    // AE analysis
    /*set<string> allElems;
    allElems.insert("a+b");
    allElems.insert("a*b");
    allElems.insert("a+1");
    MFP<string> mfpobj(allElems);
    mfpobj.SolveEquations();*/

    // RD analysis
   /* set<string> allElems;
    allElems.insert("x,?");
    allElems.insert("y,?");
    allElems.insert("x,1");
    allElems.insert("y,2");
    allElems.insert("x,4");
    allElems.insert("y,5");
    allElems.insert("x,6");
    MFP<string> mfpobj(allElems);
    mfpobj.SolveEquations(); */

    // LV analysis (examples 1 and 2)
    /*set<string> allElems;
    allElems.insert("x");
    allElems.insert("y");
    allElems.insert("z");
    MFP<string> mfpobj(allElems);
    mfpobj.SolveEquations();*/

    // VBE analysis
    //set<string> allElems;
    //allElems.insert("a-b");
    //allElems.insert("b-a");
    //MFP<string> mfpobj(allElems);
    //mfpobj.SolveEquations();

    map<string,set<char>> curSigns;
    curSigns["x"]={'0','-'};
    curSigns["y"]={'+'};
    curSigns["z"]={'-'};
    curSigns["a"]={'+','0','-'};
    //curSigns["B"]={'+','0','-'};
    curSigns["B"] = {'0'};


    SignHandler signHandler(&curSigns);
    //string expr = "a:=x*z/(-8)";
    string expr = "B[9+28]:=8+B[9-z[800/400]]*5";
    string lhs = expr.substr(0, expr.find(":="));
    set<char> res = signHandler.evaluateSigns(expr);
    if (lhs.find("[") < lhs.length()-1) { // does not look safe, but who cares
        lhs = lhs.substr(0, expr.find("["));
        set_union(curSigns[lhs].begin(),curSigns[lhs].end(), res.begin(), res.end(),inserter(curSigns[lhs],curSigns[lhs].begin()));
    }
    else curSigns[lhs]=res;

    //vector<char> resv(res.begin(), res.end());
    //for (unsigned i=0; i<resv.size(); i++) cout << resv[i] << endl;

    for(map<string, set<char>>::const_iterator it = curSigns.begin();
    it != curSigns.end(); ++it)
    {
        cout << it->first << ": ";//" " << it->second.first << " " << it->second.second << "\n";
        vector<char> tempVect (it->second.begin(), it->second.end());
        for (unsigned i=0; i<tempVect.size(); i++) cout << tempVect[i] << " ";
        cout << endl;
    }

    performAnalysis(RD, "FIFO");

    return 0;
}


