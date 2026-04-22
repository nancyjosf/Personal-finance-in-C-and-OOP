#pragma once
#include <string>
#include "FinanceTracker.h"
using namespace std;


using namespace std;
class User {

public:
    string name;
    FinanceTracker* tracker;

    User(string n) {
        name = n;
        tracker = new FinanceTracker();
    }

    ~User() {
        if (tracker != NULL) {
            delete tracker;
        }
    }
};

