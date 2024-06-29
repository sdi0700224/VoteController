#include "Voter.h"
#include <cstdio>
#include <iostream>

using namespace std;

Voter::Voter(int pin, const char surname[], const char name[], int pc, int maxNameSize) : HasVoted(false), MaxNameSize(maxNameSize), PIN(pin), Surname(new char[MaxNameSize]), Name(new char[MaxNameSize]), PostalCode(pc)
{
	for (int i = 0; i < MaxNameSize; i++)
	{
		Surname[i] = '\0';
		Name[i] = '\0';
	}

	for (int i = 0; i < MaxNameSize; i++)
	{
		if (surname[i] != '\0')
		{
			Surname[i] = surname[i];
		}
		else
		{
			break;
		}
	}

	
	for (int i = 0; i < MaxNameSize; i++)
	{
		if (name[i] != '\0')
		{
			Name[i] = name[i];
		}
		else
		{
			break;
		}
	}
}

Voter::~Voter()
{
	delete[] Name;
	delete[] Surname;
}

void Voter::PrintData()
{
	cout << PIN << " " << Surname << " " << Name << " " << PostalCode << " " << (HasVoted ? "Y" : "N") << endl;
}

bool Voter::Vote()
{
	if (!HasVoted)
	{
		HasVoted = true;
		cout << PIN << " Marked voted" << endl;
		return true;
	}
	else
	{
		cout << PIN << " has already voted" << endl;
		return false;
	}
}
