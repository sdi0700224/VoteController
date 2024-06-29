#include "CsvReader.h"
#include <cstring>
#include <iostream>

using namespace std;

CsvReader::CsvReader(const char* filename, int maxLineLenght) : File(nullptr), MaxLineLenght(maxLineLenght)
{
    File = fopen(filename, "r");
    if (!File) 
    {
        cout << filename << " could not be opened" << endl;
    }
}

CsvReader::~CsvReader() 
{
    if (File) 
    {
        fclose(File);
    }
}

bool CsvReader::GetNextVoter(Voter*& voter) 
{
    if (!File)
    {
        return false;
    }

    char* line = new char[MaxLineLenght];
    bool result = false;
    if (fgets(line, MaxLineLenght, File))
    {
        line[strcspn(line, "\n")] = 0;
        result = ParseLine(line, voter);
    }

    delete[] line;
    return result;
}

bool CsvReader::ParseLine(const char* line, Voter*& voter) 
{
    int pin;
    char* name = new char[MaxLineLenght];
    char* surname = new char[MaxLineLenght];
    int postalCode;
    int result = sscanf(line, "%d %s %s %d", &pin, surname, name, &postalCode);

    voter = new Voter(pin, surname, name, postalCode);

    delete[] name;
    delete[] surname;
    return result == 4;
}

bool CsvReader::ParseLinePIN(const char* line, int& pin)
{
    return sscanf(line, "%d", &pin) == 1;
}

bool CsvReader::GetNextVoterPIN(int& pin)
{
    if (!File)
    {
        return false;
    }

    char* line = new char[MaxLineLenght];
    bool result = false;
    if (fgets(line, MaxLineLenght, File))
    {
        line[strcspn(line, "\n")] = 0;
        result = ParseLinePIN(line, pin);
        if (!result)
        {
            cout << "Malformed Input" << endl;
        }
    }

    delete[] line;
    return result;
}