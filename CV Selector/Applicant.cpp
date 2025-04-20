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
#include "Applicant.h"

using namespace std;

Applicant::Applicant()
    : firstName(""), lastName(""), major(""), university(""), email(""), phone(""), country(""),
      age(0), gpa(0.0), yearOfGraduation(0), yearsOfExperience(0), skillsCount(0) {}

Applicant::Applicant(const string &firstName, const string &lastName, const string &major, const string &university, const string &email,
                     const string &phone, const string &country, int age, int yearOfGraduation, double gpa, int yearsOfExperience, int skillsCount)
    : firstName(firstName), lastName(lastName), major(major), university(university), email(email), phone(phone), country(country),
      age(age), gpa(gpa), yearOfGraduation(yearOfGraduation), yearsOfExperience(yearsOfExperience), skillsCount(skillsCount) {}

string Applicant::getFirstName() const { return firstName; }
void Applicant::setFirstName(const string &firstName) { this->firstName = firstName; }

string Applicant::getLastName() const { return lastName; }
void Applicant::setLastName(const string &lastName) { this->lastName = lastName; }

string Applicant::getMajor() const { return major; }
void Applicant::setMajor(const string &major) { this->major = major; }

string Applicant::getUniversity() const { return university; }
void Applicant::setUniversity(const string &university) { this->university = university; }

string Applicant::getEmail() const { return email; }
void Applicant::setEmail(const string &email) { this->email = email; }

string Applicant::getPhone() const { return phone; }
void Applicant::setPhone(const string &phone) { this->phone = phone; }

string Applicant::getCountry() const { return country; }
void Applicant::setCountry(const string &country) { this->country = country; }

int Applicant::getAge() const { return age; }
void Applicant::setAge(int age) { this->age = age; }

double Applicant::getGpa() const { return gpa; }
void Applicant::setGpa(double gpa) { this->gpa = gpa; }

int Applicant::getYearOfGraduation() const { return yearOfGraduation; }
void Applicant::setYearOfGraduation(int year) { this->yearOfGraduation = year; }

int Applicant::getYearsOfExperience() const { return yearsOfExperience; }
void Applicant::setYearsOfExperience(int years) { this->yearsOfExperience = years; }

int Applicant::getSkillsCount() const { return skillsCount; }
void Applicant::setSkillsCount(int count) { this->skillsCount = count; }

istream &operator>>(istream &in, Applicant &applicant)
{
    cout << "Enter First Name: ";
    getline(in, applicant.firstName);
    while (applicant.firstName.empty())
    {
        cout << "First name cannot be empty. Please enter again: ";
        getline(in, applicant.firstName);
    }

    cout << "Enter Last Name: ";
    getline(in, applicant.lastName);
    while (applicant.lastName.empty())
    {
        cout << "Last name cannot be empty. Please enter again: ";
        getline(in, applicant.lastName);
    }

    cout << "Enter Major: ";
    getline(in, applicant.major);
    while (applicant.major != "Computer Engineering" && applicant.major != "Electrical Engineering" &&
           applicant.major != "Mechanical Mathematics" && applicant.major != "Chemical Engineering" &&
           applicant.major != "Biomedical Engineering" && applicant.major != "Civil Engineering")
    {
        cout << "Major must be one of the Engineering branches. Please enter again: ";
        getline(in, applicant.major);
    }

    cout << "Enter University: ";
    getline(in, applicant.university);
    while (applicant.university.empty())
    {
        cout << "University cannot be empty. Please enter again: ";
        getline(in, applicant.university);
    }

    cout << "Enter Email: ";
    getline(in, applicant.email);
    while (applicant.email.empty() || (applicant.email.find('@') == string::npos || applicant.email.find('.') == string::npos))
    {
        cout << "Email must be valid. Please enter again: ";
        getline(in, applicant.email);
    }

    cout << "Enter Country (Enter one of the following: USA, Canada, UK, France, Germany): ";
    getline(in, applicant.country);
    while (applicant.country.empty() || (applicant.country != "USA" && applicant.country != "Canada" &&
                                         applicant.country != "UK" && applicant.country != "France" && applicant.country != "Germany"))
    {
        cout << "Country must be one of the following: USA, Canada, UK, France, Germany. Please enter again: ";
        getline(in, applicant.country);
    }

    cout << "Enter Phone Number: ";
    getline(in, applicant.phone);
    while (!all_of(applicant.phone.begin(), applicant.phone.end(), ::isdigit))
    {
        cout << "Phone number must contain only digits. Please enter again: ";
        getline(in, applicant.phone);
    }

    bool isValidPhone = false;
    while (!isValidPhone)
    {
        int phoneLength = applicant.phone.length();

        if (applicant.country == "USA" || applicant.country == "Canada")
        {
            if (phoneLength == 10)
                isValidPhone = true;
            else
                cout << "Phone number for " << applicant.country << " must be 10 digits. Please enter again: ";
        }
        else if (applicant.country == "UK")
        {
            if (phoneLength == 11)
                isValidPhone = true;
            else
                cout << "Phone number for UK must be 11 digits. Please enter again: ";
        }
        else if (applicant.country == "France" || applicant.country == "Germany")
        {
            if (phoneLength == 9)
                isValidPhone = true;
            else
                cout << "Phone number for " << applicant.country << " must be 9 digits. Please enter again: ";
        }
        else
        {
            cout << "Invalid country for phone number validation. Please check the country input." << endl;
            break;
        }

        if (!isValidPhone)
            getline(in, applicant.phone);
    }

    cout << "Enter Age: ";
    in >> applicant.age;
    while (applicant.age <= 18 || applicant.age >= 80)
    {
        cout << "Age must be valid. Please enter again: ";
        in >> applicant.age;
    }
    in.ignore(numeric_limits<streamsize>::max(), '\n');

    cout << "Enter Year of Graduation: ";
    in >> applicant.yearOfGraduation;
    while (applicant.yearOfGraduation - (2025 - applicant.age) <= 20 || applicant.yearOfGraduation < 0)
    {
        cout << "Year of graduation cannot be negative. Please enter again: ";
        in >> applicant.yearOfGraduation;
    }

    cout << "Enter GPA: ";
    in >> applicant.gpa;
    while (applicant.gpa < 0.0 || applicant.gpa > 4.0)
    {
        cout << "GPA must be between 0.0 and 4.0. Please enter again: ";
        in >> applicant.gpa;
    }

    cout << "Enter Years of Experience: ";
    in >> applicant.yearsOfExperience;
    while (applicant.yearsOfExperience < 0)
    {
        cout << "Years of experience cannot be negative. Please enter again: ";
        in >> applicant.yearsOfExperience;
    }

    cout << "Enter Skills Count: ";
    in >> applicant.skillsCount;
    while (applicant.skillsCount < 0)
    {
        cout << "Skills count cannot be negative. Please enter again: ";
        in >> applicant.skillsCount;
    }

    in.ignore(numeric_limits<streamsize>::max(), '\n');
    return in;
}

ostream &operator<<(ostream &out, const Applicant &applicant)
{
    out << "First Name: " << applicant.firstName << endl;
    out << "Last Name: " << applicant.lastName << endl;
    out << "Major: " << applicant.major << endl;
    out << "University: " << applicant.university << endl;
    out << "Email: " << applicant.email << endl;
    out << "Country: " << applicant.country << endl;
    out << "Phone: " << applicant.phone << endl;
    out << "Age: " << applicant.age << endl;
    out << "Year of Graduation: " << applicant.yearOfGraduation << endl;
    out << "GPA: " << applicant.gpa << endl;
    out << "Years of Experience: " << applicant.yearsOfExperience << endl;
    out << "Skills Count: " << applicant.skillsCount << endl;
    return out;
}

string Applicant::serialize() const
{
    ostringstream oss;
    oss << firstName << "," << lastName << "," << major << "," << university << "," << email << ","
        << country << "," << phone << "," << age << "," << yearOfGraduation << "," << gpa << ","
        << yearsOfExperience << "," << skillsCount;
    return oss.str();
}

bool Applicant::saveApplicants(Applicant *applicant, string outputFileName)
{
    ofstream file(outputFileName, ios::app);
    if (!file)
    {
        cerr << "Error opening file!" << endl;
        return false;
    }

    file << applicant->serialize() << endl;
    if (!file)
    {
        cerr << "Error writing to file!" << endl;
        return false;
    }

    file.close();
    return true;
}
