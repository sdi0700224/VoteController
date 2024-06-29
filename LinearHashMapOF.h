#pragma once
#include "Voter.h"

class LinearHashMapOF 
{
private:
    struct Bucket 
    {
        int* Keys;
        Voter** Values;
        Bucket* Next;  // for overflow bucket
        Bucket(int bucketsize);
        ~Bucket();
    };

    int M, Round, NumOfKeys, BucketCapacity, SplitPointer, BucketSize;
    Bucket** table;

    int Hash(int key, int i);
    Bucket* FindPosition(int key, int& pos, int& slot, Bucket*& parent);
    void Split();
    int GetKeyCapacity();
    double GetLoadFactor();
    void ReallocateKeys();

public:
    LinearHashMapOF(int bucketsize, int initM = 2);
    ~LinearHashMapOF();

    bool Insert(int key, Voter* value);
    Voter* Search(int key);
    void Print(bool onlyStats);
    int GetNumOfRegisteredVoters();
};