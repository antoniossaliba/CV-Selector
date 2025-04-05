#ifndef SELECTOR_H
#define SELECTOR_H

#include <iostream>
#include <vector>
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
    void modifyApplicant(const string &email, const Applicant &updatedApplicant);
    Applicant *searchApplicant(const string &email);
    void displayApplicants() const;
    void saveApplicants() const;
    void loadApplicants();
    int getNumberOfApplicants() const;
};

#endif