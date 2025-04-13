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
#include <sqlite3.h>
#include "Applicant.h"
#include "Selector.h"
#include "Employee.h"
using namespace std;

Employee::Employee(const string &username, const string &password)
{
    this->username = username;
    this->password = password;
}

void Employee::addApplicant(Applicant &applicant)
{
    selector.addApplicant(applicant);
}

void Employee::removeApplicant(const string &email)
{
    selector.removeApplicant(email); // Fixed: Use email instead of name
}

Applicant Employee::modifyApplicant(const string &email, const Applicant &updatedApplicant)
{
    Applicant a = selector.modifyApplicant(email, updatedApplicant); // Fixed: Use email instead of name
    return a;
}

string Employee::searchApplicant(const string &name)
{
    return selector.searchApplicant(name);
}

void Employee::displayApplicants(sqlite3 *db, int exitCode, char* errorMessage)
{
    selector.displayApplicants(db,exitCode,errorMessage);
}

void Employee::acceptRejectApplicants(sqlite3 *db, int exitCode, char* errorMessage)
{
    selector.acceptRejectApplicants(db, exitCode, errorMessage);
}

void Employee::saveApplicants()
{
    selector.saveApplicants();
}

void Employee::loadApplicants()
{
    selector.loadApplicants();
}

int Employee::getNumberOfApplicants() const
{
    return selector.getNumberOfApplicants();
}