#ifndef FLGNODE_H_INCLUDED
#define FLGNODE_H_INCLUDED
#include <string>
#include <vector>
using namespace std;

class flgnode
{
    public:
        flgnode(string flgval, int flglabel);
        //void setPrevs(vector<flgnode*> prevs);
        void setNexts(vector<flgnode*> nexts);
        string getVal();
        int getLabel();
        void printSingle();
    protected:
    private:
        string flgval;
        int flglabel;
        vector<flgnode*> nexts;
};


#endif // FLGNODE_H_INCLUDED
