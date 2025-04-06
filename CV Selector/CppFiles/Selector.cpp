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
    cout << "Applicant added successfully.\n";
}

void Selector::removeApplicant(const string &email)
{
    for (auto it = applicants.begin(); it != applicants.end(); ++it)
    {
        if (it->getEmail() == email)
        {
            applicants.erase(it);
            cout << "Applicant removed successfully.\n";
            return;
        }
    }
    cout << "Applicant with email " << email << " not found.\n";
}

void Selector::modifyApplicant(const string &email, const Applicant &updatedApplicant)
{
    for (auto &applicant : applicants)
    {
        if (applicant.getEmail() == email)
        {
            applicant = updatedApplicant;
            cout << "Applicant details updated successfully.\n";
            return;
        }
    }
    cout << "Applicant with email " << email << " not found.\n";
}

Applicant *Selector::searchApplicant(const string &email)
{
    for (auto &applicant : applicants)
    {
        if (applicant.getEmail() == email)
        {
            return &applicant;
        }
    }
    return nullptr;
}

void Selector::displayApplicants() const
{
    if (applicants.empty())
    {
        std::cout << "No applicants found.\n";
        return;
    }

    for (const auto &applicant : applicants)
    {
        std::cout << "Name: " << applicant.getFirstName() << " " << applicant.getLastName() << "\n"
                  << "Major: " << applicant.getMajor() << "\n"
                  << "University: " << applicant.getUniversity() << "\n"
                  << "Email: " << applicant.getEmail() << "\n"
                  << "Phone: " << applicant.getPhone() << "\n"
                  << "Country: " << applicant.getCountry() << "\n"
                  << "Age: " << applicant.getAge() << "\n"
                  << "GPA: " << applicant.getGpa() << "\n"
                  << "Year of Graduation: " << applicant.getYearOfGraduation() << "\n"
                  << "Years of Experience: " << applicant.getYearsOfExperience() << "\n"
                  << "Skills Count: " << applicant.getSkillsCount() << "\n"
                  << "------------------------------------------\n";
    }
}

void Selector::saveApplicants() const
{
    ofstream file(filename);
    if (!file.is_open())
    {
        cout << "Error opening file for writing.\n";
        return;
    }

    for (const auto &applicant : applicants)
    {
        file << applicant.getFirstName() << ","
             << applicant.getLastName() << ","
             << applicant.getEmail() << ","
             << applicant.getPhone() << ","
             << "\n";
    }

    file.close();
    cout << "Applicants saved successfully.\n";
}

int Selector::getNumberOfApplicants() const
{
    if(applicants.empty()) {
        
        cout << "There is no applicants to load!";
        return 0;
        
    } else {

        cout << "Number of applicants successfully loaded!";
        
    }

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
