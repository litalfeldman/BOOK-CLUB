//
// Created by litalfel@wincs.cs.bgu.ac.il on 14/01/2020.
//

#ifndef ASSIGNMENT3CLIENT_BOOK_H
#define ASSIGNMENT3CLIENT_BOOK_H

using namespace std;

#include <string>



class Book {


private:
    string name;
    string topic;
    string owner;

public:

    Book(string name, string topic, string owner);

    string getowner();

    string gettopic();

    string getname();

};
#endif