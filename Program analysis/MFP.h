#ifndef MFP_H_INCLUDED
#define MFP_H_INCLUDED
#include "PreProcessing.h"
#include <algorithm>
#include <set>
#include <iterator>

using namespace std;

class MFP{

    public:
        MFP(set<string> allElem, int numOfLabels);
        MFP(vector<pair<int,int>> flow,
                              set<int> extrLabels, set<string> extrVal,
                              set<string> bottomElems, bool subsOrSups, bool unOrInters,
                              vector<pair<set<string>,set<string>>> tfs, int numOfLabels, string worklistType);
        MFP(vector<pair<int,int>> flow,
                              set<int> extrLabels, set<string> extrVal,
                              set<string> bottomElems, bool subsOrSups, bool unOrInters,
                              vector<flgnode> blockList, int numOfLabels, string worklistType);
        void SolveEquationsBvf();
        void SolveEquations(); // SDA
        static string getBlockType(flgnode block); // NEW: get block type for tf for sda

    protected:

    private:
        Analysis analysis;
        //set<T> compLatt;

        vector<pair<int,int>> flow;
        set<int> extrLabels;
        set<string> extrVal;
        vector<pair<set<string>,set<string>>> tfs; // for bit vector frameworks
        set<string> bottomElems;
        bool subsOrSups;
        bool unOrInters;
        int numOfLabels;

        string worklistType;
        vector<flgnode> blockList;

        vector<set<string>> enterSols; // for bit vector frameworks
        vector<set<string>> exitSols; // for bit vector frameworks

        void InstMonFramework(set<string> allElems);

        set<string> calcTFbvf(vector<set<string>> Analysis,int label);
        set<string> calcTF(flgnode block, map<string, set<char>> sigmaHat); // SDA
};


#endif // MFP_H_INCLUDED
