#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

#include "Transaction.h"
#include "Income.h"
#include "Expense.h"
#include "Budget.h"

using namespace std;

class FinanceTracker {
private:
    double balance;

    Income incomes[1001];
    int countIncome;

    Expense expenses[1001];
    int countExpenses;

    Budget* budgets;
    int budgetCount;
    int budgetCapacity;

    
public:

    double getBalance() {
        return balance;
    }

    int getCountIncome() {
        return countIncome;
    }

    int getCountExpenses() {
        return countExpenses;
    }

    FinanceTracker() {
        balance = 0;
        countIncome = 0;
        countExpenses = 0;

        budgetCapacity = 100;
        budgetCount = 0;
        budgets = new Budget[budgetCapacity];
        loadFromFile();
    }
    //////////
    bool saveToFile() {
        ofstream outputFile;
        outputFile.open("Fainance.txt");

        if (!outputFile) {
            cout << "Error: Could not open file for saving!\n";
            return 0;
        }
        //save Income
        outputFile << countIncome << "\n";
        for (int i = 0; i < countIncome; i++) {
            outputFile << incomes[i].getSource() << "|" << incomes[i].getAmount() << "|"
                << incomes[i].getDate().day << "|" << incomes[i].getDate().month << "|" << incomes[i].getDate().year << "\n";
        }

        //save Expenses
        outputFile << countExpenses << "\n";
        for (int i = 0; i < countExpenses; i++) {
            outputFile << expenses[i].getCategory() << "|" << expenses[i].getAmount() << "|"
                << expenses[i].getDate().day << "|" << expenses[i].getDate().month << "|" << expenses[i].getDate().year << "\n";
        }

        //save Budget
        outputFile << budgetCount << "\n";
        for (int i = 0; i < budgetCount; i++) {
            outputFile << budgets[i].getCategory() << "|" << budgets[i].getLimit() << "\n";
        }
        outputFile.close();
        return 1;
    }

    bool loadFromFile() {
        ifstream inputFile;
        inputFile.open("Fainance.txt");
        if (!inputFile) {
            cout << "Error: Could not open file for reading!\n";
            return false;
        }

        string line;
        balance = 0;
        countIncome = 0;
        countExpenses = 0;
        budgetCount = 0;

        // --- 1. Load Income ---
        if (getline(inputFile, line) && !line.empty()) {
            int loadedIncomeCount = stoi(line);
            for (int i = 0; i < loadedIncomeCount && i < 1001; i++) {
                if (getline(inputFile, line) && !line.empty()) {
                    stringstream ss(line);
                    string src, amt, d, m, y;
                    if (getline(ss, src, '|') && getline(ss, amt, '|') &&
                        getline(ss, d, '|') && getline(ss, m, '|') && getline(ss, y, '|')) {

                        Date tempDate = { stoi(d), stoi(m), stoi(y) };
                        incomes[countIncome] = Income(src, stod(amt), tempDate);
                        balance += stod(amt);
                        countIncome++;
                    }
                }
            }
        }

        // --- 2. Load Expenses ---
        if (getline(inputFile, line) && !line.empty()) {
            int loadedExpenseCount = stoi(line);
            for (int i = 0; i < loadedExpenseCount && i < 1001; i++) {
                if (getline(inputFile, line) && !line.empty()) {
                    stringstream ss(line);
                    string cat, amt, d, m, y;
                    if (getline(ss, cat, '|') && getline(ss, amt, '|') &&
                        getline(ss, d, '|') && getline(ss, m, '|') && getline(ss, y, '|')) {

                        Date tempDate = { stoi(d), stoi(m), stoi(y) };
                        expenses[countExpenses] = Expense(cat, stod(amt), tempDate);
                        balance -= stod(amt);
                        countExpenses++;
                    }
                }
            }
        }

        // --- 3. Load Budget ---
        if (getline(inputFile, line) && !line.empty()) {
            int loadedBudgetCount = stoi(line);
            for (int i = 0; i < loadedBudgetCount; i++) {
                if (getline(inputFile, line) && !line.empty()) {
                    stringstream ss(line);
                    string cat, lim;
                    if (getline(ss, cat, '|') && getline(ss, lim, '|')) {
                        if (budgetCount == budgetCapacity) {
                            resizeBudgets();
                        }
                        budgets[budgetCount] = Budget(cat, stod(lim));
                        budgetCount++;
                    }
                }
            }
        }

        inputFile.close();
        return true;
    }
    /////////
    void resizeBudgets() {
        budgetCapacity *= 2;

        Budget* newArr = new Budget[budgetCapacity];

        for (int i = 0; i < budgetCount; i++) {
            newArr[i] = budgets[i];//copy array to new array
        }

        delete[] budgets;
        budgets = newArr;
    }

    void checkBudget(string categ, double newAmt) {
        int haveBudget = -1;

        for (int i = 0; i < budgetCount; i++) {
            if (budgets[i].getCategory() == categ) {
                haveBudget = i;
                break;
            }
        }
        if (haveBudget == -1) {
            return;
        }
        double totalExpen = 0;
        for (int j = 0; j < countExpenses; j++) {
            if (expenses[j].getCategory() == categ) {
                totalExpen += expenses[j].getAmount();
            }
        }

        double totalNewExpen = totalExpen + newAmt;//totel after you add expenses

        if (totalNewExpen >= budgets[haveBudget].getLimit()) {
            cout << "\nNOTIFICATION: Budget EXCEEDED for (" << categ << ")!";
            cout << "\nYour Limet to this Categary is: " << budgets[haveBudget].getLimit() << "\n Amd You Spend : " << totalNewExpen << "\n";
        }
        else if (totalNewExpen >= budgets[haveBudget].getLimit() * 0.9) {
            cout << "\nALERT: You are CLOSE to your budget for (" << categ << ")-->(90% used).\n";
            cout << "\nYour Limet to this Categary is: " << budgets[haveBudget].getLimit() << " \n And You Spend : " << totalNewExpen << "\n";
        }
    }

    bool validateIncome(string src, double amt, Date d) {
        if (src.empty()) {
            cout << "Source cannot be empty ! =(\n";
            return false;
        }

        if (amt <= 0) {
            cout << "Amount must be greater than 0! =(\n";
            return false;
        }

        if (!isValidDate(d.day, d.month, d.year)) {
            cout << "Invalid date! =(\n";
            return false;
        }

        return true;
    }

    bool validateExpense(string categ, double amt, Date d) {
        if (categ.empty()) {
            cout << "Category cannot be empty!  =(\n";
            return false;
        }
        if (amt <= 0) {
            cout << "Amount must be greater than 0! =(\n";
            return false;
        }
        if (!isValidDate(d.day, d.month, d.year)) {
            cout << "Invalid date! =(\n";
            return false;
        }
        return true;
    }

    void Addincome(string src, double amt, Date d) {

        if (countIncome >= 1000) {
            cout << "Cannot add more than 1000 income entries! =(\n";
            return;
        }

        if (!validateIncome(src, amt, d)) {
            return;
        }

        incomes[countIncome] = Income(src, amt, d);
        countIncome++;
        balance += amt;
        saveToFile();
        cout << "Income Add Successfully =)\n";
    }

    void Addexpense(string categ, double amt, Date d) {
        if (countExpenses >= 1001) {
            cout << "Cannot add more than 1001 Expense entries! =(\n";
            return;
        }

        if (validateExpense(categ, amt, d)) {
            checkBudget(categ, amt);

            expenses[countExpenses] = Expense(categ, amt, d);
            countExpenses++;
            balance -= amt;
            saveToFile();
            cout << "Expense added successfully! =)\n";
        }
    }

    void updateIncome(int index) {
        if (index < 0 || index >= countIncome) {
            cout << "Invalid index!" << endl;
            return;
        }

        int choice;
        cout << "\nWhat would you like to update for this income?\n";
        cout << "1. Source\n";
        cout << "2. Amount\n";
        cout << "3. Date\n";
        cout << "4. All details\n";
        cout << "Enter choice: ";
        while (!(cin >> choice)) {
            cout << " !! Invalid choice. Enter a number (1-4): ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        string currentSrc = incomes[index].getSource();
        double currentAmt = incomes[index].getAmount();
        Date currentDate = incomes[index].getDate();

        if (choice == 1) {
            string newSrc;
            cout << "Enter new Source: ";
            cin.ignore(1000, '\n');
            getline(cin, newSrc);

            if (validateIncome(newSrc, currentAmt, currentDate)) {
                incomes[index].setSource(newSrc);
            }
            else return;
        }
        else if (choice == 2) {
            double newAmt;
            cout << "Enter new Amount: ";
            while (!(cin >> newAmt)) {
                cout << " !! Invalid Amount. Enter a numeric value: ";
                cin.clear();
                cin.ignore(1000, '\n');
            }

            if (validateIncome(currentSrc, newAmt, currentDate)) {
                balance = balance - currentAmt + newAmt;
                incomes[index].setAmount(newAmt);
            }
            else return;
        }
        else if (choice == 3) {
            Date newDate = inputDate();

            if (validateIncome(currentSrc, currentAmt, newDate)) {
                incomes[index].setDate(newDate);
            }
            else return;
        }
        else if (choice == 4) {
            string newSrc;
            double newAmt;
            cout << "Enter new Source: ";
            cin.ignore(1000, '\n');
            getline(cin, newSrc);
            cout << "Enter new Amount: ";
            cin >> newAmt;
            cin.ignore();
            Date newDate = inputDate();

            if (validateIncome(newSrc, newAmt, newDate)) {
                balance = balance - currentAmt + newAmt;
                incomes[index].setAmount(newAmt);
                incomes[index].setSource(newSrc);
                incomes[index].setDate(newDate);
            }
            else return;
        }

        cout << "Update successful! =)\n";
        saveToFile();
    }

    void updateExpense(int index) {
        if (index < 0 || index >= countExpenses) {
            cout << "Invalid index! =(" << endl;
            return;
        }

        int choice;
        cout << "\nWhat would you like to update for this Expense?\n";
        cout << "1. Category\n";
        cout << "2. Amount\n";
        cout << "3. Date\n";
        cout << "4. All details\n";
        cout << "Enter choice: ";
        cin >> choice;

        string currentCat = expenses[index].getCategory();
        double currentAmt = expenses[index].getAmount();
        Date currentDate = expenses[index].getDate();

        if (choice == 1) {
            string newCat;
            cout << "Enter new Category: ";
            cin.ignore(1000, '\n');
            getline(cin, newCat);

            if (validateExpense(newCat, currentAmt, currentDate)) {
                expenses[index].setCategory(newCat);
            }
            else return;
        }
        else if (choice == 2) {
            double newAmt;
            cout << "Enter new Amount: ";
            cin >> newAmt;

            if (validateExpense(currentCat, newAmt, currentDate)) {
                checkBudget(currentCat, newAmt);
                balance = balance + currentAmt - newAmt;
                expenses[index].setAmount(newAmt);
            }
            else return;
        }
        else if (choice == 3) {
            Date newDate = inputDate();
            if (validateExpense(currentCat, currentAmt, newDate)) {
                expenses[index].setDate(newDate);
            }
            else return;
        }
        else if (choice == 4) {
            string newCat;
            double newAmt;
            cout << "Enter new Category: ";
            cin.ignore(1000, '\n');
            getline(cin, newCat);
            cout << "Enter new Amount: ";
            cin >> newAmt;
            cin.ignore();
            Date newDate = inputDate();

            if (validateExpense(newCat, newAmt, newDate)) {
                checkBudget(newCat, newAmt);
                balance = balance + currentAmt - newAmt;
                expenses[index].setAmount(newAmt);
                expenses[index].setCategory(newCat);
                expenses[index].setDate(newDate);
            }
            else return;
        }

        cout << "Update successful!\n";
        saveToFile();
    }

    void showIncomes() {
        cout << "\n==================== INCOME RECORDS ====================\n";
        if (countIncome == 0) {
            cout << "            ( No income records found )\n";
        }
        else {
            for (int i = 0; i < countIncome; i++) {
                cout << " " << i + 1 << ". ";
                incomes[i].display(); 
                cout << "------------------------------------------------------\n";
            }
        }
        cout << "========================================================\n";
    }

    void showExpense() {
        cout << "\n==================== EXPENSE RECORDS ===================\n";
        if (countExpenses == 0) {
            cout << "            ( No expense records found )\n";
        }
        else {
            for (int i = 0; i < countExpenses; i++) {
                cout << " " << i + 1 << ". ";
                expenses[i].display();
                cout << "------------------------------------------------------\n";
            }
        }
        cout << "========================================================\n";
    }

    void showBalance() {
        cout << "\n========== Current Balance ==========\n";
        cout << "Balance: " << balance << endl;
        cout << "=====================================\n";
    }

    void deleteIncome(int index) {
        if (index < 0 || index >= countIncome) {
            cout << "Invalid income index!=(" << endl;
            return;
        }
        if (countIncome == 0) {
            cout << "No incomes to delete.\n";
            return;
        }
        else {
            balance -= incomes[index].getAmount();

            for (int i = index; i < countIncome - 1; i++) {
                incomes[i] = incomes[i + 1];
            }

            countIncome--;

            cout << "Income deleted successfully!=)\n";
        }
        saveToFile();
    }

    void deleteExpense(int index) {
        if (index < 0 || index >= countExpenses) {
            cout << "Invalid Expense index!=(" << endl;
            return;
        }
        if (countExpenses == 0) {
            cout << "No Expense to delete.\n";
            return;
        }
        else {
            balance += expenses[index].getAmount();

            for (int i = index; i < countExpenses - 1; i++) {
                expenses[i] = expenses[i + 1];
            }

            countExpenses--;

            cout << "Expense deleted successfully!=)\n";
        }
        saveToFile();
    }

    string showCategories() {
        if (countExpenses == 0) {
            cout << "\n========================================================\n";
            cout << "            ( No expenses recorded yet! )\n";
            cout << "========================================================\n";
            return "";
        }

        string uniqueCats[1001];
        int uniqueCount = 0;

        cout << "\n================== AVAILABLE CATEGORIES =================\n";
        for (int i = 0; i < countExpenses; i++) {
            bool exists = false;
            for (int j = 0; j < uniqueCount; j++) {
                if (expenses[i].getCategory() == uniqueCats[j]) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                uniqueCats[uniqueCount] = expenses[i].getCategory();
                uniqueCount++;
                cout << " (" << uniqueCount << ") " << uniqueCats[uniqueCount - 1] << endl;
            }
        }
        cout << "--------------------------------------------------------\n";

        int choice;
        cout << " Select category number: ";

        while (!(cin >> choice) || choice < 1 || choice > uniqueCount) {
            cout << " Invalid! Enter a number between 1 and " << uniqueCount << ": ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        cin.ignore(1000, '\n');
        return uniqueCats[choice - 1];
    }

    void addBudget() {
        string selectCategory = showCategories();
        if (selectCategory == "") {
            return;
        }

        for (int i = 0; i < budgetCount; i++) {
            if (budgets[i].getCategory() == selectCategory) {
                cout << "Budget already exists for " << selectCategory << "\n";
                char c;

                while (true) {
                    cout << "Do you want to update? (Y/N): ";
                    cin >> c;

                    if (c == 'Y' || c == 'y') {
                        double newLimit;
                        cout << "Enter the new Budget Limit: ";
                        while (!(cin >> newLimit) || newLimit <= 0) {
                            cout << "Invalid input! Please enter a positive number: ";
                            cin.clear();
                            cin.ignore(1000, '\n');
                        }
                        budgets[i].setLimit(newLimit);
                        cout << "Budget updated successfully! \n";
                        saveToFile();
                        return;
                    }
                    else if (c == 'N' || c == 'n') {
                        cout << "Update cancelled. \n";
                        return;
                    }
                    else {
                        cout << "Invalid choice! Please enter Y or N.\n";
                        cin.clear();
                        cin.ignore(1000, '\n');
                    }
                }
            }
        }

        double limit;
        cout << "Enter budget limit for (" << selectCategory << "): ";

        while (!(cin >> limit) || limit <= 0) {
            cout << "Invalid input! Please enter a positive number: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }

        if (budgetCount == budgetCapacity) {
            resizeBudgets();
        }

        budgets[budgetCount] = Budget(selectCategory, limit);
        budgetCount++;

        cout << "Budget set successfully for (" << selectCategory << ") \n";
        saveToFile();
    }

    void showAllBudgets() {
        cout << "\n==================== ALL CATEGORY BUDGETS ====================\n";

        if (budgetCount == 0) {
            cout << "            ( No budgets have been set yet. )\n";
            cout << "==============================================================\n";
            return;
        }

        for (int i = 0; i < budgetCount; i++) {
            double totalSpent = 0;
            for (int j = 0; j < countExpenses; j++) {
                if (expenses[j].getCategory() == budgets[i].getCategory()) {
                    totalSpent += expenses[j].getAmount();
                }
            }

            cout << " " << i + 1 << ". Category: " << budgets[i].getCategory() << endl;
            cout << "    Limit: " << budgets[i].getLimit() << " USD" << endl;
            cout << "    Spent: " << totalSpent << " USD" << endl;

            cout << "    Status: ";
            if (totalSpent > budgets[i].getLimit()) {
                cout << "!! EXCEEDED !!";
            }
            else if (totalSpent >= budgets[i].getLimit() * 0.9) {
                cout << "! CRITICAL (90%)";
            }
            else {
                cout << "OK";
            }

            cout << "\n--------------------------------------------------------------\n";
        }
        cout << "==============================================================\n";
    }
   
    void showSummaryReport() {
        double totalIncome = 0;
        double totalExpense = 0;

        for (int i = 0; i < countIncome; i++) {
            totalIncome += incomes[i].getAmount();
        }

        for (int i = 0; i < countExpenses; i++) {
            totalExpense += expenses[i].getAmount();
        }

        cout << "\n==================== FINANCIAL SUMMARY REPORT ====================\n";
        cout << " Total Income   : " << totalIncome << " USD\n";
        cout << " Total Expenses : " << totalExpense << " USD\n";
        cout << "------------------------------------------------------------------\n";
        cout << " Net Balance    : " << balance << " USD\n";
        cout << "==================================================================\n";
    }

    void reportByCategory() {
        string selectedCat = showCategories();
        if (selectedCat == "") return;

        double totalSpent = 0;
        cout << "\n==================== DETAILED REPORT: (" << selectedCat << ") ====================\n";

        for (int i = 0; i < countExpenses; i++) {
            if (expenses[i].getCategory() == selectedCat) {
                cout << " - ";
                expenses[i].display();
                totalSpent += expenses[i].getAmount();
                cout << "------------------------------------------------------------------\n";
            }
        }

        cout << " Total Spent in [" << selectedCat << "]: " << totalSpent << " USD\n";

        for (int i = 0; i < budgetCount; i++) {
            if (budgets[i].getCategory() == selectedCat) {
                double limit = budgets[i].getLimit();
                cout << " Budget Limit: " << limit << " USD\n";

                if (totalSpent > limit)
                    cout << " Status: ! OVER BUDGET ! (Exceeded by " << (totalSpent - limit) << ")\n";
                else
                    cout << " Status: OK (Remaining: " << (limit - totalSpent) << ")\n";
                break;
            }
        }
        cout << "==================================================================\n";
    }

    void reportByYear() {
        if (countIncome == 0 && countExpenses == 0) {
            cout << "\n==================================================================\n";
            cout << "            ( No records found in the system yet. )\n";
            cout << "==================================================================\n";
            return;
        }

        int year;
        cout << "\n Enter the year you want to report: ";
        cin >> year;

        double totalIncome = 0;
        double totalExpense = 0;
        bool foundAny = false;

        cout << "\n==================== FINANCIAL REPORT FOR YEAR [" << year << "] ====================\n";

        cout << "\n --- [ INCOME RECORDS ] ---\n";
        for (int i = 0; i < countIncome; i++) {
            if (incomes[i].getDate().year == year) {
                cout << " "; incomes[i].display();
                totalIncome += incomes[i].getAmount();
                foundAny = true;
            }
        }
        if (totalIncome == 0) cout << " ( No income recorded for this year )\n";

        cout << "\n --- [ EXPENSE RECORDS ] ---\n";
        for (int i = 0; i < countExpenses; i++) {
            if (expenses[i].getDate().year == year) {
                cout << " "; expenses[i].display();
                totalExpense += expenses[i].getAmount();
                foundAny = true;
            }
        }
        if (totalExpense == 0) cout << " ( No expenses recorded for this year )\n";

        if (!foundAny) {
            cout << "\n ALERT: No data found at all for the year " << year << ".\n";
        }
        else {
            cout << "\n----------------------- YEARLY SUMMARY (" << year << ") -----------------------\n";
            cout << " Total Income   : " << totalIncome << " USD\n";
            cout << " Total Expenses : " << totalExpense << " USD\n";
            cout << " Net Savings    : " << (totalIncome - totalExpense) << " USD\n";
        }
        cout << "==================================================================\n";
    }

    ~FinanceTracker() {
        delete[] budgets;
    }

};

