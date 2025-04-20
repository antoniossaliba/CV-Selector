#ifndef JOB_H
#define JOB_H

#include <iostream>
#include <string>

class Job
{
public:
    virtual ~Job() {}

    virtual const std::string &getName() const = 0;

    static void displayPositions();

    static int selectPosition();
};

class IT : public Job
{
public:
    static int counter; // counts IT applications
    const std::string &getName() const override;
};

class HR : public Job
{
public:
    static int counter;
    const std::string &getName() const override;
};

class Finance : public Job
{
public:
    static int counter;
    const std::string &getName() const override;
};

class Marketing : public Job
{
public:
    static int counter;
    const std::string &getName() const override;
};

class Operations : public Job
{
public:
    static int counter;
    const std::string &getName() const override;
};

#endif
