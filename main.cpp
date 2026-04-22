#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <sstream>

#include "User.h"
#include "FinanceTracker.h"

using namespace std;

double getValidDouble() {
    double val;
    while (!(cin >> val)) {
        cout << " !! Invalid Input. Please enter a number: ";
        cin.clear();
        cin.ignore(1000, '\n');
    }
    return val;
}

bool isPureText(string str) {
    if (str.empty()) return false;
    for (char c : str) {
        if (isdigit(c)) return false;
    }
    return true;
}


int main() {
    
    User user1("nancy");

    int choice;

    do {
        cout << "\n==================== Personal Finance Tracker Menu ====================\n";
        cout << "1. Add Income\n";
        cout << "2. Add Expense\n";
        cout << "3. Show Incomes\n";
        cout << "4. Show Expenses\n";
        cout << "5. Show Balance\n";
        cout << "6. Update Incomes\n";
        cout << "7. Update Expenses\n";
        cout << "8. Delete Incomes\n";
        cout << "9. Delete Expenses\n";
        cout << "10. Add Budget to Category\n";
        cout << "11. Financial Summary \n";
        cout << "12. Category Report \n";
        cout << "13. Year Report \n";
        cout << "14. Show All Budget\n";

        cout << "0. Exit\n";
        cout << "Enter choice: ";
        while (!(cin >> choice)) { 
            cout << " !! Invalid Choice. Enter a number (0-14): ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        if (choice == 1) {
            string src;
            double amt;
            do {
                cout << "Enter source (text only): ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, src);
                if (!isPureText(src)) cout << " !! Error: Source cannot contain numbers.\n";
            } while (!isPureText(src));

            cout << "Enter amount: ";
            amt = getValidDouble();

            Date d = inputDate();
            user1.tracker->Addincome(src, amt, d);
            cout << "Expense added successfully! \n";
        }
        else if (choice == 2) {
            string cat;
            double amt;

            do {
                cout << "Enter category (text only): ";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                getline(cin, cat);
                if (!isPureText(cat)) cout << " !! Error: Category cannot contain numbers.\n";
            } while (!isPureText(cat));

            cout << "Enter amount: ";
            amt = getValidDouble();

            Date d = inputDate();
            user1.tracker->Addexpense(cat, amt, d);
            cout << "Expense added successfully!\n";
        }
        else if (choice == 3) {
            user1.tracker->showIncomes();
        }
        else if (choice == 4) {
            user1.tracker->showExpense();
        }
        else if (choice == 5) {
            user1.tracker->showBalance();
        }
        else if (choice == 6) {
            user1.tracker->showIncomes();

            if (user1.tracker->getCountIncome() > 0) {
                int userNumber;
                cout << "Enter the number of the income you want to update: ";
                cin >> userNumber;

                user1.tracker->updateIncome(userNumber - 1);

            }
        }
        else if (choice == 7) {
            user1.tracker->showExpense();

            if (user1.tracker->getCountExpenses() > 0) {
                int userNumber;
                cout << "Enter the number of Expenses the  you want to update: ";
                cin >> userNumber;

                user1.tracker->updateExpense(userNumber - 1);

            }
        }
        else if (choice == 8) {
            user1.tracker->showIncomes();
            int userNumber;
            cout << "Enter the number of Incomes the  you want to Delete: ";
            cin >> userNumber;
            user1.tracker->deleteIncome(userNumber - 1);
        }
        else if (choice == 9) {
            user1.tracker->showExpense();
            int userNumber;
            cout << "Enter the number of Expenses the  you want to Delete: ";
            cin >> userNumber;
            user1.tracker->deleteExpense(userNumber - 1);
        }
        else if (choice == 10) {
            user1.tracker->addBudget();
        }
        else if (choice == 11) {
            user1.tracker->showSummaryReport();
        }
        else if (choice == 12) {
            user1.tracker->reportByCategory();
        }
        else if (choice == 13) {
            user1.tracker->reportByYear();
        }
        else if (choice == 14) {
            user1.tracker->showAllBudgets();
        }
        else if (choice == 0) {
            cout << "Exiting...\n";
        }
        else {
            cout << "Invalid choice!\n";
        }

    } while (choice != 0);
    return 0;
}