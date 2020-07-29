//
// Created by litalfel@wincs.cs.bgu.ac.il on 14/01/2020.
//

#include "../include/StompProtocol.h"
#include "../include/Header.h"
#include "../include/Frame.h"
#include "../include/connectionHandler.h"
#include <boost/algorithm/string.hpp>

using namespace std;

StompProtocol::StompProtocol(User *user, ConnectionHandler *connectionHandler) : user(user),
                                                                                 connectionHandler(connectionHandler) {
    isTerminated = false;
}

void StompProtocol::runKeyboard() {
    while (!isTerminated) {
        string keyboardInput = "";
//        if (this->user->getIsConnected()) {
        cout << "enter a command" << endl;
        getline(cin, keyboardInput);
        if (keyboardInput != "") {
            string toSend = this->KeyboardProcess(keyboardInput);
            if (toSend != "") {
                connectionHandler->sendLine(toSend);
            }
        }
//        } else {
//            break;
    }

    cout << "keyBoard terminated" << endl;
}

void StompProtocol::runSocket() {
    cout << user->getIsConnected() << endl;
    while (!isTerminated) {
        cout << "waiting for a message from the socket" << endl;
        string socketInput = "";
//        if (this->user->getIsConnected()) {
        connectionHandler->getLine(socketInput);
        cout << "received\n" + socketInput << endl;
        if (socketInput != "") {
            string toSend = this->socketProcess(socketInput);
            if (toSend != "") {
                connectionHandler->sendLine(toSend);
            }
//        } else {
//            break;
        }
    }


    cout << "socket terminated" << endl;
}


string StompProtocol::KeyboardProcess(string message) {
    if (!message.empty()) {
        vector<string> messageVector = make_vector(message);


        if (messageVector[0] == "login") {
            string commend = "CONNECT";
            Header acceptVersion = Header("accept-version", "1.2");
            int pos = messageVector[1].find(":");
            Header host = Header("host", messageVector[1].substr(0, pos));
            Header name = Header("login", messageVector[2]);
            Header pass = Header("passcode", messageVector[3]);

            vector<Header> headers;
            headers.push_back(acceptVersion);
            headers.push_back(host);
            headers.push_back(name);
            headers.push_back(pass);

            Frame frame = Frame(commend, headers, "");
            return frame.frameToString();


        } else if (messageVector[0] == "join") {
            string commend = "SUBSCRIBE";
            string topic = messageVector[1];
            Header destination = Header("destination", topic);
            int subscription = user->getAndIncrementId();
            user->addSubscription(topic, to_string(subscription)); //add the topic number to the map
            Header id = Header("id", to_string(subscription));
            Header receipt = Header("receipt", to_string(user->getAndIncrementReceipt()));
            user->addReceiptMessage(receipt.getValue(), "joined club " + messageVector[1]);
            vector<Header> headers;
            headers.push_back(destination);
            headers.push_back(id);
            headers.push_back(receipt);
            Frame frame = Frame(commend, headers, "");
            return frame.frameToString();


        } else if (messageVector[0] == "Exit") {
            string commend = "UNSUBSCRIBE";
            Header destination = Header("destination", messageVector[1]);
            Header id = Header("id", to_string(user->getAndIncrementId()));
            Header receipt = Header("receipt", to_string(user->getAndIncrementReceipt()));
            user->addReceiptMessage(receipt.getValue(),
                                    "Exited club " + messageVector[1]);//will print to screen after RECIEPT
            vector<Header> headers;
            headers.push_back(destination);
            headers.push_back(id);
            headers.push_back(receipt);
            Frame frame = Frame(commend, headers, "");
            return frame.frameToString();


        } else if (messageVector[0] == "add") {
            string commend = "SEND";
            Header destination = Header("destination", messageVector[1]);
            Header id = Header("id", to_string(user->getAndIncrementId()));
            vector<Header> headers;
            headers.push_back(destination);
            headers.push_back(id);

            string topic = messageVector[1];
            int destinationStart = message.find(topic);
            string bookName = message.substr(destinationStart + topic.size() + 1);
            cout << "book name:" + bookName << endl;

            user->removeIfOnWishList(bookName); //removes if the book is on wishlist
            user->addToMyBooks(bookName, topic); //add the book to map myBook
            Frame frame = Frame(commend, headers, user->getName() + " has added the book " + bookName);
            return frame.frameToString();


        } else if (messageVector[0] == "borrow") {
            string commend = "SEND";
            Header destination = Header("destination", messageVector[1]);
            Header id = Header("id", to_string(user->getAndIncrementId()));
            vector<Header> headers;
            headers.push_back(destination);
            headers.push_back(id);

            string topic = messageVector[1];
            int destinationStart = message.find(topic);
            string bookName = message.substr(destinationStart + topic.size() + 1);
            cout << "book name: " + bookName << endl;

            user->addToWishList(messageVector[1], bookName); //add the book to the wish list
            Frame frame = Frame(commend, headers, user->getName() + " wish to borrow " + bookName);
            return frame.frameToString();


        } else if (messageVector[0] == "return") {
            string commend = "SEND";
            Header destination = Header("destination", messageVector[1]);
            Header id = Header("id", to_string(user->getAndIncrementId()));
            vector<Header> headers;
            headers.push_back(destination);
            headers.push_back(id);


            string topic = messageVector[1];
            int destinationStart = message.find(topic);
            string bookName = message.substr(destinationStart + topic.size() + 1);
            cout << "book name: " + bookName << endl;

            string bookToReturnOwner = user->removeBookFromTaken(bookName, topic);
            Frame frame = Frame(commend, headers, "Returning " + bookName + " to " + bookToReturnOwner);
            return frame.frameToString();


        } else if (messageVector[0] == "status") {
            string commend = "SEND";
            Header destination = Header("destination", messageVector[1]);
            Header id = Header("id", to_string(user->getAndIncrementId()));
            vector<Header> headers;
            headers.push_back(destination);
            headers.push_back(id);
            Frame frame = Frame(commend, headers, "book status");


            return frame.frameToString();
        } else if (messageVector[0] == "logout") {
            string commend = "DISCONNECT";
            Header id = Header("id", to_string(user->getAndIncrementId()));
            Header receipt = Header("receipt", to_string(user->getAndIncrementReceipt()));
            user->addReceiptMessage(receipt.getValue(), messageVector[0]);//will close the socket after RECIEPT
            vector<Header> headers;
            headers.push_back(id);
            headers.push_back(receipt);
            Frame frame = Frame(commend, headers, "");
            return frame.frameToString();
        }
    } else {
        cout << "message is empty!!!!!!!!!!!!!!!!!!!!!!!!!!!1" << endl;
    }
}


string StompProtocol::socketProcess(string message) {

    cout << "message recieved" << endl;

    if (!message.empty()) {
        vector<string> messageVector;
        boost::split(messageVector, message, boost::is_any_of("\n"));

        int i = 0;
        if (messageVector[0].size() == 0) {
            i = 1;
        }

        if (messageVector[i] == "CONNECTED") {
            cout << "connection was sussceful" << endl;
        } else if (messageVector[i] == "RECEIPT") {
            if (user->printReceiptMessage(messageVector[i + 1].substr(11)) == "logout")
                isTerminated = true;
            else
                cout << user->printReceiptMessage(messageVector[i + 1].substr(11)) << endl;

        } else if (messageVector[i] == "MESSAGE") {
            string frameBody = messageVector[i + 5];
            if (!frameBody.empty()) {
                vector<string> words = make_vector(frameBody);

                if (words.size() == 2) { //status message
                    string commend = "SEND";
                    string destinationS = messageVector[i + 3].substr(12);
                    Header destination = Header("destination", destinationS);
                    Header id = Header("id", to_string(user->getAndIncrementId()));
                    vector<Header> headers;
                    headers.push_back(destination);
                    headers.push_back(id);
                    string body = user->statusAnswer(destinationS);
                    Frame frame = Frame(commend, headers, body);
                    return frame.frameToString();

                } else if (words.size()>2 && words[2] == "added") {
                    cout << "added message frame recieved, there is nothing to do" << endl;

                } else if (words.size()>1 && words[1] =="has") {// user checks if the book is in his wish list so he can take it. and he take it
                    string bookName = frameBody.substr(9);
                    if (user->onWishList(bookName)) {
                        string commend = "SEND";
                        string destinationS = messageVector[3].substr(12);
                        Header id = Header("id", to_string(user->getAndIncrementId()));
                        Header destination = Header("destination", destinationS);
                        vector<Header> headers;
                        headers.push_back(destination);
                        headers.push_back(id);
                        string body = "Taking " + bookName + " from " + words[0];
                        Frame frame = Frame(commend, headers, body);

                        user->removeIfOnWishList(bookName);
//                        string genre = user->getGenre(bookName);
                        string owner = words[0];
                        Book bookToAdd = Book(bookName, destinationS, owner);
                        user->addToTaken(bookToAdd);
                        cout << user->getName() + " added the book to his taken" << endl;

                        return frame.frameToString();


                    } else {
                        return "";
                    }

                } else if (words[0] == "Taking") { //check if the user has the book and put him on the given
                    string topic = messageVector[3].substr(12);
                    int start = messageVector[5].find("from");
                    string bookName = messageVector[5].substr(7,start-8);
                    cout << "book name: " + bookName << endl;
                    string owner = messageVector[5].substr(start + 5);
                    if (owner == user->getName()) {
                        user->removeFromInventoryToGiven(bookName, topic);//remove to given
                        cout << "remove the book to given" << endl;
                    }
                    return "";

                } else if (words.size()>1 && words[1] == "wish") {//check if the user has the book in the taken or myBook
                    string topic = messageVector[3].substr(12);
                    int start = messageVector[5].find("borrow");
                    string nameBook=messageVector[5].substr(start+7);
                    if (user->borrowAnswer(topic, nameBook)) {
                        string commend = "SEND";
                        string destinationS = messageVector[3].substr(12);
                        Header id = Header("id", to_string(user->getAndIncrementId()));
                        Header destination = Header("destination", destinationS);
                        vector<Header> headers;
                        headers.push_back(destination);
                        headers.push_back(id);
                        string body = user->getName() + " has " + nameBook;
                        Frame frame = Frame(commend, headers, body);
                        cout << user->getName() + " find the book and send SEND has frame" << endl;
                        return frame.frameToString();
                    }

                } else if (words[0] == "Returning") {
                    int start = messageVector[5].find("to");
                    string owner = messageVector[5].substr(start + 3);

                    if (user->getName() == owner) {
                        string bookName = messageVector[5].substr(10, start - 11);
                        string genre = user->getGenreGiven(bookName);
                        user->removeFromGivenToMyBooks(bookName, genre);

                    }

                    return "";
                }
            }
        }
    }
    return "";
}


vector<string> StompProtocol::make_vector(string s) {
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


//void StompProtocol::connectUser() {
//    user->setIsConnected(true);
//}
