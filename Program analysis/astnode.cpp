#include "astnode.h"

using namespace std;

astnode::astnode(string val, NodeType nodeType){
    this->val = val;
    this->nodeType = nodeType;
    parent = nullptr;
    // these two are not zeros, because then initial 0 would be indistinguishable from assigned 0
    // -1 will also give error when something goes wrong
    label = -1;
    childOfNumber = -1;
}

void astnode::setParent(astnode* parent){
    this->parent = parent;
    parent->addChild(this);
}

astnode* astnode::getParent(){
    return parent;
}

void astnode::addChild(astnode* child){
    children.push_back(child);
    child->setChildOfNumber(children.size()-1);
}

void astnode::printTree(){
    cout << "Value: " << val << endl;
    cout << "Label: " << label << endl;
    cout << "Children: ";
    for (unsigned int i = 0; i<children.size(); i++){
        cout << "'" << children[i]->getVal() << "' ";
    }
    cout << endl;
    cout << endl;
    for (unsigned int i = 0; i<children.size(); i++){
       children[i]->printTree();
    }
}

string astnode::getVal(){
    return val;
}

NodeType astnode::getNodeType(){
    return nodeType;
}

vector<astnode*> astnode::getChildren(){
    return children;
}

void astnode::setLabel(int label){
    this->label = label;
}

int astnode::getLabel(){
    return label;
}

void astnode::setChildOfNumber(int childOfNumber){
    this->childOfNumber = childOfNumber;
}

int astnode::getChildOfNumber(){
    return childOfNumber;
}
