#pragma once
#pragma once
#include <string>
#include <iostream>
#include "Transaction.h"
using namespace std;

class Income : public Transaction {
private:
    string source;

public:
    Income() {}

    Income(string src, double amt, Date d) : Transaction(amt, d) {
        source = src;
    }

    string getSource() {
        return source;
    }

    void setSource(string s) {
        source = s;
    }

    void display() override {
        cout << "Income Source: " << source << endl;
        cout << "Amount: " << amount << endl;
        cout << "Date: " << date.day << "/" << date.month << "/" << date.year << endl;
    }
};
