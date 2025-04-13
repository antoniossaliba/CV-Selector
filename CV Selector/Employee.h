#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <iostream>
#include <string>
#include <sqlite3.h>
#include "Selector.h"

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
    Applicant modifyApplicant(const string &email, const Applicant &updatedApplicant);
    string searchApplicant(const string &name);
    void displayApplicants(sqlite3 *db, int exitCode, char* errorMessage);
    void acceptRejectApplicants(sqlite3 *db, int exitCode, char* errorMessage);
    void saveApplicants();
    void loadApplicants();
    int getNumberOfApplicants() const;
};

#endif