#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

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

void Selector::modifyApplicant(const string &email, const Applicant &updatedApplicant)
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
            break;
        }
    }

    if (!found)
    {
        cout << "Applicant with email " << email << " not found.\n";
        return;
    }

    // Write the updated vector back to the file
    ofstream file(filename, ios::trunc); // Open the file in truncate mode
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

    cout << "Applicant details updated successfully.\n";
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

void Selector::displayApplicants() const
{
    ifstream file(filename);
    if (!file.is_open())
    {
        cout << "Error: Could not open file " << filename << " for reading.\n";
        return;
    }

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
                 << "------------------------------------------\n";
        }
        else
        {
            cerr << "Warning: Skipping invalid line in file: " << line << endl;
        }
    }

    file.close(); // Close the file
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
