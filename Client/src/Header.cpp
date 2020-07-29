//
// Created by litalfel@wincs.cs.bgu.ac.il on 14/01/2020.
//

#include <string>
#include "../include/Header.h"

using namespace std;


Header::Header(string name, string value) : name(name), value(value){}

string Header::getValue() {
    return value;
}

string Header::getName() {
    return name;
}
