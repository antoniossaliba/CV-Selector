#ifndef SELECTOR_H
#define SELECTOR_H

#include <iostream>
#include <vector>
#include <sqlite3.h>
#include "Applicant.h"
using namespace std;

class Selector
{
private:
    vector<Applicant> applicants;
    const string filename = "applicants.txt";

public:
    Selector() {}
    void addApplicant(const Applicant &applicant);
    void removeApplicant(const string &email);
    Applicant modifyApplicant(const string &email, const Applicant &updatedApplicant);
    string searchApplicant(const string &email);
    void displayApplicants(sqlite3 *db, int exitCode, char* errorMessage) const;
    void acceptRejectApplicants(sqlite3 *db, int exitCode, char* errorMessage);
    void saveApplicants() const;
    void loadApplicants();
    int getNumberOfApplicants() const;
};

#endif