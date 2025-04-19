#ifndef JOB_H
#define JOB_H

#include <iostream>
#include <string>

/**
 * Base class for all job positions. Each derived class contains a static
 * counter that is incremented every time an applicant chooses that
 * particular position.
 */
class Job {
public:
    virtual ~Job() {}

    /** Return the human‑readable name of the position. */
    virtual const std::string &getName() const = 0;

    /** Show the list of available positions (1‑5). */
    static void displayPositions();

    /**
     * Ask the user for the desired position, validate the input, increment the
     * corresponding counter, and return the numeric choice (1‑5).
     */
    static int selectPosition();
};

// ──────────────────────────────────────────────────────────────
// Derived classes – one for each position
// ──────────────────────────────────────────────────────────────

class IT : public Job {
public:
    static int counter;                 // counts IT applications
    const std::string &getName() const override;
};

class HR : public Job {
public:
    static int counter;
    const std::string &getName() const override;
};

class Finance : public Job {
public:
    static int counter;
    const std::string &getName() const override;
};

class Marketing : public Job {
public:
    static int counter;
    const std::string &getName() const override;
};

class Operations : public Job {
public:
    static int counter;
    const std::string &getName() const override;
};

#endif // JOB_H
