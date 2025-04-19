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
#include <sqlite3.h>
#include "Job.h"
#include "Job.cpp"

using namespace std;

void displayMenu(sqlite3 *db, int exitCode, char* errorMessage);
void checkUser(sqlite3 *db, int exitCode, char* errorMessage);

int main()
{
    sqlite3 *db; // Pointer to SQLite database
    char* errorMessage = nullptr;

    int exitCode = sqlite3_open("CVSelector.db", &db); // Open the database
    if (exitCode != SQLITE_OK) // Check if the database opened successfully
    {
        cerr << "Error opening database: " << sqlite3_errmsg(db) << endl;
        return exitCode;
    }
    cout << "Database opened successfully." << endl;

        // Alter the table to add the AcceptanceStatus column if it doesn't exist
    // const char* alterTableSQL = "ALTER TABLE Applicants ADD COLUMN AcceptanceStatus BOOLEAN DEFAULT NULL;";
    // exitCode = sqlite3_exec(db, alterTableSQL, nullptr, nullptr, &errorMessage);
    // if (exitCode != SQLITE_OK) {
    //     // Ignore the error if the column already exists
    //     string errorMsg = errorMessage ? errorMessage : "";
    //     if (errorMsg.find("duplicate column name") == string::npos) {
    //         cerr << "Error altering table: " << errorMessage << endl;
    //         sqlite3_free(errorMessage);
    //         sqlite3_close(db);
    //         return exitCode;
    //     }
    //     sqlite3_free(errorMessage);
    //    } else {
    //     cout << "Table altered successfully (if needed)." << endl;
    // }

    const char* createTableSQLApplicants = 
    "CREATE TABLE IF NOT EXISTS Applicants ("
    "ID INTEGER PRIMARY KEY AUTOINCREMENT, "
    "FirstName TEXT NOT NULL, "
    "LastName TEXT NOT NULL, "
    "Major TEXT NOT NULL, "
    "University TEXT NOT NULL, "
    "Email TEXT NOT NULL , "
    "Country TEXT NOT NULL, "
    "Phone TEXT NOT NULL, "
    "Age INTEGER NOT NULL, "
    "YearOfGraduation INTEGER NOT NULL, "
    "GPA REAL NOT NULL, "
    "YearsOfExperience INTEGER NOT NULL, "
    "SkillsCount INTEGER NOT NULL, "
    "AcceptanceStatus BOOLEAN DEFAULT NULL,"
    "Position TEXT NOT NULL"
    ");";

    exitCode = sqlite3_exec(db, createTableSQLApplicants, nullptr, nullptr, &errorMessage);
    if (exitCode != SQLITE_OK) {
        cerr << "Error creating table: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        sqlite3_close(db);
        return exitCode;
    } else {
        cout << "Table created successfully!" << endl;
    }

    cout << "Welcome to the Applicant Management System!\n";
    cout << "--------------------------------------------------\n";
    checkUser(db, exitCode, errorMessage);

    sqlite3_close(db); // Close the database connection
    return 0;
}

void checkUser(sqlite3 *db, int exitCode, char* errorMessage)
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
                    displayMenu(db, exitCode, errorMessage); // Call the menu function for Employee
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
                cout << "Too many failed attempts. Returning to the main menu.\n";
            }
            break;
        }
        case 2:
        {
            Applicant applicant;
            
            int position = Job::selectPosition();
            cout << "-------------------------------------\n";

            Job* job;

            switch(position){
                case 1:
                    job = new IT();
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 2:
                    job = new HR();
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 3:
                    job = new Finance();
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 4:
                    job = new Marketing();
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 5:
                    job = new Operations();
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
            }

            

            cout << "Please enter your details:\n";
            cin >> applicant; // Collect applicant details using the overloaded istream operator
            cout << "Thank you for providing your details.\n";

            // Prepare the SQL query for inserting data
            const char* insertDataSQL = 
                "INSERT INTO Applicants (FirstName, LastName, Major, University, Email, "
                "Country, Phone, Age, YearOfGraduation, GPA, YearsOfExperience, SkillsCount, "
                "Position, AcceptanceStatus) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, NULL);";

            sqlite3_stmt* stmt;
            exitCode = sqlite3_prepare_v2(db, insertDataSQL, -1, &stmt, nullptr);
            if (exitCode != SQLITE_OK) {
                cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << endl;
                return;
            }

            // Bind the values from the Applicant object to the prepared statement
            sqlite3_bind_text(stmt, 1, applicant.getFirstName().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, applicant.getLastName().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, applicant.getMajor().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 4, applicant.getUniversity().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 5, applicant.getEmail().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 6, applicant.getCountry().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 7, std::stoi(applicant.getPhone()));
            sqlite3_bind_int(stmt, 8, applicant.getAge());
            sqlite3_bind_int(stmt, 9, applicant.getYearOfGraduation());
            sqlite3_bind_double(stmt, 10, applicant.getGpa());
            sqlite3_bind_int(stmt, 11, applicant.getYearsOfExperience());
            sqlite3_bind_int(stmt, 12, applicant.getSkillsCount());
            sqlite3_bind_text(stmt, 13, job->getName().c_str(), -1, SQLITE_STATIC); // Bind the job position

            // Execute the prepared statement
            exitCode = sqlite3_step(stmt);
            if (exitCode != SQLITE_DONE) {
                cerr << "Error inserting data: " << sqlite3_errmsg(db) << endl;
            } else {
                cout << "Applicant details inserted successfully into the database!" << endl;
            }

            // Finalize the statement to release resources
            sqlite3_finalize(stmt);

            // Optional: Save the applicant's details to a file
            ofstream file("applicants.txt", ios::app); // Open file in append mode
            if (!file.is_open()) {
                cout << "Error opening file for writing.\n";
            } else {
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
                cout << "Your details have been saved successfully to the file.\n";
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

void displayMenu(sqlite3 *db, int exitCode, char* errorMessage)
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
        cout << "7. Accept/Reject Applicants\n";
        cout << "8. Exit\n";
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

            int position = Job::selectPosition();
            cout << "-------------------------------------\n";

            Job* job;

            switch(position){
                case 1:
                    job = new IT();
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 2:
                    job = new HR();
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 3:
                    job = new Finance();
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 4:
                    job = new Marketing();
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 5:
                    job = new Operations();
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
            }

            cin >> applicant;

            const char* insertDataSQL = 
                "INSERT INTO Applicants (FirstName, LastName, Major, University, Email, "
                "Country, Phone, Age, YearOfGraduation, GPA, YearsOfExperience, SkillsCount, "
                "Position, AcceptanceStatus) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, NULL);";

            sqlite3_stmt* stmt;
            exitCode = sqlite3_prepare_v2(db, insertDataSQL, -1, &stmt, nullptr);
            if (exitCode != SQLITE_OK) {
                cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << endl;
                return;
            }

            // Bind the values from the Applicant object to the prepared statement
            sqlite3_bind_text(stmt, 1, applicant.getFirstName().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, applicant.getLastName().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, applicant.getMajor().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 4, applicant.getUniversity().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 5, applicant.getEmail().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 6, applicant.getCountry().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 7, std::stoi(applicant.getPhone()));
            sqlite3_bind_int(stmt, 8, applicant.getAge());
            sqlite3_bind_int(stmt, 9, applicant.getYearOfGraduation());
            sqlite3_bind_double(stmt, 10, applicant.getGpa());
            sqlite3_bind_int(stmt, 11, applicant.getYearsOfExperience());
            sqlite3_bind_int(stmt, 12, applicant.getSkillsCount());
            sqlite3_bind_text(stmt, 13, job->getName().c_str(), -1, SQLITE_STATIC); // Bind the job position

            // Execute the prepared statement
            exitCode = sqlite3_step(stmt);
            if (exitCode != SQLITE_DONE) {
                cerr << "Error inserting data: " << sqlite3_errmsg(db) << endl;
            } else {
                cout << "Applicant details inserted successfully into the database!" << endl;
            }

            // Finalize the statement to release resources
            sqlite3_finalize(stmt);

            employee.addApplicant(applicant);
            break;
        }
        case 2:
        {
            string emailToRemove;
            cout << "Enter email of applicant to remove: ";
            cin >> emailToRemove;
            employee.removeApplicant(emailToRemove);

            const char* deleteSQL = "DELETE FROM Applicants WHERE Email = ?;";
            sqlite3_stmt* stmt;
        
            // Prepare the SQL statement
            exitCode = sqlite3_prepare_v2(db, deleteSQL, -1, &stmt, nullptr);
            if (exitCode != SQLITE_OK) {
                cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << endl;
                break;
            }
        
            // Bind the email to the prepared statement
            sqlite3_bind_text(stmt, 1, emailToRemove.c_str(), -1, SQLITE_STATIC);
        
            // Execute the prepared statement
            exitCode = sqlite3_step(stmt);
            if (exitCode == SQLITE_DONE) {
                cout << "Applicant with email " << emailToRemove << " removed successfully from the database.\n";
            } else {
                cerr << "Error removing applicant: " << sqlite3_errmsg(db) << endl;
            }
        
            // Finalize the statement to release resources
            sqlite3_finalize(stmt);

            break;
        }
        case 3:
        {
            string emailToModify;
            cout << "Enter email of applicant to modify: ";
            cin >> emailToModify;
            Applicant a = employee.modifyApplicant(emailToModify, applicant);

            cout<<"Enter the updated position [1-5]: ";
            int position = Job::selectPosition();
            Job* job;

            switch(position){
                case 1:
                    job = new IT();
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 2:
                    job = new HR();
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 3:
                    job = new Finance();
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 4:
                    job = new Marketing();
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 5:
                    job = new Operations();
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
            }

            // Prepare the SQL query for inserting data
            const string insertDataSQL = 
            "UPDATE Applicants SET "
            "FirstName = ?, LastName = ?, Major = ?, University = ?, Email = ?, "
            "Country = ?, Phone = ?, Age = ?, YearOfGraduation = ?, GPA = ?, "
            "YearsOfExperience = ?, SkillsCount = ?, Position = ?, AcceptanceStatus = NULL "
            "WHERE Email = '" + emailToModify + "';";

            sqlite3_stmt* stmt;
            exitCode = sqlite3_prepare_v2(db, insertDataSQL.c_str(), -1, &stmt, nullptr);
            if (exitCode != SQLITE_OK) {
            cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << endl;
            return;
            }

            // Bind the values from the Applicant object to the prepared statement
            sqlite3_bind_text(stmt, 1, a.getFirstName().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 2, a.getLastName().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 3, a.getMajor().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 4, a.getUniversity().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 5, a.getEmail().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, 6, a.getCountry().c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, 7, std::stoi(a.getPhone()));
            sqlite3_bind_int(stmt, 8, a.getAge());
            sqlite3_bind_int(stmt, 9, a.getYearOfGraduation());
            sqlite3_bind_double(stmt, 10, a.getGpa());
            sqlite3_bind_int(stmt, 11, a.getYearsOfExperience());
            sqlite3_bind_int(stmt, 12, a.getSkillsCount());
            sqlite3_bind_text(stmt, 13, job->getName().c_str(), -1, SQLITE_STATIC);

            // Execute the prepared statement
            exitCode = sqlite3_step(stmt);
            if (exitCode != SQLITE_DONE) {
            cerr << "Error inserting data: " << sqlite3_errmsg(db) << endl;
            } else {
            cout << "Applicant details inserted successfully into the database!" << endl;
            }

            // Finalize the statement to release resources
            sqlite3_finalize(stmt);

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
            employee.displayApplicants(db,exitCode,errorMessage);
            break;
        case 6:
            employee.saveApplicants();
            break;
        case 7:
            employee.acceptRejectApplicants(db, exitCode, errorMessage);
            break;
        case 8:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 8);
}