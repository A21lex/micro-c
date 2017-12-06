#ifndef PREPROCESSING_H_INCLUDED
#define PREPROCESSING_H_INCLUDED
#include "flgnode.h"
#include <set>
#include <iostream>
#include <sstream>
//#include <iterator>
#include <algorithm>
#include<map>



using namespace std;

enum Analysis{
    RD,
    LV,
    AE,
    VBE,
    SDA
};

class PreProcessing{
    public:
        //PreProcessing(Analysis analysis);
        PreProcessing(vector<flgnode> blocks, Analysis analysis); // also need a flow graph

        void calcTokensOfLabels();

        set<string> getFreeVariables(flgnode block); // rename to calc
        set<string> getExpressions(flgnode block); // rename to calc
        void calcAllElems(); // the whole program basically
        set<string> getAllElems();
        //vector<set<string>> calcKills (flgnode block);
        //vector<set<string>> calcGens (flgnode block);

       void calcExtremalValues();

        set<string> calcKillsForBlock (flgnode block);
        set<string> calcGensForBlock (flgnode block);
        void calcKillsGens();
        //vector<set<string>> getKills();
        //vector<set<string>> getGens();
        vector<pair<set<string>,set<string>>> getKillsGens();
        set<string> getExtremalValues();
        //bool isVar(string token);
        //vector<string> getTokens(string expr);


    protected:

    private:
        Analysis analysis;
        vector<flgnode> blocks;

        set<string> allElems;
        map<int,vector<string>> tokensAtLabels;

        set<string> extremalValues;

        //vector<set<string>> killsGens;
        vector<pair<set<string>, set<string>>> killsGens;
        //vector<set<string>> gens;
};

#endif // PREPROCESSING_H_INCLUDED
