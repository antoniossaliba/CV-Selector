#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <sqlite3.h>
#include "Selector.h"
#include "Applicant.h"
#include "Employee.h"

using namespace std;

void Selector::addApplicant(const Applicant &applicant)
{
    applicants.push_back(applicant);
    saveApplicants();
    cout << "Applicant added successfully.\n";
}

void Selector::removeApplicant(const string &email)
{

    loadApplicants();

    bool found = false;

    for (auto it = applicants.begin(); it != applicants.end(); ++it)
    {
        if (it->getEmail() == email)
        {
            applicants.erase(it);
            found = true;
            break;
        }
    }

    if (!found)
    {
        cout << "Applicant with email " << email << " not found.\n";
        return;
    }

    ofstream file(filename, ios::trunc);
    if (!file.is_open())
    {
        cout << "Error: Could not open file " << filename << " for writing.\n";
        return;
    }

    for (const auto &applicant : applicants)
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
    }

    file.close();

    cout << "Applicant with email " << email << " removed successfully.\n";
}

Applicant Selector::modifyApplicant(const string &email, const Applicant &updatedApplicant)
{
    // Load the latest data from the file into the vector
    loadApplicants();

    bool found = false;

    // Check if the email exists in the in-memory list of applicants
    for (auto &applicant : applicants)
    {
        if (applicant.getEmail() == email)
        {
            found = true;

            // Use the istream operator to get updated details immediately
            cout << "Enter updated details for the applicant:\n";
            cin >> applicant; // Calls the overloaded istream operator of the Applicant class
            return applicant;
            break;
        }
    }

    if (!found)
    {
        cout << "Applicant with email " << email << " not found.\n";
        return updatedApplicant;
    }

    // Write the updated vector back to the file
    ofstream file(filename, ios::trunc); // Open the file in truncate mode
    if (!file.is_open())
    {
        cout << "Error: Could not open file " << filename << " for writing.\n";
        return updatedApplicant;
    }

    for (const auto &applicant : applicants)
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
    }

    file.close();

    cout << "Applicant details updated successfully.\n";

    return updatedApplicant; // Return the updated applicant object
}

string Selector::searchApplicant(const string &email)
{

    loadApplicants();

    for (const auto &applicant : applicants)
    {
        if (applicant.getEmail() == email)
        {

            ostringstream details;
            details << "Name: " << applicant.getFirstName() << " " << applicant.getLastName() << "\n"
                    << "Major: " << applicant.getMajor() << "\n"
                    << "University: " << applicant.getUniversity() << "\n"
                    << "Email: " << applicant.getEmail() << "\n"
                    << "Phone: " << applicant.getPhone() << "\n"
                    << "Country: " << applicant.getCountry() << "\n"
                    << "Age: " << applicant.getAge() << "\n"
                    << "GPA: " << applicant.getGpa() << "\n"
                    << "Year of Graduation: " << applicant.getYearOfGraduation() << "\n"
                    << "Years of Experience: " << applicant.getYearsOfExperience() << "\n"
                    << "Skills Count: " << applicant.getSkillsCount() << "\n";
            return details.str();
        }
    }

    return "Applicant with email " + email + " not found.";
}

void Selector::displayApplicants(sqlite3 *db, int exitCode, char* errorMessage) const
{
const char* selectSQL = "SELECT FirstName, LastName, Major, University, Email, Country, Phone, "
                            "Age, YearOfGraduation, GPA, YearsOfExperience, SkillsCount, Position, AcceptanceStatus "
                            "FROM Applicants;";

    sqlite3_stmt* stmt;
    exitCode = sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr);
    if (exitCode != SQLITE_OK) {
        cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    cout << "Applicants Information:\n";
    cout << "------------------------------------------\n";

    // Execute the query and iterate through the results
    while ((exitCode = sqlite3_step(stmt)) == SQLITE_ROW) {
        string firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        string lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string major = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        string university = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        string email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        string country = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        string phone = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        string position = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));
        int age = sqlite3_column_int(stmt, 7);
        int yearOfGraduation = sqlite3_column_int(stmt, 8);
        double gpa = sqlite3_column_double(stmt, 9);
        int yearsOfExperience = sqlite3_column_int(stmt, 10);
        int skillsCount = sqlite3_column_int(stmt, 11);
        const unsigned char* acceptanceStatusRaw = sqlite3_column_text(stmt, 13);
        string acceptanceStatus = acceptanceStatusRaw ? reinterpret_cast<const char*>(acceptanceStatusRaw) : "Pending";
        
        cout << "Name: " << firstName << " " << lastName << "\n"
             << "Major: " << major << "\n"
             << "University: " << university << "\n"
             << "Email: " << email << "\n"
             << "Phone: " << phone << "\n"
             << "Country: " << country << "\n"
             << "Age: " << age << "\n"
             << "GPA: " << gpa << "\n"
             << "Year of Graduation: " << yearOfGraduation << "\n"
             << "Years of Experience: " << yearsOfExperience << "\n"
             << "Skills Count: " << skillsCount << "\n"
             << "Position: " << position << "\n"
             << "Acceptance Status: " 
             << (acceptanceStatus == "1" ? "Accepted" : (acceptanceStatus == "0" ? "Rejected" : "Pending")) << "\n"
             << "------------------------------------------\n";
    }

    if (exitCode != SQLITE_DONE) {
        cerr << "Error retrieving data: " << sqlite3_errmsg(db) << endl;
    }

    // Finalize the statement to release resources
    sqlite3_finalize(stmt);

    // ifstream file(filename);
    // if (!file.is_open())
    // {
    //     cout << "Error: Could not open file " << filename << " for reading.\n";
    //     return;
    // }

    // string line;
    // while (getline(file, line))
    // {
    //     istringstream iss(line);
    //     string firstName, lastName, major, university, email, phone, country;
    //     int age, yearOfGraduation, yearsOfExperience, skillsCount;
    //     double gpa;

    //     if (getline(iss, firstName, ',') &&
    //         getline(iss, lastName, ',') &&
    //         getline(iss, major, ',') &&
    //         getline(iss, university, ',') &&
    //         getline(iss, email, ',') &&
    //         getline(iss, country, ',') &&
    //         getline(iss, phone, ',') &&
    //         iss >> age &&
    //         iss.ignore(1, ',') &&
    //         iss >> yearOfGraduation &&
    //         iss.ignore(1, ',') &&
    //         iss >> gpa &&
    //         iss.ignore(1, ',') &&
    //         iss >> yearsOfExperience &&
    //         iss.ignore(1, ',') &&
    //         iss >> skillsCount)
    //     {

    //         cout << "Name: " << firstName << " " << lastName << "\n"
    //              << "Major: " << major << "\n"
    //              << "University: " << university << "\n"
    //              << "Email: " << email << "\n"
    //              << "Phone: " << phone << "\n"
    //              << "Country: " << country << "\n"
    //              << "Age: " << age << "\n"
    //              << "GPA: " << gpa << "\n"
    //              << "Year of Graduation: " << yearOfGraduation << "\n"
    //              << "Years of Experience: " << yearsOfExperience << "\n"
    //              << "Skills Count: " << skillsCount << "\n"
    //              << "------------------------------------------\n";
    //     }
    //     else
    //     {
    //         cerr << "Warning: Skipping invalid line in file: " << line << endl;
    //     }
    // }

    // file.close(); // Close the file
}

void Selector::saveApplicants() const
{
    ofstream file(filename, ios::app);
    if (!file.is_open())
    {
        cout << "Error opening file for writing.\n";
        return;
    }

    if (!applicants.empty())
    {
        Applicant applicant = applicants.back();
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
             << applicant.getSkillsCount() << ","
             << "\n";
    }
    else
    {
        cout << "There are no applicants to save!\n";
    }

    file.close();
}

int Selector::getNumberOfApplicants() const
{
    return applicants.size();
}

void Selector::loadApplicants()
{
    ifstream file(filename);
    if (!file)
    {
        cerr << "Error: Could not open file " << filename << " for reading." << endl;
        return;
    }

    applicants.clear();

    string line;
    while (getline(file, line))
    {
        istringstream iss(line);
        string firstName, lastName, major, university, email, phone, country;
        int age, yearOfGraduation, yearsOfExperience, skillsCount;
        double gpa;

        if (getline(iss, firstName, ',') &&
            getline(iss, lastName, ',') &&
            getline(iss, major, ',') &&
            getline(iss, university, ',') &&
            getline(iss, email, ',') &&
            getline(iss, country, ',') &&
            getline(iss, phone, ',') &&
            iss >> age &&
            iss.ignore(1, ',') &&
            iss >> yearOfGraduation &&
            iss.ignore(1, ',') &&
            iss >> gpa &&
            iss.ignore(1, ',') &&
            iss >> yearsOfExperience &&
            iss.ignore(1, ',') &&
            iss >> skillsCount)
        {

            applicants.emplace_back(firstName, lastName, major, university, email, phone, country,
                                    age, yearOfGraduation, gpa, yearsOfExperience, skillsCount);
        }
        else
        {
            cerr << "Warning: Skipping invalid line in file: " << line << endl;
        }
    }

    file.close();
}

void Selector::acceptRejectApplicants(sqlite3* db, int exitCode, char* errorMessage){
    const char* selectSQL = "SELECT FirstName, LastName, Major, University, Email, Country, Phone, "
                            "Age, YearOfGraduation, GPA, YearsOfExperience, SkillsCount, Position, AcceptanceStatus "
                            "FROM Applicants;";

    sqlite3_stmt* stmt;
    exitCode = sqlite3_prepare_v2(db, selectSQL, -1, &stmt, nullptr);
    if (exitCode != SQLITE_OK) {
        cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    cout << "Applicants Information:\n";
    cout << "------------------------------------------\n";

    // Execute the query and iterate through the results
    while ((exitCode = sqlite3_step(stmt)) == SQLITE_ROW) {
        string firstName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        string lastName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string major = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        string university = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        string email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        string country = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 5));
        string phone = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));
        string position = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 12));
        int age = sqlite3_column_int(stmt, 7);
        int yearOfGraduation = sqlite3_column_int(stmt, 8);
        double gpa = sqlite3_column_double(stmt, 9);
        int yearsOfExperience = sqlite3_column_int(stmt, 10);
        int skillsCount = sqlite3_column_int(stmt, 11);
        const unsigned char* acceptanceStatusRaw = sqlite3_column_text(stmt, 13);
        string acceptanceStatus = acceptanceStatusRaw ? reinterpret_cast<const char*>(acceptanceStatusRaw) : "Pending";
        
        cout << "Name: " << firstName << " " << lastName << "\n"
             << "Major: " << major << "\n"
             << "University: " << university << "\n"
             << "Email: " << email << "\n"
             << "Phone: " << phone << "\n"
             << "Country: " << country << "\n"
             << "Age: " << age << "\n"
             << "GPA: " << gpa << "\n"
             << "Year of Graduation: " << yearOfGraduation << "\n"
             << "Years of Experience: " << yearsOfExperience << "\n"
             << "Skills Count: " << skillsCount << "\n"
             << "Position: " << position << "\n"
             << "Acceptance Status: " 
             << (acceptanceStatus == "1" ? "Accepted" : (acceptanceStatus == "0" ? "Rejected" : "Pending")) << "\n"
             << "------------------------------------------\n";

        string change = "";

        cout<< "Do you want to change the acceptance status of "<< firstName << " "<< lastName<<" ? [y/n] ";
        cin >> change;
        while(change != "y" && change != "n"){
            cout<< "Please enter a valid input [y/n] ";
            cin>> change;
        }
        if(change == "y"){
            cout<< "Please enter the acceptance status [Accepted/Rejected] ";
            string status;
            cin>> status;
            string updateSQL = "UPDATE Applicants SET AcceptanceStatus = ? WHERE Email = '"+ email +"';";
            sqlite3_stmt* updateStmt;
            exitCode = sqlite3_prepare_v2(db, updateSQL.c_str(), -1, &updateStmt, nullptr);
            if (exitCode != SQLITE_OK) {
                cerr << "Error preparing SQL statement: " << sqlite3_errmsg(db) << endl;
                continue;
            }
            sqlite3_bind_int(updateStmt, 1, (status == "Accepted" ? 1 : 0));
            exitCode = sqlite3_step(updateStmt);
            if (exitCode != SQLITE_DONE) {
                cerr << "Error updating data: " << sqlite3_errmsg(db) << endl;
            } else {
                cout << "Acceptance status updated successfully for " << firstName << " " << lastName << ".\n";
            }
            sqlite3_finalize(updateStmt);
        }
        else if(change == "n"){
            cout<< "No changes made to the acceptance status of "<< firstName << " "<< lastName<<".\n";
        }
    }

    if (exitCode != SQLITE_DONE) {
        cerr << "Error retrieving data: " << sqlite3_errmsg(db) << endl;
    }

    // Finalize the statement to release resources
    sqlite3_finalize(stmt);

}
