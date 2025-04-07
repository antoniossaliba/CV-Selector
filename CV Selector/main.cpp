#include "Applicant.h"
#include "Selector.h"
#include "Employee.h"
#include "Employee.cpp"
#include "Selector.cpp"
#include "Applicant.cpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <cstdlib>
#include <ctime>
#include <limits>
#include <iomanip>

using namespace std;

void displayMenu();
void checkUser();

int main()
{
    cout << "Welcome to the Applicant Management System!\n";
    cout << "--------------------------------------------------\n";
    checkUser();
    return 0;
}

void displayMenu()
{
    Employee employee("admin", "password");
    Selector selector;
    Applicant applicant;

    int choice;
    do
    {
        cout << "1. Add Applicant\n";
        cout << "2. Remove Applicant\n";
        cout << "3. Modify Applicant\n";
        cout << "4. Search Applicant\n";
        cout << "5. Display Applicants\n";
        cout << "6. Save Applicants\n";
        cout << "7. Exit\n";
        cout << "--------------------------------------------------\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "--------------------------------------------------\n";
        cout << endl;

        switch (choice)
        {
        case 1:
        {
            cin >> applicant;
            employee.addApplicant(applicant);
            break;
        }
        case 2:
        {
            string emailToRemove;
            cout << "Enter email of applicant to remove: ";
            cin >> emailToRemove;
            employee.removeApplicant(emailToRemove);
            break;
        }
        case 3:
        {
            string emailToModify;
            cout << "Enter email of applicant to modify: ";
            cin >> emailToModify;
            employee.modifyApplicant(emailToModify, applicant);
            break;
        }
        case 4:
        {
            string emailToSearch;
            cout << "Enter email of applicant to search: ";
            cin >> emailToSearch;
            if (employee.searchApplicant(emailToSearch) != "Applicant with email " + emailToSearch + " not found.")
                cout << "Applicant found.\n";
            else
                cout << "Applicant not found.\n";
            break;
        }
        case 5:
            employee.displayApplicants();
            break;
        case 6:
            employee.saveApplicants();
            break;
        case 7:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 7);
}

void checkUser()
{
    int userType;
    do
    {
        cout << "Select user type:\n";
        cout << "1. Employee\n";
        cout << "2. Applicant\n";
        cout << "3. Quit\n";
        cout << "Enter your choice: ";
        cin >> userType;

        if (cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            userType = -1;
        }

        switch (userType)
        {
        case 1:
        {
            string username, password;
            bool loginSuccessful = false;

            for (int attempts = 0; attempts < 3; ++attempts)
            {
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                cout << endl;
                cout << "--------------------------------------\n";
                cout << endl;

                if (username == "Antonios" && password == "Saliba") // Database check should go here
                {
                    cout << "Login successful. You are an Employee.\n";
                    cout << endl;
                    cout << "-------------------------------------\n";
                    displayMenu();
                    loginSuccessful = true;
                    break;
                }
                else
                {
                    cout << "Invalid username or password. Try again.\n";
                }
            }

            if (!loginSuccessful)
            {
                cout << "Too many failed attempts. Proceeding to the menu.\n";
            }
            break;
        }
        case 2:
        {
            Applicant applicant;
            cout << "Please enter your details:\n";
            cin >> applicant; // Collect applicant details using the overloaded istream operator
            cout << "Thank you for providing your details.\n";

            // Save the applicant's details to the file
            ofstream file("applicants.txt", ios::app); // Open file in append mode
            if (!file.is_open())
            {
                cout << "Error opening file for writing.\n";
            }
            else
            {
                file << applicant.getFirstName() << ","
                     << applicant.getLastName() << ","
                     << applicant.getMajor() << ","
                     << applicant.getUniversity() << ","
                     << applicant.getEmail() << ","
                     << applicant.getCountry() << ","
                     << applicant.getPhone() << ","
                     << applicant.getAge() << ","
                     << applicant.getYearOfGraduation() << ","
                     << applicant.getGpa() << ","
                     << applicant.getYearsOfExperience() << ","
                     << applicant.getSkillsCount() << "\n";
                file.close();
                cout << "Your details have been saved successfully.\n";
            }
            cout << endl;
            break;
        }
        case 3:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (userType != 3);
}