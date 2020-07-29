//
// Created by litalfel@wincs.cs.bgu.ac.il on 14/01/2020.
//

#ifndef ASSIGNMENT3CLIENT_STOMPPROTOCOL_H
#define ASSIGNMENT3CLIENT_STOMPPROTOCOL_H

using namespace std;
#include "../include/User.h"
#include "../include/connectionHandler.h"
#include "../include/Book.h"



class StompProtocol {

    private:
    User * user;
    bool isTerminated;
    ConnectionHandler *connectionHandler;


    public:

    StompProtocol(User * user,ConnectionHandler *connectionHandler);

    void runKeyboard();

    string KeyboardProcess(string message);

    void runSocket();

    string socketProcess(string message);

    vector<string> splitIntoRows(string message);

    vector<string> make_vector(string s);

//    void connectUser();
};


#endif //ASSIGNMENT3CLIENT_STOMPPROTOCOL_H