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
    cout << "Database opened successfully." << endl; // database is created

    // Creating applicants table with their informations

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

    // Creating a table for job positions counter

    const char* createTableSQLJob = 
    "CREATE TABLE IF NOT EXISTS Positions ("
    "ID INTEGER PRIMARY KEY AUTOINCREMENT,"
    "IT INTEGER DEFAULT 0,"
    "HR INTEGER DEFAULT 0,"
    "Finance INTEGER DEFAULT 0,"
    "Marketing INTEGER DEFAULT 0,"
    "Operations INTEGER DEFAULT 0"
    ");";

    exitCode = sqlite3_exec(db, createTableSQLJob, nullptr, nullptr, &errorMessage);
    if (exitCode != SQLITE_OK) {
        cerr << "Error creating table: " << errorMessage << endl;
        sqlite3_free(errorMessage);
        sqlite3_close(db);
        return exitCode;
    } else {
        cout << "Positions table created successfully!" << endl;
    }

    const char* checkPositionsSQL = "SELECT COUNT(*) FROM Positions;";
    sqlite3_stmt* stmt;
    exitCode = sqlite3_prepare_v2(db, checkPositionsSQL, -1, &stmt, nullptr);
    if (exitCode != SQLITE_OK) {
        cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return exitCode;
    }

    int count = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        count = sqlite3_column_int(stmt, 0);
    }
    sqlite3_finalize(stmt);

    if (count == 0) {
        const char* insertDefaultPositionsSQL = 
        "INSERT INTO Positions (IT, HR, Finance, Marketing, Operations) VALUES (0, 0, 0, 0, 0);";
        exitCode = sqlite3_exec(db, insertDefaultPositionsSQL, nullptr, nullptr, &errorMessage);
        if (exitCode != SQLITE_OK) {
            cerr << "Error initializing Positions table: " << errorMessage << endl;
            sqlite3_free(errorMessage);
            sqlite3_close(db);
            return exitCode;
        } else {
            cout << "Positions table initialized successfully!" << endl;
        }
    }

    const char* getCounterSQL = 
    "SELECT IT, HR, Finance, Marketing, Operations FROM Positions;";

    exitCode = sqlite3_prepare_v2(db, getCounterSQL, -1, &stmt, nullptr);
    if (exitCode != SQLITE_OK) {
        cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << endl;
        return 0;
    }

    int itCount = 0, hrCount = 0, financeCount = 0, marketingCount = 0, operationsCount = 0;
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        itCount = sqlite3_column_int(stmt, 0);
        hrCount = sqlite3_column_int(stmt, 1);
        financeCount = sqlite3_column_int(stmt, 2);
        marketingCount = sqlite3_column_int(stmt, 3);
        operationsCount = sqlite3_column_int(stmt, 4);
    }

    IT::counter = itCount;
    HR::counter = hrCount;
    Finance::counter = financeCount;
    Marketing::counter = marketingCount;
    Operations::counter = operationsCount;

    sqlite3_finalize(stmt);

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


            const char* updatePositionSQL;
            


            Job* job;

            switch(position){
                case 1:
                    job = new IT();
                    updatePositionSQL = "UPDATE Positions SET IT = IT + 1 WHERE ID = 1;";
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 2:
                    job = new HR();
                    updatePositionSQL = "UPDATE Positions SET HR = HR + 1 WHERE ID = 1;";
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 3:
                    job = new Finance();
                    updatePositionSQL = "UPDATE Positions SET Finance = Finance + 1 WHERE ID = 1;";
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 4:
                    job = new Marketing();
                    updatePositionSQL = "UPDATE Positions SET Marketing = Marketing + 1 WHERE ID = 1;";
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 5:
                    job = new Operations();
                    updatePositionSQL = "UPDATE Positions SET Operations = Operations + 1 WHERE ID = 1;";
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
            }

            exitCode = sqlite3_exec(db, updatePositionSQL, nullptr, nullptr, &errorMessage);
            if (exitCode != SQLITE_OK) {
                cerr << "Error updating Positions table: " << errorMessage << endl;
                sqlite3_free(errorMessage);
            } else {
                cout << "Position counter updated successfully!" << endl;
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
            sqlite3_bind_text(stmt, 7, applicant.getPhone().c_str(), -1, SQLITE_STATIC);
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

    cout<< "Welcome to the Employee Menu!\n";
    cout << "--------------------------------------------------\n";

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
        cout << "8. Show Positions\n";
        cout << "9. Exit\n";
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
            const char* updatePositionSQL;
            Job* job;

            switch(position){
                case 1:
                    job = new IT();
                    updatePositionSQL = "UPDATE Positions SET IT = IT + 1 WHERE ID = 1;";
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 2:
                    job = new HR();
                    updatePositionSQL = "UPDATE Positions SET HR = HR + 1 WHERE ID = 1;";
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 3:
                    job = new Finance();
                    updatePositionSQL = "UPDATE Positions SET Finance = Finance + 1 WHERE ID = 1;"; 
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 4:
                    job = new Marketing();
                    updatePositionSQL = "UPDATE Positions SET Marketing = Marketing + 1 WHERE ID = 1;";
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 5:
                    job = new Operations();
                    updatePositionSQL = "UPDATE Positions SET Operations = Operations + 1 WHERE ID = 1;";
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
            }
            exitCode = sqlite3_exec(db, updatePositionSQL, nullptr, nullptr, &errorMessage);
            if (exitCode != SQLITE_OK) {
                cerr << "Error updating Positions table: " << errorMessage << endl;
                sqlite3_free(errorMessage);
            } else {
                cout << "Position counter updated successfully!" << endl;
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

            // getting the position of the applicant to remove before removing

            const char* selectPositionSQL = "SELECT Position FROM Applicants WHERE Email = ?;";
            sqlite3_stmt* stmt;
            exitCode = sqlite3_prepare_v2(db, selectPositionSQL, -1, &stmt, nullptr);
            if (exitCode != SQLITE_OK) {
                cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << endl;
                break;
            }

            sqlite3_bind_text(stmt, 1, emailToRemove.c_str(), -1, SQLITE_STATIC);

            string position;
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                position = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            } else {
                cerr << "Error: Applicant with email " << emailToRemove << " not found in the database.\n";
                sqlite3_finalize(stmt);
                break;
            }
            sqlite3_finalize(stmt);

            // removing the applicant from the table Applicants

            const char* deleteAppSQL = "DELETE FROM Applicants WHERE Email = ?;";
            
            // Prepare the SQL statement
            exitCode = sqlite3_prepare_v2(db, deleteAppSQL, -1, &stmt, nullptr);
            if (exitCode != SQLITE_OK) {
                cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << endl;
                break;
            }
        
            // Bind the email to the prepared statement
            sqlite3_bind_text(stmt, 1, emailToRemove.c_str(), -1, SQLITE_STATIC);
        
            // Execute the prepared statement
            exitCode = sqlite3_step(stmt);
            if (exitCode == SQLITE_DONE) {
                cout << "\n";
            } else {
                cerr << "Error removing applicant: " << sqlite3_errmsg(db) << endl;
            }
        
            // Finalize the statement to release resources
            sqlite3_finalize(stmt);

            const char* updatePositionSQL;
            if (position == "IT") {
                updatePositionSQL = "UPDATE Positions SET IT = IT - 1 WHERE ID = 1;";
                IT::counter--;
            } else if (position == "HR") {
                updatePositionSQL = "UPDATE Positions SET HR = HR - 1 WHERE ID = 1;";
                HR::counter--;
            } else if (position == "Finance") {
                updatePositionSQL = "UPDATE Positions SET Finance = Finance - 1 WHERE ID = 1;";
                Finance::counter--;
            } else if (position == "Marketing") {
                updatePositionSQL = "UPDATE Positions SET Marketing = Marketing - 1 WHERE ID = 1;";
                Marketing::counter--; 
            } else if (position == "Operations") {
                updatePositionSQL = "UPDATE Positions SET Operations = Operations - 1 WHERE ID = 1;";
                Operations::counter--;
            }

            exitCode = sqlite3_exec(db, updatePositionSQL, nullptr, nullptr, &errorMessage);
            if (exitCode != SQLITE_OK) {
                cerr << "Error updating Positions table: " << errorMessage << endl;
                sqlite3_free(errorMessage);
            }

            cout << "Applicant with email "<< emailToRemove << "removed successfully!\n";


            break;
        }
        case 3:
        {
            string emailToModify;
            cout << "Enter email of applicant to modify: ";
            cin >> emailToModify;

            // getting the old position before change
            const char* selectPositionSQL = "SELECT Position FROM Applicants WHERE Email = ?;";
            sqlite3_stmt* stmt;
            exitCode = sqlite3_prepare_v2(db, selectPositionSQL, -1, &stmt, nullptr);
            if (exitCode != SQLITE_OK) {
                cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << endl;
                break;
            }

            sqlite3_bind_text(stmt, 1, emailToModify.c_str(), -1, SQLITE_STATIC);

            string Oldposition;
            if (sqlite3_step(stmt) == SQLITE_ROW) {
                Oldposition = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            } else {
                cerr << "Error: Applicant with email " << emailToModify << " not found in the database.\n";
                sqlite3_finalize(stmt);
                break;
            }
            sqlite3_finalize(stmt);

            // updating position counter in the classes and in the database
            const char* updatePositionSQL;
            if(Oldposition == "IT"){
                updatePositionSQL = "UPDATE Positions SET IT = IT - 1 WHERE ID = 1;";
                IT::counter--;
            }else if(Oldposition == "HR"){
                updatePositionSQL = "UPDATE Positions SET HR = HR - 1 WHERE ID = 1;";
                HR::counter--;
            }else if(Oldposition == "Finance"){
                updatePositionSQL = "UPDATE Positions SET Finance = Finance - 1 WHERE ID = 1;";
                Finance::counter--;
            }else if(Oldposition == "Marketing"){
                updatePositionSQL = "UPDATE Positions SET Marketing = Marketing - 1 WHERE ID = 1;";
                Marketing::counter--;
            }else if(Oldposition == "Operations"){
                updatePositionSQL = "UPDATE Positions SET Operations = Operations - 1 WHERE ID = 1;";
                Operations::counter--;
            }
            exitCode = sqlite3_exec(db, updatePositionSQL, nullptr, nullptr, &errorMessage);
            if (exitCode != SQLITE_OK) {
                cerr << "Error updating Positions table: " << errorMessage << endl;
                sqlite3_free(errorMessage);
            }


            // modifying the applicant
            Applicant a = employee.modifyApplicant(emailToModify, applicant);

            // updating the new position with its counter
            cout<<"Enter the updated position [1-5]: ";
            int position = Job::selectPosition();
            Job* job;

            const char* updateNewPositionSQL;
            switch(position){
                case 1:
                    job = new IT();
                    updateNewPositionSQL = "UPDATE Positions SET IT = IT + 1 WHERE ID = 1;";
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 2:
                    job = new HR();
                    updateNewPositionSQL = "UPDATE Positions SET HR = HR + 1 WHERE ID = 1;";
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 3:
                    job = new Finance();
                    updateNewPositionSQL = "UPDATE Positions SET Finance = Finance + 1 WHERE ID = 1;";
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 4:
                    job = new Marketing();
                    updateNewPositionSQL = "UPDATE Positions SET Marketing = Marketing + 1 WHERE ID = 1;";
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
                case 5:
                    job = new Operations();
                    updateNewPositionSQL = "UPDATE Positions SET Operations = Operations + 1 WHERE ID = 1;";
                    cout << "You have selected " << job->getName() << ".\n";
                    break;
            }
            exitCode = sqlite3_exec(db, updateNewPositionSQL, nullptr, nullptr, &errorMessage);
            if (exitCode != SQLITE_OK) {
                cerr << "Error updating Positions table: " << errorMessage << endl;
                sqlite3_free(errorMessage);
            }

            // Updating all the other info of the applicant and Prepare the SQL query for inserting data
            const string insertDataSQL = 
            "UPDATE Applicants SET "
            "FirstName = ?, LastName = ?, Major = ?, University = ?, Email = ?, "
            "Country = ?, Phone = ?, Age = ?, YearOfGraduation = ?, GPA = ?, "
            "YearsOfExperience = ?, SkillsCount = ?, Position = ?, AcceptanceStatus = NULL "
            "WHERE Email = '" + emailToModify + "';";

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
            cout << "Positions:\n";
            cout << "IT: " << IT::counter << "\n";
            cout << "HR: " << HR::counter << "\n";
            cout << "Finance: " << Finance::counter << "\n";
            cout << "Marketing: " << Marketing::counter << "\n";
            cout << "Operations: " << Operations::counter << "\n";
            cout << "Total Applicants: " << IT::counter + HR::counter + Finance::counter + Marketing::counter + Operations::counter << "\n";
            cout << "--------------------------------------------------\n \n";
            break;
        case 9:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 9);
}