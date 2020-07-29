//
// Created by litalfel@wincs.cs.bgu.ac.il on 14/01/2020.
//

#include "../include/Book.h"
#include "../include/User.h"

using namespace std;



Book::Book(string name, string topic, string owner) : name(name), topic(topic),
                                                     owner(owner) {}


string Book::getowner() {
    return owner;
}

string Book::getname() {
    return name;
}

string Book::gettopic() {
    return topic;
}

