# How To

## Project description

Project has classes so each job is done seperatelly and cleanly.

CsvReader is used to access csv archieves and make some Voters
LinearHashMapOF is the hashmap we store the voters. Of course it has memmory ownership of all voters.
VoteController is the general controller that operates the terminal and runs the vote.
Voter is just a struct actually, thought it has some small functionally in it.
VotersCatalogue is the 2d list that keeps track of the Voters that have already Voted.

Debug helps only with Visual studio, something like valgrid, but without errors but better memmory check.
I checked also with valgrid in VsCode.

I do not find a lot to write here but I tried to code it very cleanly, give names to variables and
functions that describe their puspose as good as possible and made memmory checks both in Visual Studio
that I made the Implementation and VSCode that used to compile for linux and make the Makefile.

## Makefile

Added Dubug command to use valgrid
Clean removes files