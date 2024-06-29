#include <iostream>
#include <cstring>
#include "VoteController.h"

using namespace std;

VoteController::VoteController(int bucketSize, char* filename) : HashMap(bucketSize), Reader(filename), AlreadyVotedNum(0)
{
}

VoteController::~VoteController()
{
}

bool VoteController::SubscribeVoter(int key, Voter* value)
{
	return HashMap.Insert(key, value);
}

void VoteController::InsertregisteredVoters()
{
	Voter* voter;
	while (Reader.GetNextVoter(voter))
	{
		if (!HashMap.Insert(voter->PIN, voter))
		{
			cout << "Error! Insertion of registered voters failed. Check file format";
			exit(EXIT_FAILURE);
		}
	}
}

void VoteController::VoteFromFile(char* filename)
{
    CsvReader reader(filename);
    int pin;
    while (reader.GetNextVoterPIN(pin))
    {
        Voter* voter = HashMap.Search(pin);
        if (voter)
        {
            if (voter->Vote())
            {
                Catalogue.Insert(*voter);
                AlreadyVotedNum++;
            }
        }
        else
        {
            cout << pin << " does not exist" << endl;
        }
    }
}

void VoteController::RunTerminal(int maxCommandLength)
{
    char* cmd = new char[maxCommandLength];
    while (true)
    {
        printf("mvote> ");
        fgets(cmd, maxCommandLength, stdin);

        // Overwrite newline character
        cmd[strlen(cmd) - 1] = '\0';

        if (strncmp(cmd, "l ", 2) == 0) 
        {
            int pin;
            if (sscanf(cmd, "l %d", &pin) != 1)
            {
                cout << "Malformed Pin" << endl;
                continue;
            }
            Voter* voter = HashMap.Search(pin);
            if (voter)
            {
                voter->PrintData();
            }
            else
            {
                cout << "Participant " << pin << " not in cohort" << endl;
            }
        }
        else if (strncmp(cmd, "i ", 2) == 0) 
        {
            int pin;
            char* name = new char[25];
            char* surname = new char[25];
            int postalCode;
            if (sscanf(cmd, "i %d %s %s %d", &pin, surname, name, &postalCode) != 4)
            {
                cout << "Malformed Input" << endl;
                continue;
            }
            if (!HashMap.Search(pin))
            {
                HashMap.Insert(pin, new Voter(pin, surname, name, postalCode));
            }
            else
            {
                cout << pin << " already exist" << endl;
            }
            
            delete[] name;
            delete[] surname;
        }
        else if (strncmp(cmd, "m ", 2) == 0) 
        {
            int pin;
            if (sscanf(cmd, "m %d", &pin) != 1)
            {
                cout << "Malformed Input" << endl;
                continue;
            }
            Voter* voter = HashMap.Search(pin);
            if (voter)
            {
                if (voter->Vote())
                {
                    Catalogue.Insert(*voter);
                    AlreadyVotedNum++;
                }
            }
            else
            {
                cout << pin << " does not exist" << endl;
            }
        }
        else if (strncmp(cmd, "bv ", 3) == 0)
        {
            char* filename = new char[256];
            if (sscanf(cmd, "bv %s", filename) != 1)
            {
                cout << "Error reading command: " << cmd << endl;
                continue;
            }
            VoteFromFile(filename);

            delete[] filename;
        }
        else if (strncmp(cmd, "v", 1) == 0)
        {
            cout << "Voted So Far " << AlreadyVotedNum << endl;
        }
        else if (strncmp(cmd, "perc", 4) == 0)
        {
            double perc = HashMap.GetNumOfRegisteredVoters() > 0 ? (double)AlreadyVotedNum / HashMap.GetNumOfRegisteredVoters() : 0;
            printf("%.2f%%\n", perc * 100);
        }
        else if (strncmp(cmd, "z ", 2) == 0)
        {
            int zip;
            if (sscanf(cmd, "z %d", &zip) != 1)
            {
                cout << "Error reading command: " << cmd << endl;
                continue;
            }
            Catalogue.PrintVotersByPostalCode(zip);
        }
        else if (strncmp(cmd, "o", 1) == 0)
        {
            Catalogue.PrintAllSortedByVoterCount();
        }
        else if (strcmp(cmd, "exit") == 0) 
        {
            break;
        }
        else 
        {
            printf("Unknown Command\n");
        }
    }
    delete[] cmd;
}

void VoteController::Init()
{
	InsertregisteredVoters();
    RunTerminal();
}
