#include "LinearHashMapOF.h"
#include <iostream>

using namespace std;

// Bucket constructor
LinearHashMapOF::Bucket::Bucket(int bucketsize) : Next(nullptr)
{
    Keys = new int[bucketsize];
    Values = new Voter*[bucketsize];
    for (int i = 0; i < bucketsize; i++)
    {
        Keys[i] = -1;
        Values[i] = nullptr;
    }
}

LinearHashMapOF::Bucket::~Bucket()
{
    delete[] Keys;
    delete[] Values;
}

LinearHashMapOF::LinearHashMapOF(int bucketsize, int initM) : M(initM), Round(0), NumOfKeys(0), SplitPointer(0), BucketSize(bucketsize)
{
    BucketCapacity = M;
    table = new Bucket * [BucketCapacity];
    for (int i = 0; i < BucketCapacity; i++) 
    {
        table[i] = new Bucket(BucketSize);
    }
}

LinearHashMapOF::~LinearHashMapOF() 
{
    for (int i = 0; i < BucketCapacity; i++) 
    {
        Bucket* current = table[i];
        while (current) 
        {
            for (int slot = 0; slot < 2; slot++)
            {
                if (current->Values[slot] != nullptr)
                {
                    delete current->Values[slot];
                }
            }
            Bucket* toDelete = current;
            current = current->Next;
            delete toDelete;
        }
    }
    delete[] table;
}

int LinearHashMapOF::Hash(int key, int i)
{
    return key % ((1 << i) * M);
}

LinearHashMapOF::Bucket* LinearHashMapOF::FindPosition(int key, int& pos, int& slot, Bucket*& parent)
{
    pos = Hash(key, Round);
    if (pos < SplitPointer)
    {
        pos = Hash(key, Round + 1);
    }

    Bucket* current = table[pos];
    while (current)
    {
        for (slot = 0; slot < BucketSize; slot++)
        {
            if (current->Keys[slot] == key)
            {
                return current;
            }
        }
        parent = current;
        current = current->Next;
    }
    return nullptr;
}

void LinearHashMapOF::Split() 
{
    BucketCapacity++; // Increase by one bucket

    Bucket** newTable = new Bucket*[BucketCapacity];

    // Move old data to new table
    for (int i = 0; i < BucketCapacity - 1; i++)
    {
        newTable[i] = table[i];
    }
    newTable[BucketCapacity - 1] = new Bucket(BucketSize);

    delete[] table;
    table = newTable;

    SplitPointer++;
    ReallocateKeys();
    
    if (BucketCapacity == (M * (2 << Round)))
    {
        Round++;
        SplitPointer = 0;
    }
}

int LinearHashMapOF::GetKeyCapacity()
{
    return BucketCapacity * BucketSize;
}

double LinearHashMapOF::GetLoadFactor()
{
    return (double)NumOfKeys / GetKeyCapacity();
}

void LinearHashMapOF::ReallocateKeys()
{
    Bucket* splitBucket = table[SplitPointer - 1];
    table[SplitPointer - 1] = new Bucket(BucketSize);

    Bucket* current = splitBucket;
    while (current)
    {
        for (int slot = 0; slot < BucketSize; slot++)
        {
            if (current->Keys[slot] >= 0)
            {
                NumOfKeys--;
                if (!Insert(current->Keys[slot], current->Values[slot]))
                {
                    cout << "Error! Reallocation failed!";
                    exit(EXIT_FAILURE);
                }
            }
        }
        Bucket* toDelete = current;
        current = current->Next;
        delete toDelete;
    }
    return;
}

bool LinearHashMapOF::Insert(int key, Voter* value)
{
    if (key < 0)
    {
        cout << "Error inserting key: " << key << endl;
        return false;
    }
    int pos, slot;
    Bucket* parent;
    Bucket* bucket = FindPosition(key, pos, slot, parent);

    // if key exists return false
    if (bucket != nullptr)
    {
        return false;
    }

    // if there is space in last bucket insert
    bool isInserted = false;
    for (slot = 0; slot < BucketSize; slot++)
    {
        if (parent->Keys[slot] == -1)
        {
            parent->Keys[slot] = key;
            parent->Values[slot] = value;
            isInserted = true;
            break;
        }
    }

    // else create new bucket and attach to last one
    if (!isInserted)
    {
        Bucket* newBucket = new Bucket(BucketSize);
        newBucket->Keys[0] = key;
        newBucket->Values[0] = value;
        parent->Next = newBucket;
    }

    NumOfKeys++;

    if (GetLoadFactor() >= 0.75)
    {
        Split();
    }

    return true;
}

Voter* LinearHashMapOF::Search(int key) 
{
    if (key < 0)
    {
        cout << "Error searching for key: " << key << endl;
        return nullptr;
    }
    int pos, slot;
    Bucket* parent;
    Bucket* bucket = FindPosition(key, pos, slot, parent);
    return bucket ? bucket->Values[slot] : nullptr;
}

void LinearHashMapOF::Print(bool onlyStats)
{
    if (table == nullptr)
    {
        std::cout << "The hash table is empty or not initialized." << std::endl;
        return;
    }

    // Print hash table stats
    std::cout << "=== Hash Table Statistics ===" << std::endl;
    std::cout << "Total Bucket Capacity: " << BucketCapacity << std::endl;
    std::cout << "Total Key Capacity(Non overflow): " << GetKeyCapacity() << std::endl;
    std::cout << "Total Number of Keys: " << NumOfKeys << std::endl;
    std::cout << "Load factor: " << GetLoadFactor() << std::endl;
    std::cout << "Current Round: " << Round << std::endl;
    std::cout << "Split Pointer: " << SplitPointer << std::endl;

    int nonEmptyBuckets = 0;
    int maxChainLength = 0;

    for (int i = 0; i < BucketCapacity; i++)
    {
        int currentChainLength = 0;
        Bucket* current = table[i];

        if (current != nullptr)
        {
            nonEmptyBuckets++;
            while (current != nullptr)
            {
                currentChainLength++;
                current = current->Next;
            }
        }
        maxChainLength = std::max(maxChainLength, currentChainLength);
    }

    std::cout << "Bucket Utilization: " << nonEmptyBuckets << "/" << BucketCapacity << std::endl;
    std::cout << "Average Number of Keys per Bucket: " << static_cast<double>(NumOfKeys) / BucketCapacity << std::endl;
    std::cout << "Max Chain Length due to Overflows: " << maxChainLength << std::endl;
    std::cout << "==============================" << std::endl;

    if (onlyStats)
    {
        return;
    }

    for (int i = 0; i < BucketCapacity; i++)
    {
        std::cout << "Bucket [" << i << "]: ";

        Bucket* current = table[i];

        // If the current bucket is null (not used or initialized yet)
        if (current == nullptr)
        {
            std::cout << "Empty" << std::endl;
            continue;
        }

        // If the current bucket contains keys and values
        while (current != nullptr)
        {
            std::cout << "{ ";
            for (int j = 0; j < BucketSize; j++)
            {
                if (j > 0)
                {
                    std::cout << ", ";
                }
                std::cout << "Key: " << current->Keys[j] << ", Value: ";

                // Cast the void* to char* and print as a string
                if (current->Values[j] != nullptr)
                {
                    std::cout << "\"" << (current->Values[j])->PIN << "\"";
                }
                else
                {
                    std::cout << "nullptr";
                }
            }
            std::cout << " }";

            // Move to the overflow bucket if it exists
            current = current->Next;

            if (current != nullptr)
            {
                std::cout << " -> ";
            }
        }
        std::cout << std::endl <<std::endl;
    }
}

int LinearHashMapOF::GetNumOfRegisteredVoters()
{
    return NumOfKeys;
}
