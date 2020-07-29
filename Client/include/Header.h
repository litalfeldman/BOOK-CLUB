//
// Created by litalfel@wincs.cs.bgu.ac.il on 14/01/2020.
//

#ifndef ASSIGNMENT3CLIENT_HEADER_H
#define ASSIGNMENT3CLIENT_HEADER_H

using namespace std;
#include <string>

class Header {

private:
    string value;
    string name;
public:
    Header(string name, string value);
    string getName();
    string getValue();

};


#endif //BOOST_ECHO_CLIENT_HEADER_H
