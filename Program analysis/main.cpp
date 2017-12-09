#include "FlowGraphConstructor.h"
#include "MFP.h"
#include "SignHandler.h"
#include "PreProcessing.h"
#include <iostream>

using namespace std;

//REMOVE> just testing
//flgnode blockToTestStuff ("$x=5;",1);
//flgnode blockToTestStuff ("$x < 32;", 2);
//flgnode blockToTestStuff ("int $a;", 2);
//flgnode blockToTestStuff("break;", 3);
//flgnode blockToTestStuff("write $a;", 3);
//flgnode blockToTestStuff("$a[32+b]=6;",3);
//flgnode blockToTestStuff("int $a[25];",3);
flgnode blockToTestStuff("read $A[32+b];",3);
// getting reverse flow from regular one by substituting all elems by one another and putting order in reverse
vector<pair<int, int>> reverseFlow (vector<pair<int, int>> flow){
    for (unsigned int i = 0; i<flow.size(); i++){
        int tempLabel = flow[i].first;
        flow[i].first = flow[i].second;
        flow[i].second = tempLabel;
    }
    reverse(flow.begin(),flow.end());
    return flow;
}
// Analysis - RD/LV/AE/VBE/SDA, worklistType - LIFO/FIFO
void performAnalysis(Analysis analysis, string worklistType){
    FlowGraphConstructor fgConstr;

    //Get FLOW and corresponding nodes from flow graph constructor class
    //vector<pair<int, int>> flow = fgConstr.prepareAST("C:\\PA_BENCHMARKS\\anytree.txt");
    //vector<pair<int, int>> flow = fgConstr.prepareAST("C:\\Users\\dklevale\\Documents\\CodeBlockProjects\\anytree.txt");
    vector<pair<int, int>> flow = fgConstr.prepareAST("/home/aleksandrs/Documents/Studies/anytree.txt");
    vector<flgnode> blockList = fgConstr.getBlockList();
    cout << endl;
    cout << endl;
    // for now only RD is supported automatically
    if (analysis == RD) {
        PreProcessing preproc (blockList, RD); // calculates allElems -> and kills/gens for BVF
        set<string> allElems = preproc.getAllElems(); // gets allElems (extremal values and other elems, (x,?, x,l, etc)) (used in calcKillsForBlocks)
        set<string> bottomElems; // bottom elements of a complete lattice (empty set for RDA)
        set<int> extrLabels = fgConstr.getInit(); // extremal labels ( {init(S*)} for RDA)
        set<string> extrVal = preproc.getExtremalValues(); // iotas/extremal values (x,?,  in FV(S*) for RDA)
        bool subsOrSups = false; // subset is the "less than" operator for RDA
        bool unOrInters = false; // union is the least upper bound operator for RDA
        preproc.calcKillsGens();
        vector<pair<set<string>,set<string>>>  killsGens = preproc.getKillsGens(); //get kills/gens for BVF TF
        // instantiate MFP class and solve the equations for BVF
        MFP mfpobj(flow, extrLabels, extrVal, bottomElems, subsOrSups, unOrInters, killsGens, blockList.size(), worklistType);
        mfpobj.SolveEquationsBvf();

        //NEW. Testing block type detection.
        string blockType = preproc.getBlockType(blockToTestStuff);
        for (unsigned int i = 0; i < blockList.size(); i++){
            string blockTypeHaha = preproc.getBlockType(blockList[i]);
            cout << blockTypeHaha << endl;
        }

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
    // aka sigma hat
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
        // here we check what gets added to array and union its signs with what already is signs of array
        lhs = lhs.substr(0, expr.find("["));
        set_union(curSigns[lhs].begin(),curSigns[lhs].end(), res.begin(), res.end(),inserter(curSigns[lhs],curSigns[lhs].begin()));
    }
    else curSigns[lhs]=res; // else just update signs of some variable

    //vector<char> resv(res.begin(), res.end());
    //for (unsigned i=0; i<resv.size(); i++) cout << resv[i] << endl;

    // print sigma hat - variables and their calculated signs
    for(map<string, set<char>>::const_iterator it = curSigns.begin();
    it != curSigns.end(); ++it)
    {
        cout << it->first << ": ";//" " << it->second.first << " " << it->second.second << "\n";
        vector<char> tempVect (it->second.begin(), it->second.end());
        for (unsigned i=0; i<tempVect.size(); i++) cout << tempVect[i] << " ";
        cout << endl;
    }

    performAnalysis(RD, "LIFO");


    return 0;
}


