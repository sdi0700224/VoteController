#pragma once
#define NAME_MAX_SIZE 256

class Voter
{
private:
	bool HasVoted;
	int MaxNameSize;

public:
	int PIN;
	char* Surname;
	char* Name;
	int PostalCode;

	Voter(int pin, const char surname[], const char name[], int pc, int maxNameSize = 256);
	~Voter();

	void PrintData();
	bool Vote();
};

