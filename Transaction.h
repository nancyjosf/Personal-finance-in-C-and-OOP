#pragma once
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct Date {
    int day;
    int month;
    int year;
};

inline bool isValidDate(int d, int m, int y) {
    if (y < 1900 || y > 2026) { 
        return false;
    }
    if (m < 1 || m > 12) { 
        return false;
    }
    if (d < 1 || d > 31) { 
        return false; 
    }
    if (m == 2 && d > 29) { 
        return false;
    }
    if ((m == 4 || m == 6 || m == 9 || m == 11) && d > 30) { 
        return false;
    }
    return true;
}

inline Date inputDate() {
    string dateStr;
    int d, m, y;
    char s1, s2;
    while (true) {
        cout << "Enter date (DD/MM/YYYY): ";
        cin >> dateStr;
        stringstream ss(dateStr);
        if (ss >> d >> s1 >> m >> s2 >> y && s1 == '/' && s2 == '/') {
            if (isValidDate(d, m, y)) return { d, m, y };
        }
        cout << "Invalid format! Please use DD/MM/YYYY.\n";
    }
}

class Transaction {
protected:
    double amount;
    Date date;
public:

    double getAmount() {
        return amount;
    }

    void setAmount(double amt) {
        amount = amt;
    }

    Date getDate() {
        return date;
    }

    void setDate(Date d) {
        date = d;
    }


    Transaction() {
        amount = 0.0;
        date = { 0, 0, 0 };
    }
    Transaction(double amount, Date date) {
        this->amount = amount;
        this->date = date;
    }
    virtual void display() = 0;
};
