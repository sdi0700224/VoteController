#pragma once
#include "Voter.h"
#include "LinearHashMapOF.h"
#include "VotersCatalogue.h"
#include "CsvReader.h"

class VoteController
{
private:
	LinearHashMapOF HashMap;
	VotersCatalogue Catalogue;
	CsvReader Reader;
	int AlreadyVotedNum;

	bool SubscribeVoter(int key, Voter* value);
	void InsertregisteredVoters();
	void RunTerminal(int maxCommandLength = 124);
	void VoteFromFile(char* filename);


public:
	VoteController(int bucketSize, char* filename);
	~VoteController();

	void Init();
};

