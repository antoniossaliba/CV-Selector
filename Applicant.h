#ifndef APPLICANT_H
#define APPLICANT_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>
using namespace std;

class Applicant
{
private:
    string firstName;
    string lastName;
    string major;
    string university;
    string email;
    string phone;
    string country;
    int age;
    double gpa;
    int yearOfGraduation;
    int yearsOfExperience;
    int skillsCount;

public:
    Applicant();

    Applicant(const string &firstName, const string &lastName, const string &major, const string &university,
              const string &email, const string &phone, const string &country, int age, int yearOfGraduation,
              double gpa, int yearsOfExperience, int skillsCount);

    // Getters and Setters
    string getFirstName() const;
    void setFirstName(const string &firstName);

    string getLastName() const;
    void setLastName(const string &lastName);

    string getMajor() const;
    void setMajor(const string &major);

    string getUniversity() const;
    void setUniversity(const string &university);

    string getEmail() const;
    void setEmail(const string &email);

    string getPhone() const;
    void setPhone(const string &phone);

    string getCountry() const;
    void setCountry(const string &country);

    int getAge() const;
    void setAge(int age);

    double getGpa() const;
    void setGpa(double gpa);

    int getYearOfGraduation() const;
    void setYearOfGraduation(int year);

    int getYearsOfExperience() const;
    void setYearsOfExperience(int years);

    int getSkillsCount() const;
    void setSkillsCount(int count);

    friend istream &operator>>(istream &in, Applicant &applicant);
    friend ostream &operator<<(ostream &out, const Applicant &applicant);

    string serialize() const;

    bool saveApplicants(Applicant *applicant, string outputFileName);
};

#endif // APPLICANT_H