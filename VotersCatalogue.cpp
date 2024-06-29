#include "VotersCatalogue.h"
#include <iostream>
#include <cstdlib>

using namespace std;

int VotersCatalogue::Compare(const void* a, const void* b)
{
    PCNode* pcA = *(PCNode**)a;
    PCNode* pcB = *(PCNode**)b;

    return pcB->NumOfVoters - pcA->NumOfVoters;  // For descending order
}

VotersCatalogue::VotersCatalogue()
{
	PostalCodesHead = nullptr;
}

VotersCatalogue::~VotersCatalogue()
{
    PCNode* currentPostalCode = PostalCodesHead;

    while (currentPostalCode != nullptr)
    {
        VoterNode* currentVoter = currentPostalCode->VotersHead;

        // Delete all VoterNode for the current postal code node
        while (currentVoter != nullptr)
        {
            VoterNode* tempVoter = currentVoter;
            currentVoter = currentVoter->Next;
            delete tempVoter;  // Delete the VoterNode
        }

        // Move on to the next postal code node and delete the current one
        PCNode* tempPostalCode = currentPostalCode;
        currentPostalCode = currentPostalCode->Next;
        delete tempPostalCode;
    }
}

void VotersCatalogue::Insert(Voter& voter)
{
    PCNode* currentPostalCode = PostalCodesHead;
    PCNode* previousPostalCode = nullptr;

    // Find the position of the postal code or the position where it should be added
    while (currentPostalCode != nullptr && currentPostalCode->PostalCode < voter.PostalCode) 
    {
        previousPostalCode = currentPostalCode;
        currentPostalCode = currentPostalCode->Next;
    }

    // Postal code exists, so add the voter to the voters list of that node
    if (currentPostalCode != nullptr && currentPostalCode->PostalCode == voter.PostalCode) 
    {
        VoterNode* newVoterNode = new VoterNode;
        newVoterNode->Value = &voter;
        newVoterNode->Next = currentPostalCode->VotersHead;
        currentPostalCode->VotersHead = newVoterNode;
        currentPostalCode->NumOfVoters++;
    }
    else // Postal code does not exist, so create a new node for the postal code and add the voter
    {
        PCNode* newPostalCodeNode = new PCNode;
        newPostalCodeNode->PostalCode = voter.PostalCode;
        newPostalCodeNode->Next = currentPostalCode;

        VoterNode* newVoterNode = new VoterNode;
        newVoterNode->Value = &voter;
        newVoterNode->Next = nullptr;

        newPostalCodeNode->VotersHead = newVoterNode;
        newPostalCodeNode->NumOfVoters = 1;

        // If adding at the beginning of the list
        if (previousPostalCode == nullptr) 
        {
            PostalCodesHead = newPostalCodeNode;
        }
        else // Otherwise insert it after the previous node
        {
            previousPostalCode->Next = newPostalCodeNode;
        }
    }
}

void VotersCatalogue::PrintVotersByPostalCode(int postalCode)
{
    PCNode* currentPostalCode = PostalCodesHead;

    // Search for the postal code node
    while (currentPostalCode != nullptr && currentPostalCode->PostalCode != postalCode)
    {
        currentPostalCode = currentPostalCode->Next;
    }

    // If found the postal code node
    if (currentPostalCode != nullptr)
    {
        cout << currentPostalCode->NumOfVoters << " voted in " << postalCode << endl;

        VoterNode* currentVoter = currentPostalCode->VotersHead;
        while (currentVoter != nullptr)
        {
            cout << currentVoter->Value->PIN << endl;
            currentVoter = currentVoter->Next;
        }
    }
    else
    {
        std::cout << "No voters exist for postal code " << postalCode << std::endl;
    }
}

void VotersCatalogue::PrintAllSortedByVoterCount()
{
    // Convert linked list to array
    int count = 0;
    PCNode* current = PostalCodesHead;
    while (current != nullptr)
    {
        count++;
        current = current->Next;
    }

    if (count == 0)
    {
        std::cout << "No postal codes found." << std::endl;
        return;
    }

    PCNode** pcArray = new PCNode * [count];
    current = PostalCodesHead;
    for (int i = 0; i < count; i++)
    {
        pcArray[i] = current;
        current = current->Next;
    }

    // Sort the array using qsort()
    qsort(pcArray, count, sizeof(PCNode*), Compare);

    // Print the sorted postal codes and their voter count
    for (int i = 0; i < count; i++)
    {
        std::cout << pcArray[i]->PostalCode << " " << pcArray[i]->NumOfVoters << std::endl;
    }

    delete[] pcArray;
}
