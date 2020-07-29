//
// Created by litalfel@wincs.cs.bgu.ac.il on 14/01/2020.
//
#ifndef ASSIGNMENT3CLIENT_FRAME_H
#define ASSIGNMENT3CLIENT_FRAME_H

#include <vector>
#include "../include/Header.h"

using namespace std;


class Frame {

private:
    string command;
    vector<Header> headers;
    string frameBody;


public:
    Frame(string Command, vector<Header> headers, string frameBody);

    vector<Header> getHeaders();

    string getCommand();

    string getFrameBody();

    string frameToString();
};

#endif