#include "flgnode.h"
#include <iostream>

using namespace std;

flgnode::flgnode(string flgval, int flglabel){
    this->flgval = flgval;
    this->flglabel = flglabel;
}

void flgnode::setNexts(vector<flgnode*> nexts){
    this->nexts = nexts;
}

string flgnode::getVal(){
    return flgval;
}

int flgnode::getLabel(){
    return flglabel;
}

void flgnode::printSingle(){
    cout << "[" << flgval << "] with label " << flglabel << endl;
}
