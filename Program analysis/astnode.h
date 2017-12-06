#ifndef ASTNODE_H_INCLUDED
#define ASTNODE_H_INCLUDED

#include <string>
#include <vector>
#include <iostream>

using namespace std;

enum NodeType{
    // has 2 children : condition and body
    loop,
    // has 2 or 3 children: condition and what happens if condition holds OR
    //condition, what happens if condition holds and what happens if it does not hold (else)
    branch,
    // trivial operations: assignemnts, reads, writes, single declaration, etc.
    trivial,
    // intermediate blocks (for example, second child of loop is an intermadiate block,
    // second and potentiall third child of branch are intermediate blocks
    intermediate
};

class astnode
{
    public:
        astnode(string val, NodeType nodeType);
        void setParent(astnode* parent);
        astnode* getParent();
        void addChild(astnode* child);
        void printTree();
        string getVal();
        NodeType getNodeType();
        vector<astnode*> getChildren();
        void setLabel(int label);
        int getLabel();
        void setChildOfNumber(int childOfNumber);
        int getChildOfNumber();
    protected:
    private:
        string val;
        NodeType nodeType;
        astnode* parent;
        vector<astnode*> children;
        int label;
        int childOfNumber; // cur node is this var-th child in its parent's list of children
};



#endif // ASTNODE_H_INCLUDED
