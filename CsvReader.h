#pragma once
#include "Voter.h"
#include <cstdio>

class CsvReader 
{
private:
    FILE* File;
    int MaxLineLenght;

    bool ParseLine(const char* line, Voter*& voter);
    bool ParseLinePIN(const char* line, int& voter);

public:
    CsvReader(const char* filename, int maxLineLenght = 256);
    ~CsvReader();

    bool GetNextVoter(Voter*& voter);
    bool GetNextVoterPIN(int& pin);
};

