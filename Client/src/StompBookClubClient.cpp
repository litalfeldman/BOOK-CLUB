//
// Created by litalfel@wincs.cs.bgu.ac.il on 16/01/2020.
//

#include <thread>
#include "../include/connectionHandler.h"
#include "../include/StompProtocol.h"
#include "../include/User.h"
#include <iostream>

vector<string> make_vector(string basicString);

using namespace std;
//login 127.0.0.1:6666 lital fel
// login 127.0.0.1:6666 bob alice
// login 127.0.0.1:6666 ofir ben


int main(int argc, const char *argv[]) {



    cout << "enter command:" << endl;
    string input;
    getline(cin, input);
    vector<string> messageVector = make_vector(input);
    if (messageVector.size() == 4) {


        if (messageVector[0] == "login") {
            string commend = "CONNECT";
            Header acceptVersion = Header("accept-version", "1.2");
            int pos = messageVector[1].find(":");
            Header host = Header("host", messageVector[1].substr(0, pos));
            Header name = Header("login", messageVector[2]);
            Header pass = Header("passcode", messageVector[3]);
            string host1 = messageVector[1].substr(0, pos);
            short port = stoi(messageVector[1].substr(pos + 1));
            vector<Header> headers;
            headers.push_back(acceptVersion);
            headers.push_back(host);
            headers.push_back(name);
            headers.push_back(pass);
            Frame frame = Frame(commend, headers, "");
            string toSend = frame.frameToString();

            ConnectionHandler *connectionHandler = new ConnectionHandler(host1, port);
            User *user=new User(messageVector[2]);
            StompProtocol proto = StompProtocol(user,connectionHandler);
            if (connectionHandler->connect()) {
                connectionHandler->sendLine(toSend);
                cout<<"message was sent:"+toSend<<endl;
                string response;
                if (connectionHandler->getLine(response)) {
                    cout << "server got string:\n"+ response << endl;
                    if (response.substr(0, response.find('\n')) != "ERROR") {
                        cout << "Login successful." << endl;
//                        proto.connectUser();//user is connected

                        thread thread1(&StompProtocol::runKeyboard, &proto);
                        thread thread2(&StompProtocol::runSocket, &proto);
                        thread1.join();
                        thread2.join();

                        cout << "about to close connection" << endl;
                        cout << "deleting..." << endl;
                    }
                }
            } else
                cout << "Could not connect to server" << endl;
            delete (connectionHandler);
        } else {
            cout << "wrong input" << endl;
            return 0;
        }
    } else {
        cout << "wrong input" << endl;
        return 0;
    }
}

vector<string> make_vector(string s) {
    string word;
    vector<string> v;
    for (char i : s) {
        if (i != ' ')
            word = word + i;

        else {
            v.push_back(word);
            word = "";
        }
    }
    if (!word.empty())
        v.push_back(word);
    return v;
}

