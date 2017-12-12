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
        PreProcessing(vector<flgnode> blocks, Analysis analysis); // also need a flow graph

        void calcTokensOfLabels();

        set<string> getFreeVariables(flgnode block); // rename to calc
        set<string> getExpressions(flgnode block); // rename to calc
        void calcAllElems(); // the whole program basically
        set<string> getAllElems();

        void calcExtremalValues();

        set<string> calcKillsForBlock (flgnode block);
        set<string> calcGensForBlock (flgnode block);
        void calcKillsGens();
        vector<pair<set<string>,set<string>>> getKillsGens();
        set<string> getExtremalValues();
        map<string,set<char>> getExtremalValuesSDA();
        map<int,vector<string>> getTokensAtLabels(); // for SDA

    protected:

    private:
        Analysis analysis;
        vector<flgnode> blocks;

        set<string> allElems;
        map<int,vector<string>> tokensAtLabels;

        set<string> extremalValues;
        map<string,set<char>> extremalValuesSDA;

        vector<pair<set<string>, set<string>>> killsGens;
};

#endif // PREPROCESSING_H_INCLUDED
