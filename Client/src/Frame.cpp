//
// Created by litalfel@wincs.cs.bgu.ac.il on 14/01/2020.
//


#include <string>
#include "../include/Frame.h"
#include <vector>
#include "../include/Header.h"

using namespace std;


Frame::Frame(string command, vector<Header> headers, string frameBody) : command(command), headers(headers),
                                                                         frameBody(frameBody) {}


vector<Header> Frame::getHeaders() {
    return headers;
}

string Frame::getCommand() {
    return command;
}

string Frame::getFrameBody() {
    return frameBody;
}

string Frame::frameToString() {
    string output = command + "\n";
    string headerLine;

    for (Header header : headers) {
        headerLine = header.getName() + ":" + header.getValue() + "\n";
        output += headerLine;
    }
    output += "\n";
    output += frameBody + "\n";

    return output;
}
