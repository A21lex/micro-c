#ifndef FLOWGRAPHCONSTRUCTOR_H_INCLUDED
#define FLOWGRAPHCONSTRUCTOR_H_INCLUDED
#include "astnode.h"
#include "flgnode.h"
#include <stack>
#include <utility>
#include <fstream>
#include <memory>
#include <set>
//#include <algorithm>

using namespace std;

class FlowGraphConstructor
{
    public:
        FlowGraphConstructor();
        //astnode* prepareAST(string path); // to be written
        vector<pair<int,int>> prepareAST(string path);
        vector<flgnode> getBlockList();

        set<int> getInit();
        set<int> getFinal();
    protected:
    private:
        int labelNum;
        vector<flgnode> blockList;
        stack<astnode*> whiles;

        vector<astnode*> sStar; // children of root, whole program

        vector<pair<int,astnode*>> connectData;

        vector<pair<int,int>> constructFlowGraph(astnode* astroot);
        void calcLabels(vector<astnode*> prgseq);
        int calcInit(vector<astnode*> prgseq);
        vector<int> calcFinal(vector<astnode*> prgseq);
        vector<int> calcBreakCont(vector<astnode*> prgseq, string brOrCont);
        vector<pair<int,int>> calcFlow(vector<astnode*> prgseq);

        int calcTreeLevel(string line);
        astnode* findParent(int numOfEquals, vector<pair<int,astnode*>> connectInfo);
        pair<string, NodeType> convNames(string line); // to convert from Alex's notation to my notation

        template<typename T>
        vector<T> vectUnion(vector<T> v1, vector<T> v2){
            vector<T> resVect = v1;
            for (unsigned int j = 0; j<v2.size(); j++){
                if (!vectContains(v1,v2[j])) resVect.push_back(v2[j]);
            }
            return resVect;
        }

        template<typename T>
        bool vectContains(vector<T> v, T elem){
            for (unsigned int i = 0; i < v.size(); i++){
                if (v[i] == elem) return true;
            }
            return false;
        }
};

#endif // FLOWGRAPHCONSTRUCTOR_H_INCLUDED
