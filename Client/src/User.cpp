//
// Created by litalfel@wincs.cs.bgu.ac.il on 14/01/2020.
//

#include "../include/Header.h"
//#include map
#include "../include/User.h"


using namespace std;

User::User(const string &name) : name(name), isConnected(true), myBooks(), given(), taken(), receiptMessage(),
                                 wishList(), id(), receipt(), id_subscribe() {}

int User::getAndIncrementId() {
    int tmp = id;
    id++;
    return tmp;
}

int User::getAndIncrementReceipt() {
    int tmp = receipt;
    receipt++;
    return tmp;
}


string User::getName() {
    return name;
}

void User::addToMyBooks(string nameBook, string genere) {
    Book newBook = Book(nameBook, genere, this->getName());
    if (myBooks.count(genere) == 0) {
        vector<Book> vec;
        vec.push_back(newBook);
        myBooks.insert(pair<string, vector<Book >>(genere, vec));
    } else {
        for (auto it = myBooks.begin(); it != myBooks.end(); it++) {
            if (it->first == genere) {
                it->second.push_back(newBook);
            }
        }
    }
}


// removing the book from taken map and returning the pre owner of the book
string User::removeBookFromTaken(string nameBook, string genere) {
    string owner = "";
    for (auto it = taken.begin(); it != taken.end(); it++) {
        if (it->first == genere) {
            vector<Book> vec = it->second;
            for (auto it2 = vec.begin(); it2 != vec.end(); it2++) {
                if (it2->getname() == nameBook) {
                    Book temp = *it2;
                    owner = temp.getowner();
                    vec.erase(it2);
                    it->second = vec;
                }
                break;
            }
            if (vec.size() == 0) {
                taken.erase(it);
            }
        }
    }
    return owner;
}

void User::removeIfOnWishList(string bookName) {
    for (auto it = wishList.begin(); it != wishList.end(); it++) {
        if (it->second == bookName)
            wishList.erase(it);
        break;
    }
}

void User::addToWishList(string genre, string bookName) {
    pair<string, string> toAdd(genre, bookName);
    wishList.push_back(toAdd);
}


bool User::getIsConnected() {
    return isConnected;
}

void User::addReceiptMessage(string idMessage, string toPrint) {
    receiptMessage.insert({idMessage, toPrint});
}

void User::addSubscription(string topic, string subs) {
    receiptMessage.insert({topic, subs});
}

string User::printReceiptMessage(string idMessage) {
    return receiptMessage.at(idMessage);
}


string User::statusAnswer(string topic) { //returning all the books of this genre that he has
    string ans = this->getName() + ":";

    for (auto it = taken.begin(); it != taken.end(); it++) {
        if (it->first == topic) {
            vector<Book> vec = it->second;
            for (auto it2 = vec.begin(); it2 != vec.end(); it2++)
                ans += it2->getname() + ",";
        }
    }

    for (auto it = myBooks.begin(); it != myBooks.end(); it++) {
        if (it->first == topic) {
            vector<Book> vec = it->second;
            for (auto it2 = vec.begin(); it2 != vec.end(); it2++)
                ans += it2->getname() + ",";
        }
    }

    if (ans[ans.length() - 1] == ',')
        ans.substr(0, ans.length() - 2);
    else
        ans += "has no books of this genre";

    return ans;
}

bool User::onWishList(string bookName) {
    for (auto it = wishList.begin(); it != wishList.end(); it++) {
        if (it->second == bookName)
            return true;
    }
    return false;
}


bool User::borrowAnswer(string topic, string name) { //check if the user has the book in the taken or myBook
    string ans = this->getName() + ":";

    for (auto it = taken.begin(); it != taken.end(); it++) {
        if (it->first == topic) {
            vector<Book> vec = it->second;
            for (auto book: vec) {
                if (book.getname() == name)
                    return true;
            }
        }
    }

    for (auto it = myBooks.begin(); it != myBooks.end(); it++) {
        if (it->first == topic) {
            vector<Book> vec = it->second;
            for (auto book: vec) {
                if (book.getname() == name)
                    return true;
            }
        }
    }
    return false;
}

void User::addBookToGiven(Book book) {
    if (given.count(book.gettopic()) == 0) {
        vector<Book> vec;
        vec.push_back(book);
        given.insert(pair<string, vector<Book >>(book.gettopic(), vec));
    } else {
        for (auto it = given.begin(); it != given.end(); it++) {
            if (it->first == book.gettopic()) {
                it->second.push_back(book);
            }
        }
    }
}


void User::removeFromInventoryToGiven(string nameBook, string genre) {
    for (auto it = myBooks.begin(); it != myBooks.end(); it++) {
        if (it->first == genre) {
            vector<Book> vec = it->second;
            for (auto it1 = vec.begin(); it1 != vec.end(); it1++) {
                if (it1->getname() == nameBook) {
                    Book book1 = *it1;
                    vec.erase(it1);
                    it->second = vec;
                    addBookToGiven(book1);
                    break;
                }
            }
            if (vec.size() == 0) {
                myBooks.erase(it);
            }
        }
    }
    for (auto it = taken.begin(); it != taken.end(); it++) {
        if (it->first == genre) {
            vector<Book> vec = it->second;
            for (auto it2 = vec.begin(); it2 != vec.end(); it2++) {
                if (it2->getname() == nameBook) {
                    Book book = *it2;
                    vec.erase(it2);
                    it->second = vec;
                    addBookToGiven(book);
                    break;
                }
            }
            if (vec.size() == 0) {
                taken.erase(it);
            }
        }
    }
}

string User::getGenre(string bookName) {
    for (auto it = wishList.begin(); it != wishList.end(); it++) {
        if (it->second == bookName)
            return it->first;
    }
}

string User::getGenreGiven(string bookName) {
    for (auto it = given.begin(); it != given.end(); it++) {
        vector<Book> vec = it->second;
        for (auto it2 = vec.begin(); it2 != vec.end(); it2++) {
            if (it2->getname() == bookName) {
                return it2->gettopic();
            }
        }
    }
}

void User::addToTaken(Book book) {
    if (taken.count(book.gettopic()) == 0) {
        vector<Book> vec;
        vec.push_back(book);
        taken.insert(pair<string, vector<Book >>(book.gettopic(), vec));
    } else {
        for (auto it = taken.begin(); it != taken.end(); it++) {
            if (it->first == book.gettopic()) {
                it->second.push_back(book);
            }
        }
    }
}

void User::removeFromGivenToMyBooks(string nameBook, string genre) { // TODO: check this method
    for (auto it = given.begin(); it != given.end(); it++) {
        if (it->first == genre) {
            vector<Book> vec = it->second;
            for (auto it2 = vec.begin(); it2 != vec.end(); it2++) {
                if (it2->getname() == nameBook) {
                    Book book = *it2;
                    vec.erase(it2);
                    it->second = vec; /////TODO: is this OK???????


                    if (vec.size() == 0) {
                        given.erase(it);
                    }

                    if (book.getowner() == this->name) {//add book to my books
                        if (myBooks.count(book.gettopic()) == 0) {
                            vector<Book> vec;
                            vec.push_back(book);
                            myBooks.insert(pair<string, vector<Book >>(book.gettopic(), vec));
                        } else {
                            for (auto it = myBooks.begin(); it != myBooks.end(); it++) {
                                if (it->first == book.gettopic()) {
                                    it->second.push_back(book);
                                }
                            }
                        }
                    } else {//add book to taken
                        if (taken.count(book.gettopic()) == 0) {
                            vector<Book> vec;
                            vec.push_back(book);
                            taken.insert(pair<string, vector<Book >>(book.gettopic(), vec));
                        } else {
                            for (auto it = taken.begin(); it != taken.end(); it++) {
                                if (it->first == book.gettopic()) {
                                    it->second.push_back(book);
                                }
                            }
                        }
                    }

                    break;
                }
            }
            break;
        }
    }
}



