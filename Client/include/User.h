//
// Created by litalfel@wincs.cs.bgu.ac.il on 14/01/2020.
//

#ifndef ASSIGNMENT3CLIENT_USER_H
#define ASSIGNMENT3CLIENT_USER_H

using namespace std;

#include <map>
#include <string>
#include <vector>
#include "../include/Frame.h"
#include "../include/Book.h"

#include <mutex>
#include <queue>

class User {

private:

    string name;
    bool isConnected;
    map<string, vector<Book>> myBooks;
    map<string, vector<Book>> given;
    map<string, vector<Book>> taken;
    map<string, string> receiptMessage;
    map<string, string> id_subscribe;

    //    mutex mutex;
    vector<pair<string, string>> wishList; //topic, bookName
    int id = 0;
    int receipt = 0;

public:
    User(const string &name);

    bool getIsConnected();

    int getAndIncrementId();

    int getAndIncrementReceipt();

    string getName();

    void addToMyBooks(string nameBook, string genere);

    string removeBookFromTaken(string nameBook, string genere);

    void removeFromInventoryToGiven(string nameBook, string genere);

    void addToWishList(string genre, string bookName);

    void addReceiptMessage(string idMessage, string toPrint);

    string printReceiptMessage(string idMessage);

    string statusAnswer(string topic);

    void removeIfOnWishList(string bookName);

    bool onWishList(string bookName);

    bool borrowAnswer(string topic, string name);

    void addBookToGiven(Book book);

    string getGenre(string bookName);

    void addToTaken(Book book);

    void removeFromGivenToMyBooks(string nameBook, string genre);

    void setIsConnected(bool bool1);

    void addSubscription(string topic, string subs);

    string getGenreGiven(string bookName);
};

#endif //ASSIGNMENT3CLIENT_USER_H
