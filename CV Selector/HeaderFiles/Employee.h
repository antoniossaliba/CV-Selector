#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <iostream>
#include <string>
#include "Selector.cpp"

using namespace std;

class Employee
{

private:
    string username;
    string password;
    Selector selector;

public:
    Employee(const string &username, const string &password);
    void addApplicant(Applicant &applicant);
    void removeApplicant(const string &email);
    void modifyApplicant(const string &email, const Applicant &updatedApplicant);
    Applicant *searchApplicant(const string &name);
    void displayApplicants();
    void saveApplicants();
    void loadApplicants();
    int getNumberOfApplicants() const;
};

#endif