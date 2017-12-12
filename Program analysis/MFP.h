#ifndef MFP_H_INCLUDED
#define MFP_H_INCLUDED
#include "PreProcessing.h"
#include "SignHandler.h"
#include <algorithm>
#include <set>
#include <iterator>

#include <regex>

using namespace std;

class MFP{

    public:
        MFP(set<string> allElem, int numOfLabels);
        MFP(vector<pair<int,int>> flow,
                              set<int> extrLabels, set<string> extrVal,
                              set<string> bottomElems, bool subsOrSups, bool unOrInters,
                              vector<pair<set<string>,set<string>>> tfs, int numOfLabels, string worklistType);
        MFP(vector<pair<int,int>> flow,
                              set<int> extrLabels, map<string,set<char>> extrVal,
                              map<string,set<char>> bottomElems, bool subsOrSups, bool unOrInters,
                              vector<flgnode> blockList, int numOfLabels, string worklistType,
                              map<int,vector<string>> tokensAtLabels);
        void SolveEquationsBvf();
        void SolveEquations(); // SDA

    protected:

    private:
        Analysis analysis;
        //set<T> compLatt;

        vector<pair<int,int>> flow;
        set<int> extrLabels;
        set<string> extrVal;
        map<string,set<char>> extrValSDA;
        vector<pair<set<string>,set<string>>> tfs; // for bit vector frameworks
        set<string> bottomElems;
        map<string,set<char>> bottomElemsSDA;
        bool subsOrSups;
        bool unOrInters;
        int numOfLabels;

        string worklistType;
        vector<flgnode> blockList;

        vector<set<string>> enterSols; // for bit vector frameworks
        vector<set<string>> exitSols; // for bit vector frameworks

        vector<map<string, set<char>>> enterSolsSDA;
        vector<map<string, set<char>>> exitSolsSDA;

        SignHandler signHandler;
        map<int,vector<string>> tokensAtLabels; // for SDA

        void InstMonFramework(set<string> allElems);

        set<string> calcTFbvf(vector<set<string>> Analysis,int label);
        map<string, set<char>> calcTF(flgnode block, map<string, set<char>> sigmaHat); // SDA

        //string getBlockType(flgnode block);
};


#endif // MFP_H_INCLUDED
