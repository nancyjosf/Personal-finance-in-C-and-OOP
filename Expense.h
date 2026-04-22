#pragma once
#pragma once
#include <string>
#include <iostream>
#include "Transaction.h"
using namespace std;

class Expense : public Transaction {
private:
    string category;

public:
    Expense() {}

    Expense(string cat, double amt, Date d) : Transaction(amt, d) {
        category = cat;
    }

    string getCategory() {
        return category;
    }

    void setCategory(string c) {
        category = c;
    }

    void display() override {
        cout << "Expense Category: " << category << endl;
        cout << "Amount: " << amount << endl;
        cout << "Date: " << date.day << "/" << date.month << "/" << date.year << endl;
    }
};
