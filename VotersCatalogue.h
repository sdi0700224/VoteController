#pragma once
#include "Voter.h"

class VotersCatalogue
{
private:
    struct VoterNode
    {
        Voter* Value;
        VoterNode* Next;  // for overflow bucket
    };

    struct PCNode
    {
        int PostalCode;
        VoterNode* VotersHead;
        PCNode* Next;  // for overflow bucket
        int NumOfVoters;
    };

    PCNode* PostalCodesHead;
    static int Compare(const void* a, const void* b);

public:
    VotersCatalogue();
    ~VotersCatalogue();
    void Insert(Voter& voter);
    void PrintVotersByPostalCode(int postalCode);
    void PrintAllSortedByVoterCount();
};

