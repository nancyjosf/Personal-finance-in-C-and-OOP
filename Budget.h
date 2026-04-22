#pragma once
#include <string>
using namespace std;

class Budget {
private:
    string category;
    double limit;

public:
    Budget() {
        category = "";
        limit = 0;
    }

    Budget(string cat, double lim) {
        category = cat;
        limit = lim;
    }

    string getCategory() const {
        return category;
    }

    double getLimit() const {
        return limit;
    }

    void setCategory(const string& cat) {
        category = cat;
    }

    void setLimit(double lim) {
        limit = lim;
    }
};
