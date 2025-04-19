#include "Job.h"
#include <limits>

using namespace std;

/* ====== Static counters ====== */
int IT::counter         = 0;
int HR::counter         = 0;
int Finance::counter    = 0;
int Marketing::counter  = 0;
int Operations::counter = 0;

/* ====== Derived‑class getName() ====== */
const string &IT::getName()         const { static const string n = "IT";         return n; }
const string &HR::getName()         const { static const string n = "HR";         return n; }
const string &Finance::getName()    const { static const string n = "Finance";    return n; }
const string &Marketing::getName()  const { static const string n = "Marketing";  return n; }
const string &Operations::getName() const { static const string n = "Operations"; return n; }

/* ====== Job utility functions ====== */
void Job::displayPositions() {
    cout << "Choose a position to apply for:\n"
         << "1. IT\n"
         << "2. HR\n"
         << "3. Finance\n"
         << "4. Marketing\n"
         << "5. Operations\n";
}

int Job::selectPosition() {
    int choice = 0;
    do {
        displayPositions();
        cout << "Enter your choice (1 - 5): ";
        if (!(cin >> choice) || choice < 1 || choice > 5) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please try again.\n";
            choice = 0;
        }
    } while (choice == 0);

    switch (choice) {
        case 1: ++IT::counter;         break;
        case 2: ++HR::counter;         break;
        case 3: ++Finance::counter;    break;
        case 4: ++Marketing::counter;  break;
        case 5: ++Operations::counter; break;
    }
    return choice;
}
